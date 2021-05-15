#ifndef FA_LAB7_1_BINARY_TREE_H
#define FA_LAB7_1_BINARY_TREE_H

#include <utility>
#include "strategy.h"

template <class T>
class BinaryTree
{
protected:
    Strategy<T> *strategy;
public:
    explicit BinaryTree(Strategy<T> *s) : strategy(std::move(s))
    {}
    virtual void insertElement(T data)
    {}
    virtual void deleteElement(T data)
    {}
    virtual void findElement(T data)
    {}
};

#endif
