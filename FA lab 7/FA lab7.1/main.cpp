#include <iostream>
#include "strategy.h"
#include "avl_tree.h"
#include "rb_tree.h"

using namespace std;


int main()
{
    StrategyOne<int>* strategy_one = new StrategyOne<int>;

    cout<< "Avl tree:\n";
    AvlTree<int> avl_tree(strategy_one);
    avl_tree.insertElement(312);
    avl_tree.insertElement(6);
    avl_tree.insertElement(1);
    avl_tree.insertElement(2);
    avl_tree.insertElement(3);
    avl_tree.insertElement(10);
    avl_tree.insertElement(5);
    avl_tree.findElement(3);
    avl_tree.findElement(5);
    avl_tree.deleteElement(3);
    avl_tree.deleteElement(5);
    avl_tree.findElement(3);
    avl_tree.findElement(5);
    avl_tree.printTree();
    avl_tree.printRotations();

    cout<< "\nRed-black tree:\n";
    RBTree<int> rb_tree(strategy_one);
    rb_tree.insertElement(312);
    rb_tree.insertElement(6);
    rb_tree.insertElement(1);
    rb_tree.insertElement(2);
    rb_tree.insertElement(3);
    rb_tree.insertElement(10);
    rb_tree.insertElement(5);
    rb_tree.findElement(3);
    rb_tree.findElement(5);
    rb_tree.deleteElement(3);
    rb_tree.deleteElement(5);
    rb_tree.findElement(3);
    rb_tree.findElement(5);
    rb_tree.printTree();
    rb_tree.printRotations();

    return 0;
}
