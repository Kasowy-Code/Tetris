#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <sstream>

const unsigned int TileSize = 32;
const unsigned int PlayAreaWidth = 10;
const unsigned int PlayAreaHeight = 20;
bool isDropping = true;

class Mino;

std::vector<Mino*> Minos;
bool MapPlaces[10][23];

enum Directions {
    left, right, down, hardDrop
};
enum Colors {
    red, green, blue, orange, yellow, cyan, purple
};


class Mino
{
public:
    Mino() {}
    Mino(Colors color, int X, int Y, const sf::Texture &texture);
    void Move(Directions direction);
    int getPositionX() { return this->X; }
    int getPositionY() { return this->Y; }
    sf::Sprite block;

    void setPositionX(int X) { this->X = X; }
    void setPositionY(int Y) { this->Y = Y; }
private:
    int X = 0;
    int Y = 0;
};

Mino::Mino(Colors color, int X, int Y, const sf::Texture &texture)
{
    this->X = X;
    this->Y = Y;
    this->block.setTexture(texture);
    this->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
    this->block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
}


void Mino::Move(Directions direction) {
    bool finished = false;
    switch (direction) {
    case down:
        if (MapPlaces[this->getPositionX()][this->getPositionY() + 4] != true && this->getPositionY() != PlayAreaHeight - 1) {
            this->Y++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw -1;
        }
        break;
    case right:
        if (this->X < PlayAreaWidth-1 && MapPlaces[this->getPositionX()+1][this->getPositionY()+3] != true) {
            this->X++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw("right");
        }
        break;
    case left:
        if (this->X > 0 && MapPlaces[this->getPositionX() - 1][this->getPositionY()+3] != true) {
            X--;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw("left");
        }
        break;
   
    }
}

class Tetromino
{
public:
    Tetromino(Colors color, const sf::Texture& texture);
    Tetromino() {};
    void Move(Directions direction);
    virtual void Rotate();
    std::vector<Mino*> blocks;
    int rotation = 0;
};

Tetromino::Tetromino(Colors color, const sf::Texture& texture)
{
    for (int i = 0; i < 4; i++) {
        Mino* el = new Mino(color, -1, -1, texture);
        el->block.setTexture(texture);
        el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
        blocks.push_back(el);
    }
}
void Tetromino::Move(Directions direction) {
    std::vector<Mino> saveState;
    for (const auto& el : blocks) {
        saveState.push_back(*el);
    }
    if (direction == hardDrop) {
        try {
            while (true) {

                for (const auto &el : blocks) {
                    saveState.push_back(*el);
                }
                for (auto& el : blocks) {
                    el->Move(down);
                }
                saveState.clear();
            }
        }
        catch (int e) {
            for (int i = 0; i < 4; i++) {
                blocks[i]->setPositionX(saveState[i].getPositionX());
                blocks[i]->setPositionY(saveState[i].getPositionY());
                blocks[i]->block.setPosition(sf::Vector2f(blocks[i]->getPositionX() * 32.0f, blocks[i]->getPositionY() * 32.0f));
            }
            isDropping = false;
            for (const auto& el : this->blocks) {
                Minos.push_back(el);
                MapPlaces[el->getPositionX()][el->getPositionY()+3] = true;
            }
        }
        
    }
    else {
        try {
            for (auto& el : blocks) {
                el->Move(direction);
            }
        }
        catch (int e) {
            for (int i = 0; i < 4; i++) {
                blocks[i]->setPositionX(saveState[i].getPositionX());
                blocks[i]->setPositionY(saveState[i].getPositionY());
                blocks[i]->block.setPosition(sf::Vector2f(blocks[i]->getPositionX() * 32.0f, blocks[i]->getPositionY() * 32.0f));
            }
            isDropping = false;
            for (auto& el : this->blocks) {
                Minos.push_back(el);
                MapPlaces[el->getPositionX()][el->getPositionY()+3] = true;
            }
        }
        catch (...) {
            for (int i = 0; i < 4; i++) {
                blocks[i]->setPositionX(saveState[i].getPositionX());
                blocks[i]->setPositionY(saveState[i].getPositionY());
                blocks[i]->block.setPosition(sf::Vector2f(blocks[i]->getPositionX() * 32.0f, blocks[i]->getPositionY() * 32.0f));
            }

        }
    }
}

void Tetromino::Rotate() {
    std::vector<Mino> saveState;
    for (const auto& el : blocks) {
        saveState.push_back(*el);
    }

    Mino temp = *blocks[1];
    try {
        for (auto& el : blocks) {
            int x = el->getPositionY() - temp.getPositionY();
            int y = el->getPositionX() - temp.getPositionX();

            el->setPositionX(temp.getPositionX() - x);
            el->setPositionY(temp.getPositionY() + y);
            if (MapPlaces[el->getPositionX()][el->getPositionY() + 3] || el->getPositionX() < 0 || el->getPositionX() > PlayAreaWidth-1) {
                throw 1;
            }
        }
    }
    catch (...) {
        for (int i = 0; i < 4; i++) {
            blocks[i]->setPositionX(saveState[i].getPositionX());
            blocks[i]->setPositionY(saveState[i].getPositionY());
        }
    }

    for (auto& el : blocks) {
        el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
    }
}



class LShape : public Tetromino
{
public:
    LShape(Colors color, const sf::Texture& texture) : Tetromino{color, texture} {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(5);
        this->blocks[2]->setPositionY(-1);

         this->blocks[3]->setPositionX(5);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
private:

};

class JShape : public Tetromino
{
public:
    JShape(Colors color, const sf::Texture& texture) : Tetromino{color, texture} {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(5);
        this->blocks[2]->setPositionY(-1);

        this->blocks[3]->setPositionX(3);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }

private:

};

class OShape : public Tetromino
{
public:
    OShape(Colors color, const sf::Texture& texture) : Tetromino{color, texture} {

        this->blocks[0]->setPositionX(4);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(5);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(4);
        this->blocks[2]->setPositionY(-2);

         this->blocks[3]->setPositionX(5);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }

private:

};

class SShape : public Tetromino
{
public:
    SShape(Colors color, const sf::Texture& texture) : Tetromino{ color, texture } {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(4);
        this->blocks[2]->setPositionY(-2);

        this->blocks[3]->setPositionX(5);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
private:

};

class ZShape : public Tetromino
{
public:
    ZShape(Colors color, const sf::Texture& texture) : Tetromino{ color, texture } {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-2);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-2);

        this->blocks[2]->setPositionX(4);
        this->blocks[2]->setPositionY(-1);

        this->blocks[3]->setPositionX(5);
        this->blocks[3]->setPositionY(-1);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
private:

};

class TShape : public Tetromino
{
public:
    TShape(Colors color, const sf::Texture& texture) : Tetromino{ color, texture } {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(5);
        this->blocks[2]->setPositionY(-1);

        this->blocks[3]->setPositionX(4);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
private:

};

class IShape : public Tetromino
{
public:
    IShape(Colors color, const sf::Texture& texture) : Tetromino{ color, texture } {

        this->blocks[0]->setPositionX(3);
        this->blocks[0]->setPositionY(-1);

        this->blocks[1]->setPositionX(4);
        this->blocks[1]->setPositionY(-1);

        this->blocks[2]->setPositionX(5);
        this->blocks[2]->setPositionY(-1);

        this->blocks[3]->setPositionX(6);
        this->blocks[3]->setPositionY(-1);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
    virtual void Rotate() {
        std::vector<Mino> saveState;
        for (const auto& el : blocks) {
            saveState.push_back(*el);
        }

        try {
            switch (rotation) {

            case 0:
                //              #
                // # # # #   => # 
                //              #
                //              #

                blocks[1]->setPositionX(blocks[1]->getPositionX() + 1);


                blocks[0]->setPositionX(blocks[1]->getPositionX());
                blocks[0]->setPositionY(blocks[1]->getPositionY() - 1);

                blocks[2]->setPositionX(blocks[1]->getPositionX());
                blocks[2]->setPositionY(blocks[1]->getPositionY() + 1);

                blocks[3]->setPositionX(blocks[1]->getPositionX());
                blocks[3]->setPositionY(blocks[1]->getPositionY() + 2);

                rotation = 1;
                break;
            case 1:

                // #           
                // #   =>  
                // #       # # # #
                // #

                blocks[1]->setPositionY(blocks[1]->getPositionY() + 1);


                blocks[0]->setPositionX(blocks[1]->getPositionX() + 1);
                blocks[0]->setPositionY(blocks[1]->getPositionY());

                blocks[2]->setPositionX(blocks[1]->getPositionX() - 1);
                blocks[2]->setPositionY(blocks[1]->getPositionY());

                blocks[3]->setPositionX(blocks[1]->getPositionX() - 2);
                blocks[3]->setPositionY(blocks[1]->getPositionY());

                rotation = 2;
                break;

            case 2:

                //               #
                //          =>   #
                // # # # #       #
                //               #

                blocks[1]->setPositionX(blocks[1]->getPositionX() - 1);

                blocks[0]->setPositionX(blocks[1]->getPositionX());
                blocks[0]->setPositionY(blocks[1]->getPositionY() + 1);

                blocks[2]->setPositionX(blocks[1]->getPositionX());
                blocks[2]->setPositionY(blocks[1]->getPositionY() - 1);

                blocks[3]->setPositionX(blocks[1]->getPositionX());
                blocks[3]->setPositionY(blocks[1]->getPositionY() - 2);

                rotation = 3;
                break;
            case 3:

                // #           
                // #   =>  
                // #       # # # #
                // #

                blocks[1]->setPositionY(blocks[1]->getPositionY() - 1);

                blocks[0]->setPositionX(blocks[1]->getPositionX() - 1);
                blocks[0]->setPositionY(blocks[1]->getPositionY());

                blocks[2]->setPositionX(blocks[1]->getPositionX() + 1);
                blocks[2]->setPositionY(blocks[1]->getPositionY());

                blocks[3]->setPositionX(blocks[1]->getPositionX() + 2);
                blocks[3]->setPositionY(blocks[1]->getPositionY());

                rotation = 0;
                break;
            }
            for (auto& el : blocks) {
                if (MapPlaces[el->getPositionX()][el->getPositionY() + 3] || el->getPositionX() < 0 || el->getPositionX() > PlayAreaWidth - 1) {
                    throw 1;
                }
            }

            for (auto& el : blocks) {
                el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
            }
        }
        catch (...) {
            if (rotation == 0) {
                rotation = 3;
            }
            else {
                rotation--;
            }
            for (int i = 0; i < 4; i++) {
                blocks[i]->setPositionX(saveState[i].getPositionX());
                blocks[i]->setPositionY(saveState[i].getPositionY());
            }
        }
    }

private:

};

int main()
{
    srand(time(0));
    sf::Texture texture;
    texture.loadFromFile("../../assets/TileTextures.png");
    sf::RenderWindow window(sf::VideoMode(320, 640), "Tetris");
    sf::Clock clock;
    unsigned int linesCleared = 0;
    while (window.isOpen())
    {

        Tetromino* mino;
        switch (rand() % 7) {
        case 0:
            mino = new LShape(orange, texture);
            break;
        case 1:
            mino = new JShape(blue, texture);
            break;
        case 2:
            mino = new OShape(yellow, texture);
            break;
        case 3: 
            mino = new SShape(green, texture);
            break;
        case 4: 
            mino = new ZShape(red, texture);
            break;
        case 5: 
            mino = new TShape(purple, texture);
            break;
        case 6: 
            mino = new IShape(cyan, texture);
            break;
        }
        isDropping = true;
        while (isDropping) {

            bool shouldPause = false;
            sf::Event event;
            sf::Time time = clock.getElapsedTime();
            if (time.asMilliseconds() >= 400) {
                mino->Move(down);
                clock.restart();
            }
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    isDropping = false;
                    window.close();
                }

                if (event.type == event.KeyPressed) {
                    switch (event.key.code) {
                    case sf::Keyboard::W:
                        mino->Rotate();
                        break;
                    case sf::Keyboard::S:
                        mino->Move(down);                       
                        break;
                    case sf::Keyboard::D:
                        try {
                            mino->Move(right);
                        }
                        catch (...) {}
                        break;
                    case sf::Keyboard::A:
                        try {
                            mino->Move(left);
                        }
                        catch (...) {}
                        break;
                    case sf::Keyboard::Space:
                        mino->Move(hardDrop);
                        isDropping = false;
                        break;
                    }
                    clock.restart();
                    shouldPause = true;
                }
            }
            for (auto el : mino->blocks) {
                window.draw(el->block);
            }

            for (auto el : Minos) {
                window.draw(el->block);  
            }

            window.display();
            if (shouldPause) {
                sf::sleep(sf::milliseconds(200));
                shouldPause = false;
            }
            window.clear();
        }
        std::vector<int> rowsToClear;
        for (int i = 3; i < PlayAreaHeight+3; i++) {
            int tmp = 0;
            for (int j = 0; j < PlayAreaWidth; j++) {
                if (MapPlaces[j][i]) {
                    tmp++;
                }
            }
            if (tmp == 10) {
                linesCleared++;
                rowsToClear.push_back(i-3);
            }
        }

        std::vector<Mino*> temp;
        for (const auto& Y : rowsToClear) {
            for (const auto& el : Minos) {
                if (el->getPositionY() == Y) {
                    temp.push_back(el);
                }
            }
        }
        for (const auto& el : temp) {
            std::vector<Mino*>::iterator it;
            MapPlaces[el->getPositionX()][el->getPositionY() + 3] = false;
            Minos.erase(std::remove(Minos.begin(), Minos.end(), el), Minos.end());
        }
        temp.clear();

        for (const auto& Y : rowsToClear) {
            for (auto& el : Minos) {
                MapPlaces[el->getPositionX()][el->getPositionY() + 3] = false;
                if (el->getPositionY() < Y) {
                    el->setPositionY(el->getPositionY()+1);
                    el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
                }
            }
        }
        for (const auto& el : Minos) {
            MapPlaces[el->getPositionX()][el->getPositionY() + 3] = true;
        }
        
        for (int i = 0; i < PlayAreaWidth; i++) {
            if (MapPlaces[i][0] == true || MapPlaces[i][1] == true || MapPlaces[i][2] == true) {
                sf::Event event2;
                bool userInput = false;
                bool isDrawn = false;
                while (!userInput) {
                    while (window.pollEvent(event2)) {
                        if (event2.type == event2.KeyPressed || event2.type == event2.Closed) {
                            userInput = true;
                            Minos.clear();
                            rowsToClear.clear();
                            for (int j = 0; j < PlayAreaWidth; j++) {
                                for (int k = 0; k < PlayAreaHeight+3; k++) {
                                    MapPlaces[j][k] = false;
                                }
                            }

                            window.clear();
                        }
                    }
                    sf::RectangleShape bg(sf::Vector2f(320.0f, 160.0f));
                    bg.setFillColor(sf::Color(255, 255, 255));

                    bg.setPosition(sf::Vector2f(0.0f, 200.0f));

                    std::stringstream txt;
                    txt << "Lines cleared: " << linesCleared;
                    sf::Font font;
                    font.loadFromFile("../../assets/arial.ttf");
                    sf::Text GameOver;
                    sf::Text Score;
                    GameOver.setFont(font);
                    Score.setFont(font);
                    GameOver.setString("Game Over");
                    Score.setString(txt.str());
                    GameOver.setPosition(sf::Vector2f(100.0f, 240.0f));
                    Score.setPosition(sf::Vector2f(100.0f, 300.0f));
                    GameOver.setCharacterSize(24);
                    Score.setCharacterSize(24);
                    GameOver.setFillColor(sf::Color(255, 0, 0));
                    Score.setFillColor(sf::Color(255, 0, 0));

                    if (!isDrawn) {
                        for (auto el : Minos) {
                            window.draw(el->block);
                        }
                        window.draw(bg);
                        window.draw(GameOver);
                        window.draw(Score);

                        window.display();
                        isDrawn = true;
                    }
                }
                break;
            }
        }
    }

    return 0;
}