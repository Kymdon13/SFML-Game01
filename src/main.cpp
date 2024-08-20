// 可更改参数都有“参数”作为注释
#include <iostream>
#include "Engine.h"


class Box
{
private:
public:
    //Variables
    sf::RectangleShape rect;
    float mass;
    float size;
    float x;
    float velocity;
    float momentum;

    //Constructor and Destructor
    Box(bool left, float mass, float velocity);
    ~Box();
    
    //Object Logic
    void update();
};

Box::Box(bool left, float mass, float velocity=0.f): mass(mass), velocity(-velocity)
{
    //Default color
    rect.setFillColor(sf::Color::White);
    //Mass decides size
    size = mass * 1.f;
    if (size < 10.f)
        size = 10.f;
    if (size > 500.f)
        size = 500.f;
    rect.setSize(sf::Vector2f(size, size));
    //Whether left or right side
    if (left)
    {
        x = 50.f; // box1初始位置，参数
        rect.setPosition(sf::Vector2f(x, 720.f - size));
    }
    else
    {
        x = 100.f; // box2初始位置，参数
        rect.setPosition(sf::Vector2f(x, 720.f - size));
    }
}

Box::~Box()
{
}

void Box::update()
{
    x += velocity / 120.f;
    rect.setPosition(sf::Vector2f(x, 720.f - size));
    if (x < 0.f)
    {
        x = -x;
        velocity = -velocity;
    }
    momentum = mass * velocity;
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Collision Detection Example");
    window.setFramerateLimit(120);

    // 创建两个矩形形状，设定其左右位置和质量大小，参数
    float m1 = 1.f;
    float m2 = 1000000.f;
    float v = 50.f;
    Box box1(true, m1);
    Box box2(false, m2, v);

    //uiText
    sf::Font font;
    if (!font.loadFromFile("assets/Fonts/Dosis-Bold.ttf"))
    {
        std::cout << "Failed to load font!" << '\n';
    }
    sf::Text uiText;
    uiText.setFont(font);
    uiText.setCharacterSize(24);
    uiText.setFillColor(sf::Color::White);
    int collisionTimes = 0;
    std::stringstream ss;
    ss << "Collision Times: " << collisionTimes;
    uiText.setString(ss.str());
    ss.str(""); //Clear the stringstream

    while (window.isOpen())
    {
        //Event loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || event.key.scancode == sf::Keyboard::Scan::Escape)
                window.close();
            if (event.type == sf::Event::KeyReleased && event.key.scancode == sf::Keyboard::Scan::Space)
            {
                box1.x = 50.f;
                box1.velocity = 0.f;
                box1.rect.setPosition(sf::Vector2f(box1.x, 720.f - box1.size));
                box2.x = 100.f;
                box2.velocity = -v;
                box2.rect.setPosition(sf::Vector2f(box2.x, 720.f - box2.size));
                collisionTimes = 0;
                ss << "Collision Times: " << collisionTimes << std::endl;
                uiText.setString(ss.str());
                ss.str(""); //clear the stringstream
            }
        }

        // 检查两个矩形是否相交
        if (box1.x + box1.size >= box2.x)
        {
            float b1v = box1.velocity;
            float b2v = box2.velocity;
            //Formula
            box1.velocity = b1v*(box1.mass-box2.mass)/(box1.mass+box2.mass)+
                            b2v*(2*box2.mass)/(box1.mass+box2.mass);

            box2.velocity = b1v*(2*box1.mass)/(box1.mass+box2.mass)+
                            b2v*(box2.mass-box1.mass)/(box1.mass+box2.mass);
            //Updating the text
            collisionTimes += 1;
            ss << "Collision Times: " << collisionTimes;
            uiText.setString(ss.str());
            ss.str(""); //Clear the stringstream
            
            if (box1.velocity > 0.f && box2.velocity > 0.f && box1.velocity <= box2.velocity) // detect the ending condition
            {
                ss << "Collision Times: " << collisionTimes << std::endl
                    << "Box2 is faster than Box1, press <Space> to see it again.";
                uiText.setString(ss.str());
                ss.str(""); //clear the stringstream
            }
        }

        //Update
        box1.update();
        box2.update();

        //Render
        window.clear();
        window.draw(box1.rect);
        window.draw(box2.rect);
        window.draw(uiText);
        window.display();
    }

    return 0;
}