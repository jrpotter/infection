#include "window.hpp"

// Constructors/Destructor
// ==========================================================

Window::Window(Graph *network, Graph *metaNetwork)
       :network(network)
       ,metaNetwork(metaNetwork)
{
    // Global settings; this must be established before the window
    // is created, and changing afterward will not affect the 
    // rendering without reinitializing the window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 30;
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), 
        "Khan Academy Demo", sf::Style::Default, settings);

    // For use in student names
    if(!nameFont.loadFromFile(NEOTERIC_FONT)) {
        cerr << "Could not load font file!" << endl;
    }
}

Window::~Window()
{
    delete window;
}


// Utility Methods
// ==========================================================

double* Window::getGridFactors(sf::Vector2u dist)
{
    int n = network->nodes.size();
    double *factors = new double[4]{0, 0, 0, 0};

    // When distributing the nodes, we can imagine a gridlike
    // overlay on the window, in which all intersections are
    // where the points are placed. The offset determines how
    // many additional nodes could not fit within the given
    // factorization of n, and an additional row is created
    // to accomodate them
    factors[OFFSET] = n - (dist.x * dist.y);

    // Here we determine the spacing of the points, making sure
    // to pad the distribution on both sides to center all points
    // as well as adding an additional row if necessary
    factors[DELTA_X] = window->getSize().x / (dist.x + 1);
    factors[DELTA_Y] = window->getSize().y / (dist.y + 1 + (factors[OFFSET] > 0 ? 1 : 0));
    factors[RADIUS] = 0.25f * min(factors[DELTA_X], factors[DELTA_Y]);

    return factors;
}

// Try and place all points evenly throughout given window space
sf::Vector2u Window::getDistribution()
{
    int n = network->nodes.size();

    // We perform simple trial division until we obtain a nonprime
    // number, to which we can then try and distribute evenly
    bool isPrime = true;
    for(; !isPrime; n--) {
        if(n <= 3) {
            isPrime = n > 1;
        } else if(n % 2 == 0 || n % 3 == 0) {
            isPrime = false;
        } else {
            for(int i = 5; i * i <= n; i += 6) {
                if(n % i == 0 || n % (i + 2) == 0) {
                    isPrime = false;
                    break;
                }
            }
        }
    }

    // Determine distribution
    // We continue working downward until we have some
    // factor that can break up the given n fairly evenly.
    // We start from the sqrt to try and be as even as possible
    int hcCount = 1;
    for(int i = sqrt(n); i > 1; i--) {
        if(n % i == 0) {
            hcCount = i;
            break;
        } 
    }

    return sf::Vector2u(hcCount, n / hcCount);
}

// Actually define the points and the connections between them, as well
// as their position relative to each other
vector<Point*> Window::generatePoints(sf::Vector2u dist)
{
    // Collect variables
    vector<Point*> points;
    int n = network->nodes.size();
    double *factors = getGridFactors(dist);

    // Here we define each point's new position
    for(int i = 0; i < n; i++) {

        // Determine Position
        int row = i / dist.x;
        int col = i % dist.x;
        int x = (col + 1) * factors[DELTA_X];
        int y = (row + 1) * factors[DELTA_Y];

        // We save the following information to
        // later reverse the calculation and determine
        // how all nodes connect to one another
        eItr itr = network->nodes.begin();
        advance(itr, i);
        
        Student *current = static_cast<Student*>(itr->second);
        current->index = x;
        current->low_index = y;

        // Generate Point
        points.push_back(new Point(x, y, factors[RADIUS], current));
    }

    // Next we go through a second time, and connect
    // all points together
    for(int i = 0; i < points.size(); i++) {
        Node *current = points[i]->student;
        eItr nItr = current->edges.begin();
        for(; nItr != current->edges.end(); nItr++) {
            int x = nItr->second->index;
            int y = nItr->second->low_index;
            int col = (x / factors[DELTA_X]) - 1;
            int row = (y / factors[DELTA_Y]) - 1;
            points[i]->addPoint(points[(row * dist.x) + col]);
        }
    }

    // Allow for repeated calls
    network->reset();
    delete[] factors;

    return points;
}


// Main Method
// ==========================================================

void Window::run()
{
    // Setup Points
    sf::Vector2u dist = getDistribution();
    double *factors = getGridFactors(dist);
    vector<Point*> points = generatePoints(dist);

    // Main Loop
    while(window->isOpen()) {

        sf::Event event;
        while(window->pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window->close();
            }
        }

        // Currently selected user
        Student *activeUser = nullptr;

        // Mouse Coordinates
        // We want to allow the user to mouse over a position, and notify
        // what node is being moused over. In addition, if a node is selected,
        // we allow an infection to occur.
        sf::Vector2i pos = sf::Mouse::getPosition(*window);

        // Find closest grid intersection (points where circles are placed)
        int frontX = pos.x - (pos.x % (int) factors[DELTA_X]);
        int frontY = pos.y - (pos.y % (int) factors[DELTA_Y]);
        int backX = frontX + (int) factors[DELTA_X];
        int backY = frontY + (int) factors[DELTA_Y];

        // Place actual coordinate positions
        int x_coor = (abs(pos.x - backX) < abs(pos.x - frontX)) ? backX : frontX;
        int y_coor = (abs(pos.y - backY) < abs(pos.y - frontY)) ? backY : frontY;

        // Get current point if mouse is over it
        int offset = -1;
        if(sqrt(pow(pos.x - x_coor, 2) + pow(pos.y - y_coor, 2)) <= factors[RADIUS]) {
            int col = (x_coor / factors[DELTA_X]) - 1;
            int row = (y_coor / factors[DELTA_Y]) - 1;
            offset = (row * dist.x) + col;
        }

        // Mouse Events
        // Allow for interacting with the network. When the user selects a node, 
        // we simulate a total infection, relaying the version of the given node, 
        // and then resetting the color of the nodes revised
        // Determine whether or not the user clicked inside a given point
        // If so, begin a total infection from the selected point
        if(offset > -1 && offset < points.size()) {
            activeUser = static_cast<Student*>(points[offset]->student);
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                Infection infection(network, metaNetwork);
                infection.total_infection(activeUser);
            }
        }

        // Draw out onto the window
        // We must ensure we first draw edges, then points, then names
        window->clear();
        for(int i = 0; i < points.size(); i++) {
            points[i]->drawEdges(window);
        }
        for(int i = 0; i < points.size(); i++) {
            points[i]->drawPoint(window);
        }
        if(activeUser != nullptr) {
            sf::Text nodeName(activeUser->name, nameFont);
            nodeName.setColor(sf::Color::White);
            nodeName.setPosition(10, 0);
            window->draw(nodeName);
        }
        window->display();
    }

    delete[] factors;
}
