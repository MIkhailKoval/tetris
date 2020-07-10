#include <SFML/Graphics.hpp>
#include <cassert>
#include <random>
#include <ctime>
class Game {
public:
	Game(): window(sf::VideoMode(600, 400), "Tetris"), field(20, std::vector<int>(14, -1)), width(14), height(20), x0(13), y0(13) {
				srand(time(NULL));
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
	void drawStatistic();
	void newGame();
	void pauseButtons( const sf::Event&, Shape*, int&, int& );
	void playButtons( const sf::Event&, Shape*, int&, int& );
	void endButtons( const sf::Event&, Shape*, int&, int& );
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
	int score = 0;
	int rows = 0;
	bool isOver = 0;
};

void Game::drawContur() {
	sf::RectangleShape rectangle(sf::Vector2f(252, 360));
	rectangle.move(13, 13);
	rectangle.setFillColor(sf::Color(175, 180, 240));
	rectangle.setOutlineThickness(3.f);
	rectangle.setOutlineColor(sf::Color(0,0,0));
	window.draw(rectangle);
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
	int ost = 0, count = 0;
	for( int i = height - 1; i >= 0; i-- ) {
		int sum = 0;
		for( int j = 0; j < width; ++j ) {
			sum += ( field[i][j] != -1 );
		}
		if( sum == width ) {
			++ost;
			++rows;
			++count;
			score += 100 * count;
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
		if( sum == width ) i++;
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
			} else if( isOver ) {
				endButtons( event, shape, x, y );
			} else if( stop ) {
				pauseButtons( event, shape, x, y );
			} else {
				playButtons( event, shape, x, y );
			}
		}
		window.clear(sf::Color(119, 221, 119, 0));
		if( isOver ) {

		} else if (timer > pause && canMoveDown(shape, x, y) && !stop && !isOver ) {
			y += 18;
			timer = 0;
		} else if( timer > pause && !stop ) {
			timer = 0;
			score += 10;
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
			if( !canStandHere(shape,x,y)) {
				isOver = 1;
			}	
		}
		drawContur();
		if( !isOver && canStandHere(shape,x,y) ) {
			drawShape(shape, spr, x, y);
		}
		drawField();
		drawStatistic();
		if( isOver ) {
			sf::Font font;
			font.loadFromFile("fonts/arial.ttf");
			sf::Text text("Tap Space to continue",font,20);
			text.setFillColor(sf::Color::Black);
			text.setPosition(300, 300);
			window.draw(text);	
		}
		window.display();
		
	}
}
void Game::drawField() {
	for( int i = 0; i < height; ++i ) {
		for( int j = 0; j < width; ++j ) {
			if( field[i][j] == -1 ) continue;
			sf::Texture texture;
			texture.loadFromFile("sprites/graphics.png", sf::IntRect(field[i][j] * 18, 0, 18,18));
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
void Game::drawStatistic() {
	sf::RectangleShape rectangle(sf::Vector2f(150, 150));
	rectangle.move(300, 13);
	rectangle.setFillColor(sf::Color(175, 180, 240));
	rectangle.setOutlineThickness(3);
	rectangle.setOutlineColor(sf::Color(0,0,0));
	window.draw(rectangle);
	sf::Font font;
	font.loadFromFile("fonts/arial.ttf");
	
	sf::Text text("Score:" + std::to_string(score),font,20);
	text.setFillColor(sf::Color::Black);
	text.setPosition(305, 18);
	window.draw(text);

	sf::Text text1("Rows:" + std::to_string(rows), font, 20);
	text1.setFillColor(sf::Color::Black);
	text1.setPosition(305, 45);
	window.draw(text1);	

}
void Game::pauseButtons(const sf::Event& event, Shape* shape, int& x, int& y) {
	if( event.type == sf::Event::KeyPressed ) {
		if( event.key.code == sf::Keyboard::Space ) {
				stop ^= 1; 
		}
	}
}
void Game::endButtons(const sf::Event& event, Shape* shape, int& x, int& y) {
	if( event.type == sf::Event::KeyPressed ) {
		if( event.key.code == sf::Keyboard::Space ) {
			newGame();
		}
	}
}
void Game::playButtons(const sf::Event& event, Shape* shape, int& x, int& y) {
	if( event.type == sf::Event::KeyPressed ) {
		if( event.key.code == sf::Keyboard::Up && canRotateFigure(shape, x, y) ) {
			shape -> rotate();
		} else if( event.key.code == sf::Keyboard::Right && canMoveRight(shape, x, y)  ) {
			x += 18;
		} else if( event.key.code == sf::Keyboard::Left && canMoveLeft(shape, x, y) )  {
			x -= 18;
		} else if( event.key.code == sf::Keyboard::Down && canMoveDown(shape, x, y) ) {
				y += 18;
		} else if( event.key.code == sf::Keyboard::Enter ) {
			while( canMoveDown( shape, x, y ) ) {
				y += 18;
			}
		} else if( event.key.code == sf::Keyboard::Space ) {
			stop ^= 1;
		} 
	}
} 
void Game::newGame() {
	score = 0;
	stop = 0;
	rows = 0;
	isOver = 0;
	field.assign(20, std::vector<int>(14, -1));
	start();
}