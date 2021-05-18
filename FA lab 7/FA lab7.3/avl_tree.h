#ifndef FA_LAB7_3_AVL_TREE_H
#define FA_LAB7_3_AVL_TREE_H

#include "binary_tree.h"

template <typename T>
class AvlTree : public BinaryTree<T>
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

        if(BinaryTree<T>::strategy->compareValues(node->data, data)==-1)
            node->left = createNode(node->left, data);
        else
            node->right = createNode(node->right, data);
        return balanceNode(node);
    }

    int findNode(Node *node, T data)
    {
        if(node==nullptr)
            return false;
        int res=BinaryTree<T>::strategy->compareValues(node->data, data);
        if(res==1)
            return findNode(node->right, data);
        else if(res==-1)
            return findNode(node->left, data);
        else
            return true;
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

        if(BinaryTree<T>::strategy->compareValues(node->data, data))
            node->left = removeNode(node->left, data);
        else if(BinaryTree<T>::strategy->compareValues(data, node->data))
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
    int insertElement(T data) override
    {
        root=createNode(root, data);
    }
    int deleteElement(T data) override
    {
        root=removeNode(root, data);
    }
    int findElement(T data) override
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