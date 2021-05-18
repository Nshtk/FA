#ifndef FA_LAB7_3_RELATION_H
#define FA_LAB7_3_RELATION_H

#include <forward_list>
#include <algorithm>
#include "avl_tree.h"
#include "parser.h"
#include "rb_tree.h"

template <typename T>
class Relation
{
private:
    std::forward_list<BinaryTree<T*>*> indexes;
    std::forward_list<T*> values;

public:
    void insertValue(T *data)
    {
        values.push_front(data);
        for(auto &it : indexes)
            it->insertElement(data);
    }
    void deleteValue(T *data)
    {
        values.remove(data);
        for(auto &it : indexes)
            it->deleteElement(data);
    }
    bool findValue(T *data)
    {
        for(auto &it : indexes)
            if(it->findElement(data))
                return true;
        return false;
    }
    /*void printValues()
    {
        for(auto &val : values)
           std::cout << val->time.day << ' ';
        std::cout << "\n";
    }*/
    void insertIndex(BinaryTree<T*> *index)
    {
        for(auto it : values)
            index->insertElement(it);
        indexes.push_front(index);
    }
    void deleteIndex(BinaryTree<T*> *index)
    {
        indexes.remove(index);
    }
};
#endif