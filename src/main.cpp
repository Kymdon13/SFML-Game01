#include <iostream>
#include "Engine.h"

int main()
{
    //Init srand
    std::srand(static_cast<unsigned>(time(NULL)));

    //Init Game Engine
    Engine engine;

    //Game loop
    while(engine.running()) {
        //Update
        engine.update();

        //Rendering
        engine.render();
    }
    
    return 0;
}