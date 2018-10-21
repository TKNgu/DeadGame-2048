#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

#define SIZE 4

uint32_t score=0;

void showBlock(int, int, int, sf::RenderWindow&, sf::Sprite&, sf::Text&);
void drawBoard(uint8_t board[SIZE][SIZE], sf::RenderWindow&, sf::Sprite&, sf::Text&);

uint8_t findTarget(uint8_t array[SIZE],uint8_t x,uint8_t stop) {
    uint8_t t;
    // if the position is already on the first, don't evaluate
    if (x==0) {
        return x;
    }
    for(t=x-1; t>=0; t--) {
        if (array[t]!=0) {
            if (array[t]!=array[x]) {
                // merge is not possible, take next position
                return t+1;
            }
            return t;
        } else {
            // we should not slide further, return this one
            if (t==stop) {
                return t;
            }
        }
    }
    // we did not find a
    return x;
}

void addRandom(uint8_t board[SIZE][SIZE]) {
    static bool initialized = false;
    uint8_t x,y;
    uint8_t r,len=0;
    uint8_t n,list[SIZE*SIZE][2];

    if (!initialized) {
        srand(time(NULL));
        initialized = true;
    }

    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE; y++) {
            if (board[x][y]==0) {
                list[len][0]=x;
                list[len][1]=y;
                len++;
            }
        }
    }

    if (len>0) {
        r = rand()%len;
        x = list[r][0];
        y = list[r][1];
        n = (rand()%10)/9+1;
        board[x][y]=n;
    }
}

bool slideArray(uint8_t array[SIZE]) {
    bool success = false;
    uint8_t x,t,stop=0;

    for (x=0; x<SIZE; x++) {
        if (array[x]!=0) {
            t = findTarget(array,x,stop);
            // if target is not original position, then move or merge
            if (t!=x) {
                // if target is zero, this is a move
                if (array[t]==0) {
                    array[t]=array[x];
                } else if (array[t]==array[x]) {
                    // merge (increase power of two)
                    array[t]++;
                    // increase score
                    score+=(uint32_t)1<<array[t];
                    // set stop to avoid double merge
                    stop = t+1;
                }
                array[x]=0;
                success = true;
            }
        }
    }
    return success;
}

void rotateBoard(uint8_t board[SIZE][SIZE]) {
    uint8_t i,j,n=SIZE;
    uint8_t tmp;
    for (i=0; i<n/2; i++) {
        for (j=i; j<n-i-1; j++) {
            tmp = board[i][j];
            board[i][j] = board[j][n-i-1];
            board[j][n-i-1] = board[n-i-1][n-j-1];
            board[n-i-1][n-j-1] = board[n-j-1][i];
            board[n-j-1][i] = tmp;
        }
    }
}

void initBoard(uint8_t board[SIZE][SIZE]) {
    uint8_t x,y;
    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE; y++) {
            board[x][y]=0;
        }
    }
    addRandom(board);
    addRandom(board);
}

bool moveUp(uint8_t board[SIZE][SIZE]) {
    bool success = false;
    uint8_t x;
    for (x=0; x<SIZE; x++) {
        success |= slideArray(board[x]);
    }
    return success;
}

bool moveLeft(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}

bool moveDown(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    rotateBoard(board);
    return success;
}

bool findPairDown(uint8_t board[SIZE][SIZE]) {
    bool success = false;
    uint8_t x,y;
    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE-1; y++) {
            if (board[x][y]==board[x][y+1]) return true;
        }
    }
    return success;
}

bool moveRight(uint8_t board[SIZE][SIZE]) {
    bool success;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    success = moveUp(board);
    rotateBoard(board);
    return success;
}

uint8_t countEmpty(uint8_t board[SIZE][SIZE]) {
    uint8_t x,y;
    uint8_t count=0;
    for (x=0; x<SIZE; x++) {
        for (y=0; y<SIZE; y++) {
            if (board[x][y]==0) {
                count++;
            }
        }
    }
    return count;
}

bool gameEnded(uint8_t board[SIZE][SIZE]) {
    bool ended = true;
    if (countEmpty(board)>0) return false;
    if (findPairDown(board)) return false;
    rotateBoard(board);
    if (findPairDown(board)) ended = false;
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return ended;
}

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


    uint8_t board[SIZE][SIZE];
    initBoard(board);

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
                    success = moveLeft(board);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    success = moveRight(board);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    success = moveUp(board);
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    success = moveDown(board);
                }
            }
        }

        if (success) {
            if(countAdd--) {
                drawBoard(board, window, sprite, text);
            } else {
                addRandom(board);
                drawBoard(board, window, sprite, text);
                success = false;
                countAdd = 5;
            }
            isEnd = gameEnded(board);
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