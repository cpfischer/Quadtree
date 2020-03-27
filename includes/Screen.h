#ifndef SCREEN_H
#define SCREEN_H
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"


inline void drawScreen(Surface & surface)
{
    surface.lock();
    surface.fill(BLACK);
}


inline void runScreen(Surface & surface)
{
    surface.unlock();
    surface.flip();
    delay(10); 
}


inline void closeGame(Event & event, bool * quitGame)
{
    if (event.poll() && event.type() == QUIT)
    {
        *quitGame = 1;
    }
}

#endif
