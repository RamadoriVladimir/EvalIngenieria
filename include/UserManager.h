#pragma once

#include <string>
#include <vector>
#include <memory>
#include <SqliteDatabaseHandler.h>

class User {
private:
    int id;
    std::string name;

public:
    User(int id, const std::string& name) : id(id), name(name) {}

    int getId() const { return id; }
    std::string getName() const { return name; }
    
    void setName(const std::string& newName) { name = newName; }
};


class UserManager
{
private:
    std::shared_ptr<SqliteDatabaseHandler> dbHandler;

    User mapRowToUser(const Row& row) {
        int id = std::stoi(row.at("id")); 
        std::string name = row.at("name");
        return User(id, name);
    }

public:
    UserManager(std::shared_ptr<SqliteDatabaseHandler> dbHandler) : dbHandler(dbHandler) {}

    bool addUser(const User& user) {
        dbHandler->prepareQuery("INSERT INTO users (name) VALUES (?);");
        dbHandler->addParameter(1, user.getName());
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    bool removeUser(int id) {
        dbHandler->prepareQuery("DELETE FROM users WHERE id = ?;");
        dbHandler->addParameter(1, std::to_string(id));
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    bool updateUser(const User& user) {
        dbHandler->prepareQuery("UPDATE users SET name = ? WHERE id = ?;");
        dbHandler->addParameter(1, user.getName());
        dbHandler->addParameter(2, std::to_string(user.getId()));
        dbHandler->execute();
        return !dbHandler->getErrorStatus();
    }

    std::vector<User> listUsers() {
        std::vector<User> users;
        dbHandler->prepareQuery("SELECT * FROM users;");
        Table result = dbHandler->fetchAll();
        for (const auto& row : result) {
            users.push_back(mapRowToUser(row));
        }
        return users;
    }
};
