// 可更改参数都有“参数”作为注释
#include <iostream>
#include <vector>
#include <ctime>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


int main()
{
    sf::VideoMode videoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(videoMode, "Collision Detection Example");
    window.setFramerateLimit(60);

    //Clock
    float dt;
    sf::Clock dt_clock;

    //Set grid size
    const float gridSize = 50.f;
    
    //Player
    const float playerSpeed = 500.f;
    sf::Vector2f movement;
    sf::RectangleShape player;
    sf::FloatRect nextPos;
    player.setFillColor(sf::Color::Green);
    player.setSize(sf::Vector2f(gridSize, gridSize));

    //Wall
    std::vector<sf::RectangleShape> walls;
    sf::RectangleShape wall;
    wall.setFillColor(sf::Color::Red);
    wall.setSize(sf::Vector2f(gridSize, gridSize));
    wall.setPosition(sf::Vector2f(2 * gridSize, 3 * gridSize));
    walls.push_back(wall);
    sf::RectangleShape wall1;
    wall1.setFillColor(sf::Color::Red);
    wall1.setSize(sf::Vector2f(gridSize, gridSize));
    wall1.setPosition(sf::Vector2f(1 * gridSize, 0 * gridSize));
    walls.push_back(wall1);
    
    //Candidate box
    sf::RectangleShape candidate;
    candidate.setOutlineColor(sf::Color::Red);
    candidate.setOutlineThickness(4.f);
    candidate.setFillColor(sf::Color::Transparent);
    candidate.setSize(sf::Vector2f(gridSize, gridSize));
    candidate.setPosition(sf::Vector2f(2 * gridSize, 3 * gridSize));

    // Game loop
    while (window.isOpen())
    {
        bool wallHasAdded = false;

        //Event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.key.scancode == sf::Keyboard::Scan::Escape)
                window.close();
        }
        
        //Add wall by clicking
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window) / (int)gridSize;
            bool exist = false;
            for (size_t i = 0; i < walls.size(); i++)
            {
                if (walls[i].getPosition().x / (int)gridSize == mousePos.x
                    && walls[i].getPosition().y / (int)gridSize == mousePos.y)
                {
                    exist = true;
                }
            }
            if (!exist)
            {
                wall.setPosition(mousePos.x * gridSize, mousePos.y * gridSize);
                walls.push_back(wall);
                wallHasAdded = true;
            }
        }
        sf::Vector2i mousePos = sf::Mouse::getPosition(window) / (int)gridSize;
        candidate.setPosition(mousePos.x * gridSize, mousePos.y * gridSize);
        
        //Print if wall has been added
        if (wallHasAdded)
        {
            std::cout << "walls: " << walls.size() << std::endl;
            wallHasAdded = false;
        }

        //Get dt
        dt = dt_clock.restart().asSeconds();
        
        //Player movement
        movement.x = 0.f;
        movement.y = 0.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            movement.y -= playerSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            movement.y += playerSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            movement.x -= playerSpeed * dt;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            movement.x += playerSpeed * dt;
        }

        //Get temp data
        sf::Vector2f pos_tmp = player.getPosition();
        sf::FloatRect playerBounds = player.getGlobalBounds();
        nextPos = playerBounds;
        nextPos.left += movement.x;
        nextPos.top += movement.y;

        //Object collision, including edge and vertex collision detection
        for (auto &wall: walls)
        {
            sf::FloatRect wallBounds = wall.getGlobalBounds();
            if (wallBounds.intersects(nextPos))
            {
                float playerRight = playerBounds.left + playerBounds.width;
                float playerBottom = playerBounds.top + playerBounds.height;
                float wallRight = wallBounds.left + wallBounds.width;
                float wallBottom = wallBounds.top + wallBounds.height;
                //Left side of the wall
                if (playerRight <= wallBounds.left
                    && playerBounds.top < wallBottom //So that player in the range of collision
                    && playerBottom > wallBounds.top) //So that player in the range of collision
                {
                    movement.x = 0.f;
                    player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, pos_tmp.y));
                    continue;
                }
                //Right side of the wall
                if (playerBounds.left >= wallRight
                    && playerBounds.top < wallBottom //So that player in the range of collision
                    && playerBottom > wallBounds.top) //So that player in the range of collision
                {
                    movement.x = 0.f;
                    player.setPosition(sf::Vector2f(wallRight, pos_tmp.y));
                    continue;
                }
                //Up side of the wall
                if (playerBounds.left < wallRight //So that player in the range of collision
                    && playerRight > wallBounds.left //So that player in the range of collision
                    && playerBottom <= wallBounds.top)
                {
                    movement.y = 0.f;
                    player.setPosition(sf::Vector2f(pos_tmp.x, wallBounds.top - playerBounds.height));
                    continue;
                }
                //Down side of the wall
                if (playerBounds.left < wallRight //So that player in the range of collision
                    && playerRight > wallBounds.left //So that player in the range of collision
                    && playerBounds.top >= wallBottom)
                {
                    movement.y = 0.f;
                    player.setPosition(sf::Vector2f(pos_tmp.x, wallBottom));
                }
                //Left-Top vertex of the wall
                if (playerRight <= wallBounds.left
                    && playerBottom <= wallBounds.top)
                {
                    float x_offset = playerRight - wallBounds.left;
                    float y_offset = playerBottom - wallBounds.top;
                    if (x_offset > y_offset) //Next position is closer to the upside of the wall
                    {
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(pos_tmp.x, wallBounds.top - playerBounds.height));
                    } else if (x_offset < y_offset) { //Next position is closer to the leftside of the wall
                        movement.x = 0.f;
                        player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, pos_tmp.y));
                    } else { //Next position is closer to neither side of the wall
                        movement.x = 0.f;
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, wallBounds.top - playerBounds.height));
                    }
                }
                //Right-Top vertex of the wall
                if (playerBounds.left >= wallRight
                    && playerBottom <= wallBounds.top)
                {
                    float x_offset = wallRight - playerBounds.left;
                    float y_offset = playerBottom - wallBounds.top;
                    if (x_offset > y_offset) //Next position is closer to the upside of the wall
                    {
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(pos_tmp.x, wallBounds.top - playerBounds.height));
                    } else if (x_offset < y_offset) { //Next position is closer to the rightside of the wall
                        movement.x = 0.f;
                        player.setPosition(sf::Vector2f(wallRight, pos_tmp.y));
                    } else { //Next position is closer to neither side of the wall
                        movement.x = 0.f;
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(wallRight, wallBounds.top - playerBounds.height));
                    }
                }
                //Right-Bottom vertex of the wall
                if (playerBounds.left >= wallRight
                    && playerBounds.top >= wallBottom)
                {
                    float x_offset = wallRight - playerBounds.left;
                    float y_offset = wallBottom - playerBounds.top;
                    if (x_offset > y_offset) //Next position is closer to the upside of the wall
                    {
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(pos_tmp.x, wallBottom));
                    } else if (x_offset < y_offset) { //Next position is closer to the rightside of the wall
                        movement.x = 0.f;
                        player.setPosition(sf::Vector2f(wallRight, pos_tmp.y));
                    } else { //Next position is closer to neither side of the wall
                        movement.x = 0.f;
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(wallRight, wallBottom));
                    }
                }
                //Left-Bottom vertex of the wall
                if (playerRight <= wallBounds.left
                    && playerBounds.top >= wallBottom)
                {
                    float x_offset = playerRight - wallBounds.left;
                    float y_offset = wallBottom - playerBounds.top;
                    if (x_offset > y_offset) //Next position is closer to the upside of the wall
                    {
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(pos_tmp.x, wallBottom));
                    } else if (x_offset < y_offset) { //Next position is closer to the rightside of the wall
                        movement.x = 0.f;
                        player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, pos_tmp.y));
                    } else { //Next position is closer to neither side of the wall
                        movement.x = 0.f;
                        movement.y = 0.f;
                        player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, wallBottom));
                    }
                }
            }
        }
        
        //Screen collision, with next position, it will not 鬼畜
        if (nextPos.left < 0.f)
        {
            movement.x = 0.f;
            player.setPosition(sf::Vector2f(0.f, pos_tmp.y));
        }
        else if (nextPos.left + nextPos.width > WINDOW_WIDTH)
        {
            movement.x = 0.f;
            player.setPosition(sf::Vector2f(WINDOW_WIDTH - playerBounds.width, pos_tmp.y));
        }
        if (nextPos.top < 0.f)
        {
            movement.y = 0.f;
            player.setPosition(sf::Vector2f(pos_tmp.x, 0.f));
        }
        else if (nextPos.top + nextPos.height > WINDOW_HEIGHT)
        {
            movement.y = 0.f;
            player.setPosition(sf::Vector2f(pos_tmp.x, WINDOW_HEIGHT - playerBounds.height));
        }
        
        player.move(movement);

        //Render
        window.clear();
        window.draw(player);
        for (auto &wall: walls)
        {
            window.draw(wall);
        }
        window.draw(candidate); //Draw candidate box
        window.display();
    }

    return 0;
}