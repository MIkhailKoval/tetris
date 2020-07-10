#include <SFML/Graphics.hpp>
#include <cassert>
#include <random>
class Game {
public:
	Game(): window(sf::VideoMode(700, 400), "Tetris"), field(20, std::vector<int>(14, -1)), width(14), height(20), x0(13), y0(13) {
				srand(454);
			}
	void drawContur();
	Shape* makeFigure();
	void drawShape( Shape* shape, sf::Sprite& spr, int x, int y );
	bool canStandHere( Shape* shape, int& x, int& y ) const;
	bool canRotateFigure( Shape* shape, int& x, int& y ) const;
	bool canMoveRight( Shape* shape, int& x, int& y ) const;
	bool canMoveLeft( Shape* shape, int& x, int& y ) const;
	bool canMoveDown( Shape* shape, int& x, int& y ) const;
	void clearFullRows();
	void start();
	void drawField();
	~Game();
private:
	sf::RenderWindow window;
	std::vector< std::vector<int> > field;
	std::vector<Shape*> s = {new Square(), new Snake(), new L(), new AntiSnake(), new Piedestal(), new J(), new Rectangle() };
	size_t width;
	size_t height;
	size_t x0;
	size_t y0;
	bool stop = 0;
};

void Game::drawContur() {
	int wise = 3, x = 10, y = 10;
	sf::RectangleShape second_line(sf::Vector2f(y + 360, wise)), first_line(sf::Vector2f(x + 252 - wise, wise));
	first_line.setFillColor(sf::Color(0,0,0));
	second_line.setFillColor(sf::Color(0,0,0));
	second_line.rotate(90.f); 
	second_line.setPosition(x + 252 + wise * 2, y);
	first_line.setPosition(x, y + 360 + wise * 2);
	window.draw(first_line);
	window.draw(second_line);
	second_line.setPosition(x, y);
	first_line.setPosition(x, y);
	window.draw(first_line);
	window.draw(second_line);
}
Shape* Game::makeFigure() {
	int x = rand() % s.size(); 
	return s[x];
}
void Game::drawShape( Shape* shape, sf::Sprite& spr, int x, int y ) {
	for( int i = 0; i < shape -> height; ++i ) {
		for( int j = 0; j < shape -> width; ++j ) {
			if( (*shape)[i][j] ) {
				spr.setPosition(x + j * 18, y + i * 18);
				window.draw(spr);
			}
		}
	}
}
bool Game::canStandHere( Shape* shape, int& x, int& y ) const {
	for( int i = 0; i < shape -> height; ++i ) {
		for( int j = 0; j < shape -> width; ++j ) {
			int x1 = (x - x0) / 18 + j, y1 = (y - y0) / 18 + i;
			if( (*shape)[i][j] && ( ( x1 < width && y1 < height && field[y1][x1] != -1 ) || ( x1 >= width || y1 >= height ) ) ) {
				return false;
			}
		}
	}
	return true;
}
bool Game::canRotateFigure( Shape* shape, int& x, int& y ) const {
	shape -> rotate();
	bool ans = canStandHere(shape, x, y);
	shape -> rotate();
	shape -> rotate();
	shape -> rotate();
	return ans;
}
bool Game::canMoveRight( Shape* shape, int& x, int& y ) const {
	x += 18;
	bool ans = canStandHere(shape, x, y);
	x -= 18;
	return ans;
}
bool Game::canMoveLeft( Shape* shape, int& x, int& y ) const {
	x -= 18;
	bool ans = canStandHere(shape, x, y);
	x += 18;
	return ans;
}
bool Game::canMoveDown( Shape* shape, int& x, int& y ) const {
	y += 18;
	bool ans = canStandHere(shape, x, y);
	y -= 18;
	return ans;
}
void Game::clearFullRows() {
	int ost = 0;
	for( int i = height - 1; i >= 0; i-- ) {
		int sum = 0;
		for( int j = 0; j < width; ++j ) {
			sum += ( field[i][j] != -1 );
		}
		if( sum == width ) {
			++ost;
		}
		if( i < ost ) {
			for( int j = 0; j < width; ++j ) {
				field[i][j] = -1;
			}	
		} else {
			for( int j = 0; j < width; ++j ) {
					field[i][j] = field[i - ost][j];
			}
		}
	}
}
void Game::start() {
	int x = x0, y = y0;
	Shape* shape = makeFigure();
	sf::Sprite spr(shape -> texture);
	float timer = 0, pause = 0.3;	
	sf::Clock clock;
	while( window.isOpen() ) {
		sf::Event event;
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;
		while( window.pollEvent(event) ) {
			if( event.type == sf::Event::Closed ) {
				window.close();
			}
			if( event.type == sf::Event::KeyPressed ) {
				if( event.key.code == sf::Keyboard::Up && !stop && canRotateFigure(shape, x, y) ) {
					shape -> rotate();
				} else if( event.key.code == sf::Keyboard::Right && !stop && canMoveRight(shape, x, y)  ) {
					x += 18;
				} else if( event.key.code == sf::Keyboard::Left && !stop && canMoveLeft(shape, x, y) )  {
					x -= 18;
				} else if( event.key.code == sf::Keyboard::Down && !stop && canMoveDown(shape, x, y) ) {
						y += 18;
				} else if( event.key.code == sf::Keyboard::Enter && !stop ) {
					while( canMoveDown( shape, x, y ) ) {
						y += 18;
					}
				} else if( event.key.code == sf::Keyboard::Space ) {
					stop ^= 1;
				} 
			}
		}
		window.clear(sf::Color(250, 220, 100, 0));
		drawContur();
		if (timer > pause && canMoveDown(shape, x, y) && !stop ) {
			y += 18;
			timer = 0;
		} else if( timer > pause && !stop ) {
			timer = 0;
			for( int i = 0; i < shape -> height; ++i ) {
				for( int j = 0; j < shape -> width; ++j ) {
					int x1 = (x - x0) / 18 + j, y1 = (y - y0) / 18 + i;
					field[y1][x1] = ( (*shape)[i][j] )? shape ->type_of_texture: field[y1][x1];
				}
			}
			shape = makeFigure();
			spr = sf::Sprite(shape -> texture);
			x = x0;
			y = y0;	
			clearFullRows();	
		}																																																
		drawShape(shape, spr, x, y);
		drawField();
		window.display();
		
	}
}
void Game::drawField() {
	for( int i = 0; i < height; ++i ) {
		for( int j = 0; j < width; ++j ) {
			if( field[i][j] == -1 ) continue;
			sf::Texture texture;
			texture.loadFromFile("graphics.png", sf::IntRect(field[i][j] * 18, 0, 18,18));
			sf::Sprite spr(texture);
			spr.setPosition(x0 + j * 18, y0 + i * 18);
			window.draw(spr);
		}
	}		
}
Game::~Game() {
	for( int i = 0; i < s.size(); ++i ) {
		delete s[i];
	}
}
