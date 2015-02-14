/**
 *
 *
*/
#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <cstring>
using namespace std;

#include "node.hpp"
#include "graph.hpp"

#define STUDENT_TABLE_SQL      "SELECT * FROM student"
#define STUDENT_JOIN_TABLE_SQL "SELECT * FROM j_student"

class Student : public Node
{
    public:
        Student();
        Student(int argc, char **argv, char **colName);
        ~Student();

        static int joinGraph(void *data, int argc, char **argv, char **colName);
        static int populateGraph(void *data, int argc, char **argv, char **colName);

    private:
        int id;
        string name;
        int version;
};

#endif
