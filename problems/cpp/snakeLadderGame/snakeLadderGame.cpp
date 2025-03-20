#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <thread>
#include <mutex>

class Snake {
    int start;
    int end;

public:
    Snake(int start, int end) : start(start), end(end) {}

    int getStart() const { return start; }
    int getEnd() const { return end; }
};

class Ladder {
    int start;
    int end;

public:
    Ladder(int start, int end) : start(start), end(end) {}

    int getStart() const { return start; }
    int getEnd() const { return end; }
};

class Board {
private:
    int size;
    std::vector<Snake*> snakes;
    std::vector<Ladder*> ladders;

public:
    Board(int size) : size(size) {}

    void addSnake(Snake* snake) {
        snakes.push_back(snake);
    }

    void addLadder(Ladder* ladder) {
        ladders.push_back(ladder);
    }

    int getSize() const { return size; }
    int getNextPosition(int currPos) const {
        for (Snake* snake : snakes) {
            if (snake->getStart() == currPos) {
                return snake->getEnd();
            }
        }

        for (Ladder* ladder : ladders) {
            if (ladder->getEnd() == currPos) {
                return ladder->getStart();
            }
        }
        return currPos;
    }
};

class Dice {
public:
    int roll() const {
        return (std::rand() % 6) + 1;
    }
};

class Player {
private:
    std::string name;
    int position;

public:
    Player(std::string name) : name(name) {}

    std::string getName() const { return name; }
    int getPos() const { return position; }
    void setPos(int newPos) { position = newPos; }
};

class SnakeLadderGame {
private:
    Board board;
    std::vector<Player*> players;
    Dice dice;
    int curr_idx;
    bool isGameOver;

public:
    SnakeLadderGame(int boardSize, std::vector<std::string> playerNames)
        : board(boardSize), curr_idx(0), isGameOver(false) {
            for (std::string name : playerNames) {
                players.push_back(new Player(name));
            }
        }

    void setUpBoard() {
        // Initilize snakes
        board.addSnake(new Snake(16, 6));
        board.addSnake(new Snake(48, 26));
        board.addSnake(new Snake(64, 60));
        board.addSnake(new Snake(93, 73));

        // initilize ladders
        board.addLadder(new Ladder(1, 38));
        board.addLadder(new Ladder(4, 14));
        board.addLadder(new Ladder(9, 31));
        board.addLadder(new Ladder(21, 42));
        board.addLadder(new Ladder(28, 84));
        board.addLadder(new Ladder(51, 67));
        board.addLadder(new Ladder(80, 99));
    }

    Player* getCurrPlayer() {
        return players[curr_idx];
    }

    void movePlayer(Player* player, int steps) {
        int newPos = player->getPos() + steps;

        if (newPos > board.getSize()) {
            std::cout << "Cannot move, need exact number to finish!" << std::endl;
            return;
        }
        newPos = board.getNextPosition(newPos);
        player->setPos(newPos);
        std::cout << player->getName() << " moved to position " << newPos << std::endl;

        if (newPos == board.getSize()) {
            isGameOver = true;
            std::cout << player->getName() << " WON. GAME OVER! " << std::endl;
        }
        return;
    }

    void play() {
        if (players.empty()) {
            std::cout << "Cannot play as no players in the game!" << std::endl;
            return;
        }
        while (!isGameOver) {
            Player* player = getCurrPlayer();
            int roll = dice.roll();
            std::cout << player->getName() << " rolled a dice " << roll << std::endl;
            movePlayer(player, roll);
            curr_idx = (curr_idx + 1) % players.size();
        }
    }
};

int main() {
    std::vector<std::string> players = {"Player1", "Player2", "Player3"};
    SnakeLadderGame* game = new SnakeLadderGame(100, players);
    game->setUpBoard();
    game->play();

    return 0;
}