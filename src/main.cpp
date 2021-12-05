#include <iostream>
#include <cstdio>
#include <set>
#include "game.h"

bool handleInput(Game& game, char inKey);

int main() {
    Game& game = Game::init();
    std::set<char> acceptedValues{
        'a',  // left
        's',  // down
        'd',  // right
        'w',  // up
        // 'z',  // undo
        'q'  // quit
    };
    char inKey{'\0'};

    game.displayBoard();
    inKey = std::getchar();
    
    while (acceptedValues.count(inKey) == 0 || handleInput(game, inKey)) {
        if (!game.canPlay()) {
            std::cout << "Game over!" << std::endl;
            break;
        }

        game.displayBoard();
        inKey = std::getchar();
    }

    return 0;
}

bool handleInput(Game& game, char inKey) {
    switch (inKey) {
    case 'a':
        if (game.canSlideLeft()) {
            game.slideLeft();
            game.fillCell();
        }
        return true;
    case 's':
        if (game.canSlideDown()) {
            game.slideDown();
            game.fillCell();
        }
        return true;
    case 'd':
        if (game.canSlideRight()) {
            game.slideRight();
            game.fillCell();
        }
        return true;
    case 'w':
        if (game.canSlideUp()) {
            game.slideUp();
            game.fillCell();
        }
        return true;
    // case 'z':
    //     break;
    case 'q':
        return false;
    default:
        // Should never come here
        return false;
    }
}
