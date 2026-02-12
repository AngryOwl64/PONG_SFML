#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
//farbiger score für looser und winner
//player werden zurückgesetzt
//menu machen + main eigene methode
void shuffle(float &ballMoveX, float &ballMoveY, std::mt19937 &gen) {
	std::uniform_real_distribution<float>dist(-5.0f, 5.0f);
	do {
		ballMoveX = dist(gen);
		ballMoveY = dist(gen);
	} while (ballMoveX < 1 && ballMoveX > -1 || ballMoveY < 1 && ballMoveY > -1.0f);
	std::cout << "function shuffle called" << std::endl;
	std::cout << "speed X: " << ballMoveX << " Speed Y: " << ballMoveY << std::endl;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float>dist(-5.0f, 5.0f);
	//Variables
	float playerspeed = 3.0f;
	float ballMoveIncrement = 0.5f;
	int score1 = 0;
	int score2 = 0;

	float ballMoveX;
	float ballMoveY;
	shuffle(ballMoveX, ballMoveY, gen);

	

	//window
	unsigned int width = 1920;
	unsigned int height = 1080;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width, height }), "Pong");
	window->setFramerateLimit(120);
	
	//frame / border
	sf::RectangleShape borderUp({ 1920.0f, 5.0f });
	borderUp.setOrigin({ 960.0f, 2.5f });
	borderUp.setPosition({ 960.0f, 2.5f });

	sf::RectangleShape borderDown({ 1920.0f, 5.0f });
	borderDown.setPosition({ 960.0f, 1077.5 });
	borderDown.setOrigin({ 960.0f, 2.5f });

	//player1
	sf::RectangleShape player1;
	player1.setSize({20.0f, 270.0f});
	player1.setOrigin({10.0f, 135.0f});
	player1.setPosition({ 40.0f, 540.0f });

	//player2
	sf::RectangleShape player2;
	player2.setSize({ 20.0f, 270.0f });
	player2.setOrigin({ 10.0f, 135.0f });
	player2.setPosition({ 1880.0f, 540.0f });

	//goals
	sf::RectangleShape goalPlayer1;
	goalPlayer1.setSize({ 1.0f, 1080.0f });
	goalPlayer1.setOrigin({ 0.5f, 540.0f });
	goalPlayer1.setPosition({ -1.0f, 540.0f });

	sf::RectangleShape goalPlayer2;
	goalPlayer2.setSize({ 1.0f, 1080.0f });
	goalPlayer2.setOrigin({ 0.5f, 540.0f });
	goalPlayer2.setPosition({ 1921.0f, 540.0f });

	//ball
	sf::CircleShape ball(20.0f);
	ball.setOrigin(ball.getGeometricCenter());
	ball.setFillColor(sf::Color::Magenta);
	ball.setPosition({ 960.0f, 540.0f });
	ball.setPointCount(50);

	//Assets
	sf::Font font;
	//For Release
	/*if (!font.openFromFile("../x64/Release/assets/fonts/myFont.ttf")) {
		std::cerr << "Font couldnt get loaded!" << std::endl;
	} */
	
	//For own
	if (!font.openFromFile("assets/fonts/myFont.ttf")) {
		std::cerr << "Font couldnt get loaded!" << std::endl;
	}
	
	//Scores
	sf::Text scorePlayer1(font);
	scorePlayer1.setString(std::to_string(score1));
	scorePlayer1.setCharacterSize(30);
	sf::FloatRect textRect = scorePlayer1.getLocalBounds();
	scorePlayer1.setOrigin(textRect.getCenter());
	scorePlayer1.setPosition({30.0f, 30.0f});

	sf::Text scorePlayer2(font);
	scorePlayer2.setString(std::to_string(score2));
	scorePlayer2.setCharacterSize(30);
	scorePlayer2.setOrigin(textRect.getCenter());
	scorePlayer2.setPosition({ 1890.0f, 30.0f });

	//Game
	while (window->isOpen()) {
		while (const std::optional event = window->pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window->close();
			}
			else if (const auto* keypressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keypressed->scancode == sf::Keyboard::Scancode::Escape) {
					window->close();
				}
			}
		}

		ball.move({ ballMoveX, ballMoveY });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
			if (!player1.getGlobalBounds().findIntersection(borderUp.getGlobalBounds())) {
				player1.move({ 0.0f, -playerspeed });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
			if (!player1.getGlobalBounds().findIntersection(borderDown.getGlobalBounds())) {
				player1.move({ 0.0f, playerspeed });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
			if (!player2.getGlobalBounds().findIntersection(borderUp.getGlobalBounds())) {
				player2.move({ 0.0f, -playerspeed });
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
			if (!player2.getGlobalBounds().findIntersection(borderDown.getGlobalBounds())) {
				player2.move({ 0.0f, playerspeed });
			}
		}

		if (ball.getGlobalBounds().findIntersection(borderUp.getGlobalBounds())) { // - -> +
			ballMoveY = -ballMoveY;
			ballMoveY += ballMoveIncrement;
		}
		if (ball.getGlobalBounds().findIntersection(borderDown.getGlobalBounds())) { // + -> -
			ballMoveY = -ballMoveY;
			ballMoveY -= ballMoveIncrement;
		}
		if (ball.getGlobalBounds().findIntersection(player1.getGlobalBounds())) { // - -> +
			ballMoveX = -ballMoveX;
			ballMoveX += ballMoveIncrement;
		}
		if (ball.getGlobalBounds().findIntersection(player2.getGlobalBounds())) { // + -> -
			ballMoveX = -ballMoveX;
			ballMoveX -= ballMoveIncrement;
		}
		if (ball.getGlobalBounds().findIntersection(goalPlayer1.getGlobalBounds())) {
			ball.setPosition({ 960.0f, 540.0f });
			score2++;
			scorePlayer2.setString(std::to_string(score2));
			shuffle(ballMoveX, ballMoveY, gen);
			player1.setPosition({ 40.0f, 540.0f });
			player2.setPosition({ 1880.0f, 540.0f });
		}
		if (ball.getGlobalBounds().findIntersection(goalPlayer2.getGlobalBounds())) {
			ball.setPosition({ 960.0f, 540.0f });
			score1++;
			scorePlayer1.setString(std::to_string(score1));
			shuffle(ballMoveX, ballMoveY, gen);
			player1.setPosition({ 40.0f, 540.0f });
			player2.setPosition({ 1880.0f, 540.0f });
		}

		window->clear(sf::Color::Black);

		window->draw(scorePlayer1);
		window->draw(scorePlayer2);

		window->draw(player1);
		window->draw(player2);
		
		window->draw(ball);
		
		window->draw(borderDown);
		window->draw(borderUp);

		window->display();
	}
	delete window;
	return 0;
}