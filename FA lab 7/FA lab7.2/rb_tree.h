#ifndef FA_LAB7_2_RB_TREE_H
#define FA_LAB7_2_RB_TREE_H

#include "binary_tree.h"

template <typename T>
class RBTree : public BinaryTree<T>
{
private:
    unsigned int rotations;
    struct Node
    {
        T data;
        char color;
        Node *parent;
        Node *left;
        Node *right;
    } *root, *empty;

    Node *findNode(Node *node, T data)
    {
        if (data == node->data)
        {
            std::cout << data << " was found.\n";
            return node;
        }
        if (node == empty)
        {
            std::cout << data << " not found.\n";
            return node;
        }
        if (data < node->data)
            return findNode(node->left, data);
        return findNode(node->right, data);
    }

    void rbTransplant(Node *u, Node *v)
    {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        v->parent = u->parent;
    }
    Node *findMinNode(Node *node)
    {
        while (node->left != empty)
            node = node->left;
        return node;
    }
    void fixAfterDelete(Node *x)
    {
        Node *s;
        while (x != root && x->color == 0)
        {
            if (x == x->parent->left)
            {
                s = x->parent->right;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rotateLeft(x->parent);
                    s = x->parent->right;
                }

                if (s->left->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->right->color == 0)
                    {
                        s->left->color = 0;
                        s->color = 1;
                        rotateRight(s);
                        s = x->parent->right;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->right->color = 0;
                    rotateLeft(x->parent);
                    x = root;
                }
            }
            else
            {
                s = x->parent->left;
                if (s->color == 1)
                {
                    s->color = 0;
                    x->parent->color = 1;
                    rotateRight(x->parent);
                    s = x->parent->left;
                }

                if (s->right->color == 0 && s->right->color == 0)
                {
                    s->color = 1;
                    x = x->parent;
                }
                else
                {
                    if (s->left->color == 0)
                    {
                        s->right->color = 0;
                        s->color = 1;
                        rotateLeft(s);
                        s = x->parent->left;
                    }

                    s->color = x->parent->color;
                    x->parent->color = 0;
                    s->left->color = 0;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = 0;
    }

    void removeNode(Node *node, T data)
    {
        Node *z = empty;
        Node *x, *y;
        while (node != empty)
        {
            int res=BinaryTree<T>::strategy->compareValues(node->data, data);
            if (res==0)
                z = node;
            if (res==1)
                node = node->left;
            else
                node = node->right;
        }

        if (z == empty)
        {
            std::cout<<"Node not found.\n";
            return;
        }

        y = z;
        int y_original_color = y->color;

        if (z->left == empty)
        {
            x = z->right;
            rbTransplant(z, z->right);
        }
        else if (z->right == empty)
        {
            x = z->left;
            rbTransplant(z, z->left);
        }
        else
        {
            y = findMinNode(z->right);
            y_original_color = y->color;
            x = y->right;

            if (y->parent == z)
                x->parent = y;
            else
            {
                rbTransplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }

            rbTransplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        delete z;
        if (y_original_color == 0)
            fixAfterDelete(x);
    }

    void fixInsert(Node *k)
    {
        Node *u;
        while (k->parent->color == 1)
        {
            if (k->parent == k->parent->parent->right)
            {
                u = k->parent->parent->left;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->left)
                    {
                        k = k->parent;
                        rotateRight(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rotateLeft(k->parent->parent);
                }
            }
            else
            {
                u = k->parent->parent->right;
                if (u->color == 1)
                {
                    u->color = 0;
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    k = k->parent->parent;
                }
                else
                {
                    if (k == k->parent->right)
                    {
                        k = k->parent;
                        rotateLeft(k);
                    }
                    k->parent->color = 0;
                    k->parent->parent->color = 1;
                    rotateRight(k->parent->parent);
                }
            }
            if (k == root)
            {
                break;
            }
        }
        root->color = 0;
    }

    Node *createNode(T data)
    {
        Node *node = new Node;
        node->parent = nullptr;
        node->data = data;
        node->left = empty;
        node->right = empty;
        node->color = 1;

        Node *y = nullptr;
        Node *x = this->root;

        while (x != empty)
        {
            y = x;
            if (BinaryTree<T>::strategy->compareValues(node->data, x->data)==-1)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (BinaryTree<T>::strategy->compareValues(node->data, y->data)==-1)
            y->left = node;
        else
            y->right = node;

        if (node->parent == nullptr)
        {
            node->color = 0;
            return nullptr;
        }

        if (node->parent->parent == nullptr)
        {
            return nullptr;
        }

        fixInsert(node);
    }

    void rotateLeft(Node *x)
    {
        Node *y = x->right;
        x->right = y->left;

        if (y->left != empty)
            y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        rotations++;
        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node *x)
    {
        Node *y = x->left;
        x->left = y->right;

        if (y->right != empty)
            y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            this->root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        rotations++;
        y->right = x;
        x->parent = y;
    }
    void printRbTree(Node *node)
    {
        if (node != empty)
        {
            std::cout << node->data;
            node->color? std::cout << " RED " : std::cout << " BLACK ";
            printRbTree(node->left);
            printRbTree(node->right);
        }
    }
public:
    explicit RBTree(Strategy<T> *s) : BinaryTree<T>(s)
    {
        empty = new Node;
        empty->color = 0;
        empty->left = nullptr;
        empty->right = nullptr;
        root = empty;
        rotations=0;
    }

    int insertElement(T data) override
    {
        createNode(data);
    }
    int deleteElement(T data) override
    {
        removeNode(root, data);
    }
    int findElement(T data) override
    {
        findNode(root, data);
    }
    void printTree()
    {
        printRbTree(root);
    }
    void printRotations()
    {
        std::cout << "Rotations: " << rotations << '\n';
    }
};

#endif