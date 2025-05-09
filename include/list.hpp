// include/list.hpp
#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

template<typename T>
class List {
    struct Node {
        T        data;
        Node*    next;
        Node(const T& d): data(d), next(nullptr) {}
    };
    Node* head;

public:
    // 1) default ctor
    List(): head(nullptr) {}

    // 2) copy ctor (deep copy!)
    List(const List& other): head(nullptr) {
        for (Node* p = other.head; p; p = p->next) {
            push_back(p->data);
        }
    }

    // 3) copy assign
    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            for (Node* p = other.head; p; p = p->next) {
                push_back(p->data);
            }
        }
        return *this;
    }

    // 4) move ctor
    List(List&& other) noexcept
      : head(other.head) {
        other.head = nullptr;
    }
    // 5) move assign
    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            other.head = nullptr;
        }
        return *this;
    }

    // 6) dtor
    ~List() {
        clear();
    }

    // push_back
    void push_back(const T& value) {
        Node* n = new Node(value);
        if (!head) {
            head = n;
        } else {
            Node* p = head;
            while (p->next) p = p->next;
            p->next = n;
        }
    }

    // clear
    void clear() {
        Node* p = head;
        while (p) {
            Node* nxt = p->next;
            delete p;
            p = nxt;
        }
        head = nullptr;
    }

    // begin/end for iteration
    struct Iterator {
        Node* cur;
        Iterator(Node* c): cur(c) {}
        T& operator*() const { return cur->data; }
        Iterator& operator++() { cur = cur->next; return *this; }
        bool operator==(const Iterator& o) const { return cur == o.cur; }
        bool operator!=(const Iterator& o) const { return cur != o.cur; }
    };
    Iterator begin() const { return Iterator(head); }
    Iterator end()   const { return Iterator(nullptr); }

    // size
    std::size_t size() const {
        std::size_t cnt = 0;
        for (Node* p = head; p; p = p->next) ++cnt;
        return cnt;
    }
    // get non-const element by index
    T& operator[](std::size_t idx) {
        auto it = begin();
        while (idx-- && it != end()) ++it;
        return *it;  // undefined if idx >= size()
    }

    // get const element by index
    const T& operator[](std::size_t idx) const {
        auto it = begin();
        while (idx-- && it != end()) ++it;
        return *it;
    }

};

#endif // LIST_HPP
