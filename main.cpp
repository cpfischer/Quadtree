/****************************************************************************
 Yihsiang Liow
 Copyright
 ****************************************************************************/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "circle.h"
#include "src/tree.cpp"
#include "Screen.h"

int main(int argc, char* argv[])
{
    Event event;
    Surface surface(W, H);

    int keyDelay = 10;
    bool isTree = 1;
    std::vector< Circle* > c;
    for (int i = 0; i < CIRCLES; ++i)
    {
        //Circle cobj;
        c.push_back(new Circle);
    }
    c.shrink_to_fit();

    while (1)
    {
        if (event.poll() && event.type() == QUIT) break;

        KeyPressed keypressed = get_keypressed();
        if (keypressed[SDLK_q] && keyDelay == 0)
        {
            isTree = !isTree;
            keyDelay = 10;
        }

        if (keyDelay > 0) --keyDelay;
        
        for (int i = 0; i < CIRCLES; ++i)
        {
            c[i]->move();
        }

        if (isTree)
        {
            Tree * qt = new Tree(c);
            drawScreen(surface);
            for (int i = 0; i < CIRCLES; ++i)
            {
                c[i]->draw_circle(surface);
            }
            
            qt->drawNodes(surface, qt->getTop());
            runScreen(surface);
            delete qt;
            
        }
        else if (!isTree)
        {
            for (int i = 0; i < CIRCLES; ++i)
            {
                for (int j = i + 1; j < CIRCLES; ++j)
                {
                    c[i]->checkCollision(*(c[j]));
                }
            }
            drawScreen(surface);
            for (int i = 0; i < CIRCLES; ++i)
            {
                c[i]->draw_circle(surface);
            }
            
            runScreen(surface);
        }
    }

    for (int i = 0; i < CIRCLES; ++i)
    {
        delete c[i];
    }
    //test_event();
    //test_pixel();
    //test_line();
    //test_unfilled_circle();
    //test_rect();
    //test_image();
    //helloworld();			// Of course we must have a hello world right?
    //test_keyboard();
    //test_key_up_down(); // NEW 2013
    //test_sound();
    //fancyhelloworld();		// eye candy
    //test_polygon(false);
    //test_polygon(true);
    
    // mouse
    //test_mouse(); // NEW 2013
    //test_mouse_event(); // NEW 2013
    //test_drag_n_drop(); // NEW 2013

    // gamepad
    //test_gamepad(); // NEW 2013
    
    return 0;
}


