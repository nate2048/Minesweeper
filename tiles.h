#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <unordered_map>
#include <iostream>
using namespace std;

class tiles {

public:
    tiles();
    sf::Sprite sprite;
    string index;
    bool revealed;
    bool flagged;
    bool mined;
    int numAdjMines;

    vector<tiles*> adjacentTiles;

};