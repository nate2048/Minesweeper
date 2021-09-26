#pragma once
#include "tiles.h"
#include "userInterface.h"
#include "random.h"
#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>

struct board {

    board(sf::RenderWindow& window, int rows, int columns, unsigned int mines);
    void drawBoard(sf::RenderWindow& window);
    void revealTile(sf::Vector2i mousePos, sf::RenderWindow& window);
    void addFlag(sf::Vector2i mousePos);
    void addNumbers(sf::RenderWindow& window);
    string indexShift(int i, int j, int yShift, int xShift);
    bool newMine(vector<string> mineList, string newMine);
    void recursiveReveal(tiles* baseTile,sf::RenderWindow& window);
    void minesRemaining();

private:

    void generateBoard(sf::RenderWindow& window, int loadSelect);
    void fileMines(int i);
    void randomMines();
    string mouseIndex(int x, int y);
    unordered_map<string, tiles> realTiles, flagMap, mineMap, numMinesMap;
    int numRows;
    int numColumns;
    int numMinesNotFlagged;
    int tilesRevealed;
    unsigned int numMines, numMinesStart;
    userInterface face, debug, test1, test2, test3, digit1, digit2, digit3, digit4;
    float middle, bottom;
    bool canPlay;
    bool showMines;
    bool negative;
    unordered_map<int, sf::Texture> digits;

};