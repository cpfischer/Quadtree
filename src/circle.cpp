/*****************************************************************************
This function, test_circle(), shows you how to draw a circle:

surface.put_circle(x, y, rad, r, g, b);

This draws a circle on surface with center at (x,y), radius r, and with random
color.

Exercise. Modify the program to draw circles with radius 10.
*****************************************************************************/
#include <iostream>
#include "Includes.h"
#include "Constants.h"
#include "circle.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

void Circle::draw_circle(Surface & surface)
{
    surface.put_circle(this->x, this->y, this->r,
                       this->colorR, this->colorG, this->colorB);
    
}


void Circle::move()
{
    bool changed = 0;
    if ((xrPlus() >= W - 1 || xrMinus() <= 1 || isRepel) && changeDelay == 0)
    {
        changed = 1;
        this->dx *= -1;
    }
    
    if ((yrPlus() >= H - 1 || yrMinus() <= 1 || isRepel) && changeDelay == 0)
    {
        changed = 1;
        this->dy *= -1;
    }

    if (changeDelay > 0) --changeDelay;
    if (changed) changeDelay = 10;
    if (isRepel) isRepel = 0;
    
    this->x += this->dx * this->speed;
    this->y += this->dy * this->speed;
}


int Circle::gx() const
{
    return this->x;
}

int & Circle::gx()
{
    return this->x;
}

int Circle::gy() const
{
    return this->y;
}

int & Circle::gy()
{
    return this->y;
}

int Circle::gr() const
{
    return this->r;
}

int Circle::xrPlus() const
{
    return x + r;
}
    
int Circle::xrMinus() const
{
    return x - r;
}
    
int Circle::yrPlus() const
{
    return y + r;
}
    
int Circle::yrMinus() const
{
    return y - r;
}

//if x value is inside of c on the right and circle y is within the diameter
bool Circle::checkRight(const Circle & c) const
{
    return ((this->xrPlus() < c.xrPlus() && this->xrPlus() > c.xrMinus())
            && this->y < c.yrPlus() && this->y > c.yrMinus());
}

bool Circle::checkTop(const Circle & c) const
{
    return ((this->yrPlus() < c.yrPlus() && this->yrPlus() > c.yrMinus())
            && this->x < c.xrPlus() && this->x > c.xrMinus());
}

void Circle::changeColor(Circle & c)
{
    c.colorR = 225;
    c.colorG = 225;
    c.colorB = 5;
}
    
void Circle::checkCollision(Circle & c)
{
    if (!isRepel && changeDelay == 0)
            
    {
        bool collision = checkRight(c) || checkTop(c);
        // std::cout << "HIT! x1 = " << this->x << " x2: " << c.gx() + c.gr()
        //           << " x3: " << c.gx() - c.gr();
        if (collision)
        {
            this->isRepel = 1;
            c.isRepel = 1;
            changeColor(*this);
            changeColor(c);
        }
    }
}
