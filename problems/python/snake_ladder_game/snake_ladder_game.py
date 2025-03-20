from enum import Enum
import random
import threading

class GameStatus(Enum):
    NEW = "NEW"
    STARTED = "STARTED"
    ENDED = "ENDED"

class Snake:
    def __init__(self, start, end):
        self.start = start
        self.end = end

    def get_start(self):
        return self.start

    def get_end(self):
        return self.end

class Ladder:
    def __init__(self, start, end):
        self.start = start
        self.end = end

    def get_start(self):
        return self.start

    def get_end(self):
        return self.end

class Board:

    def __init__(self):
        self.size = 100
        self.snakes = []
        self.ladders = []
        self.initialize_snakes_and_ladders()

    def initialize_snakes_and_ladders(self):
        # Initilize snakes
        self.snakes.append(Snake(16, 6))
        self.snakes.append(Snake(48, 26))
        self.snakes.append(Snake(64, 60))
        self.snakes.append(Snake(93, 73))

        # Initilize ladders
        self.ladders.append(Ladder(1, 38))
        self.ladders.append(Ladder(4, 14))
        self.ladders.append(Ladder(9, 31))
        self.ladders.append(Ladder(21, 42))
        self.ladders.append(Ladder(28, 84))
        self.ladders.append(Ladder(51, 67))
        self.ladders.append(Ladder(80, 99))

    def get_board_size(self):
        return self.size

    def get_new_position_after_snake_or_ladder(self, position):
        for snake in self.snakes:
            if snake.get_start() == position:
                return snake.get_end()
        
        for ladder in self.ladders:
            if ladder.get_end() == position:
                return ladder.get_start()
        return position

class Player:
    def __init__(self, name):
        self.name = name
        self.position = 0

    def get_name(self):
        return self.name

    def get_position(self):
        return self.position

    def set_position(self, position):
        self.position = position

class Dice:
    def roll(self):
        return random.randint(1, 6)

class SnakeLadderGame:
    def __init__(self, player_names):
        self.board = Board()
        self.dice = Dice()
        self.players = [Player(name) for name in player_names]
        self.curr_idx = 0

    def play(self):
        while not self.is_game_over():
            curr_player = self.players[self.curr_idx]
            dice_roll = self.dice.roll()
            new_pos = curr_player.get_position() + dice_roll

            if new_pos <= self.board.get_board_size():
                curr_player.set_position(self.board.get_new_position_after_snake_or_ladder(new_pos))
                print(f"{curr_player.get_name()} rolled a {dice_roll} and moved to position {curr_player.get_position()}")

            if curr_player.get_position() == self.board.get_board_size():
                print(f"{curr_player.get_name()} won. Game over!")
                break
            self.curr_idx = (self.curr_idx + 1) % len(self.players)


    def is_game_over(self):
        for player in self.players:
            if player.get_position() == self.board.get_board_size():
                return True
        return False

class GameHandler:
    instance = None
    lock = threading.Lock()

    def __init__(self):
        self.games = []
        self.theads = []

    def __new__(cls):
        with cls.lock:
            if not cls.instance:
                cls.instance = super().__new__(cls)
                cls.instance.games = []
        return cls.instance

    def start_new_game(self, player_names):
        game = SnakeLadderGame(player_names)
        self.games.append(game)
        thread = threading.Thread(target=game.play).start()
        self.theads.append(thread)

    def end_games(self):
        for thread in self.theads:
            thread.join()
        print("Finished all games!")

class Main:
    @staticmethod
    def run():
        game_handler = GameHandler()
        players1 = ["Player1", "Player2", "Player3"]
        game_handler.start_new_game(players1)

        players2 = ["Player4", "Player5"]
        game_handler.start_new_game(players2)

if __name__ == "__main__":
    Main.run()