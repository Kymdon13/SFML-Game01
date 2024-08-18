#include "Engine.h"

//Private functions
void Engine::initVariables()
{
    this->window = nullptr;
    
    //Init and set the color of the ray
    this->ray = new sf::VertexArray(sf::Lines, 2);
    (*this->ray)[0].color = sf::Color::White; // Start point color
    (*this->ray)[1].color = sf::Color::Green; // End point color
    this->startPoint = sf::Vector2f(640.f, 360.f);

    //Game logic
    this->health = 10;
    this->points = 0;
    this->mouseHeld = false;
}

void Engine::initWindow()
{
    this->videoMode.height = 720;
    this->videoMode.width = 1280;

    this->window = new sf::RenderWindow(this->videoMode, "Ray collision", sf::Style::Titlebar | sf::Style::Close);
    
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
}

//Constructors | Destructors
Engine::Engine()
{
    this->initVariables();
    this->initWindow();
    this->initFonts();
    this->initText();
}

Engine::~Engine()
{
    delete this->window;
    delete this->ray;
}

//Accessors
const bool Engine::running() const
{
    return this->window->isOpen();
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
            case sf::Event::MouseButtonPressed:
                if (this->ev.mouseButton.button == sf::Mouse::Left)
                {
                    // Set the starting point of the ray to the mouse position
                    this->startPoint = this->mousePosView;
                }
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

void Engine::update()
{
    this->pollEvents();
    
    //Update the Text(always update the text no matter if game is over)
    this->updateText();
    
    //Update the mouse position
    this->updateMousePosition();

    // Update the position of the ray
    this->endPoint = this->mousePosView;
    (*this->ray)[0].position = this->startPoint;
    (*this->ray)[1].position = this->endPoint;
}

void Engine::renderText(sf::RenderTarget &target)
{
    target.draw(this->uiText);
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
    
    //Render text
    this->renderText(*this->window);
    
    //Render the ray
    this->window->draw(*this->ray);

    this->window->display();
}
