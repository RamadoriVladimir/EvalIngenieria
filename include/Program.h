#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "GamesManager.h"
#include "UserManager.h"
#include "UsersGamesManager.h"

class Program {
private:
    std::shared_ptr<SqliteDatabaseHandler> dbHandler;
    std::unique_ptr<GamesManager> gamesManager;
    std::unique_ptr<UserManager> userManager;
    std::unique_ptr<UsersGamesManager> usersGamesManager;

    void MainMenu();
    void UserMenu();
    void GameMenu();
    void AssociationMenu();
    void JoinMenu();
    void BackToMenu();
    void ClearScreen();

public:
    Program(std::shared_ptr<SqliteDatabaseHandler> dbHandler);
    void run();
};
