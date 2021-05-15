#ifndef FA_LAB7_2_BINARY_TREE_H
#define FA_LAB7_2_BINARY_TREE_H

#include <utility>
#include "strategy.h"

template <typename T>
class BinaryTree
{
protected:
    Strategy<T> *strategy;
public:
    explicit BinaryTree(Strategy<T> *s) : strategy(std::move(s))
    {}
    virtual int insertElement(T data)
    {}
    virtual int deleteElement(T data)
    {}
    virtual int findElement(T data)
    {}
};

#endif
