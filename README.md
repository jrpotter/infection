Infection
=========

Total Infection
---------------
 
When rolling out big new features, we like to enable them slowly, starting with just the KA 
team, then a handful of users, then some more users, and so on, until we’ve ramped up to 
100%. This insulates the majority of users from bad bugs that crop up early in the life of a 
feature. 
 
This strategy can cause problems somewhat unique to our user base. It’s not uncommon for a 
classroom of students to be using the site together, so it would be confusing to show half of 
them a completely different version of the site. Children are not software engineers and often 
have a poor understanding of deployment and a/b testing, so inconsistent colors, layout, and 
interactions effectively mean the site is broken. 
 
Ideally we would like every user in any given classroom to be using the same version of the 
site. Enter “infections”. We can use the heuristic that each teacher­student pair should be on 
the same version of the site. So if A coaches B and we want to give A a new feature, then B 
should also get the new feature. Note that infections are transitive ­ if B coaches C, then C 
should get the new feature as well. Also, infections are transferred by both the “coaches” and 
“is coached by” relations. 
 
First, model users (one attribute of a user is the version of the site they see) and the coaching relations between 
them. A user can coach any number of ​other users. You don’t need to worry about handling self­-coaching relationships. 
 
Now implement the infection algorithm. Starting from any given user, the entire connected 
component of the coaching graph containing that user should become infected.

 
Limited Infection
-----------------

The problem with infection is lack of control over the number of users that eventually become 
infected. Starting an infection could cause only that person to become infected or at the 
opposite (unrealistic) extreme it could cause all users to become infected. 
 
We would like to be able to infect close to a given number of users. Ideally we’d like a coach 
and all of their students to either have a feature or not. However, that might not always be 
possible. 
 
Implement a procedure for limited infection. You will not be penalized for interpreting the 
specification as you see fit. There are many design choices and tradeoffs, so be prepared to 
justify your decisions. 


How To Install
--------------

Note the following project relies on SQLite3 and SFML, the former of which is bundled in the
project. To install the latter, refer to [SFML Download](http://www.sfml-dev.org/download.php).

Once SFML has been installed, build the project as follows:
```
gcc -c -Iinclude src/sqlite3.c -lpthread -ldl
g++ main.cpp sqlite3.o src/*.cpp -Iinclude \
    -lpthread -ldl -lsfml-graphics -lsfml-window -lsfml-system \
    -std=c++11 -o infection
```


Running Infection
-----------------

In order for "infection" to run correctly, there must be a SQLite database file to pull from,
following the schema provided in the two test cases bundled with the project. If no database
exists, one will try to be generated ("infection.db"), based on the passed seed file (via the
-f flag).

Afterwards run the program according to the following flags:
* -f {file}: Path of file to seed database with (needs to exist)
* -d {database}: Path of database (does not need to exist) to seed and pull from
* -t {number}: Pass id of student to see total infected
* -l {number}: Pass target infected to see optimal student
* -v: Show an interactive visual interface for infections
* -h: Print out this help documentation

In particular, note the -t and -l flags. If the -t flag is passed, we infect as many users
as possible from the given id, and this grouping is displayed. If the -l flag is passed,
the program tries to find the student id that infects a number of students closest to the
value passed.


Visual Interface
----------------

The following allows one to have a visual representation of total infections in particular.
When selecting a node we generate a total infection from the selected node.

![Screenshot](https://raw.githubusercontent.com/jrpotter/Infection/master/assets/screenshot.png)

