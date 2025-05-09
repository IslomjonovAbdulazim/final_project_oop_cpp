#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

// Singly-linked list: movable but not copyable
template<typename T>
class List {
    struct Node {
        T       data;
        Node*   next;
        Node(const T& d):       data(d), next(nullptr) {}
        Node(T&&      d):       data(std::move(d)), next(nullptr) {}
    };

    Node* head;
    std::size_t count;

public:
    List(): head(nullptr), count(0) {}
    ~List() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
    }

    List(const List&) = delete;
    List& operator=(const List&) = delete;

    List(List&& o) noexcept : head(o.head), count(o.count) {
        o.head = nullptr; o.count = 0;
    }
    List& operator=(List&& o) noexcept {
        if (this != &o) {
            while (head) { Node* tmp = head; head = head->next; delete tmp; }
            head = o.head; count = o.count;
            o.head = nullptr; o.count = 0;
        }
        return *this;
    }

    template<typename U>
    void add(U&& item) {
        Node* n = new Node(std::forward<U>(item));
        n->next = head;
        head = n;
        ++count;
    }

    T& get(std::size_t idx) {
        if (idx >= count) throw std::out_of_range("List::get index out of range");
        Node* cur = head;
        for (std::size_t i = 0; i < idx; ++i) cur = cur->next;
        return cur->data;
    }
    const T& get(std::size_t idx) const { return const_cast<List*>(this)->get(idx); }

    std::size_t size() const { return count; }
    bool empty() const     { return count == 0; }

    struct Iterator {
        Node* ptr;
        Iterator(Node* p): ptr(p) {}
        T& operator*() const { return ptr->data; }
        Iterator& operator++() { ptr = ptr->next; return *this; }
        bool operator!=(const Iterator& o) const { return ptr != o.ptr; }
    };

    Iterator begin()       { return Iterator(head); }
    Iterator end()         { return Iterator(nullptr); }
    const Iterator begin() const { return Iterator(head); }
    const Iterator end()   const { return Iterator(nullptr); }
};

#endif // LIST_HPP
