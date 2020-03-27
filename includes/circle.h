#ifndef CIRCLE_H
#define CIRCLE_H
#include <iostream>
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

class Circle
{
public:
    Circle()
        : r(3), isRepel(0), speed(3), colorR(169), colorG(69), colorB(169),
          changeDelay(10)
    {
        x = rand() % W;
        y = rand() % H;
        dx = rand() % 2 - 1;
        dy = rand() % 2 - 1;

        if (x < 3) x += r * 2;
        if (x > W) x -= r * 2;
        if (y < 3) y += r * 2;
        if (y > W) y -= r * 2;
        
        while (1)
        {
            if (dx == 0 && dy == 0)
            {
                dx = rand() % 2 - 1;
                dy = rand() % 2 - 1;
            }
            else break;
        }
    }

   
    void move();

    void draw_circle(Surface & surface);
    int gx() const;

    int & gx();

    int gy() const;

    int & gy();

    int gr() const;

    int xrPlus() const;
    
    int xrMinus() const;
    
    int yrPlus() const;
    
    int yrMinus() const;

    //if x value is inside of c on the right and circle y is within the diameter
    bool checkRight(const Circle & c) const;

    bool checkTop(const Circle & c) const;

    void changeColor(Circle & c);
    
    void checkCollision(Circle & c);
    
private:
    bool isRepel;
    int changeDelay;
    int speed;
    int dx;
    int dy;
    int r;
    int x;
    int y;
  
    int colorR;
    int colorG;
    int colorB;
};

#endif
