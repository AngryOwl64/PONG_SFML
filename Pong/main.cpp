#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
//menu machen + main eigene methode
void shuffle(float &ballMoveX, float &ballMoveY, std::mt19937 &gen) {
	std::uniform_real_distribution<float>dist(-600.0f, 600.0f);
	do {
		ballMoveX = dist(gen);
		ballMoveY = dist(gen);
	} while (ballMoveX < 120.0f && ballMoveX > -120.0f || ballMoveY < 120.0f && ballMoveY > -120.0f);
	std::cout << "function shuffle called" << std::endl;
	std::cout << "speed X: " << ballMoveX << " Speed Y: " << ballMoveY << std::endl;
}

int main() {
	std::random_device rd;
	std::mt19937 gen(rd());
	//Variables
	float playerspeed = 360;
	float ballMoveIncrement = 60.0f;
	int score1 = 0;
	int score2 = 0;

	float ballMoveX = 0.0f;
	float ballMoveY = 0.0f;
	shuffle(ballMoveX, ballMoveY, gen);

	sf::Clock clock;

	//window
	unsigned int width = 1920;
	unsigned int height = 1080;
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width, height }), "Pong");
	window->setFramerateLimit(120);
	
	//frame / border
	sf::RectangleShape borderUp({ (float)width, 5.0f });
	borderUp.setPosition({ width / 2.0f, 2.5f });
	borderUp.setOrigin({ width / 2.0f, 2.5f });

	sf::RectangleShape borderDown({ (float)width, 5.0f });
	borderDown.setPosition({ width / 2.0f, height - 2.5f });
	borderDown.setOrigin({ width / 2.0f, 2.5f });

	//player1
	sf::RectangleShape player1;
	player1.setSize({20.0f, 270.0f});
	player1.setOrigin({10.0f, 135.0f});
	player1.setPosition({ 40.0f, height / 2.0f });

	//player2
	sf::RectangleShape player2;
	player2.setSize({ 20.0f, 270.0f });
	player2.setOrigin({ 10.0f, 135.0f });
	player2.setPosition({ width - 40.0f, height / 2.0f });

	//goals
	sf::RectangleShape goalPlayer1;
	goalPlayer1.setSize({ 1.0f, (float)height});
	goalPlayer1.setOrigin({ 0.5f, height / 2.0f });
	goalPlayer1.setPosition({ 0.5f, height / 2.0f });

	sf::RectangleShape goalPlayer2;
	goalPlayer2.setSize({ 1.0f, (float)height});
	goalPlayer2.setOrigin({ 0.5f, height / 2.0f });
	goalPlayer2.setPosition({ width - 0.5f, height / 2.0f });

	//ball
	sf::CircleShape ball(20.0f);
	ball.setOrigin(ball.getGeometricCenter());
	ball.setFillColor(sf::Color::Magenta);
	ball.setPosition({ width / 2.0f, height / 2.0f });
	ball.setPointCount(30);

	//Assets
	sf::Font font;

	if (!font.openFromFile("assets/fonts/myFont.ttf")) {
		std::cerr << "Font couldn't get loaded!\n";
	}
	
	//Scores
	sf::FloatRect textRect;
	sf::Text scorePlayer1(font);
	scorePlayer1.setString(std::to_string(score1));
	scorePlayer1.setCharacterSize(45);
	textRect = scorePlayer1.getLocalBounds();
	scorePlayer1.setOrigin(textRect.getCenter());
	scorePlayer1.setPosition({30.0f, 30.0f});

	sf::Text scorePlayer2(font);
	scorePlayer2.setString(std::to_string(score2));
	scorePlayer2.setCharacterSize(45);
	textRect = scorePlayer2.getLocalBounds();
	scorePlayer2.setOrigin(textRect.getCenter());
	scorePlayer2.setPosition({ width - 30.0f, 30.0f });

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
		float dt = clock.restart().asSeconds();

		ball.move({ ballMoveX * dt, ballMoveY * dt});

		auto p1Bounds = player1.getGlobalBounds();
		auto p2Bounds = player2.getGlobalBounds();
		auto upBounds = borderUp.getGlobalBounds();
		auto downBounds = borderDown.getGlobalBounds();
		float topLimit = borderUp.getPosition().y + borderUp.getSize().y;
		float bottomLimit = borderDown.getPosition().y;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::W)) {
			if (p1Bounds.position.y > topLimit) {
				player1.move({ 0.0f, -playerspeed * dt});
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::S)) {
			if (p1Bounds.position.y + p1Bounds.size.y < bottomLimit) {
				player1.move({ 0.0f, playerspeed * dt});
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Up)) {
			if (p2Bounds.position.y > topLimit) {
				player2.move({ 0.0f, -playerspeed * dt});
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Down)) {
			if (p2Bounds.position.y + p2Bounds.size.y < bottomLimit) {
				player2.move({ 0.0f, playerspeed * dt});
			}
		}
		auto ballBounds = ball.getGlobalBounds();
		if (ballBounds.findIntersection(upBounds)&& ballMoveY < 0) { // - -> +
			ballMoveY = -ballMoveY + ballMoveIncrement;
		}
		if (ballBounds.findIntersection(downBounds)&& ballMoveY > 0) { // + -> -
			ballMoveY = -ballMoveY - ballMoveIncrement;
		}
		if (ballBounds.findIntersection(p1Bounds)&& ballMoveX < 0) { // - -> +
			ballMoveX = -ballMoveX + ballMoveIncrement;
		}
		if (ballBounds.findIntersection(p2Bounds)&& ballMoveX > 0) { // + -> -
			ballMoveX = -ballMoveX - ballMoveIncrement;
		}
		bool goal = false;
		if (ballBounds.findIntersection(goalPlayer1.getGlobalBounds())) {
			score2++;
			goal = true;
			scorePlayer2.setString(std::to_string(score2));
			scorePlayer2.setOrigin(scorePlayer2.getLocalBounds().getCenter());
		}
		else if (ballBounds.findIntersection(goalPlayer2.getGlobalBounds())) {
			score1++;
			goal = true;
			scorePlayer1.setString(std::to_string(score1));
			scorePlayer1.setOrigin(scorePlayer1.getLocalBounds().getCenter());

		}
		if (goal) {
			player1.setPosition({ 40.0f, 540.0f });
			player2.setPosition({ 1880.0f, 540.0f });
			ball.setPosition({ 960.0f, 540.0f });
			shuffle(ballMoveX, ballMoveY, gen);
			if (score1 == score2) {
				scorePlayer1.setFillColor(sf::Color::White);
				scorePlayer2.setFillColor(sf::Color::White);
			} else if (score1 > score2) {
				scorePlayer1.setFillColor(sf::Color::Green);
				scorePlayer2.setFillColor(sf::Color::Red);
			} else if (score1 < score2) {
				scorePlayer1.setFillColor(sf::Color::Red);
				scorePlayer2.setFillColor(sf::Color::Green);
			}
		}

		window->clear(sf::Color::Black);

		window->draw(player1);
		window->draw(player2);
		
		window->draw(ball);

		window->draw(scorePlayer1);
		window->draw(scorePlayer2);
		
		window->draw(borderDown);
		window->draw(borderUp);

		window->display();
	}
	delete window;
	return 0;
}
