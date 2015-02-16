/**
 * The following is the starting point of the Khan Academy assignment "Infection."
 *
 * The basic goal is to provide a means of maintaining the same version of sites
 * across users (who may or may not be coaching one another). In this way, if someone
 * is using one version of a site and coaching another person, the person being
 * coached should be using the same version of the site as well.
 *
 * For the sake of simplicity, I maintain a couple of assumptions about the problem:
 *
 * 1) If someone is coaching someone else who is using a higher version of the program,
 *    then the coach should be upgraded to the better version.
 *
 * 2) As provided by the problem description, a person cannot coach themselves.
 *
 * 3) Khan Academy uses SQLite3 (which I incorporated since I am unsure what database
 *    environment they actually use). This should be simple to modify if the time
 *    comes for a more sophisticated environment, and provides a more realistic implementation.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#include <cstring>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

#include "infection.hpp"
#include "window.hpp"

extern "C" {
#include "sqlite3.h"
}

#define DEFAULT_DATABASE_NAME "infection.db"
#define DEFAULT_TESTFILE_NAME "infection_test.sql"

int main(int argc, char** argv) {

    // Configuration variables
    char *zErrMsg = 0;
    string testFilePath = "";
    string databasePath = DEFAULT_DATABASE_NAME;

    // Allow users to interact with graph
    bool visual = false;

    // Flag to determine whether conducting a total or limited infection
    bool totalInfection = true;

    // Specifies either id of student or limit desired
    int input = -1;


    // Command Line Arguments
    // ==========================================================

    // Before the program is run, we instantiate the database that is pulled from.
    // The name of the file to test should be passed in as a command line argument.
    // The database is then populated with this information. This only needs to be
    // run if a different database should be used besides the default (or if this
    // is the first time running the program entirely).
    for(int i = 1; i < argc; i+=2) {

        // Requesting a total infection
        if(strcmp(argv[i], "-t") == 0) {
            if(i + 1 == argc) {
                cerr << "Please specify a user id." << endl;
                return 1;
            } else {
                input = stoi(argv[i+1]);
            }

        // Requesting a limited infection
        } else if(strcmp(argv[i], "-l") == 0) {
            if(i + 1 == argc) {
                cerr << "Please specify a limit." << endl;
                return 1;
            } else {
                totalInfection = false;
                input = stoi(argv[i+1]);
            }

        // Specify the test file to seed the database with (this can be used
        // in conjunction with the -d flag)
        } else if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 == argc) {
                cerr << "Please specify a test file to seed database with." << endl;
                return 1;
            } else {
                testFilePath = argv[i+1];
            }

        // Specify the path of the database to pull from
        } else if(strcmp(argv[i], "-d") == 0) {
            if(i + 1 == argc) {
                cerr << "Please specify the path of the database to create or pull from." << endl;
                return 1;
            } else {
                databasePath = argv[i+1];
            }

        // Allow visuals
        } else if(strcmp(argv[i], "-v") == 0) {
            visual = true;

        // Display help text. Do not continue anything else.
        } else if(strcmp(argv[i], "-h") == 0) {

            cout << "Help documentation: " << endl;
            cout << "-f {file}: Path of file to seed database with (needs to exist)" << endl;
            cout << "-d {database}: Path of database (does not need to exist) to seed" << endl;
            cout << "-t {number}: Pass id of student to see total infected" << endl;
            cout << "-l {number}: Pass target infected to see optimal student" << endl;
            cout << "-v: Show an interactive visual interface for infections" << endl;
            cout << "-h: Print out this help documentation" << endl;
            cout << endl << endl;

            return 0;

        // Invalid argument passed. Simply redirect and exit.
        } else {
            cerr << "Invalid argument. Pass in -h for help";
            return 1;
        }
    }


    // Connections
    // ==========================================================

    // Next we try and instantiate the values passed (or defaulted to), creating the necessary SQLite 
    // connection, creating the database if necessary, and seeding the data if specified
    sqlite3 *database;
    if(sqlite3_open(databasePath.c_str(), &database) != SQLITE_OK) {
        cerr << "Couldn't open database: " << databasePath << endl;
        cerr << sqlite3_errmsg(database) << endl;
        return 1;
    }

    // We read in the entire file at once, and immediately execute it into the specified table.
    // This is only in the case that a file path was specified at all; if not, we assume that
    // the table has already been initialized with the test data desired.
    if(testFilePath.size() != 0) {

        bool fileError = false;
        ifstream testFile(testFilePath.c_str(), ifstream::in);

        // Attempt to execute file contents
        if(testFile.is_open()) {
            stringstream buffer;
            buffer << testFile.rdbuf();
            if(sqlite3_exec(database, buffer.str().c_str(), 0, 0, &zErrMsg) != SQLITE_OK) {
                cerr << "Could not write out to database: " << zErrMsg << endl;
                sqlite3_free(zErrMsg);
                fileError = true;
            }
        } else {
            cerr << "Could not open file path: " << testFilePath << endl;
            fileError = true;
        }

        // Cleanup. We must still close the database at this point since we could not load
        // in the desired test data.
        testFile.close();
        if(fileError) {
            sqlite3_close(database);
            return 1;
        }
    }


    // Processing
    // ==========================================================
    // At this point, we begin instantiating the graph necessary to find all strongly connected
    // component within the relationship defined by the database given. We pull in all relationships
    // from the local database specified, build up the necessary graph, and begin a total or local
    // infection.
    Graph *relations = new Graph();

    // Fills out the graph with the students in the database
    if(sqlite3_exec(database, STUDENT_TABLE_SQL, Student::populateGraph, (void*)relations, &zErrMsg) != SQLITE_OK) {
        cerr << "Could not execute SQL statement: " << STUDENT_TABLE_SQL << endl;
        cerr << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // Establishes relationships between all students; this will allow for the creation of strongly
    // connected components.
    if(sqlite3_exec(database, STUDENT_JOIN_TABLE_SQL, Student::joinGraph, (void*)relations, &zErrMsg) != SQLITE_OK) {
        cerr << "Could not execute SQL statement: " << STUDENT_JOIN_TABLE_SQL << endl;
        cerr << zErrMsg << endl;
        sqlite3_free(zErrMsg);
    }

    // The following is the graph of the original but with all strongly connected components
    // representing a node (with the number of nodes in each "meta" node). We can use this graph
    // to determine a total and limited infection accurately and relatively quickly (at least
    // in the limited case).
    Graph *connectedRelations = relations->getStrongComponents();

    // Conduct the infection if passing in argument
    if(input != -1) {
        Infection infection(relations, connectedRelations);
        if(totalInfection) {
            infection.total_infection(static_cast<Student*>(relations->nodes[input]));
        } else {
            infection.limit_infection(input);
        }

    // Allow for an interactive component if desired
    } else if(visual) {
        Window window(relations, connectedRelations);
        window.run();        
    }


    // Cleanup
    // ==========================================================

    delete relations;
    delete connectedRelations;
    sqlite3_close(database);

    return 0;
}
