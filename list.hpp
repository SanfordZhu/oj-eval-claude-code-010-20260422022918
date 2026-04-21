#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include "exceptions.hpp"
#include "algorithm.hpp"

#include <climits>
#include <cstddef>

namespace sjtu {
template<typename T>
class list {
protected:
    class node {
    public:
        T *data;
        node *prev;
        node *next;

        node() : data(nullptr), prev(nullptr), next(nullptr) {}
        node(const T &val) : data(new T(val)), prev(nullptr), next(nullptr) {}
        ~node() { delete data; }
    };

    node *head;
    size_t _size;

    node *insert(node *pos, node *cur) {
        cur->prev = pos->prev;
        cur->next = pos;
        pos->prev->next = cur;
        pos->prev = cur;
        return cur;
    }

    node *erase(node *pos) {
        node *ret = pos->next;
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        return ret;
    }

public:
    class const_iterator;
    class iterator {
    private:
        node *ptr;
        list *container;

    public:
        iterator() : ptr(nullptr), container(nullptr) {}
        iterator(node *p, list *c) : ptr(p), container(c) {}

        iterator operator++(int) {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        iterator & operator++() {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }

        iterator operator--(int) {
            if (ptr == nullptr || container == nullptr || ptr->prev == container->head)
                throw invalid_iterator();
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        iterator & operator--() {
            if (ptr == nullptr || container == nullptr || ptr->prev == container->head)
                throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        T & operator *() const {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            return *(ptr->data);
        }

        T * operator ->() const {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
        friend class const_iterator;
    };

    class const_iterator {
    private:
        const node *ptr;
        const list *container;

    public:
        const_iterator() : ptr(nullptr), container(nullptr) {}
        const_iterator(const node *p, const list *c) : ptr(p), container(c) {}
        const_iterator(const iterator &other) : ptr(other.ptr), container(other.container) {}

        const_iterator operator++(int) {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }

        const_iterator & operator++() {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            ptr = ptr->next;
            return *this;
        }

        const_iterator operator--(int) {
            if (ptr == nullptr || container == nullptr || ptr->prev == container->head)
                throw invalid_iterator();
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }

        const_iterator & operator--() {
            if (ptr == nullptr || container == nullptr || ptr->prev == container->head)
                throw invalid_iterator();
            ptr = ptr->prev;
            return *this;
        }

        const T & operator *() const {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            return *(ptr->data);
        }

        const T * operator ->() const {
            if (ptr == nullptr || container == nullptr || ptr == container->head)
                throw invalid_iterator();
            return ptr->data;
        }

        bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
        }

        bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
        }

        friend class list;
    };

    list() {
        head = new node();
        head->prev = head;
        head->next = head;
        _size = 0;
    }

    list(const list &other) {
        head = new node();
        head->prev = head;
        head->next = head;
        _size = 0;

        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }

    virtual ~list() {
        clear();
        delete head;
    }

    list &operator=(const list &other) {
        if (this == &other) return *this;

        clear();

        for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }

        return *this;
    }

    const T & front() const {
        if (empty()) throw container_is_empty();
        return *(head->next->data);
    }

    const T & back() const {
        if (empty()) throw container_is_empty();
        return *(head->prev->data);
    }

    iterator begin() {
        return iterator(head->next, this);
    }

    const_iterator cbegin() const {
        return const_iterator(head->next, this);
    }

    iterator end() {
        return iterator(head, this);
    }

    const_iterator cend() const {
        return const_iterator(head, this);
    }

    virtual bool empty() const {
        return _size == 0;
    }

    virtual size_t size() const {
        return _size;
    }

    virtual void clear() {
        while (!empty()) {
            pop_front();
        }
    }

    virtual iterator insert(iterator pos, const T &value) {
        if (pos.container != this) throw invalid_iterator();
        node *newNode = new node(value);
        insert(pos.ptr, newNode);
        _size++;
        return iterator(newNode, this);
    }

    virtual iterator erase(iterator pos) {
        if (pos.container != this) throw invalid_iterator();
        if (empty()) throw container_is_empty();
        if (pos.ptr == head) throw invalid_iterator();

        node *nextNode = erase(pos.ptr);
        delete pos.ptr;
        _size--;
        return iterator(nextNode, this);
    }

    void push_back(const T &value) {
        insert(end(), value);
    }

    void pop_back() {
        if (empty()) throw container_is_empty();
        erase(--end());
    }

    void push_front(const T &value) {
        insert(begin(), value);
    }

    void pop_front() {
        if (empty()) throw container_is_empty();
        erase(begin());
    }

    void sort() {
        if (_size <= 1) return;

        T **arr = new T*[_size];
        node *cur = head->next;
        for (size_t i = 0; i < _size; ++i) {
            arr[i] = cur->data;
            cur = cur->next;
        }

        sjtu::sort<T*>(arr, arr + _size, [](const T *a, const T *b) {
            return *a < *b;
        });

        cur = head->next;
        for (size_t i = 0; i < _size; ++i) {
            cur->data = arr[i];
            cur = cur->next;
        }

        delete[] arr;
    }

    void merge(list &other) {
        if (this == &other) return;

        node *p1 = head->next;
        node *p2 = other.head->next;

        while (p1 != head && p2 != other.head) {
            if (!(*(p2->data) < *(p1->data))) {
                p1 = p1->next;
            } else {
                node *tmp = p2;
                p2 = p2->next;
                insert(p1, tmp);
                _size++;
            }
        }

        while (p2 != other.head) {
            node *tmp = p2;
            p2 = p2->next;
            insert(head, tmp);
            _size++;
        }

        other.head->prev = other.head;
        other.head->next = other.head;
        other._size = 0;
    }

    void reverse() {
        node *cur = head;
        do {
            node *tmp = cur->next;
            cur->next = cur->prev;
            cur->prev = tmp;
            cur = tmp;
        } while (cur != head);
    }

    void unique() {
        if (_size <= 1) return;

        node *cur = head->next;
        while (cur != head && cur->next != head) {
            if (*(cur->data) == *(cur->next->data)) {
                node *toRemove = cur->next;
                erase(toRemove);
                delete toRemove;
                _size--;
            } else {
                cur = cur->next;
            }
        }
    }
};

}

#endif //SJTU_LIST_HPP
