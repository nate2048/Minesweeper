#include <iostream>
#include "tiles.h"
#include "board.h"
#include "TextureManager.h"
#include <fstream>

int main()
{

    unsigned int windowSize, mineCount, tileCount;
    int numColumns, numRows;
    int width, height;

    ifstream boardStats("../boards/config.cfg");
    if(boardStats.is_open()){
        boardStats >> numColumns;
        boardStats >> numRows;
        tileCount = numColumns * numRows;
        width = numColumns * 32;
        height = (numRows * 32) + 88;
        windowSize = width * height;
        boardStats >> mineCount;
    }

    sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
    board newBoard(window, numRows, numColumns, mineCount);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed){
                if(event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    newBoard.revealTile(mousePos, window);
                }
                if(event.mouseButton.button == sf::Mouse::Right){
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    newBoard.addFlag(mousePos);
                }
            }
        }

        window.clear();
        newBoard.drawBoard(window);
        window.display();
    }

    return 0;
}