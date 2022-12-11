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

// bool isTouching(Vector2i mouse, Vector2i rect){
//   float mx = mouse.x;
//   float my = mouse.y;

//   float rx = rect.x;
//   float ry = rect.y;
//   if ((mx >= rx && mx <= (rx + RECT_SIDE)) && (my >= ry && my <= (ry +
//   RECT_SIDE))) {
//     return true;
//   }
//   else{
//     return false;
//   }
// }

// std::shared_ptr<sNode> nodeStart = nullptr;
// std::shared_ptr<sNode> nodeEnd = nullptr;
sNode *nodeStart = nullptr;
sNode *nodeEnd = nullptr;

Color blue_half = Color(Color::Blue.toInteger() / 2);

std::vector<sNode> squares;
std::vector<RectangleShape> bridges;
std::vector<RectangleShape> paths;

bool Solve_AStar();

// void changePtr(Node** ptr_ptr, Node *parent){
//   *ptr_ptr = parent;
// }

int main() {
  RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SQUARES!!!!");

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

  // nodeStart = std::make_shared<sNode>( &squares[30] );
  // nodeEnd = std::make_shared<sNode>( &squares[26] );

  // nodeStart->shape.setFillColor(Color::Green);
  // nodeEnd->shape.setFillColor(Color::Yellow);

  nodeStart = &squares[30];
  nodeEnd = &squares[26];

  sNode *ptr = &squares[27];
  nodeEnd->setParent(ptr) ;
  std::cout << "pointer 26 " << squares[26].parent << std::endl;

  ptr = &squares[28];
  squares[27].setParent(ptr) ;

  ptr = &squares[29];
  squares[28].setParent(ptr) ;

  ptr = &squares[30];
  squares[29].setParent(ptr) ;
  std::cout << "pointer 30 " << squares[30].parent << std::endl;
  

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      // if (event.type == sf::Event::MouseButtonReleased) {
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
            // double angle = tan(dy/dx); // radians
            dx = abs(dx);
            dy = abs(dy);

            RectangleShape line(Vector2f(10, 20));
            line.setPosition(Vector2f(xa, ya));
            line.setFillColor(Color::Red);
            // line.setRotation(angle * 180 / 3.14); // angle in degrees

            paths.push_back(line);

            // Node parn = *p->parent;
            // Node **ptr = &p;
            // changePtr(ptr, p->parent);

            p = p->parent;
            // parent = p->parent;
          }
        }
      // }
    }

    window.clear();

    for (auto &s : squares) {
      // sNode s = squares[i];
      // RectangleShape n(Vector2f(200, 200));
      // std::cout << squares[i].shape << std::endl;
      // s.setFillColor(Color::Red);
      window.draw(s);
    }

    for (auto &l : paths) {
      window.draw(l);
    }

    window.display();

  }
    return 0;
}

// bool Solve_AStar()
// 	{
//     paths.clear();

//     std::cout << "Running AStar" << std::endl;
// 		// Reset Navigation Graph - default all node states
// 		for (auto n : squares)
// 			{
//         n.visited = false;
//         n.globalGoal = INFINITY;
//         n.localGoal = INFINITY;
//         n.parent = nullptr;
// 			}

// 		auto distance = [](Node* a, Node* b) // For convenience
// 		{
// 			return sqrtf((a->xpos - b->xpos)*(a->xpos - b->xpos) +
// (a->ypos - b->ypos)*(a->ypos - b->ypos));
// 		};

// 		auto heuristic = [distance](Node* a, Node* b) // So we can
// experiment with heuristic
// 		{
// 			return distance(a, b);
// 		};

// 		// Setup starting conditions
// 		Node *nodeCurrent = nodeStart;
// 		nodeCurrent->localGoal = 0.0f;
// 		nodeCurrent->globalGoal = heuristic(nodeStart, nodeEnd);
//     std::cout << "Heuristic " << nodeStart->globalGoal << std::endl;

// 		// Add start node to not tested list - this will ensure it gets
// tested.
// 		// As the algorithm progresses, newly discovered nodes get added
// to this
// 		// list, and will themselves be tested later
// 		std::list<Node*> listNotTestedNodes;
// 		listNotTestedNodes.push_back(nodeStart);

// 		// if the not tested list contains nodes, there may be better
// paths
// 		// which have not yet been explored. However, we will also stop
// 		// searching when we reach the target - there may well be better
// 		// paths but this one will do - it wont be the longest.
// 		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)//
// Find absolutely shortest path // && nodeCurrent != nodeEnd)
// 		{
// 			// Sort Untested nodes by global goal, so lowest is
// first 			listNotTestedNodes.sort([](const Node* lhs, const
// Node* rhs){ return lhs->globalGoal < rhs->globalGoal; } );

// 			// Front of listNotTestedNodes is potentially the lowest
// distance node. Our
// 			// list may also contain nodes that have been visited,
// so ditch these... 			while(!listNotTestedNodes.empty() &&
// listNotTestedNodes.front()->visited)
// listNotTestedNodes.pop_front();

// 			// ...or abort because there are no valid nodes left to
// test 			if (listNotTestedNodes.empty())
// break;

// 			nodeCurrent = listNotTestedNodes.front();
// 			nodeCurrent->visited = true; // We only explore a node
// once

// 			// Check each of this node's neighbours...
// 			for (auto nodeNeighbour : nodeCurrent->vNeighbors)
// 			{
// 				// ... and only if the neighbour is not visited
// and is
// 				// not an obstacle, add it to NotTested List
// 				if (!nodeNeighbour->visited &&
// nodeNeighbour->obstacle
// == 0)
// listNotTestedNodes.push_back(nodeNeighbour);

// 				// Calculate the neighbours potential lowest
// parent distance 				float fPossiblyLowerGoal =
// nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

// 				// If choosing to path through this node is a
// lower distance than what
// 				// the neighbour currently has set, update the
// neighbour to use this node
// 				// as the path source, and set its distance
// scores as necessary 				if (fPossiblyLowerGoal <
// nodeNeighbour->localGoal)
// 				{
// 					nodeNeighbour->parent = nodeCurrent;
// 					nodeNeighbour->localGoal =
// fPossiblyLowerGoal;

// 					// The best path length to the neighbour
// being tested has changed, so
// 					// update the neighbour's score. The
// heuristic is used to globally bias
// 					// the path algorithm, so it knows if
// its getting better or worse. At some
// 					// point the algo will realise this path
// is worse and abandon it, and then go
// 					// and search along the next best path.
// 					nodeNeighbour->globalGoal =
// nodeNeighbour->localGoal
// + heuristic(nodeNeighbour, nodeEnd);
// 				}
// 			}
// 		}

// 		return true;
// 	}