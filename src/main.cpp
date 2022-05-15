#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>

const unsigned int TileSize = 32;
const unsigned int PlayAreaWidth = 10;
const unsigned int PlayAreaHeight = 20;
bool isDropping = true;

class Mino;

std::vector<Mino*> Minos;
bool MapPlaces[10][20];

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
        if (MapPlaces[this->getPositionX()][this->getPositionY() + 1] == false && this->getPositionY() != PlayAreaHeight - 1) {
            this->Y++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw -1;
        }
        break;
    case right:
        if (this->X < PlayAreaWidth-1 && MapPlaces[this->getPositionX()+1][this->getPositionY()] != true) {
            this->X++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw("right");
        }
        break;
    case left:
        if (this->X > 0 && MapPlaces[this->getPositionX() - 1][this->getPositionY()] != true) {
            X--;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        else {
            throw("left");
        }
        break;
    //case hardDrop:
    //    /*for (int i = this->getPositionY(); i < 20; i++) {
    //            if (MapPlaces[this->getPositionX()][i+1] == true || i == 19) {
    //                std::cout << "I: " << i << std::endl;
    //                this->Y = i;
    //                Minos.push_back(this);
    //                MapPlaces[this->getPositionX()][this->getPositionY()] = true;
    //                break;
    //            }
    //        
    //    }*/

    //    while (!finished) {
    //        if (MapPlaces[this->getPositionX()][this->getPositionY() + 1] == false && this->getPositionY() != PlayAreaHeight - 1) {
    //            this->Y++;
    //            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
    //        }
    //        else {
    //            finished = true;
    //            Minos.push_back(this);
    //                MapPlaces[this->getPositionX()][this->getPositionY()] = true;
    //        }
    //    }
    //    block.setPosition(sf::Vector2f(this->X*32.0f, this->Y*32.0f));
    //    break;
    }
}

class Tetromino
{
public:
    Tetromino(Colors color, const sf::Texture& texture);
    Tetromino() {};
    void Move(Directions direction);
    //virtual ~Tetromino() {}
    std::vector<Mino*> blocks;
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
            for (auto& el : this->blocks) {
                Minos.push_back(el);
                MapPlaces[el->getPositionX()][el->getPositionY()] = true;
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
                MapPlaces[el->getPositionX()][el->getPositionY()] = true;
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

class LShape : public Tetromino
{
public:
    LShape(Colors color, const sf::Texture& texture) : Tetromino{color, texture} {

        this->blocks[0]->setPositionX(4);
        this->blocks[0]->setPositionY(0);

        this->blocks[1]->setPositionX(5);
        this->blocks[1]->setPositionY(0);

        this->blocks[2]->setPositionX(4);
        this->blocks[2]->setPositionY(-1);

         this->blocks[3]->setPositionX(4);
        this->blocks[3]->setPositionY(-2);

        for (auto& el : blocks) {
            el->block.setTexture(texture);
            el->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
            el->block.setPosition(sf::Vector2f(el->getPositionX() * 32.0f, el->getPositionY() * 32.0f));
        }
    }
   // ~LShape();

private:

};

//LShape::LShape(Colors color, const sf::Texture& texture) : Tetromino()
//{
//
//   
//    
//}




int main()
{
    sf::Texture texture;
    texture.loadFromFile("../../assets/TileTextures.png");
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tetris");
    Mino* test = new Mino(purple, 5, 7, texture);
    MapPlaces[5][7] = true;
    Minos.push_back(test);
    sf::Clock clock;
    int count = 0;
    while (window.isOpen())
    {
        count++;
        LShape* mino;
        if (count % 2 == 0) {
            mino = new LShape(red, texture);
        }
        else {
            mino = new LShape(green, texture);
        }
        isDropping = true;
        while (isDropping) {

            bool shouldPause = false;
            sf::Event event;
            sf::Time time = clock.getElapsedTime();
            if (time.asMilliseconds() >= 400) {
                mino->Move(down);
                clock.restart();
                std::cout << mino->blocks[0]->getPositionY() << " " << mino->blocks[0]->getPositionX() << std::endl;
            }
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed) {
                    isDropping = false;
                    window.close();
                }

                if (event.type == event.KeyPressed) {
                    switch (event.key.code) {
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
    }

    return 0;
}