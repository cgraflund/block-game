#include "Game.h"

// Private Functions
void Game::initVariables()
{
    this->window = nullptr;

    // Game logic
    this->points = 0;
    this->enemySpawnTimerMax = 10.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 5;
}

void Game::initWindow()
{
    this->videoMode.height = 600;
    this->videoMode.width = 800;
    this->window = new sf::RenderWindow(this->videoMode, "Game 1", sf::Style::Titlebar | sf::Style::Close);

    this->window->setFramerateLimit(60);
}

void Game::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(50.f, 50.f));
    this->enemy.setScale(sf::Vector2f(1.0f, 1.0f));
    this->enemy.setFillColor(sf::Color::Cyan);
    // this->enemy.setOutlineColor(sf::Color::Green);
    // this->enemy.setOutlineThickness(1.f);
}

// Constructors / Destructors
Game::Game()
{
    this->initVariables();
    this->initWindow();
    this->initEnemies();
}

Game::~Game()
{
    delete this->window;
}

// Accessors
const bool Game::running() const
{
    return this->window->isOpen();
}

void Game::spawnEnemy()
{
    /*
        @return void
    */
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f
    );

    this->enemy.setFillColor(sf::Color::Green);

    this->enemies.push_back(this->enemy);
}

void Game::pollEvents()
{
    // Event Polling
    while (this->window->pollEvent(this->event))
    {
        switch (this->event.type) {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->event.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        }

    }
}

void Game::updateMousePositions()
{
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Game::updateEnemies()
{
    if (this->enemies.size() < this->maxEnemies) {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax) {
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else {
            this->enemySpawnTimer += 1.f;
        }
    }

    // Move the enemies
    for (int i = 0; i < this->enemies.size(); i++) {
        bool deleted = false;
        this->enemies[i].move(0.f, 2.f);

        // Check if clicked on
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                deleted = true;

                // Gain points
                this->points += 10.f;
            }
        }

        // If the enemy is past the bottom of the screen
        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            deleted = true;
        }

        if (deleted) {
            this->enemies.erase(this->enemies.begin() + i);
        }
    }
    
}

// Functions
void Game::update()
{
    this->pollEvents();
    this->updateMousePositions();
    this->updateEnemies();

    // Update mouse position
    // Relative to the screen
    //std::cout << "Mouse pos: " << sf::Mouse::getPosition().x << " " << sf::Mouse::getPosition().y << "\n";
    // Relative to the window
    //std::cout << "Mouse pos: " << sf::Mouse::getPosition(*this->window).x << " " << sf::Mouse::getPosition(*this->window).y << "\n";
}

void Game::renderEnemies()
{
    for (auto& enemy : this->enemies) {
        this->window->draw(enemy);
    }
}

void Game::render()
{
    /*
        @return void
     
        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects.
    */
    this->window->clear();

    // Draw game objects
    this->renderEnemies();

    this->window->display();
}