Infection
=========

[VERSION 1.0.0 - 04/27/2014]
 
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

A typical usage example would be the following:

```
./infection -f tests/test.sql -l 7
./infection -v
```

which will output 5 (i.e. the starting node that comes closest to infecting 7 other students)
and the image included below (the invocation with the -v flag). This call would also generate
an "infection.db" flag, containing the imported data.

Visual Interface
----------------

The -v flag allows one to have a visual representation of total infections in particular.
When selecting a node we generate a total infection from the selected node.

![Screenshot](https://github.com/jrpotter/Infection/blob/master/assets/screenshot.png)


Miscellaneous
-------------
There are other things I'd like to incorporate but decided against as I didn't really have much time left.
* I would like to work on a more sophisticated SQL schema that perhaps incorporates classes, schools, 
  counties, etc. so that I would not have to reload a table each time I wanted to run the function,
  but instead just call the infection tool at the desired level.
* I would like to incorporate limited infection into the visualization, perhaps allowing the user to
  just enter a number and see how the search is conducted (as this makes the dynamic programming
  component more clear).
* It would be fun to incorporate notions such as "immunity" where certain nodes do not change
  their version despite a parent node being different; all subsequent nodes would not be infected
  either in that case
* Again with the visualization, a dynamic means of generating a graph would be fun but would probably
  be more time consuming than its worth.
* Because of the recursive structure of nodes, it would be nice to be able to generalize the node to
  allow for an arbitrary level; for example, I have a node that represents a student but I should
  generalize this to also allow representation of a class. Templating the node (and consequently
  the graph) would probably be smart to do later on.
