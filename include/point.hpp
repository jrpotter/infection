/**
 *
 * Author: Joshua Potter
 * Date: February 15th, 2015
*/
#ifndef POINT_HPP
#define POINT_HPP

#include <vector>
using namespace std;

#include <SFML/Graphics.hpp>

#include "student.hpp"

#define NEOTERIC_FONT        "assets/neoteric.ttf"
#define ROBOTO_MEDIUM_FONT   "assets/Roboto-Medium.ttf"

class Point
{
    public:
        Point();
        Point(double x, double y, double radius, Student *node);
        void addPoint(Point *point);
        void drawEdges(sf::RenderWindow *window);
        void drawPoint(sf::RenderWindow *window);

        // Make accessible underlying wrapper
        Student *student;
        vector<Point*> edges;

    private:
        double x;
        double y;
        sf::Font labelFont;
        sf::CircleShape circle;

        sf::Color getColor();
};

#endif
