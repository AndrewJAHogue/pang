#include <SFML/Graphics/RectangleShape.hpp>
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
    Node* parent;
    std::vector<Node*> vNeighbors;

    Node() : RectangleShape(Vector2f(RECT_SIDE, RECT_SIDE)){
    };
    
};



bool isTouching(Vector2i mouse, Vector2i rect){
  float mx = mouse.x;
  float my = mouse.y;

  float rx = rect.x;
  float ry = rect.y;
  if ((mx >= rx && mx <= (rx + RECT_SIDE)) && (my >= ry && my <= (ry + RECT_SIDE))) {
    return true;
  }
  else{
    return false;
  }
}

Node *nodeStart = nullptr;
Node *nodeEnd = nullptr;

Color blue_half = Color(Color::Blue.toInteger() / 2);
std::vector<Node> squares;
std::vector<RectangleShape> bridges;
std::vector<RectangleShape> paths;

bool Solve_AStar();
int main(){
    RenderWindow window;

    window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "WHOAHHAH");

    Event event;
    // std::vector<RectangleShape> squares;

    // create nodes
    for (int x = BUFFER; x < SCREEN_WIDTH - BUFFER ; x+= RECT_SIDE * 2) {
      for (int y = BUFFER; y < SCREEN_HEIGHT- BUFFER; y+=RECT_SIDE * 2) {
        Node square;
        square.setFillColor(Color::Blue);
        square.setSize(Vector2f(RECT_SIDE, RECT_SIDE));
        
        square.setPosition(x, y);
        square.xpos = x;
        square.ypos = y;
        squares.push_back(square);
      }
    }

    // // create interconnects
    // for (int i = 0; i < squares.size(); i++){
    //     Node s = squares[i];
    //     float x = s.xpos;
    //     float y = s.ypos;

    //     if (x >= SCREEN_WIDTH-(2*BUFFER)){
    //       continue;
    //     };

    //     RectangleShape bridge(Vector2f(20, 10));
    //     bridge.setSize(Vector2f(RECT_SIDE * 2, 10));
    //     bridge.setPosition(Vector2f(x, y + (RECT_SIDE / 4)));
    //     bridge.setFillColor(blue_half);
    //     bridges.push_back(bridge);
    // }
    // for (int i = 0; i < squares.size(); i++) {
    //     Node s = squares[i];
    //     float x = s.xpos;
    //     float y = s.ypos;
    //     if (y >= SCREEN_HEIGHT - (2 * BUFFER)) {
    //       continue;
    //     }

    //     RectangleShape bridge1(Vector2f(10, 20));
    //     bridge1.setSize(Vector2f(10, RECT_SIDE * 2));

    //     bridge1.setPosition(Vector2f(x + RECT_SIDE / 4, y + (RECT_SIDE / 4)));
    //     bridge1.setFillColor(blue_half);
    //     bridges.push_back(bridge1);
    // };

    nodeStart = &squares[30];
    nodeStart->setFillColor(Color::Green);
    nodeStart->start_end_none = 1;
    nodeEnd = &squares[32];
    nodeEnd->setFillColor(Color::Yellow);
    nodeEnd->start_end_none = 2;

    Solve_AStar();

    // game loop
    std::unordered_map<int, bool> keys;
    std::vector<int> changedkeys;
    while (window.isOpen()) {
      while (window.pollEvent(event)) {
        // close the window
        if (event.type == Event::Closed) {
          window.close();
        }
        else if (event.type == Event::MouseButtonReleased) {
            Vector2i mouse =  sf::Mouse::getPosition(window);
            // cout the mouse pos

            // iterate over all squares to find the one the mouse pressed
            for (int i = 0; i < squares.size(); i++){
              Node &s = squares[i];
              Vector2i s_pos = Vector2i(s.xpos, s.ypos);


              if (isTouching(mouse, s_pos)) {


                  if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && s.start_end_none == 3) {
                    s.obstacle = !s.obstacle;
                    if (!s.obstacle) {
                      s.setFillColor(Color::Blue);
                    } else {
                      s.setFillColor(Color::Red);
                    }
                  }
                  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && s.start_end_none != 1 ){
                    // s.setFillColor(Color::Green);
                    // s.start_end_none = 1;

                    // nodeStart->start_end_none = 3;
                    // nodeStart->setFillColor(Color::Blue);

                    // nodeStart = &s;


                  }
                  else if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && s.start_end_none != 2){
                    // s.setFillColor(Color::Yellow);
                    // s.start_end_none = 2;

                    // nodeEnd->setFillColor(Color::Blue);
                    // nodeEnd->start_end_none = 3;

                    // nodeEnd = &s;

                    
                  }
            Solve_AStar();
                }
            };


            if (nodeEnd != nullptr) {
                Node *p = nodeEnd;
                int i = 0;
                std::cout << "Just before while loop"
                << std::endl
                << "p's parent : " << p->parent
                 << std::endl;
                while (p->parent != nullptr) {

                std::cout << "Inside loop" << std::endl;

                  RectangleShape line(Vector2f(2*RECT_SIDE, 10));
                  line.setFillColor(Color::Red);
                  int parentX, parentY, currentX, currentY;
                  parentX = p->parent->xpos;
                  parentY = p->parent->ypos;
                  currentX = p->xpos;
                  currentY = p->ypos;

                std::cout << "x: " << currentX << " y: " << currentY << std::endl;
                std::cout << "parent x: " << parentX << " parent y: " << parentY << std::endl;
                  float dx = (parentX - currentX);
                  float dy = (parentY - currentY);
                std::cout << "dx: " << dx << " dy: " << dy << std::endl;

                  float angle = tan(dy/dx);
                  angle = angle * 180 / 3.14;
                std::cout << "angle: " << angle << std::endl;

                  dx = abs(dx); dy = abs(dy);

                  line.setPosition(Vector2f(dx, dy));
                  line.setRotation(angle);
                  std::cout
                  << "index: " << i << std::endl
                   << currentX << ' ' << currentY << std::endl;

                  paths.push_back(line);

                  p = p->parent;
                  i++;
                }
            }
        }
      }

      window.clear();

      for (auto &l : paths){
        window.draw(l);
      }

      for (auto &p : paths){
        window.draw(p);
      }

      // drawing the squares
      for (const auto &b : bridges){
        window.draw(b);
      }

      for (int i = 0; i < squares.size(); i++) {
        Node &v = squares[i];
        window.draw(v);
      }

      window.display();
    }

    return 0;
}


// bool Solve_Astar(){

//   paths.clear();

//   //reset all nodes
//   for(auto n : squares){
//       n.visited = false;
//       n.globalGoal = INFINITY;
//       n.localGoal = INFINITY;
//       n.parent = nullptr;
//   }

//   auto distance = [](Node* a, Node* b){
//     return sqrtf((a->xpos - b->xpos) * (a->xpos - b->xpos) + (a->ypos - b->ypos)*(a->ypos - b->ypos));
//   };

//   auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
//   {
//     return distance(a, b);
//   };

//   Node *nodeCurrent = nodeStart;
//   nodeStart->localGoal = 0.0f;
//   nodeStart->globalGoal = heuristic(nodeStart, nodeEnd);

//   std::list<Node*> listNotTestedNodes;
//   listNotTestedNodes.push_back(nodeStart);

//   while(!listNotTestedNodes.empty() && nodeCurrent != nodeEnd){
//     listNotTestedNodes.sort([](const Node* lhs, const Node* rhs){return lhs->globalGoal < rhs->globalGoal; });
    
//     while(!listNotTestedNodes.empty() && listNotTestedNodes.front()->visited){
//       listNotTestedNodes.pop_front();
//     }

//     if (listNotTestedNodes.empty()){
//       break;
//     }

//     nodeCurrent = listNotTestedNodes.front();
//     nodeCurrent->visited = true;

//     for (auto neighbor : nodeCurrent->vNeighbors){
//       if (!neighbor->visited && neighbor->obstacle == 0){
//         listNotTestedNodes.push_back(neighbor);
//       }

//       float PossiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, neighbor);

//       if (PossiblyLowerGoal < neighbor->localGoal){
//         neighbor->parent = nodeCurrent;
//         neighbor->localGoal = PossiblyLowerGoal;

//         neighbor->globalGoal = neighbor->localGoal + heuristic(neighbor, nodeEnd);
//       }
//     }
//   }

//     return true;

// };
bool Solve_AStar()
	{
    paths.clear();
    
    std::cout << "Running AStar" << std::endl;
		// Reset Navigation Graph - default all node states
		for (auto n : squares)
			{
        n.visited = false;
        n.globalGoal = INFINITY;
        n.localGoal = INFINITY;
        n.parent = nullptr;
			}

		auto distance = [](Node* a, Node* b) // For convenience
		{
			return sqrtf((a->xpos - b->xpos)*(a->xpos - b->xpos) + (a->ypos - b->ypos)*(a->ypos - b->ypos));
		};

		auto heuristic = [distance](Node* a, Node* b) // So we can experiment with heuristic
		{
			return distance(a, b);
		};

		// Setup starting conditions
		Node *nodeCurrent = nodeStart;
		nodeCurrent->localGoal = 0.0f;
		nodeCurrent->globalGoal = heuristic(nodeStart, nodeEnd);
    std::cout << "Heuristic " << nodeStart->globalGoal << std::endl;

		// Add start node to not tested list - this will ensure it gets tested.
		// As the algorithm progresses, newly discovered nodes get added to this
		// list, and will themselves be tested later
		std::list<Node*> listNotTestedNodes;
		listNotTestedNodes.push_back(nodeStart);

		// if the not tested list contains nodes, there may be better paths
		// which have not yet been explored. However, we will also stop 
		// searching when we reach the target - there may well be better
		// paths but this one will do - it wont be the longest.
		while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)// Find absolutely shortest path // && nodeCurrent != nodeEnd)
		{
			// Sort Untested nodes by global goal, so lowest is first
			listNotTestedNodes.sort([](const Node* lhs, const Node* rhs){ return lhs->globalGoal < rhs->globalGoal; } );
			
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
			for (auto nodeNeighbour : nodeCurrent->vNeighbors)
			{
				// ... and only if the neighbour is not visited and is 
				// not an obstacle, add it to NotTested List
				if (!nodeNeighbour->visited && nodeNeighbour->obstacle == 0)
					listNotTestedNodes.push_back(nodeNeighbour);

				// Calculate the neighbours potential lowest parent distance
				float fPossiblyLowerGoal = nodeCurrent->localGoal + distance(nodeCurrent, nodeNeighbour);

				// If choosing to path through this node is a lower distance than what 
				// the neighbour currently has set, update the neighbour to use this node
				// as the path source, and set its distance scores as necessary
				if (fPossiblyLowerGoal < nodeNeighbour->localGoal)
				{
					nodeNeighbour->parent = nodeCurrent;
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

void drawPath(){
  if (nodeEnd != nullptr){
    Node *p = nodeEnd;
    while(p->parent != nullptr){

    }
  }
};