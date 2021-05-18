#include <iostream>
#include <fstream>
#include <forward_list>
#include <cassert>
#include "parser.h"
#include "strategy.h"
#include "avl_tree.h"
#include "rb_tree.h"
#include "relation.h"

using namespace std;

void readFile(ifstream &fp, forward_list<Contract> &contracts)
{
    Contract ctr;
    Person prs;
    string buf;

    while (getline(fp, buf))
    {
        readPerson(&prs, buf);
        fp.ignore(2);
        getline(fp, buf);
        while(buf!="}")
        {
            if (!readContract(&ctr, &prs, buf))
                throw runtime_error("Invalid input");
            contracts.push_front(ctr);
            ctr.clear();
            getline(fp, buf);
        }
        prs.clear();
    }
}

void searchContract(Relation<Contract> &relations)
{
    Person tmp_prs;
    Contract tmp_ctr;
    int key;
    unsigned int id;
    unsigned int duration;

    cout << "Search types:\n0 - Names and id\n1 - Duration\n2 - Cost\n";
    while(true)
    {
        cin >> key;
        switch (key)
        {
            case 0:
                cin.ignore();
                cout << "Enter names:\n";
                getline(cin, tmp_prs.last_name);
                getline(cin, tmp_prs.first_name);
                getline(cin, tmp_prs.second_name);
                cout << "Enter id:\n";
                cin >> id;
                tmp_ctr.owner = tmp_prs;
                tmp_ctr.id = id;
                break;
            case 1:
                cin >> duration;
                tmp_ctr.duration = duration;
                break;
            case 2:
                cin >> tmp_ctr.price;
                break;
            default:
                return;
        }
        if(relations.findValue(&tmp_ctr))
            cout << "Node was successfully found.\n";
        else
            cout << "Node not found.\n";
        tmp_ctr.clear();
        tmp_prs.clear();
    }
}

int main()
{
    ifstream ifp ("input_file.txt");
    assert(ifp.is_open() && "Failed to open output file");

    forward_list<Contract> contracts;
    readFile(ifp, contracts);

    Strategy<Contract*> *contract_id = new StrategyId;
    Strategy<Contract*> *contract_date = new StrategyDate;
    Strategy<Contract*> *contract_price = new StrategyPrice;
    RBTree<Contract*> avl_tree_name_id(contract_id);
    AvlTree<Contract*> rb_tree_contract_date(contract_date);
    AvlTree<Contract*> avl_tree_price(contract_price);
    Relation<Contract> relations;

    for (auto &contract : contracts)
        relations.insertValue(&contract);
    relations.insertIndex(&avl_tree_price);
    relations.insertIndex(&avl_tree_name_id);
    relations.insertIndex(&rb_tree_contract_date);

    searchContract(relations);
    return 0;
}
