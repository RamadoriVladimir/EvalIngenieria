#include <UsersGamesManager.h>

UsersGamesManager::UsersGamesManager(std::shared_ptr<SqliteDatabaseHandler> dbHandler) : dbHandler(dbHandler)
{
    dbHandler->prepareQuery("CREATE TABLE IF NOT EXISTS Users_Games (user_id INTEGER, game_id INTEGER, FOREIGN KEY(user_id) REFERENCES users(id), FOREIGN KEY(game_id) REFERENCES games(id));");
    dbHandler->execute();
}

bool UsersGamesManager::addAssociation(int userId, int gameId)
{
    dbHandler->prepareQuery("INSERT INTO Users_Games(user_id, game_id) VALUES(?, ?)");
    dbHandler->addParameter(1, std::to_string(userId));
    dbHandler->addParameter(2, std::to_string(gameId));
    dbHandler->execute();
    return !dbHandler->getErrorStatus();
}

bool UsersGamesManager::removeAssociation(int userId, int gameId)
{
    dbHandler->prepareQuery("DELETE FROM Users_Games WHERE user_id = ? AND game_id = ?");
    dbHandler->addParameter(1, std::to_string(userId));
    dbHandler->addParameter(2, std::to_string(gameId));
    dbHandler->execute();
    return !dbHandler->getErrorStatus();
}

User UsersGamesManager::rowToUser(const Row& row)
{
    int id = std::stoi(row.at("user_id"));
    std::string name = row.at("user_name");
    return User(id, name);
}

Game UsersGamesManager::rowToGame(const Row& row)
{
    int id = std::stoi(row.at("game_id"));
    std::string name = row.at("game_name");
    return Game(id, name);
}

std::vector<std::pair<User, Game>> UsersGamesManager::innerJoin()
{
    std::vector<std::pair<User, Game>> result;
    dbHandler->prepareQuery("SELECT Users.id AS user_id, Users.name AS user_name, Games.id AS game_id, Games.name AS game_name "
                            "FROM Users "
                            "INNER JOIN Users_Games ON Users.id = Users_Games.user_id "
                            "INNER JOIN Games ON Games.id = Users_Games.game_id");

    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        User user = rowToUser(row);
        Game game = rowToGame(row);
        result.emplace_back(user, game);
    }
    return result;
}

std::vector<std::pair<User, Game>> UsersGamesManager::leftJoin()
{
    std::vector<std::pair<User, Game>> result;
    dbHandler->prepareQuery("SELECT Users.id AS user_id, Users.name AS user_name, Games.id AS game_id, Games.name AS game_name "
                            "FROM Users "
                            "LEFT JOIN Users_Games ON Users.id = Users_Games.user_id "
                            "LEFT JOIN Games ON Games.id = Users_Games.game_id");

    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        User user = rowToUser(row);
        Game game = rowToGame(row);
        result.emplace_back(user, game);
    }
    return result;
}

std::vector<std::pair<User, Game>> UsersGamesManager::rightJoin()
{
    std::vector<std::pair<User, Game>> result;
    dbHandler->prepareQuery("SELECT Users.id AS user_id, Users.name AS user_name, Games.id AS game_id, Games.name AS game_name "
                            "FROM Games "
                            "LEFT JOIN Users_Games ON Games.id = Users_Games.game_id "
                            "LEFT JOIN Users ON Users.id = Users_Games.user_id");

    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        User user = rowToUser(row);
        Game game = rowToGame(row);
        result.emplace_back(user, game);
    }
    return result;
}

std::vector<std::pair<User, Game>> UsersGamesManager::outerJoin()
{
    std::vector<std::pair<User, Game>> result;
    dbHandler->prepareQuery("SELECT Users.id AS user_id, Users.name AS user_name, Games.id AS game_id, Games.name AS game_name "
                            "FROM Users "
                            "LEFT JOIN Users_Games ON Users.id = Users_Games.user_id "
                            "LEFT JOIN Games ON Games.id = Users_Games.game_id "
                            "UNION "
                            "SELECT Users.id AS user_id, Users.name AS user_name, Games.id AS game_id, Games.name AS game_name "
                            "FROM Games "
                            "LEFT JOIN Users_Games ON Games.id = Users_Games.game_id "
                            "LEFT JOIN Users ON Users.id = Users_Games.user_id");

    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        User user = rowToUser(row);
        Game game = rowToGame(row);
        result.emplace_back(user, game);
    }
    return result;
}

std::vector<User> UsersGamesManager::findUsersByGame(int gameId)
{
    std::vector<User> result;
    dbHandler->prepareQuery("SELECT Users.id AS user_id, Users.name AS user_name "
                            "FROM Users "
                            "INNER JOIN Users_Games ON Users.id = Users_Games.user_id "
                            "WHERE Users_Games.game_id = ?");

    dbHandler->addParameter(1, std::to_string(gameId));
    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        result.push_back(rowToUser(row));
    }
    return result;
}

std::vector<Game> UsersGamesManager::findGamesByUser(int userId)
{
    std::vector<Game> result;
    dbHandler->prepareQuery("SELECT Games.id AS game_id, Games.name AS game_name "
                            "FROM Games "
                            "INNER JOIN Users_Games ON Games.id = Users_Games.game_id "
                            "WHERE Users_Games.user_id = ?");

    dbHandler->addParameter(1, std::to_string(userId));
    std::vector<Row> rows = dbHandler->fetchAll();
    for (const auto& row : rows) {
        result.push_back(rowToGame(row));
    }
    return result;
}
