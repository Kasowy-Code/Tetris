#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>

const unsigned int TileSize = 32;
const unsigned int PlayAreaWidth = 10;
const unsigned int PlayAreaHeight = 20;

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
private:
    int X;
    int Y;
};

Mino::Mino(Colors color, int X, int Y, const sf::Texture &texture)
{
    this->X = X;
    this->Y = Y;
    this->block.setTexture(texture);
    this->block.setTextureRect(sf::IntRect(color * 32, 0, 32, 32));
}


void Mino::Move(Directions direction) {
    switch (direction) {
    case down:
        if (this->Y < PlayAreaHeight) {
            this->Y++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        break;
    case right:
        if (this->X < PlayAreaWidth) {
            this->X++;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y*32.0f));
        }
        break;
    case left:
        if (this->X > 0) {
            X--;
            block.setPosition(sf::Vector2f(this->X * 32.0f, this->Y * 32.0f));
        }
        break;
    case hardDrop:
        for (int i = 20; i > 0; i--) {
            if (MapPlaces[this->getPositionX()][i] != true) {
                this->Y = i;
                break;
            }
        }
        block.setPosition(sf::Vector2f(this->X*32.0f, this->Y*32.0f));
        break;
    }
}

//class Tetromino
//{
//public:
//    Tetromino();
//    //virtual ~Tetromino();
//
//private:
//
//};
//
//Tetromino::Tetromino()
//{
//}


int main()
{
    sf::Texture texture;
    texture.loadFromFile("../../assets/TileTextures.png");
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Tetris");
    bool isDropping = true;
    //Mino mino(green, 0, 0, texture);
    sf::Clock clock;
    int count = 0;
    while (window.isOpen())
    {
        count++;
        Mino* mino;
        if (count % 2 == 0) {
            mino = new Mino(red, 0, 0, texture);
        }
        else {
            mino = new Mino(green, 0, 0, texture);
        }
        isDropping = true;
        while (isDropping) {

            bool shouldPause = false;
            sf::Event event;
            sf::Time time = clock.getElapsedTime();
            if (time.asMilliseconds() >= 400) {
                if(MapPlaces[mino->getPositionX()][mino->getPositionY() + 1] == false && mino->getPositionY() != 20) {
                    mino->Move(down);
                }
                else {
                    isDropping = false;
                    Minos.push_back(mino);
                    MapPlaces[mino->getPositionX()][mino->getPositionY()] = true;
                }
                clock.restart();
                std::cout << mino->getPositionY() << " " << mino->getPositionX() << std::endl;
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
                        if (MapPlaces[mino->getPositionX()][mino->getPositionY() + 1] == false && mino->getPositionY() != 20) {
                            mino->Move(down);
                        }
                        else {
                            isDropping = false;
                            Minos.push_back(mino);
                            MapPlaces[mino->getPositionX()][mino->getPositionY()] = true;
                        }
                        break;
                    case sf::Keyboard::D:
                        mino->Move(right);
                        break;
                    case sf::Keyboard::A:
                        mino->Move(left);
                        break;
                    case sf::Keyboard::Space:
                        mino->Move(hardDrop);
                        isDropping = false;
                        Minos.push_back(mino);
                        MapPlaces[mino->getPositionX()][mino->getPositionY()] = true;
                        break;
                    }
                    clock.restart();
                    shouldPause = true;
                }
            }

            window.draw(mino->block);

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