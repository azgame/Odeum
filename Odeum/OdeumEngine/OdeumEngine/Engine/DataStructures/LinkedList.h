#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../../pch.h"

template <typename Ty>
struct Node
{
    ~Node()
    {
        next = previous = nullptr;
    }

	Ty element;
    Node<Ty>* next;
    Node<Ty>* previous;
};

template <typename T>
class LinkedList
{
public:

    LinkedList() : m_size(0), head(nullptr), tail(nullptr) {}
    ~LinkedList() { clear(); }

	void push_front(T Element)
	{
        Node<T>* e = new Node<T>();
        e->element = Element;

        if (m_size == 0)
        {
            Initialize(e);
        }
        else
        {
            e->next = head;
            head->previous = e;
            head = e;
        }

        m_size++;
	}

	void push_back(T Element)
	{
        Node<T>* e = new Node<T>();
        e->element = Element;

        if (m_size == 0)
        {
            Initialize(e);
        }
        else
        {
            e->previous = tail;
            tail->next = e;
            tail = e;
        }

        m_size++;
	}

    void clear()
    {
        Node<T>* current = head;

        for (Node<T>* target = current; target != tail; target = current)
        {
            current = current->next;
            SAFE_DELETE(target);
        }

        SAFE_DELETE(tail);

        m_size = 0;
    }

	T front() { return head->element; }
	T back() { return tail->element; }

    size_t size() { return m_size; }

    Node<T>& operator* () { return *(head->element); }
    Node<T>* operator-> () { return &**this; }

    struct iterator
    {
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        iterator(Node<T>* Node) : current(Node) {}
        iterator(const iterator& Iter) : current(Iter.current) {}

        reference operator* () { return *current->element; }
        pointer operator-> () { return &**this; }

        iterator& operator++ () { if (current->next) current = current->next; return *this; }
        iterator operator++ (int) { const auto temp(*this); ++*this; return temp; }

        bool operator== (const iterator& that) const { return current == that.current; }
        bool operator!= (const iterator& that) const { return !(*this == that); }

    private:

        Node<T>* current;
    };

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(tail); }

private:

    void Initialize(Node<T>* Node)
    {
        head = tail = Node;
        head->next = tail;
        tail->previous = head;
    }

    Node<T>* head;
    Node<T>* tail;
    size_t m_size;

};

#endif