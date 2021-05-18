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

void readFile(ifstream &fp, forward_list<Message> &messages)
{
    Message msg;
    string buf;

    while (getline(fp, buf))
    {
        if (!readString(&msg, buf))
            throw runtime_error("Invalid input");
        messages.push_front(msg);
        buf.clear();
    }
}

void searchMessage(Relation<Message> &relations)
{
    Message	tmp;
    int key;

    cout << "Search types:\n0 - Time\n1 - Username\n2 - Message\n3 - Time and username\n";
    while(true)
    {
        cin >> key;
        switch (key)
        {
            case 0:
                cin >> tmp.time.day >> tmp.time.month >> tmp.time.year >> tmp.time.hour
                    >> tmp.time.minute >> tmp.time.second;
                break;
            case 1:
                cin.ignore();
                getline(cin, tmp.username);
                break;
            case 2:
                cin.ignore();
                getline(cin, tmp.text);
                break;
            case 3:
                cout << "\nEnter date and time:\n";
                cin >> tmp.time.day >> tmp.time.month >> tmp.time.year >> tmp.time.hour
                    >> tmp.time.minute >> tmp.time.second;
                cin.ignore();
                cout << "\nEnter username:\n";
                getline(cin, tmp.username);
                break;
            default:
                return;
        }
        if(relations.findValue(&tmp))
            cout << "Node was successfully found.\n";
        else
            cout << "Node not found.\n";
        tmp.clear();
    }
}

int main()
{
    ifstream ifp ("input_file.txt");
    assert(ifp.is_open() && "Failed to open output file");

    forward_list<Message> messages;
    readFile(ifp, messages);

    Strategy<Message*> *datetime = new StrategyDatetime;
    Strategy<Message*> *username = new StrategyUsername;
    Strategy<Message*> *message = new StrategyMessage;
    Strategy<Message*> *datetime_username = new StrategyDatetimeUsername;
    AvlTree<Message*> avl_datetime(datetime);
    RBTree<Message*> rb_username(username);
    AvlTree<Message*> avl_message(message);
    RBTree<Message*> rb_datetime_username(datetime_username);
    Relation<Message> relations;

    relations.insertIndex(&avl_datetime);
    for(auto &Message : messages)
        relations.insertValue(&Message);
    relations.insertIndex(&rb_username);
    relations.insertIndex(&avl_message);
    relations.insertIndex(&rb_datetime_username);

    searchMessage(relations);

    return 0;
}
