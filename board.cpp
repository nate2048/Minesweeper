#include "board.h"

board::board(sf::RenderWindow& window, int rows, int columns, unsigned int mines) {

    numRows = rows;
    numColumns = columns;
    numMines = mines;
    numMinesStart = numMines;

    generateBoard(window, 0);
}

void board::generateBoard(sf::RenderWindow& window, int loadSelect)
{
    mineMap.clear();
    flagMap.clear();
    realTiles.clear();
    numMinesMap.clear();
    //sets tiles
    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numColumns; j++){
            string index = to_string(i) + "|" + to_string(j);
            realTiles[index].index = index;
            realTiles[index].sprite.setTexture(TextureManager::GetTexture("tile_hidden"));
            realTiles[index].sprite.setPosition(j * 32, i * 32);
        }
    }

    //random Mines or file Mines
    if(loadSelect == 0){
        numMines = numMinesStart;
        randomMines();
    }
    else
        fileMines(loadSelect);

    addNumbers(window);

    numMinesNotFlagged = numMines;

    middle = (float)(numColumns * 32)/ 2;
    bottom = (float)numRows * 32;
    face.sprite.setTexture(TextureManager::GetTexture("face_happy"));
    face.sprite.setPosition(middle, bottom);
    debug.sprite.setTexture(TextureManager::GetTexture("debug"));
    debug.sprite.setPosition(middle + 128.f, bottom);
    test1.sprite.setTexture(TextureManager::GetTexture("test_1"));
    test1.sprite.setPosition(middle + 192.f, bottom);
    test2.sprite.setTexture(TextureManager::GetTexture("test_2"));
    test2.sprite.setPosition(middle + 256.f, bottom);
    test3.sprite.setTexture(TextureManager::GetTexture("test_3"));
    test3.sprite.setPosition(middle + 320.f, bottom);

    for(int i = 0; i < 10; i++){
        digits[i].loadFromFile( "images/digits.png", sf::IntRect(i*21,0,21,32));
    }
    digits[10].loadFromFile("images/digits.png", sf::IntRect(210,0,21,32));
    minesRemaining();

    canPlay = true;
    showMines = false;
    tilesRevealed = 0;
}



void board::drawBoard(sf::RenderWindow& window) {

    for(int i = 0; i < numRows; i++){
        for(int j = 0; j < numColumns; j++)
        {
            string index = to_string(i) + "|" + to_string(j);
            window.draw(realTiles[index].sprite);
            //lost/debug condition //mines become flags for win condition
            if(showMines)
                window.draw(mineMap[index].sprite);
            //draws numbers as long as not on top of a mine
            if(!realTiles[index].mined)
                window.draw(numMinesMap[index].sprite);
            //draws flags
            if(realTiles[index].flagged)
                window.draw(flagMap[index].sprite);
        }
    }

    if(negative)
        window.draw(digit1.sprite);
    window.draw(digit2.sprite);
    window.draw(digit3.sprite);
    window.draw(digit4.sprite);
    window.draw(face.sprite);
    window.draw(debug.sprite);
    window.draw(test1.sprite);
    window.draw(test2.sprite);
    window.draw(test3.sprite);

}

void board::revealTile(sf::Vector2i mousePos, sf::RenderWindow& window)
{
    string index = mouseIndex(mousePos.x, mousePos.y);
    if(!realTiles[index].flagged && mousePos.y < (numRows * 32) && canPlay && !realTiles[index].revealed)
    {
        tilesRevealed++;
        //reveals tile
        realTiles[index].sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
        realTiles[index].revealed = true;
        //recursive reveal
        if(realTiles[index].numAdjMines == 0){
            recursiveReveal(&realTiles[index], window);
        }
        //add number
        if(realTiles[index].numAdjMines > 0){
            string file = "number_" + to_string(realTiles[index].numAdjMines);
            numMinesMap[index].sprite.setTexture(TextureManager::GetTexture(file));
            numMinesMap[index].sprite.setPosition((mousePos.x/32)*32,(mousePos.y/32)*32);
        }
        //if clicked on mine
        if(realTiles[index].mined){
            face.sprite.setTexture(TextureManager::GetTexture("face_lose"));
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numColumns; j++){
                    string index = to_string(i) + "|" + to_string(j);
                    if(realTiles[index].mined)
                        realTiles[index].sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
                }
            }
            canPlay = false;
            showMines = true;
        }
        //if won the game
        if((numRows * numColumns) - tilesRevealed == numMines) {
            face.sprite.setTexture(TextureManager::GetTexture("face_win"));
            canPlay = false;

            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numColumns; j++){
                    string index = to_string(i) + "|" + to_string(j);
                    if(realTiles[index].mined){
                        mineMap[index].sprite.setTexture(TextureManager::GetTexture("flag"));
                        realTiles[index].mined = false;
                    }
                }
            }
            showMines = true;
            numMinesNotFlagged = 0;
            minesRemaining();
        }
    }

    //debug
    else if (debug.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && canPlay){
        showMines = !showMines;
    }
    //tests
    else if (test1.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        generateBoard(window, 1);
    else if (test2.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        generateBoard(window, 2);
    else if (test3.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y))
        generateBoard(window, 3);
    //restart
    else if(!canPlay && face.sprite.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
        generateBoard(window, 0);
    }
}

string board::mouseIndex(int x, int y)
{
    x /= 32;
    y /= 32;
    string index = to_string(y) + "|" + to_string(x);
    return index;
}

void board::addFlag(sf::Vector2i mousePos)
{
    string index = mouseIndex(mousePos.x, mousePos.y);
    if(!realTiles[index].flagged && !realTiles[index].revealed && mousePos.y < (numRows * 32) && canPlay)
    {
        flagMap[index].sprite.setTexture(TextureManager::GetTexture("flag"));
        flagMap[index].sprite.setPosition((mousePos.x/32)*32,(mousePos.y/32)*32);
        realTiles[index].flagged = true;
        numMinesNotFlagged--;
        minesRemaining();
    }
    else if(mousePos.y < (numRows * 32) && canPlay){
        auto eraseFlag = flagMap.find(index);
        flagMap.erase(eraseFlag);
        realTiles[index].flagged = false;
        numMinesNotFlagged++;
        minesRemaining();
    }
    else{}
}

void board::addNumbers(sf::RenderWindow &window) {

    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numColumns; j++)
        {
            int mineCount = 0;
            string index = to_string(i) + "|" + to_string(j);
            //left
            if(j>0){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,0,-1)] );
                if(realTiles[indexShift(i,j,0,-1)].mined)
                    mineCount++;
            }
            //up
            if(i>0){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,-1,0)] );
                if(realTiles[indexShift(i,j,-1,0)].mined)
                    mineCount++;
            }
            //right
            if(j < numColumns - 1){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,0,1)] );
                if(realTiles[indexShift(i,j,0,1)].mined)
                    mineCount++;
            }
            //down
            if(i < numRows - 1){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,1,0)] );
                if(realTiles[indexShift(i,j,1,0)].mined)
                    mineCount++;
            }
            //top-left
            if(j>0 && i>0){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,-1,-1)] );
                if(realTiles[indexShift(i,j,-1,-1)].mined)
                    mineCount++;
            }
            //top-right
            if(i>0 && j < numColumns - 1){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,-1,1)] );
                if(realTiles[indexShift(i,j,-1,1)].mined)
                    mineCount++;
            }
            //bot-left
            if(j>0 && i < numRows - 1){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,1,-1)] );
                if(realTiles[indexShift(i,j,1,-1)].mined)
                    mineCount++;
            }
            //bot-right
            if(j < numColumns -1 && i < numRows - 1){
                realTiles[index].adjacentTiles.push_back( &realTiles[indexShift(i,j,1,1)] );
                if(realTiles[indexShift(i,j,1,1)].mined)
                    mineCount++;
            }
            realTiles[index].numAdjMines = mineCount;
        }
    }
}

string board::indexShift(int i, int j, int yShift, int xShift) {
    string returnString = to_string(i + yShift) + "|" + to_string(j + xShift);
    return returnString;
}

//checks to make sure a mine isn't being place on the same position twice
bool board::newMine(vector<string> mineList, string newMine) {

    for(int i = 0; i < mineList.size(); i++)
    {
        if(mineList[i] == newMine)
            return true;
    }
    if(mineList.empty())
        return false;

    return false;
}

void board::recursiveReveal(tiles* baseTile,sf::RenderWindow &window) {

    for(auto & i : baseTile->adjacentTiles){
        //empty space
        if(i->numAdjMines == 0 && !(i->mined) && !(i->revealed) && !(i->flagged)){
            tilesRevealed++;
            i->sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
            i->revealed = true;
            recursiveReveal(i, window);
        }
        //reveal number
        else if(!(i->mined) && !(i->revealed) && !(i->flagged)){
            tilesRevealed++;
            i->sprite.setTexture(TextureManager::GetTexture("tile_revealed"));
            i->revealed = true;
            string file = "number_" + to_string(i->numAdjMines);
            numMinesMap[i->index].sprite.setTexture(TextureManager::GetTexture(file));
            numMinesMap[i->index].sprite.setPosition(i->sprite.getPosition());
        }
        //base condition
        else
            continue;
    }
}

void board::minesRemaining() {

    int hundreds, tens, ones, temp;
    negative = false;
    temp = numMinesNotFlagged;
    if(numMinesNotFlagged < 0){
        negative = true;
        temp *= -1;
    }
    hundreds = (temp / 100) % 100;
    temp -= (hundreds * 100);
    tens = (temp / 10) % 10;
    temp -= (tens * 10);
    ones = (temp / 1);

    digit1.sprite.setTexture(digits[10]);
    digit1.sprite.setPosition(0,bottom);
    digit2.sprite.setTexture(digits[hundreds]);
    digit2.sprite.setPosition(21,bottom);
    digit3.sprite.setTexture(digits[tens]);
    digit3.sprite.setPosition(42,bottom);
    digit4.sprite.setTexture(digits[ones]);
    digit4.sprite.setPosition(63,bottom);
}

void board::randomMines(){
    vector<string> mineLoc;
    for(int i = 0; i <= numMines; i++)
    {
        string placement = "";
        int randRow;
        int randColumn;
        while (newMine(mineLoc, placement)) {
            randRow = random::randomInt(0, numRows - 1);
            randColumn = random::randomInt(0, numColumns - 1);
            placement = to_string(randRow) + "|" + to_string(randColumn);
        }
        mineLoc.push_back(placement);
        mineMap[placement].sprite.setTexture(TextureManager::GetTexture("mine"));
        mineMap[placement].sprite.setPosition(randColumn * 32, randRow * 32);
        realTiles[placement].mined = true;
    }
}

void board::fileMines(int i) {

    numMines = 0;

    ifstream file;
    if (i == 1)
        file.open("boards/testboard1.brd");
    else if (i == 2)
        file.open("boards/testboard2.brd");
    else if (i == 3)
        file.open("boards/testboard3.brd");

    if(file.is_open()){
        string line;
        vector<string> lines;
        while(getline(file, line)){
            lines.push_back(line);
        }

        for(int i = 0; i < numRows; i++){
            for(int j = 0; j < numColumns; j++){
                if(lines[i][j] == '1'){
                    string index = to_string(i) + "|" + to_string(j);
                    mineMap[index].sprite.setTexture(TextureManager::GetTexture("mine"));
                    mineMap[index].sprite.setPosition(j * 32, i * 32);
                    realTiles[index].mined = true;
                    numMines++;
                }
            }
        }
    }
    file.close();
}