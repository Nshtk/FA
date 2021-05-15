#ifndef FA_LAB7_1_AVL_TREE_H
#define FA_LAB7_1_AVL_TREE_H

#include "binary_tree.h"

template <class T>
class AvlTree : BinaryTree<T>
{
private:
    unsigned int rotations;
    struct Node
    {
        T data;
        char height;
        Node *left;
        Node *right;
    } *root;

    char getHeight(Node *node)
    {
        return node? node->height : 0;
    }

    void fixHeight(Node *node)
    {
        char h_l = getHeight(node->left);
        char h_r = getHeight(node->right);
        node->height = (h_l>h_r?h_l:h_r)+1;
    }

    int bfactor(Node *node)
    {
        return getHeight(node->right)-getHeight(node->left);
    }

    Node *rotateRight(Node *node)
    {
        Node* node_l = node->left;
        node->left = node_l->right;
        node_l->right = node;

        fixHeight(node);
        fixHeight(node_l);

        rotations++;
        return node_l;
    }

    Node *rotateLeft(Node *node)
    {
        Node* node_r = node->right;
        node->right = node_r->left;
        node_r->left = node;

        fixHeight(node);
        fixHeight(node_r);

        rotations++;
        return node_r;
    }

    Node *balanceNode(Node *node)
    {
        fixHeight(node);
        if(bfactor(node)==2)
        {
            if(bfactor(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if(bfactor(node)==-2)
        {
            if(bfactor(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    Node *createNode(Node *node, T data)
    {
        if(node == nullptr)
        {
            node = new Node;
            node->data=data;
            node->left= nullptr;
            node->right=nullptr;
            return node;
        }
        int res=BinaryTree<T>::strategy->compareValues(node->data, data);
        if(res==1)
            node->left = createNode(node->left, data);
        else
            node->right = createNode(node->right, data);
        return balanceNode(node);
    }

    void findNode(Node *node, T data)
    {
        if(node==nullptr)
        {
            std::cout << data << " not found.\n";
            return;
        }
        int res=BinaryTree<T>::strategy->compareValues(node->data, data);
        if(res==1)
            return findNode(node->left, data);
        else if(res==-1)
            return findNode(node->right, data);
        else
        {
            std::cout << data << " was found.\n";
            return;
        }
    }

    Node *findMinNode(Node *node)
    {
        return node->left?findMinNode(node->left):node;
    }

    Node *removeMinNode(Node *node)
    {
        if(node->left==nullptr)
            return node->right;
        node->left = removeMinNode(node->left);
        return balanceNode(node);
    }

    Node *removeNode(Node *node, T data)
    {
        if(node==nullptr)
            return nullptr;
        int res=BinaryTree<T>::strategy->compareValues(node->data, data);
        if(res==1)
            node->left = removeNode(node->left, data);
        else if(res==-1)
            node->right = removeNode(node->right, data);
        else
        {
            Node* node_l = node->left;
            Node* node_r = node->right;
            delete node;
            if(node_r==nullptr)
                return node_l;
            Node* min = findMinNode(node_r);
            min->right = removeMinNode(node_r);
            min->left = node_l;
            return balanceNode(min);
        }
        return balanceNode(node);
    }
    void printAvlTree(Node* node)
    {
        if(node == nullptr)
            return;
        printAvlTree(node->left);
        std::cout << node->data << " ";
        printAvlTree(node->right);
    }

public:
    explicit AvlTree(Strategy<T> *s) : BinaryTree<T>(s), root(nullptr)
    {
        rotations=0;
    }
    void insertElement(T data) override
    {
        root=createNode(root, data);
    }
    void deleteElement(T data) override
    {
        root=removeNode(root, data);
    }
    void findElement(T data) override
    {
        return findNode(root, data);
    }
    void printTree()
    {
        printAvlTree(root);
    }
    void printRotations()
    {
        std::cout << "Rotations: " << rotations << '\n';
    }
};
#endif