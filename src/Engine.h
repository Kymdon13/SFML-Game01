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

    // Create a VertexArray to draw the line
    sf::VertexArray *ray;
    sf::Vector2f startPoint;
    sf::Vector2f endPoint;

    //Assets
    sf::Font font;
    
    //Text
    sf::Text uiText;

    //Game logic
    unsigned points;
    int health;
    bool mouseHeld;

    //Private functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
public:
    Engine();
    virtual ~Engine();
    
    //Accessors
    const bool running() const;
    const bool getGameOver() const;

    void pollEvents();
    void updateMousePosition();
    void updateText();
    void update();

    void renderText(sf::RenderTarget &target);
    void render();
};
