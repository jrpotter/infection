/**
 * Represents a user of the Khan Academy system, and also serves
 * as a node in a graph representing the coaching relationships
 * between students.
 *
 * Note the user should mirror the fields defined in the SQL schema.
 * In particular, we attach a version to the student, representing
 * the version of the website the student is currently on.
 *
 * Author: Joshua Potter
 * Date: February 14th, 2015
*/
#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <cstring>
using namespace std;

#include "sql.hpp"
#include "node.hpp"
#include "graph.hpp"

class Student : public Node
{
    public:
        Student();
        Student(int argc, char **argv, char **colName);
        ~Student();
        Student(const Student& other);
        Student& operator= (const Student& other);
        Student(Student&& other);
        Student& operator= (Student&& other);

        // Static methods to pull in SQL data and populate the graph relating
        // students (note the @data parameter is this graph cast to void *)
        static int joinGraph(void *data, int argc, char **argv, char **colName);
        static int populateGraph(void *data, int argc, char **argv, char **colName);

    private:
        string name;
        int version;
};

#endif
