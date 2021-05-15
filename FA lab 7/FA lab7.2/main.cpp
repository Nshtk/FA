#include <iostream>
#include <cstdlib>
#include <ctime>
#include "strategy.h"
#include "avl_tree.h"
#include "rb_tree.h"
#include "relation.h"

using namespace std;


int main()
{
    StrategyOne<int> *strategy_one = new StrategyOne<int>;
    RBTree<int> rb_tree((strategy_one));
    Relation<int> relation;

    int *arr;
    int val=100;
    for(int i=0; i<5; i++)
    {
        arr = new int[1];
        arr[0]=rand() % 200;
        relation.insertValue(*arr);
    }
    relation.insertIndex(&rb_tree);
    relation.deleteValue(val);
    relation.printValues();
    relation.findValue(42);
    return 0;
}
