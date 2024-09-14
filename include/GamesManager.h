#pragma once

#include <string>
#include <vector>
#include <memory>
#include <SqliteDatabaseHandler.h>

class Game {
private:
    int id;
    std::string name;

public:
    Game(int id, const std::string& name) : id(id), name(name) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    
    void setName(const std::string& newName) { name = newName; }
};

class GamesManager
{
private:
    std::shared_ptr<SqliteDatabaseHandler> dbHandler;

    Game mapRowToGame(const Row& row) {
        int id = std::stoi(row.at("id"));  
        std::string name = row.at("name");
        return Game(id, name);
    }

public:
    GamesManager(std::shared_ptr<SqliteDatabaseHandler> dbHandler) : dbHandler(dbHandler) {}

    bool addGame(const Game& game) {
        dbHandler->prepareQuery("INSERT INTO games (name) VALUES (?);");
        dbHandler->addParameter(1, game.getName());
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    bool removeGame(int id) {
        dbHandler->prepareQuery("DELETE FROM games WHERE id = ?;");
        dbHandler->addParameter(1, std::to_string(id));
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    bool updateGame(const Game& game) {
        dbHandler->prepareQuery("UPDATE games SET name = ? WHERE id = ?;");
        dbHandler->addParameter(1, game.getName());
        dbHandler->addParameter(2, std::to_string(game.getId()));
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    std::vector<Game> listGames() {
        std::vector<Game> games;
        dbHandler->prepareQuery("SELECT * FROM games;");
        Table result = dbHandler->fetchAll();
        for (const auto& row : result) {
            games.push_back(mapRowToGame(row));
        }
        return games;
    }
};