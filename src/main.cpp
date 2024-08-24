// 可更改参数都有“参数”作为注释
#include <iostream>
#include "Engine.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720


int main()
{
    sf::VideoMode videoMode(WINDOW_WIDTH, WINDOW_HEIGHT);
    sf::RenderWindow window(videoMode, "Collision Detection Example");
    window.setFramerateLimit(120);

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
    wall.setPosition(sf::Vector2f(2 * gridSize, 2 * gridSize));
    
    walls.push_back(wall);

    while (window.isOpen())
    {
        //Event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.key.scancode == sf::Keyboard::Scan::Escape)
                window.close();
        }

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
            }
        }
        std::cout<<walls.size()<<std::endl;

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

        //Object collision
        for (auto &wall: walls)
        {
            sf::FloatRect wallBounds = wall.getGlobalBounds();
            if (wallBounds.intersects(nextPos))
            {
                //Left side of the wall
                if (playerBounds.left + playerBounds.width <= wallBounds.left
                    && playerBounds.top < wallBounds.top + wallBounds.height //So that player in the range of collision
                    && playerBounds.top + playerBounds.height > wallBounds.top) //So that player in the range of collision
                {
                    movement.x = 0.f;
                    player.setPosition(sf::Vector2f(wallBounds.left - playerBounds.width, pos_tmp.y));
                    continue;
                }
                //Right side of the wall
                if (playerBounds.left >= wallBounds.left + wallBounds.width
                    && playerBounds.top < wallBounds.top + wallBounds.height //So that player in the range of collision
                    && playerBounds.top + playerBounds.height > wallBounds.top) //So that player in the range of collision
                {
                    movement.x = 0.f;
                    player.setPosition(sf::Vector2f(wallBounds.left + wallBounds.width, pos_tmp.y));
                    continue;
                }
                //Up side of the wall
                if (playerBounds.left < wallBounds.left + wallBounds.width //So that player in the range of collision
                    && playerBounds.left + playerBounds.width > wallBounds.left //So that player in the range of collision
                    && playerBounds.top + playerBounds.height <= wallBounds.top)
                {
                    movement.y = 0.f;
                    player.setPosition(sf::Vector2f(pos_tmp.x, wallBounds.top - playerBounds.height));
                    continue;
                }
                //Down side of the wall
                if (playerBounds.left < wallBounds.left + wallBounds.width //So that player in the range of collision
                    && playerBounds.left + playerBounds.width > wallBounds.left //So that player in the range of collision
                    && playerBounds.top >= wallBounds.top + wallBounds.height)
                {
                    movement.y = 0.f;
                    player.setPosition(sf::Vector2f(pos_tmp.x, wallBounds.top + wallBounds.height));
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
        window.display();
    }

    return 0;
}