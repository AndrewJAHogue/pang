#include "SFML/Graphics.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <iostream>
#include <list>
#include <math.h>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace sf;

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;

const int NUM_OF_CIRCS = 2;
const int CIRC_DIAMETER = SCREEN_WIDTH / NUM_OF_CIRCS;
const int RECT_SIDE = 20;
const int BUFFER = 50;

class sNode : public RectangleShape {
  public:
    int start_end_none = 3;
    bool obstacle = false;
    bool visited = false;
    float globalGoal;
    float localGoal;
    int x;
    int y;
    std::vector<sNode *> vNeighbors;
    sNode *parent = nullptr;

    sNode() : RectangleShape(Vector2f(RECT_SIDE, RECT_SIDE)){};

    void setParent(sNode *&newNode){
        this->parent = newNode;
    }
};



// struct sNode {
//   bool obstacle = false;
//   bool visited = false;
//   float globalGoal;
//   float localGoal;
//   int x;
//   int y;
//   RectangleShape shape;
//   std::vector<sNode *> vNeighbors;
//   std::vector<sNode > vParents;
//   sNode *parent;
// };

bool isTouching(Vector2i mouse, Vector2i rect){
  float mx = mouse.x;
  float my = mouse.y;

  float rx = rect.x;
  float ry = rect.y;
  if ((mx >= rx && mx <= (rx + RECT_SIDE)) && (my >= ry && my <= (ry +
  RECT_SIDE))) {
    return true;
  }
  else{
    return false;
  }
}

// std::shared_ptr<sNode> nodeStart = nullptr;
// std::shared_ptr<sNode> nodeEnd = nullptr;
sNode *nodeStart = nullptr;
sNode *nodeEnd = nullptr;

Color blue_half = Color(Color::Blue.toInteger() / 2);

std::vector<sNode> squares;
std::vector<RectangleShape> bridges;
std::vector<RectangleShape> paths;

bool Solve_AStar();

double deg2rad(double degrees){
  return 3.14 * degrees / 180;
};

double rad2deg(double radians){
  return radians * 180 / 3.14;
};


int num_rect_y = ( SCREEN_HEIGHT - BUFFER ) / (2* RECT_SIDE );
int num_rect_x = ( SCREEN_WIDTH - BUFFER ) / (2* RECT_SIDE );

int main() {
  RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SQUARES!!!!");
  std::cout << num_rect_y << std::endl;
  std::cout << num_rect_x
   << std::endl
   << num_rect_x * num_rect_y
   << std::endl;

  // create map

  // for (int x = BUFFER; x < SCREEN_WIDTH - BUFFER; x += RECT_SIDE * 2) {
  //   for (int y = BUFFER; y < SCREEN_HEIGHT- BUFFER; y += RECT_SIDE * 2) {
  for (int x = BUFFER; x < SCREEN_WIDTH - BUFFER; x += RECT_SIDE * 2) {
    for (int y = BUFFER; y < SCREEN_HEIGHT- BUFFER; y += RECT_SIDE * 2) {
      sNode s;
      s.x = x;
      s.y = y;

      s.setPosition(Vector2f(x, y));
      s.setFillColor(Color::Blue);

      squares.push_back(s);
    }
  }


// add neighbors together
  for (int y = 0; y < squares.size(); y++){
    sNode &s = squares[y];
    int colIndex = y / num_rect_y;
    int rowIndex = (y + 1) % num_rect_y;
    // std::cout << "y " << y + 1 << " % " << (y + 1) % num_rect_y<< std::endl;

    if (colIndex == 0){
      // bottom
      s.vNeighbors.push_back(&squares[y + 1]);
      // right
      s.vNeighbors.push_back(&squares[y + num_rect_y]);
      if (y != 0){
        // up 
        s.vNeighbors.push_back(&squares[y - 1]);
      }
    }

    // for (int x = 1; x < rowIndex; x++){
    //   if (colIndex != 0 && colIndex != num_rect_x - 1) {
    //     // s.setFillColor(Color::Yellow);
    //     // s.vNeighbors.push_back(&squares[x*num_rect_y ]);
    //     // s.vNeighbors.push_back(&squares[x*num_rect_y - 1]);
    //     // // s.vNeighbors.push_back(&squares[x + 1]);
    //     // s.vNeighbors.push_back(&squares[x*num_rect_x + y]);

    //   }
    // }


    // far right column
    if (colIndex == num_rect_x - 1){
        // s.setFillColor(Color::Red);
        // below first of the column
        s.vNeighbors.push_back(&squares[y - num_rect_y]);
        s.vNeighbors.push_back(&squares[y - 1]);
    }


    // exclude first and last rows
    if (y % num_rect_y != 0 && y % num_rect_y != num_rect_y - 1 && y != 0 
      // exclude first and last columns
      && (colIndex != 0 && colIndex != num_rect_x - 1)) {
      // s.setFillColor(Color::Green);

      // right 
      s.vNeighbors.push_back(&squares[y + num_rect_y]);
      // left 
      s.vNeighbors.push_back(&squares[y - num_rect_y]);
      // up 
      s.vNeighbors.push_back(&squares[y - 1]);
      // down 
      s.vNeighbors.push_back(&squares[y + 1]);
    }
  }

// initial conditions; arbitrary
  nodeStart = &squares[30];
  nodeEnd = &squares[26];
  // squares[0].setFillColor(Color::Cyan);
  // squares.back().setFillColor(Color::Magenta);

  nodeStart->setFillColor(Color::Green);
  nodeEnd->setFillColor(Color::Yellow);

  // nodeStart->start_end_none = 1;
  // nodeEnd->start_end_none = 2;

// debugs
  // sNode *ptr = &squares[27];
  // nodeEnd->setParent(ptr) ;
  // std::cout << "pointer 26 " << squares[26].parent << std::endl;

  // ptr = &squares[28];
  // squares[27].setParent(ptr) ;

  // ptr = &squares[29];
  // squares[28].setParent(ptr) ;

  // ptr = &squares[30];
  // squares[29].setParent(ptr) ;
  // std::cout << "pointer 30 " << squares[30].parent << std::endl;

  // ptr = & squares[]
  
  //  Solve_AStar();

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::MouseButtonReleased) {
        // mouse event
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (auto &s : squares) {
          Vector2i sPos = Vector2i(s.x, s.y);
          if (isTouching(mousePos, sPos)) {
            // set new start node
              // toggle an obstacle
            if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) &&
                !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) &&
                s.start_end_none == 3) {
              s.obstacle = !s.obstacle;

              if (s.obstacle)
                s.setFillColor(Color::Red);
              else
                s.setFillColor(Color::Blue);
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
              s.setFillColor(Color::Green);
              s.start_end_none = 1;

              // revert the original nodeStart
              nodeStart->setFillColor(Color::Blue);
              nodeStart->start_end_none = 3;

              nodeStart = &s;
            }
            // set new end node
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
              s.setFillColor(Color::Yellow);
              s.start_end_none = 2;

              // revert the original nodeEnd
              nodeEnd->setFillColor(Color::Blue);
              nodeEnd->start_end_none = 3;

              nodeEnd = &s;
            }
          }
        }

        Solve_AStar();
        
        // draw the path
        if (nodeEnd != nullptr) {

          // std::shared_ptr<sNode> &p = nodeEnd;
          sNode *p = nodeEnd;
          while (p->parent != nullptr) {
            std::cout << "while loop running"
             << std::endl
            << "p's parent: " << p->parent
             << std::endl;

            double xa, ya, xb, yb;
            xa = p->x;
            ya = p->y;

            xb = p->parent->x;
            yb = p->parent->y;

            // distance
            double dx = xb - xa;
            double dy = yb - ya;
            double hypotonuse = std::sqrt(dx*dx + dy*dy);
            // angle to rotate
            double angle = sin(dy/hypotonuse); // radians
            angle = rad2deg(angle);
            // dx = abs(dx);
            // dy = abs(dy);


            RectangleShape line(Vector2f(20, 10));
            Vector2f linePos = Vector2f(xa, ya);
            line.setPosition(linePos);
            line.setFillColor(Color::Red);

              int sign = 1;
            if (dx != 0){
              if (dx < 0) {
                sign = -1;
              }

              // line.setSize(Vector2f(20, 10));
              line.setRotation(0);
              Vector2f newPos = linePos + Vector2f(sign*RECT_SIDE, RECT_SIDE / 4);
              line.setPosition(newPos);

            }
            else if (dy != 0){
              if (dy < 0) {
                sign = -1;
              }
              // line.setSize(Vector2f(10, 20));
              Vector2f newPos = linePos + Vector2f(RECT_SIDE/2, sign*RECT_SIDE );
              line.setPosition(newPos);
              line.setRotation(90);
            }


          // debug
          std::cout << "dx: " << dx
           << std::endl
           << "dy: " << dy
           << std::endl
          << "hypot: " << hypotonuse
           << std::endl
           << "angle: " << angle
           << std::endl
           << "current angle: " << line.getRotation()
           << std::endl;

            // line.setRotation(angle); // angle in degrees

            paths.push_back(line);

            p = p->parent;
          }
        }

      }
    }

    window.clear();

    for (auto &s : squares) {
      window.draw(s);
    }

    for (auto &l : paths) {
      window.draw(l);
    }

    window.display();

  }
    return 0;
}

bool Solve_AStar()
	{
    paths.clear();

		// Reset Navigation Graph - default all node states
		for (auto &n : squares)
			{
        n.globalGoal = INFINITY;
        n.localGoal = INFINITY;
        n.visited = false;
        n.parent = nullptr;
			}

		auto distance = [](sNode* a, sNode* b) // For convenience
		{
			return sqrtf((a->x - b->x)*(a->x - b->x) + (a->y - b->y)*(a->y - b->y));
		};

		auto heuristic = [distance](sNode* a, sNode* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		sNode *nodeCurrent = nodeStart;
		nodeStart->localGoal = 0.0f;
		nodeStart->globalGoal= heuristic(nodeStart, nodeEnd);

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		std::list<sNode*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const sNode* lhs, const sNode* rhs){ return lhs->globalGoal < rhs->globalGoal; } );
			
			// Front of listNotTestedNodes is potentially the lowest distance node. Our
			// list may also contain nodes that have been visited, so ditch these...
			while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
				listNotTestedNodes.pop_front();

			// ...or abort because there are no valid nodes left to test
			if (listNotTestedNodes.empty())
				break;

			nodeCurrent = listNotTestedNodes.front();
			nodeCurrent->visited = true; // We only explore a node once
			
					
			// Check each of this node's neighbours...
      // std::cout << "neighbors?" << std::endl;
			for (auto &nodeNeighbour : nodeCurrent->vNeighbors)
			{
        // std::cout << "neighbors yes!!" << std::endl;
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->visited && nodeNeighbour->obstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
        // // std::cout << "Just before possibly lower goal conditional" << std::endl;
        // // std::cout << "neighbor localgoal "
        //  << nodeNeighbour->localGoal
        //  << std::endl
        //  << "possibly lower goal " << fPossiblyLowerGoal
        //  << std::endl;
				if (fPossiblyLowerGoal < nodeNeighbour->localGoal)
				{

          // std::cout << "setting new parent for: " << nodeNeighbour << std::endl;
					nodeNeighbour->parent = nodeCurrent;
          sNode *ptr = nodeCurrent;
          nodeNeighbour->setParent(ptr);
					nodeNeighbour->localGoal = fPossiblyLowerGoal;

					// The best path length to the neighbour being tested has changed, so
					// update the neighbour's score. The heuristic is used to globally bias
					// the path algorithm, so it knows if its getting better or worse. At some
					// point the algo will realise this path is worse and abandon it, and then go
					// and search along the next best path.
					nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd);
				}
			}	
		}

		return true;
	}