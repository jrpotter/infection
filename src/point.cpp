#include "point.hpp"

// Constructors/Destructor
// ==========================================================

Point::Point()
{
    Point(0, 0, 0, nullptr);
}

Point::Point(double x, double y, double radius, Student *node)
      :x(x), y(y), student(node), edges({}), circle(radius)
{
    if(!labelFont.loadFromFile(ROBOTO_MEDIUM_FONT)) {
        cerr << "Could not load font file!" << endl;
    }
}


// Constructors/Destructor
// ==========================================================

void Point::addPoint(Point *point)
{
    edges.push_back(point);
}

sf::Color Point::getColor()
{
    // We generate a distribution of color based on the version
    // number. Ideally these colors will not clash with similar
    // version colors (unfortunately Z mod 256 is not a group and
    // as such the primes are not cyclic generators of the non-group)
    if(student != nullptr) {
        int red = (37 * student->version) % 256;
        int blue = (93 * student->version) % 256;
        int green = (153 * student->version) % 256;
        return sf::Color(red, green, blue);
    }

    return sf::Color(0, 0, 0);
}

// We break this up by edges and points so the points
// can be drawn on top of all edges. Note the edges
// should be drawn in the same color as the point
void Point::drawEdges(sf::RenderWindow *window)
{
    // Draw the basic connections
    for(int i = 0; i < edges.size(); i++) {
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(x, y)),
            sf::Vertex(sf::Vector2f(edges[i]->x, edges[i]->y))
        };
        window->draw(line, 2, sf::Lines);
    }
}

// Draw the points on top (so make sure this function is
// called lastly)
void Point::drawPoint(sf::RenderWindow *window)
{
    circle.setPosition(x - circle.getRadius(), y - circle.getRadius());
    circle.setFillColor(getColor());
    window->draw(circle);

    sf::Text label(to_string(student->id), labelFont);
    sf::FloatRect bounds = label.getGlobalBounds();
    int labelX = x - (bounds.width / 2);
    int labelY = y - (bounds.height / 2);

    label.setPosition(labelX, labelY);
    label.setColor(sf::Color::White);
    window->draw(label);
}

