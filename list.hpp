#pragma once
#ifndef SJTU_LIST_HPP
#define SJTU_LIST_HPP

#include <memory>
#include <cstddef>

namespace sjtu {

template <typename T>
class list {
private:
    struct node {
        node *prev;
        node *next;
        alignas(T) char data[sizeof(T)];
        
        node() : prev(nullptr), next(nullptr) {}
        
        T* get_data() {
            return reinterpret_cast<T*>(data);
        }
        
        const T* get_data() const {
            return reinterpret_cast<const T*>(data);
        }
    };
    
    node *head;
    node *tail;
    size_t _size;
    std::allocator<node> node_alloc;
    
    void init() {
        head = node_alloc.allocate(1);
        tail = node_alloc.allocate(1);
        new (head) node();
        new (tail) node();
        head->next = tail;
        tail->prev = head;
        _size = 0;
    }
    
    void destroy() {
        clear();
        head->~node();
        tail->~node();
        node_alloc.deallocate(head, 1);
        node_alloc.deallocate(tail, 1);
    }

public:
    class const_iterator;
    
    class iterator {
    private:
        node *ptr;
        friend class list<T>;
        friend class const_iterator;
        
    public:
        iterator() : ptr(nullptr) {}
        explicit iterator(node *p) : ptr(p) {}
        
        iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }
        
        iterator operator++(int) {
            iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        
        iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        }
        
        iterator operator--(int) {
            iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        
        T &operator*() const noexcept {
            return *ptr->get_data();
        }
        
        T *operator->() const noexcept {
            return ptr->get_data();
        }
        
        friend bool operator==(const iterator &a, const iterator &b) {
            return a.ptr == b.ptr;
        }
        
        friend bool operator!=(const iterator &a, const iterator &b) {
            return a.ptr != b.ptr;
        }
        
        friend bool operator==(const iterator &a, const const_iterator &b) {
            return a.ptr == b.ptr;
        }
        
        friend bool operator!=(const iterator &a, const const_iterator &b) {
            return a.ptr != b.ptr;
        }
    };
    
    class const_iterator {
    private:
        node *ptr;
        friend class list<T>;
        friend class iterator;
        
    public:
        const_iterator() : ptr(nullptr) {}
        explicit const_iterator(node *p) : ptr(p) {}
        const_iterator(const iterator &it) : ptr(it.ptr) {}
        
        const_iterator &operator++() {
            ptr = ptr->next;
            return *this;
        }
        
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ptr = ptr->next;
            return tmp;
        }
        
        const_iterator &operator--() {
            ptr = ptr->prev;
            return *this;
        }
        
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            ptr = ptr->prev;
            return tmp;
        }
        
        const T &operator*() const noexcept {
            return *ptr->get_data();
        }
        
        const T *operator->() const noexcept {
            return ptr->get_data();
        }
        
        friend bool operator==(const const_iterator &a, const const_iterator &b) {
            return a.ptr == b.ptr;
        }
        
        friend bool operator!=(const const_iterator &a, const const_iterator &b) {
            return a.ptr != b.ptr;
        }
        
        friend bool operator==(const const_iterator &a, const iterator &b) {
            return a.ptr == b.ptr;
        }
        
        friend bool operator!=(const const_iterator &a, const iterator &b) {
            return a.ptr != b.ptr;
        }
    };

public:
    list() {
        init();
    }
    
    list(const list &other) {
        init();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*p->get_data());
        }
    }
    
    list &operator=(const list &other) {
        if (this == &other) return *this;
        clear();
        for (node *p = other.head->next; p != other.tail; p = p->next) {
            push_back(*p->get_data());
        }
        return *this;
    }
    
    ~list() {
        destroy();
    }
    
    T &front() noexcept {
        return *head->next->get_data();
    }
    
    T &back() noexcept {
        return *tail->prev->get_data();
    }
    
    const T &front() const noexcept {
        return *head->next->get_data();
    }
    
    const T &back() const noexcept {
        return *tail->prev->get_data();
    }
    
    iterator begin() noexcept {
        return iterator(head->next);
    }
    
    const_iterator cbegin() const noexcept {
        return const_iterator(head->next);
    }
    
    iterator end() noexcept {
        return iterator(tail);
    }
    
    const_iterator cend() const noexcept {
        return const_iterator(tail);
    }
    
    bool empty() const noexcept {
        return _size == 0;
    }
    
    size_t size() const noexcept {
        return _size;
    }
    
    void clear() noexcept {
        node *p = head->next;
        while (p != tail) {
            node *next = p->next;
            p->get_data()->~T();
            p->~node();
            node_alloc.deallocate(p, 1);
            p = next;
        }
        head->next = tail;
        tail->prev = head;
        _size = 0;
    }
    
    iterator insert(iterator pos, const T &value) {
        node *new_node = node_alloc.allocate(1);
        new (new_node) node();
        new (new_node->get_data()) T(value);
        
        node *pos_node = pos.ptr;
        node *prev_node = pos_node->prev;
        
        prev_node->next = new_node;
        new_node->prev = prev_node;
        new_node->next = pos_node;
        pos_node->prev = new_node;
        
        ++_size;
        return iterator(new_node);
    }
    
    iterator erase(iterator pos) noexcept {
        node *pos_node = pos.ptr;
        node *prev_node = pos_node->prev;
        node *next_node = pos_node->next;
        
        prev_node->next = next_node;
        next_node->prev = prev_node;
        
        pos_node->get_data()->~T();
        pos_node->~node();
        node_alloc.deallocate(pos_node, 1);
        --_size;
        
        return iterator(next_node);
    }
    
    void push_front(const T &value) {
        insert(begin(), value);
    }
    
    void push_back(const T &value) {
        insert(end(), value);
    }
    
    void pop_front() noexcept {
        erase(begin());
    }
    
    void pop_back() noexcept {
        erase(iterator(tail->prev));
    }
};

} // namespace sjtu

#endif // SJTU_LIST_HPP
