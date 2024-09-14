#pragma once

#include <string>
#include <vector>
#include <memory>
#include <SqliteDatabaseHandler.h>
#include <UserManager.h>
#include <GamesManager.h>

class UsersGamesManager
{
private:
    std::shared_ptr<SqliteDatabaseHandler> dbHandler;

    User rowToUser(const Row& row);
    Game rowToGame(const Row& row);

public:
    UsersGamesManager(std::shared_ptr<SqliteDatabaseHandler> dbHandler);

    bool addAssociation(int userId, int gameId);
    bool removeAssociation(int userId, int gameId);
    std::vector<std::pair<User, Game>> innerJoin();
    std::vector<std::pair<User, Game>> leftJoin();
    std::vector<std::pair<User, Game>> rightJoin();
    std::vector<std::pair<User, Game>> outerJoin();
    std::vector<User> findUsersByGame(int gameId);
    std::vector<Game> findGamesByUser(int userId);    
};
