/**
 * The following is the starting point of the Khan Academy assignment "Infection."
 *
 * The basic goal is to provide a means of maintaining the same version of sites
 * across users (who may or may not be coaching one another). In this way, if someone
 * is using one version of a site and coaching another person, the person being
 * coached should be using the same version of the site as well.
 *
 * For the sake of simplicity, I maintain a couple of assumptions about the problem:
 * 1) If someone is coaching someone else who is using a higher version of the program,
 *    then the coach should be upgraded to the better version.
 *
 * 2) As provided by the problem description, a person cannot coach themselves.
 *
 * 3) Khan Academy uses SQLite3 (which I incorporated since I am unsure what database
 *    environment they actually use). This should be simple to modify if the time
 *    comes to a more sophisticated environment, and provides a more realistic implementation.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#include <cstring>
#include <iostream>

extern "C" {
#include "sqlite3.h"
}

#define DEFAULT_DATABASE_NAME "infection.db"
#define DEFAULT_TESTFILE_NAME "infection_test.sql"

int main(int argc, char** argv) {

    std::string testFilePath = DEFAULT_TESTFILE_NAME;
    std::string databasePath = DEFAULT_DATABASE_NAME;

    // Before the program is run, we instantiate the database that is pulled from.
    // The name of the file to test should be passed in as a command line argument.
    // The database is then populated with this information. This only needs to be
    // run if a different database should be used besides the default (or if this
    // is the first time running the program entirely).
    for(int i = 1; i < argc; i+=2) {

        // Specify the test file to seed the database with (this can be used
        // in conjunction with the -d flag)
        if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 == argc) {
                std::cerr << "Please specify a test file to seed database with." << std::endl;
                return 1;
            } else {
                testFilePath = argv[i+1];
            }

        // Specify the path of the database to pull from
        } else if(strcmp(argv[i], "-d") == 0) {
            if(i + 1 == argc) {
                std::cerr << "Please specify the path of the database to create or pull from." << std::endl;
                return 1;
            } else {
                databasePath = argv[i+1];
            }

        // Display help text. Do not continue anything else.
        } else if(strcmp(argv[i], "-h") == 0) {

            std::cout << "Help documentation: " << std::endl;
            std::cout << "-f {file}: Path of file to seed database with (needs to exist)" << std::endl;
            std::cout << "-d {database}: Path of database (does not need to exist) to seed" << std::endl;
            std::cout << "-h: Print out this help documentation";
            std::cout << std::endl << std::endl;

            return 0;

        // Invalid argument passed. Simply redirect and exit.
        } else {
            std::cerr << "Invalid argument. Pass in -h for help";
            return 1;
        }
    }

    // Next we try and instantiate the values passed (or defaulted to), creating the necessary SQLite 
    // connection, creating the database if necessary, and seeding the data if specified
    sqlite3 *database;
    if(sqlite3_open(databasePath.c_str(), &database) != SQLITE_OK) {
        std::cerr << "Couldn't open database: " << std::endl;
        std::cerr << sqlite3_errmsg(database) << std::endl;
        return 1;
    }





    sqlite3_close(database);
        

    return 0;
}
