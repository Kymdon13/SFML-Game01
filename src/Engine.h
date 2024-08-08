#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

class Engine
{
private:
    //Variables
    //Window
    sf::RenderWindow* window;
    sf::VideoMode videoMode;
    sf::Event ev;
    
    //Mouse position
    sf::Vector2i mousePosWindow;
    sf::Vector2f mousePosView; // Convert the integer to float.

    //Assets
    sf::Font font;
    
    //Text
    sf::Text uiText;
    sf::Text gameOverText;

    //Game logic
    bool gameOver;
    unsigned points;
    int health;
    float enemySpawnTimer;
    float enemySpawnTimerMax;
    int maxEnemies;
    bool mouseHeld;

    //Game objects
    std::vector<sf::RectangleShape> enemies;
    sf::RectangleShape enemy;

    //Private functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initEnemies();
public:
    Engine();
    virtual ~Engine();
    
    //Accessors
    const bool running() const;
    const bool getGameOver() const;

    //Functions
    void spawnEnemy();

    void pollEvents();
    void updateMousePosition();
    void updateText();
    void updateEnemies();
    void update();

    void renderText(sf::RenderTarget &target);
    void renderEnemies(sf::RenderTarget &target);
    void renderGameOver(sf::RenderTarget &target);
    void render();
};
