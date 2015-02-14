#ifndef SET_STACK_HPP
#define SET_STACK_HPP

#include <set>
#include <stack>

template<typename T>
class setstack
{
    public:
        setstack();
        ~setstack();

        // Set Operations
        bool find(T obj);

        // Stack Operations
        void push(T obj);
        void pop();

    private:
        std::stack<T> ordering;
        std::set<T> collection;
};

template<typename T>
setstack<T>::setstack()
            :ordering({})
            ,collection({})
{

}

template<typename T>
setstack<T>::~setstack()
{

}

template<typename T>
bool setstack<T>::find(T obj)
{
    return collection.find(obj);
}

template<typename T>
void setstack<T>::push(T obj)
{
    ordering.push(obj);
}

template<typename T>
void setstack<T>::pop() 
{
    ordering.pop();
}

#endif
