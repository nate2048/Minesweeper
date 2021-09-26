#pragma once
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include <string>
#include <unordered_map>
using namespace std;

class userInterface {

public:

    userInterface();
    userInterface(sf::Texture texture);

    sf::Sprite sprite;
};