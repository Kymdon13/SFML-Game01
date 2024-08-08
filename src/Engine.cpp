#include "Engine.h"

//Private functions
void Engine::initVariables()
{
    this->window = nullptr;
    
    //Game logic
    this->gameOver = false;
    this->health = 10;
    this->points = 0;
    this->enemySpawnTimerMax = 30.f;
    this->enemySpawnTimer = this->enemySpawnTimerMax;
    this->maxEnemies = 10;
    this->mouseHeld = false;
}

void Engine::initWindow()
{
    this->videoMode.height = 1080;
    this->videoMode.width = 1920;

    this->window = new sf::RenderWindow(this->videoMode, "My window", sf::Style::Titlebar | sf::Style::Close);
    
    this->window->setFramerateLimit(60);
}

void Engine::initFonts()
{
    if (!this->font.loadFromFile("assets/Fonts/Dosis-Bold.ttf")) {
        std::cout << "Failed to load font!" << '\n';
    }
}

void Engine::initText()
{
    //uiText
    this->uiText.setFont(this->font);
    this->uiText.setCharacterSize(24);
    this->uiText.setFillColor(sf::Color::White);
    this->uiText.setString("");
    
    //gameOverText
    this->gameOverText.setFont(this->font);
    this->gameOverText.setCharacterSize(128);
    this->gameOverText.setFillColor(sf::Color::Red);
    this->gameOverText.setOutlineThickness(10.f);
    this->gameOverText.setOutlineColor(sf::Color::Yellow);
    this->gameOverText.setPosition(sf::Vector2f(500.f, 500.f));
    this->gameOverText.setString("Game Over!\nPress <Space> to restart.");
    
    sf::FloatRect gameOverTextRect = this->gameOverText.getLocalBounds();
    this->gameOverText.setOrigin(gameOverTextRect.width / 2.f, gameOverTextRect.height / 2.f);
    this->gameOverText.setPosition(this->window->getSize().x / 2.f, this->window->getSize().y / 2.f);
}

void Engine::initEnemies()
{
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(sf::Vector2f(100.f, 100.f));
    this->enemy.setFillColor(sf::Color::Cyan);
}

//Constructors | Destructors
Engine::Engine()
{
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
    this->initEnemies();
}

Engine::~Engine()
{
    delete this->window;
}

//Accessors
const bool Engine::running() const
{
    return this->window->isOpen();
}

const bool Engine::getGameOver() const
{
    return this->gameOver;
}

void Engine::spawnEnemy()
{
    /*
        @return void
        
        Spawns enemies and sets their types and colors randomly,
        and spawns them at random positions.
        - Sets a random position.
        - Sets a random color.
        - Adds enemy to the vector(refer to container here).
    */
    
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        -100.f
    );
    
    //Randomize enemy type
    unsigned enemyType = rand() % 5;
    
    switch (enemyType) {
        case 0:
            this->enemy.setSize(sf::Vector2f(20.f, 20.f));
            this->enemy.setFillColor(sf::Color::Magenta);
            break;
        case 1:
            this->enemy.setSize(sf::Vector2f(30.f, 30.f));
            this->enemy.setFillColor(sf::Color::Blue);
            break;
        case 2:
            this->enemy.setSize(sf::Vector2f(50.f, 50.f));
            this->enemy.setFillColor(sf::Color::Cyan);
            break;   
        case 3:
            this->enemy.setSize(sf::Vector2f(70.f, 70.f));
            this->enemy.setFillColor(sf::Color::Red);
            break;   
        case 4:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Green);
            break;    
        default:
            this->enemy.setSize(sf::Vector2f(100.f, 100.f));
            this->enemy.setFillColor(sf::Color::Yellow);
            break;
    }
    
    //Spawn the enemy
    this->enemies.push_back(this->enemy);
}

// Functions
void Engine::pollEvents()
{
    // pop the top event from the event queue
    while(this->window->pollEvent(this->ev))
    {
        // check the type of the event
        switch(this->ev.type)
        {
            // if close button been clicked
            case sf::Event::Closed:
                this->window->close();
                break;
            // if Esc been pressed
            case sf::Event::KeyPressed:
                if(this->ev.key.code == sf::Keyboard::Escape)
                    this->window->close();
                break;
            // restart
            case sf::Event::KeyReleased:
                if (this->ev.key.code == sf::Keyboard::Space) {
                    if (this->gameOver) {
                        this->gameOver = false;
                        this->enemies.clear();
                        this->health = 10;
                    }
                }
                break;
        }
    }
}

void Engine::updateMousePosition()
{
    /*
        @return void
         
        Update the mouse position:
        - Mouse position relative to window (Vector2i)
    */
    this->mousePosWindow = sf::Mouse::getPosition(*this->window);
    this->mousePosView = this->window->mapPixelToCoords(this->mousePosWindow);
}

void Engine::updateText()
{
    /*
        A unique type of stream that can contain
        different kinds of data and mix them together.
    */
    std::stringstream ss;
    
    ss << "Points: " << this->points << '\n'
        << "Health: " << this->health << '\n';
    
    this->uiText.setString(ss.str());
}

void Engine::updateEnemies()
{
    /*
        @return void
        
        Update the enemy spawn timer and spawns enemies
        when the total amount of enemies is smaller than the maximum.
        
        Moves the enemies downwards.
        
        Removes the enemies at the edge of the screen. //TODO
    */
   
    //Updating the timer for enemy spawning
    if (this->enemies.size() <= this->maxEnemies)
    {
        if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
        {
            //Spawn the enemy and reset the timer
            this->spawnEnemy();
            this->enemySpawnTimer = 0.f;
        }
        else
            this->enemySpawnTimer += 1.f;
    }
    
    //Move and update the enemies
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, 5.f);
        
        //If the enemy is past the bottom of the screen
        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
        }
    }
    
    //Check if clicked upon
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (!this->mouseHeld) {
            this->mouseHeld = true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i++) {
                if (this->enemies[i].getGlobalBounds().contains(this->mousePosView)) {
                    //Gain points
                    if (this->enemies[i].getFillColor() == sf::Color::Magenta)
                        this->points += 10;
                    else if (this->enemies[i].getFillColor() == sf::Color::Blue)
                        this->points += 7;
                    else if (this->enemies[i].getFillColor() == sf::Color::Cyan)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == sf::Color::Red)
                        this->points += 3;
                    else if (this->enemies[i].getFillColor() == sf::Color::Green)
                        this->points += 1;

                    //Delete the enemy
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                }
            }
        }
    } else {
        //Convert the value when mouse was click in two consecutive frames
        this->mouseHeld = false;
    }
}

void Engine::update()
{
    this->pollEvents();
    
    if (!this->gameOver) {
        //Update mouse position
        this->updateMousePosition();
        
        //Update enemies
        this->updateEnemies();
    }
    //Update the Text(always update the text no matter if game is over)
    this->updateText();
}

void Engine::renderText(sf::RenderTarget &target)
{
    target.draw(this->uiText);
}

void Engine::renderEnemies(sf::RenderTarget &target)
{
    //Rendering all the enemies
    for (auto &e: this->enemies)
    {
        target.draw(e);
    }
}

void Engine::renderGameOver(sf::RenderTarget &target)
{
    target.draw(this->gameOverText);
}

void Engine::render()
{
    /*
        @return void

        - clear old frame
        - render objects
        - display frame in window

        Render the game objects.
    */
    this->window->clear();
    
    //Draw Game Objects
    this->renderEnemies(*this->window);
    
    this->renderText(*this->window);
    
    if (this->health <= 0)
    {
        this->gameOver = true;
        this->renderGameOver(*this->window);
    }

    this->window->display();
}
