#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

// Define player class
class Player {
public:
    Player(const std::string& name) : name(name), isSpectator(false) {}

    std::string getName() const { return name; }
    bool getIsSpectator() const { return isSpectator; }
    void becomeSpectator() { isSpectator = true; }

private:
    std::string name;
    bool isSpectator;
};

// Define land class
class Land {
public:
    Land(int id) : id(id), owner(nullptr) {}

    int getId() const { return id; }
    Player* getOwner() const { return owner; }
    void setOwner(Player* player) { owner = player; }

private:
    int id;
    Player* owner;
};

// Define game class
class Game {
public:
    Game(int numLands, const std::vector<std::string>& playerNames) {
        // Initialize lands
        for (int i = 0; i < numLands; ++i) {
            lands.push_back(new Land(i));
        }

        // Initialize players
        for (const std::string& name : playerNames) {
            players.push_back(new Player(name));
        }

        // Randomly assign lands to players
        srand(time(0));
        for (Land* land : lands) {
            land->setOwner(players[rand() % players.size()]);
        }
    }

    ~Game() {
        for (Player* player : players) {
            delete player;
        }
        for (Land* land : lands) {
            delete land;
        }
    }

    void start() {
        while (true) {
            printGameState();
            std::string attackerName, defenderName;
            int landId;
            std::cout << "Enter attacker name, defender name, and land ID: ";
            std::cin >> attackerName >> defenderName >> landId;

            Player* attacker = getPlayerByName(attackerName);
            Player* defender = getPlayerByName(defenderName);
            Land* land = getLandById(landId);

            if (attacker && defender && land && land->getOwner() == defender) {
                battle(attacker, defender, land);
                if (isGameOver()) {
                    std::cout << attacker->getName() << " wins the game!" << std::endl;
                    break;
                }
            } else {
                std::cout << "Invalid input. Try again." << std::endl;
            }
        }
    }

private:
    std::vector<Player*> players;
    std::vector<Land*> lands;

    Player* getPlayerByName(const std::string& name) {
        for (Player* player : players) {
            if (player->getName() == name && !player->getIsSpectator()) {
                return player;
            }
        }
        return nullptr;
    }

    Land* getLandById(int id) {
        for (Land* land : lands) {
            if (land->getId() == id) {
                return land;
            }
        }
        return nullptr;
    }

    void battle(Player* attacker, Player* defender, Land* land) {
        // Simple battle logic based on random outcome
        if (rand() % 2 == 0) {
            std::cout << attacker->getName() << " wins the battle for land " << land->getId() << "!" << std::endl;
            land->setOwner(attacker);
            defender->becomeSpectator();
        } else {
            std::cout << defender->getName() << " defends land " << land->getId() << " successfully!" << std::endl;
            attacker->becomeSpectator();
        }
    }

    bool isGameOver() {
        int activePlayers = 0;
        for (Player* player : players) {
            if (!player->getIsSpectator()) {
                ++activePlayers;
            }
        }
        return activePlayers == 1;
    }

    void printGameState() {
        std::cout << "Game State:" << std::endl;
        for (Land* land : lands) {
            if (land->getOwner()) {
                std::cout << "Land " << land->getId() << " owned by " << land->getOwner()->getName() << std::endl;
            }
        }
    }
};

int main() {
    std::vector<std::string> playerNames = {"Alice", "Bob", "Charlie", "Diana"};
    Game game(10, playerNames);
    game.start();
    return 0;
}
