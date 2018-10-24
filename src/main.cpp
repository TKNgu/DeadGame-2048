#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "Game2048.hpp"

#define SIZE 4

uint32_t score=0;

void showBlock(int, int, int, sf::RenderWindow&, sf::Sprite&, sf::Text&);
void drawBoard(uint8_t board[SIZE][SIZE], sf::RenderWindow&, sf::Sprite&, sf::Text&);

int main(int argc, char** argv) {
    std::string ROOT_PATH = argc < 2 ? "../data/" : argv[1];
    std::cout << ROOT_PATH << std::endl;


    // Create the main window
    sf::RenderWindow window(sf::VideoMode(256, 320), "SFML window");
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile(ROOT_PATH + "box.png"))
        return EXIT_FAILURE;
    sf::Sprite sprite(texture);


    sf::Texture textureScores;
    if (!textureScores.loadFromFile(ROOT_PATH + "scores.png"))
        return EXIT_FAILURE;
    sf::Sprite spriteScores(textureScores);

    sf::Texture textureEnd;
    if (!textureEnd.loadFromFile(ROOT_PATH + "end.png"))
        return EXIT_FAILURE;
    sf::Sprite spriteEnd(textureEnd);


    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile(ROOT_PATH + "Ubuntu-L.ttf"))
        return EXIT_FAILURE;
    sf::Text text("Hello SFML", font, 24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::Red);
    // Load a music to play
    // sf::Music music;
    // if (!music.openFromFile("nice_music.ogg"))
    // return EXIT_FAILURE;
    // Play the music
    // music.play();
    // Start the game loop


    Game2048 game;

    board_data (&board)[SIZE][SIZE] = game.getBoard();
    // initBoard(board);

    bool success;
    int countAdd = 5;
    bool isEnd = false;

    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if(success) {
                continue;
            }
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();

            if(!isEnd) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    success = game.moveLeft();
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    success = game.moveRight();
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    success = game.moveUp();
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    success = game.moveDown();
                }
            }
        }

        if (success) {
            if(countAdd--) {
                drawBoard(board, window, sprite, text);
            } else {
                game.addRandom();
                drawBoard(board, window, sprite, text);
                success = false;
                countAdd = 5;
            }
            isEnd = game.isEnd();
        }

        // Clear screen
        window.clear();
        // Draw the sprite

        window.draw(spriteScores);
        drawBoard(board, window, sprite, text);

        text.setString(std::to_string(score));
        text.setPosition(sf::Vector2f(125, 18));
        // Draw the string
        window.draw(text);
        // Update the window

        if(isEnd) {
            spriteEnd.setPosition(11, 150);
            window.draw(spriteEnd);
        }
        window.display();
        sf::sleep(sf::milliseconds(1000.0f / 24.0f));
    }
    return EXIT_SUCCESS;
}

void drawBoard(uint8_t board[SIZE][SIZE], sf::RenderWindow& window, sf::Sprite& sprite, sf::Text& text) {
    for(int indexY = 0; indexY < SIZE; indexY++) {
        for(int indexX = 0; indexX < SIZE; indexX++) {
            showBlock(indexX, indexY, board[indexX][indexY], window, sprite, text);
        }
    }
}

void showBlock(int x, int y, int value, sf::RenderWindow& window, sf::Sprite& sprite, sf::Text& text) {
    if(!value) {
        return;
    }
    sprite.setPosition(sf::Vector2f(x * 64.0f, y * 64.0f  + 64.0f));
    window.draw(sprite);
    text.setString(std::to_string((uint32_t)1<<value));
    text.setPosition(sf::Vector2f(x * 64.0f + 3, y * 64.0f  + 80.0f));
    window.draw(text);
}