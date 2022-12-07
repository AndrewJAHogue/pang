#include "SFML/Graphics.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace sf;

int main(){
    RenderWindow window;

    window.create(VideoMode(800, 600), "WHOAHHAH");

    window.setKeyRepeatEnabled(false);

    std::unordered_map<int, bool> keys;
    std::vector<int> changedkeys;

    while (window.isOpen()){
        Event event;
        while (window.pollEvent(event)){
          if (event.type == Event::Closed) {
            window.close();
          }

          if (event.type == Event::KeyPressed){
            keys[event.key.code] = 0;
            std::cout << "stupido hehehthtehe" << std::endl;
          }
          if (event.type == Event::KeyReleased){
            keys.erase(event.key.code);
          }

        }
        


    }

    window.clear();
    window.display();

    return 0;
}