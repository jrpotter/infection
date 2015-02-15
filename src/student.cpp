#include "student.hpp"

// Seeding
// ==========================================================

int Student::populateGraph(void *data, int argc, char **argv, char **colName) {
    Graph *graph = static_cast<Graph*>(data);
    Student *student = new Student(argc, argv, colName);
    graph->addNode(student->id, student);
    return 0;
}

int Student::joinGraph(void *data, int argc, char **argv, char **colName) {

    int mentorId = 0;
    int menteeId = 0;

    for(int i = 0; i < argc; i++) {
        if(strcmp(colName[i], "mentor") == 0) {
            mentorId = stoi(argv[i]);
        } else if(strcmp(colName[i], "mentee") == 0) {
            menteeId = stoi(argv[i]);
        }
    }

    // Establish link
    if(mentorId != 0 && menteeId != 0) {
        Graph *graph = static_cast<Graph*>(data);
        graph->connect(mentorId, menteeId);
    }

    return 0;
}


// Constructors/Destructor
// ==========================================================

Student::Student()
        :Node(0)
        ,name("")
        ,version(0)
{}

// The following is called when constructing from the database
// Simply loop through column names, setting members to the
// correct column entry
Student::Student(int argc, char **argv, char **colName)
        :Node(0)
        ,name("")
        ,version(0)
{
    for(int i = 0; i < argc; i++) {
        if(strcmp(colName[i], "id") == 0) {
            id = stoi(argv[i]);
        } else if(strcmp(colName[i], "name") == 0) {
            name = argv[i];
        } else if(strcmp(colName[i], "version") == 0) {
            version = stoi(argv[i]);
        }
    }
}

Student::~Student()
{}

Student::Student(const Student& other)
        :Node(other)
        ,name(other.name)
        ,version(other.version)
{}
        
Student& Student::operator= (const Student& other)
{
    if(this != &other) {
        Node::operator=(other);
        name = other.name;
        version = other.version;
    }

    return *this;
}

Student::Student(Student&& other)
        :Node(other)
        ,name(other.name)
        ,version(other.version)
{}
       
Student& Student::operator= (Student&& other)
{
    if(this != &other) {
        Node::operator=(std::move(other));
        name = other.name;
        version = other.version;
    }

    return *this;
}
