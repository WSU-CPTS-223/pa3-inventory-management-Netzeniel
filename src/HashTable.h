#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <functional>
#include <cstddef>

template <typename K, typename V>
class HashTable {
private:
    struct Node {
        K key;
        V value;
        Node* next;
        Node(const K& k, const V& v, Node* n = nullptr)
            : key(k), value(v), next(n) {}
    };

    Node** buckets;
    std::size_t capacity;
    std::size_t count;
    double maxLoadFactor;

    std::size_t indexForKey(const K& key) const {
        std::hash<K> hasher;
        return hasher(key) % capacity;
    }

    void initBuckets(std::size_t cap) {
        capacity = cap;
        buckets = new Node*[capacity];
        for (std::size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;
        count = 0;
    }

    void freeBuckets() {
        for (std::size_t i = 0; i < capacity; ++i) {
            Node* cur = buckets[i];
            while (cur) {
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
        }
        delete[] buckets;
        buckets = nullptr;
        capacity = 0;
        count = 0;
    }

    void rehash(std::size_t newCap) {
        Node** oldBuckets = buckets;
        std::size_t oldCap = capacity;

        buckets = new Node*[newCap];
        capacity = newCap;
        for (std::size_t i = 0; i < capacity; ++i)
            buckets[i] = nullptr;

        count = 0;

        for (std::size_t i = 0; i < oldCap; ++i) {
            Node* cur = oldBuckets[i];
            while (cur) {
                insert(cur->key, cur->value);
                Node* next = cur->next;
                delete cur;
                cur = next;
            }
        }
        delete[] oldBuckets;
    }

    void ensureCapacity() {
        double load = (capacity == 0) ? 0.0 : (double)count / (double)capacity;
        if (load > maxLoadFactor) {
            std::size_t newCap = capacity * 2 + 1;
            if (newCap < 11) newCap = 11;
            rehash(newCap);
        }
    }

public:
    HashTable(std::size_t initialCapacity = 101, double loadFactor = 0.7)
        : buckets(nullptr), capacity(0), count(0), maxLoadFactor(loadFactor) {
        if (initialCapacity < 11) initialCapacity = 11;
        initBuckets(initialCapacity);
    }

    ~HashTable() {
        freeBuckets();
    }

    HashTable(const HashTable& other)
        : buckets(nullptr), capacity(0), count(0), maxLoadFactor(other.maxLoadFactor) {
        initBuckets(other.capacity);
        for (std::size_t i = 0; i < other.capacity; ++i) {
            Node* cur = other.buckets[i];
            while (cur) {
                insert(cur->key, cur->value);
                cur = cur->next;
            }
        }
    }

    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            freeBuckets();
            maxLoadFactor = other.maxLoadFactor;
            initBuckets(other.capacity);
            for (std::size_t i = 0; i < other.capacity; ++i) {
                Node* cur = other.buckets[i];
                while (cur) {
                    insert(cur->key, cur->value);
                    cur = cur->next;
                }
            }
        }
        return *this;
    }

    void insert(const K& key, const V& value) {
        std::size_t idx = indexForKey(key);
        Node* cur = buckets[idx];

        while (cur) {
            if (cur->key == key) {
                cur->value = value;
                return;
            }
            cur = cur->next;
        }

        buckets[idx] = new Node(key, value, buckets[idx]);
        ++count;
        ensureCapacity();
    }

    V* get(const K& key) {
        std::size_t idx = indexForKey(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->key == key)
                return &cur->value;
            cur = cur->next;
        }
        return nullptr;
    }

    const V* getConst(const K& key) const {
        std::size_t idx = indexForKey(key);
        Node* cur = buckets[idx];
        while (cur) {
            if (cur->key == key)
                return &cur->value;
            cur = cur->next;
        }
        return nullptr;
    }

    bool contains(const K& key) const {
        return getConst(key) != nullptr;
    }

    bool remove(const K& key) {
        std::size_t idx = indexForKey(key);
        Node* cur = buckets[idx];
        Node* prev = nullptr;

        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next;
                else buckets[idx] = cur->next;
                delete cur;
                --count;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }
};

#endif
