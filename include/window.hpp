/**
 * Draws infections dynamically, given the provided graph.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <vector>
using namespace std;

#include <cmath>
#include <SFML/Graphics.hpp>

#include "graph.hpp"
#include "point.hpp"
#include "infection.hpp"

#define WINDOW_WIDTH    500
#define WINDOW_HEIGHT   500

#define OFFSET          0
#define DELTA_X         1
#define DELTA_Y         2
#define RADIUS          3

class Window
{
    public:
        // Note this graph should be of Students!
        Window(Graph *network, Graph *metaNetwork);
        ~Window();
        void run();
    
    private:
        Graph *network;
        Graph *metaNetwork;

        sf::Font nameFont;
        sf::RenderWindow *window;

        // Determines how nodes should be distributed out
        // onto the screen to the most even possible. If
        // the number of nodes is prime, we reduce down to
        // a composite value. Note the return value's x * y
        // does not necessarily equal the number of nodes
        // in the graph; it is merely a suggested distribution
        sf::Vector2u getDistribution();

        // Given the network passed initially to the window, 
        // we do our best to place a series of points on the
        // given window based on the distribution determined
        // above.
        vector<Point*> generatePoints(sf::Vector2u dist);

        // Convenience function to get certain factors in between
        // function calls. Grid indice values correspond to definitions
        // above
        double* getGridFactors(sf::Vector2u dist);
};

#endif
