/**
 * The following is a utility class enabling maintaining a stack, while also being
 * able to quickly determine whether an object is in the stack. This is done by
 * composing the STL set and stack.
 *
 * This is necessary for implementing the Tarjan's Strongly Connected Component
 * algorithm, which maintains a stack to allow just one DFS search (instead of the
 * Kosaraju algorithm which requires two).
 *
 * Note, we also maintain an index which keeps count of how many elements were added
 * to the stack over the course of the stack's lifetime. This is useful for popping
 * off elements until a certain index has been achieved.
 *
 * Author: Joshua Potter
 * Date: February 14, 2015
*/
#ifndef INDEX_STACK_HPP
#define INDEX_STACK_HPP

#include <set>
#include <stack>


// IndexStack Declaration
// ==========================================================

template<typename T>
class IndexStack
{
    public:
        IndexStack();
        ~IndexStack();
        IndexStack(IndexStack& other);
        IndexStack& operator= (const IndexStack& other);
        IndexStack(IndexStack&& other);
        IndexStack& operator= (IndexStack&& other);

        // Set Operations
        bool find(T obj);

        // Stack Operations
        T pop();
        int push(T obj);

    private:
        unsigned int index;
        std::stack<T> ordering;
        std::set<T> collection;
};


// IndexStack Constructors
// ==========================================================

template<typename T>
IndexStack<T>::IndexStack()
              :index(0)
{}

template<typename T>
IndexStack<T>::~IndexStack()
{}

template<typename T>
IndexStack<T>::IndexStack(IndexStack<T>& other)
              :index(other.index)
              ,ordering(other.ordering)
              ,collection(other.collection)
{}

template<typename T>
IndexStack<T>& IndexStack<T>::operator= (const IndexStack<T>& other)
{
    if(this != &other) {
        index = other.index;
        ordering = other.ordering;
        collection = other.collection;
    }
    
    return *this;
}

template<typename T>
IndexStack<T>::IndexStack(IndexStack<T>&& other)
              :index(other.index)
{
    ordering = std::move(other.ordering);
    collection = std::move(other.collection);
    other.ordering.clear();
    other.collection.clear();
}

template<typename T>
IndexStack<T>& IndexStack<T>::operator= (IndexStack<T>&& other)
{
    if(this != &other) {
        ordering = std::move(other.ordering);
        collection = std::move(other.collection);
        other.ordering.clear();
        other.collection.clear();
    }

    return *this;
}


// Index Collection Methods
// ==========================================================

template<typename T>
bool IndexStack<T>::find(T obj)
{
    return collection.find(obj) != collection.end();
}

template<typename T>
int IndexStack<T>::push(T obj)
{
    collection.insert(obj);
    ordering.push(obj);
    return ++index;
}

template<typename T>
T IndexStack<T>::pop() 
{
    T top = ordering.top();
    collection.erase(top);
    ordering.pop();
    return top;
}

#endif
