#include <iostream>
#include <memory>
#include "SqliteDatabaseHandler.h"
#include "Program.h"

int main() {
    try {
        auto dbHandler = std::make_shared<SqliteDatabaseHandler>();
        dbHandler->setConfFile("configuration.ini");

        Program program(dbHandler);

        program.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
