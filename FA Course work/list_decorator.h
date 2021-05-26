#ifndef FA_KURS_LIST_DECORATOR_H
#define FA_KURS_LIST_DECORATOR_H

template <class T>
struct Node
{
    T data;
    Node *next;
};

template <class T>
class List
{
private:
    Node<T> *head;
    Node<T> *temp;
public:
    List()
    {
        head=nullptr;
    }

    bool empty() const
    {
        return (head==nullptr);
    }
    void push_front(const T &data)
    {
        temp = new Node<T>;
        temp->data = data;
        if(empty())
            temp->next = nullptr;
        else
            temp->next = head;
        head = temp;
    }
    void pop_front()
    {
        if(!empty())
        {
            Node<T> *tmp=head;
            head=head->next;
            delete tmp;
        }
    }
    void clear()
    {
        if(empty())
            return;
        for(typename List<T>::iterator it=begin(); it!=end(); it++)
            delete *it;
    }

    class iterator
    {
    friend class List;
    private:
        const Node<T>* current;
    public:
        iterator() : current(nullptr) {}
        iterator(const Node<T>* p_node) : current(p_node) {}

        iterator& operator=(Node<T>* node)
        {
            this->current = node;
            return *this;
        }
        iterator& operator++()
        {
            if(current)
                current = current->next;
            return *this;
        }
        iterator operator++(int)
        {
            iterator iterator=*this;
            ++*this;
            return iterator;
        }
        bool operator==(const iterator& it) const
        {
            return current == it.current;
        }
        bool operator!=(const iterator& it) const
        {
            return current != it.current;
        }
        T operator*() const
        {
            return current->data;
        }
    };
    iterator begin() const
    {
        return iterator(head);
    }
    iterator end() const
    {
        return iterator(nullptr);
    }
};

template<template<typename> class Container, typename T>
class ListBasicInterface
{
private:
    Container<T> lst;
public:
    ListBasicInterface() = default;
    typename Container<T>::iterator begin()
    {
        return lst.begin();
    }
    typename Container<T>::iterator end()
    {
        return lst.end();
    }
    bool empty() const
    {
        return lst.empty();
    }
    void push_front(const T &data)
    {
        lst.push_front(data);
    }
    void pop_front()
    {
        lst.pop_front();
    }
    void clear()
    {
        lst.clear();
    }
};

#endif