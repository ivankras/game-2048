#pragma once
#include <vector>
#include <ctime>
#include <cstdlib>

#define BOARD_SIZE 4
#define N2 6
#define D2 9
// TODO: remove array, use vector


class Game {
private:
    int _score;
    std::vector<std::vector<int>> _board;

    Game();
    bool _flipCoin();
    int _weighted2or4();
    int _getCoord();
    void _getCoords(int& x, int& y);

public:
    static Game& init() {
        static Game instance;
        srand(time(0));
        return instance;
    }

    Game(Game const&) = delete;
    Game(Game&&) = delete;
    Game operator=(Game const&) = delete;
    Game& operator=(Game&&) = delete;

    /**
     * @brief Select an empty cell at random then fill it with a
     *  randomly chosen 2 or 4 using the probability p2=N2/D2
     */
    void fillCell();

    /**
     * @brief Display the board and score
     */
    void displayBoard();

    /** 
     * @return true if the player can legally slide in the given direction
     * @return false otherwise
     */
    bool canSlideLeft();
    bool canSlideRight();
    bool canSlideUp();
    bool canSlideDown();

    /** 
     * @return true if the player can undo their last slide
     * @return false otherwise
     */
    // bool canUndo();

    /**
     * @brief Slide the board in the given direction and update the score
     */
    void slideLeft();
    void slideRight();
    void slideUp();
    void slideDown();

    /**
     * @brief One-move undo; it is a legal move if a slide has been previously performed
     * and the last move was not an undo. If no slides are allowed but undo is allowed,
     * the game is not over
     */
    // void undo();

    /** 
     * @return true if the player has any possible moves left
     * @return false otherwise
     */
    bool canPlay();
};
