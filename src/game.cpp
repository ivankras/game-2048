#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include "game.h"

Game::Game() : _score{0}, _undoable{false} {
    for (int i=0; i < BOARD_SIZE; ++i) {
        std::vector<int> emptyRow(BOARD_SIZE, 0);
        _board.push_back(emptyRow);
    }

    fillCell();
    fillCell();
}

bool Game::_flipCoin() {
    rand();
	return rand() % 2 == 0;
}

int Game::_weighted2or4() {
    rand();
	return rand() % D2 < N2 ? 2 : 4;
}

int Game::_getCoord() {
    int result{0};
    int currentSize{BOARD_SIZE};
    
    while (currentSize > 1) {
        currentSize /= 2;
        result = _flipCoin() ? result + currentSize : result;
    }

    return result;
}

void Game::_getCoords(int& row, int& col) {
    row = _getCoord();
    col = _getCoord();

    while (_board.at(row).at(col) != 0) {
        row = _getCoord();
        col = _getCoord();
    }
}

void Game::_saveBoard() {
    _previousState = _board;
}

void Game::fillCell() {
    int row, col;
    _getCoords(row, col);
    _board.at(row).at(col) = _weighted2or4();
}

void Game::displayBoard() {
    std::cout << "\033[2J\033[1;1H";  // Clear screen
    
    std::cout << "|_______________________|" << std::endl;
    std::cout << "|-----|-----|-----|-----|" << std::endl;
    for (int row=0; row < BOARD_SIZE; ++row) {    
        std::cout << "|";
        for (int col=0; col < BOARD_SIZE; ++col) {
            const int& value = _board.at(row).at(col);
            if (value < 10) {
                std::cout << "  " << value << "  |";
            } else if (value < 100) {
                std::cout << "  " << value << " |";
            } else if (value < 1000) {
                std::cout << " " << value << " |";
            } else if (value < 10000) {
                std::cout << " " << value << "|";
            } else {
                std::cout << value << "|";
            }
        }
        std::cout << std::endl;
    }
    std::cout << "|-----|-----|-----|-----|" << std::endl;
    std::cout << "|_______________________|" << std::endl;
    std::cout << "Score:\t" << _score << std::endl;
}

bool Game::canSlideLeft() {
    for (int row=0; row < BOARD_SIZE; ++row) {
        for (int col=BOARD_SIZE-1; col > 0; --col) {
            const int& curVal = _board.at(row).at(col);
            if (curVal == 0) continue;

            const int& nextVal = _board.at(row).at(col-1);
            if (nextVal == 0 || curVal == nextVal) {
                return true;
            }
        }
    }

    return false;
}

bool Game::canSlideRight() {
    for (int row=0; row < BOARD_SIZE; ++row) {
        for (int col=0; col < BOARD_SIZE-1; ++col) {
            const int& curVal = _board.at(row).at(col);
            if (curVal == 0) continue;

            const int& nextVal = _board.at(row).at(col+1);
            if (nextVal == 0 || curVal == nextVal) {
                return true;
            }
        }
    }

    return false;
}

bool Game::canSlideUp() {
    for (int row=BOARD_SIZE-1; row > 0; --row) {
        for (int col=0; col < BOARD_SIZE; ++col) {
            const int& curVal = _board.at(row).at(col);
            if (curVal == 0) continue;

            const int& nextVal = _board.at(row-1).at(col);
            if (nextVal == 0 || curVal == nextVal) {
                return true;
            }
        }
    }

    return false;
}

bool Game::canSlideDown() {
    for (int row=0; row < BOARD_SIZE-1; ++row) {
        for (int col=0; col < BOARD_SIZE; ++col) {
            const int& curVal = _board.at(row).at(col);
            if (curVal == 0) continue;

            const int& nextVal = _board.at(row+1).at(col);
            if (nextVal == 0 || curVal == nextVal) {
                return true;
            }
        }
    }

    return false;
}

bool Game::canUndo() {
    return _undoable && _score <= TOP_UNDO_SCORE;
}

void Game::slideLeft() {
    _saveBoard();
    for (int row=0; row < BOARD_SIZE; ++row) {
        int dest = 0;
        int hold = -1;
        
        for (int col=0; col < BOARD_SIZE; ++col) {
            int& curVal = _board.at(row).at(col);
            if (curVal) {
                int& destVal = _board.at(row).at(dest);
                if (curVal == hold) {
                    curVal *= 2;
                    _score += curVal;

                    destVal = curVal;
                    ++dest;

                    hold = -1;
                } else if (hold != -1) {
                    destVal = hold;
                    ++dest;
                    hold = curVal;
                } else {
                    hold = curVal;
                }
            }
        }

        if (hold != -1) {
            _board.at(row).at(dest++) = hold;
        }

        while (dest < BOARD_SIZE) {
            _board.at(row).at(dest++) = 0;
        }
    }

    _undoable = true;
}

void Game::slideRight() {
    _saveBoard();
    for (int row=0; row < BOARD_SIZE; ++row) {
        int dest = BOARD_SIZE-1;
        int hold = -1;
        
        for (int col=BOARD_SIZE-1; col >= 0; --col) {
            int& curVal = _board.at(row).at(col);
            if (curVal) {
                int& destVal = _board.at(row).at(dest);
                if (curVal == hold) {
                    curVal *= 2;
                    _score += curVal;

                    destVal = curVal;
                    --dest;

                    hold = -1;
                } else if (hold != -1) {
                    destVal = hold;
                    --dest;
                    hold = curVal;
                } else {
                    hold = curVal;
                }
            }
        }

        if (hold != -1) {
            _board.at(row).at(dest--) = hold;
        }

        while (dest >= 0) {
            _board.at(row).at(dest--) = 0;
        }
    }

    _undoable = true;
}

void Game::slideUp() {
    _saveBoard();

    for (int col=0; col < BOARD_SIZE; ++col) {
        int dest = 0;
        int hold = -1;
        
        for (int row=0; row < BOARD_SIZE; ++row) {
            int& curVal = _board.at(row).at(col);
            if (curVal) {
                int& destVal = _board.at(dest).at(col);
                if (curVal == hold) {
                    curVal *= 2;
                    _score += curVal;

                    destVal = curVal;
                    ++dest;

                    hold = -1;
                } else if (hold != -1) {
                    destVal = hold;
                    ++dest;
                    hold = curVal;
                } else {
                    hold = curVal;
                }
            }
        }

        if (hold != -1) {
            _board.at(dest++).at(col) = hold;
        }

        while (dest < BOARD_SIZE) {
            _board.at(dest++).at(col) = 0;
        }
    }

    _undoable = true;
}

void Game::slideDown() {
    _saveBoard();

    for (int col=0; col < BOARD_SIZE; ++col) {
        int dest = BOARD_SIZE-1;
        int hold = -1;
        
        for (int row=BOARD_SIZE-1; row >= 0; --row) {
            int& curVal = _board.at(row).at(col);
            if (curVal) {
                int& destVal = _board.at(dest).at(col);
                if (curVal == hold) {
                    curVal *= 2;
                    _score += curVal;

                    destVal = curVal;
                    --dest;

                    hold = -1;
                } else if (hold != -1) {
                    destVal = hold;
                    --dest;
                    hold = curVal;
                } else {
                    hold = curVal;
                }
            }
        }

        if (hold != -1) {
            _board.at(dest--).at(col) = hold;
        }

        while (dest >= 0) {
            _board.at(dest--).at(col) = 0;
        }
    }

    _undoable = true;
}

void Game::undo() {
    _undoable = false;
    _board = _previousState;
}

bool Game::canPlay() {
    return canSlideLeft()
            || canSlideRight()
            || canSlideUp()
            || canSlideDown()
            || canUndo();
}