#include <SFML/Graphics/RectangleShape.hpp>
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
#include <functional>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

using namespace sf;

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;

const int NUM_OF_CIRCS = 2;
const int CIRC_DIAMETER = SCREEN_WIDTH / NUM_OF_CIRCS;
const int RECT_SIDE = 20;
const int BUFFER = 100;

class Node :  public RectangleShape{
  private:

  public:
    double xpos;
    double ypos;
    bool active;
    Node() : RectangleShape(Vector2f(RECT_SIDE, RECT_SIDE)){
      active = false;
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

int main(){
    RenderWindow window;

    window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "WHOAHHAH");

    Event event;
    std::vector<Node> squares;
    std::vector<RectangleShape> bridges;
    // std::vector<RectangleShape> squares;

    // create nodes
    for (int x = BUFFER; x < SCREEN_WIDTH - BUFFER; x+=RECT_SIDE*2) {
      for (int y = BUFFER; y < SCREEN_HEIGHT - BUFFER ; y+=RECT_SIDE*2) {
        Node square;
        square.setFillColor(Color::Red);
        
        square.setPosition(x, y);
        square.xpos = x;
        square.ypos = y;
        squares.push_back(square);
      }
    }

    // create interconnects
    for (const auto &s : squares){
        float x = s.xpos;
        float y = s.ypos;

        RectangleShape bridge(Vector2f(20, 10));
        if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT){
          x += RECT_SIDE ;
          // y += RECT_SIDE ;
        }


        bridge.setPosition(Vector2f(x, y));
        int col = Color::Blue.toInteger() / 2;
        bridge.setFillColor(Color(col));
        bridges.push_back(bridge);
    };

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
            std::cout << mouse.x << ' ' << mouse.y << std::endl;

            // iterate over all squares to find the one the mouse pressed
            for (int i = 0; i < squares.size(); i++){
              Node &s = squares[i];
              Vector2i s_pos = Vector2i(s.xpos, s.ypos);
              if (isTouching(mouse, s_pos)) {

                  s.active = !s.active;
                if (s.active) {
                  s.setFillColor(Color::Blue);
                } else {
                  s.setFillColor(Color::Red);
                }
                }
            };
        }
      }


      window.clear();

      // drawing the squares
      for (const auto &b : bridges){
        window.draw(b);
      }
      for (int i = 0; i < squares.size(); i++) {
        Node &v = squares[i];
        // window.draw(v);
      }
      window.display();
    }

    return 0;
}