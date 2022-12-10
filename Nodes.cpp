#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <list>
#include <algorithm>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Mouse.hpp>
#include <cmath>
#include "SFML/Graphics.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cstddef>
#include <functional>
#include <iostream>
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


class Node :  public RectangleShape{
  private:

  public:
    double xpos;
    double ypos;
    bool active = false;
    bool obstacle = false;
    int start_end_none = 3;
    bool visited = false;
    float globalGoal;
    float localGoal;
    // std::vector<Node> vecParent;
    Node *parent;
    std::vector<Node> vNeighbors;

    Node() : RectangleShape(Vector2f(RECT_SIDE, RECT_SIDE)){
    };

    // Node getParent(){
    //   return vecParent[0];
    // }

    // void setParent(Node p){
    //   vecParent.clear();
    //   vecParent.push_back(p);
    // }
    
};



// bool isTouching(Vector2i mouse, Vector2i rect){
//   float mx = mouse.x;
//   float my = mouse.y;

//   float rx = rect.x;
//   float ry = rect.y;
//   if ((mx >= rx && mx <= (rx + RECT_SIDE)) && (my >= ry && my <= (ry + RECT_SIDE))) {
//     return true;
//   }
//   else{
//     return false;
//   }
// }

Node nodeStart;
Node nodeEnd;

Color blue_half = Color(Color::Blue.toInteger() / 2);
std::vector<Node> squares;
std::vector<RectangleShape> bridges;
std::vector<RectangleShape> paths;

bool Solve_AStar();

int main(){
  RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "what");

  // create map

  for (int x = BUFFER; x < SCREEN_HEIGHT * .8 ; x+=RECT_SIDE*2){
    for (int y = BUFFER; y < SCREEN_WIDTH * .8; y+=RECT_SIDE*2) {
        Node s;
        s.setPosition(Vector2f(x, y));
        s.xpos = x; s.ypos = y;
        s.setFillColor(Color::Blue);

        squares.push_back(s);
    }
  }

  // squares[30].parent = &squares[29];
  // squares[29].parent = &squares[28];
  // squares[28].parent = &squares[27];
  // squares[27].parent = &squares[26];

    for (int i = 0; i < 4; i++){
      Node &s = squares[30];
      s.parent = &squares[30 - i];
      // s.setParent(squares[30 - i]);
      // Node parent = s.getParent();
      Node *parent = s.parent;


      double xa, ya, xb, yb;
      xa = s.xpos;
      ya = s.ypos;
      
      xb = parent->xpos; 
      yb = parent->ypos; 


      std::cout << "s " << s.xpos <<  ' ' << s.ypos << std::endl;
      std::cout << "[30-i]: " << squares[30 - i].xpos <<  ' ' << squares[30 - i].ypos << std::endl;
      std::cout << xb << ' ' << yb << std::endl;
    }


  while(window.isOpen()){
    sf::Event event;
    while(window.pollEvent(event)){
      if (event.type == sf::Event::Closed){
        window.close();
      }
    }

    // for (int i = 0; i < 4; i++){
    //   Node &s = squares[30 -i];
    //   Node *parent = s.parent;
    //   int xa, ya, xb, yb;
    //   xa = s.xpos;
    //   ya = s.ypos;
      
    //   xb = parent->xpos; 
    //   yb = parent->ypos; 

    //   std::cout << xb << ' ' << yb << std::endl;
    // }


    window.clear();
    
    for(auto &s : squares){
      window.draw(s);
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
// 			return sqrtf((a->xpos - b->xpos)*(a->xpos - b->xpos) + (a->ypos - b->ypos)*(a->ypos - b->ypos));
// 		};

// 		auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
// 		{
// 			return distance(a, b);
// 		};

// 		// Setup starting conditions
// 		Node *nodeCurrent = nodeStart;
// 		nodeCurrent->localGoal = 0.0f;
// 		nodeCurrent->globalGoal = heuristic(nodeStart, nodeEnd);
//     std::cout << "Heuristic " << nodeStart->globalGoal << std::endl;

// 		// Add start node to not tested list - this will ensure it gets tested.
// 		// As the algorithm progresses, newly discovered nodes get added to this
// 		// list, and will themselves be tested later
// 		std::list<Node*> listNotTestedNodes;
// 		listNotTestedNodes.push_back(nodeStart);

// 		// if the not tested list contains nodes, there may be better paths
// 		// which have not yet been explored. However, we will also stop 
// 		// searching when we reach the target - there may well be better
// 		// paths but this one will do - it wont be the longest.
// 		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
// 		{
// 			// Sort Untested nodes by global goal, so lowest is first
// 			listNotTestedNodes.sort([](const Node* lhs, const Node* rhs){ return lhs->globalGoal < rhs->globalGoal; } );
			
// 			// Front of listNotTestedNodes is potentially the lowest distance node. Our
// 			// list may also contain nodes that have been visited, so ditch these...
// 			while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited)
// 				listNotTestedNodes.pop_front();

// 			// ...or abort because there are no valid nodes left to test
// 			if (listNotTestedNodes.empty())
// 				break;

// 			nodeCurrent = listNotTestedNodes.front();
// 			nodeCurrent->visited = true; // We only explore a node once
			
					
// 			// Check each of this node's neighbours...
// 			for (auto nodeNeighbour : nodeCurrent->vNeighbors)
// 			{
// 				// ... and only if the neighbour is not visited and is 
// 				// not an obstacle, add it to NotTested List
// 				if (!nodeNeighbour->visited && nodeNeighbour->obstacle == 0)
// 					listNotTestedNodes.push_back(nodeNeighbour);

// 				// Calculate the neighbours potential lowest parent distance
// 				float fPossiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

// 				// If choosing to path through this node is a lower distance than what 
// 				// the neighbour currently has set, update the neighbour to use this node
// 				// as the path source, and set its distance scores as necessary
// 				if (fPossiblyLowerGoal < nodeNeighbour->localGoal)
// 				{
// 					nodeNeighbour->parent = nodeCurrent;
// 					nodeNeighbour->localGoal = fPossiblyLowerGoal;

// 					// The best path length to the neighbour being tested has changed, so
// 					// update the neighbour's score. The heuristic is used to globally bias
// 					// the path algorithm, so it knows if its getting better or worse. At some
// 					// point the algo will realise this path is worse and abandon it, and then go
// 					// and search along the next best path.
// 					nodeNeighbour->globalGoal = nodeNeighbour->localGoal + heuristic(nodeNeighbour, nodeEnd);
// 				}
// 			}	
// 		}

// 		return true;
// 	}