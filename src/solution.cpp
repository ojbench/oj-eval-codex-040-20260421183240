#include <iostream>
#include <memory>
#include <optional>

// Implementation of a doubly linked list using shared_ptr/weak_ptr
// as per the assignment skeleton.

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(std::move(node)) {}

        T& operator*() {
            return current->value;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            if (current) current = current->next;
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return current.get() == other.current.get();
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    DoublyLinkedList() : head(nullptr), length(0) {}

    bool empty() const {
        return length == 0;
    }

    size_t size() const {
        return length;
    }

    void push_front(const T& value) {
        auto n = std::make_shared<Node>(value);
        n->next = head;
        if (head) {
            head->prev = n;
        }
        head = n;
        if (length == 0) {
            tail = head;
        }
        ++length;
    }

    void push_back(const T& value) {
        auto n = std::make_shared<Node>(value);
        if (length == 0) {
            head = n;
            tail = n;
            length = 1;
            return;
        }
        auto last = tail.lock();
        if (last) {
            last->next = n;
            n->prev = last;
            tail = n;
            ++length;
        } else {
            // Fallback: list inconsistent, reset to single node
            head = n;
            tail = n;
            length = 1;
        }
    }

    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        T ret = head->value;
        auto next = head->next;
        head = next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --length;
        return ret;
        
    }

    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        auto last = tail.lock();
        if (!last) {
            return std::nullopt;
        }
        T ret = last->value;
        auto prev = last->prev.lock();
        if (!prev) {
            // Only one element
            head.reset();
            tail.reset();
        } else {
            prev->next.reset();
            tail = prev;
        }
        --length;
        return ret;
    }

    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

// Note: No main() here. The OJ harness provides its own main.
