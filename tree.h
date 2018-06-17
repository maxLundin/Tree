//
// Created by max on 17.06.18.
//

#ifndef TREE_TREE_H
#define TREE_TREE_H


#include <stdlib.h>
#include <utility>
#include <string>
#include <iostream>
#include <iterator>

template<typename T>

class Tree {
    struct Node {
        Node *left;
        Node *right;
        T val;
        int prior;

        Node(const T &data, int prio, Node *l = nullptr, Node *r = nullptr) : val(data) {
            left = l;
            right = r;
            prior = prio;
        }

        Node(Node const &other) : left(other.left), right(other.right), val(other.val), prior(other.prior) {
        }

    };

    Node *root;

public:
    template<typename S>
    struct iterator1 {
        friend class Tree;

    private:
        Node *temp;
        Node *root;

        iterator1(Node *data, Node *root1) : temp(data), root(root1) {}

    public:
        template<typename C>
        iterator1(const iterator1<C> &data) {
            temp = data.temp;
        }

        S &operator*() const {
            return (temp)->val;
        }

        S *operator->() const {
            return *(temp->val);
        }

        iterator1 &operator++() {
            if (temp != nullptr) {
                temp = next(*root, (temp)->val);
            } else {
                temp = getLast(root);
            }
            return *this;
        }

        iterator1 &operator--() {
            if (temp != nullptr) {
                temp = prev(*root, (temp)->val);
            } else {
                temp = getLast(root);
            }
            return *this;
        }

        iterator1 operator++(int) {
            Node *cur = temp;
            temp = next(*root, (temp)->val);
            return iterator1(cur, root);
        }

        iterator1 operator--(int) {
            Node *cur = temp;
            temp = prev(*root, (temp)->val);
            return iterator1(cur, root);
        }

        bool operator==(iterator1 const second) const {
            return temp == second.temp;
        }

        bool operator!=(iterator1 const second) const {
            return temp != second.temp;
        }

        typedef std::ptrdiff_t difference_type;
        typedef S value_type;
        typedef S *pointer;
        typedef S &reference;
        typedef std::bidirectional_iterator_tag iterator_category;

    };

    typedef iterator1<const T> iterator;
    typedef iterator1<const T> const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;


private:

    static Node *getLast(Node *node) {

        while (node->right != nullptr) {
            node = node->right;
        }
        return node;
    }

    std::pair<Node *, Node *> split(Node *head, T const &k) {
        if (head == nullptr) {
            Node *null = nullptr;
            return std::make_pair(null, null);
        } else if (k > head->val) {
            std::pair<Node *, Node *> pair1 = split(head->right, k);
            head->right = pair1.first;
            return std::make_pair(head, pair1.second);
        } else {
            std::pair<Node *, Node *> pair1 = split(head->left, k);
            head->left = pair1.second;
            return std::make_pair(pair1.first, head);
        }
    }

    Node *merge(Node *node1, Node *node2) {
        if (node1 == nullptr) {
            return node2;
        }
        if (node2 == nullptr) {
            return node1;
        }
        if (node1->prior > node2->prior) {
            node1->right = merge(node1->right, node2);
            return node1;
        }
        node2->left = merge(node1, node2->left);
        return node2;

    }

    std::pair<Node *, Node *> add(T const &val) {
        std::pair<Node *, Node *> pair1 = split(root, val);
        Node *node1 = new Node(val, rand());
        auto w = merge(merge(pair1.first, node1), pair1.second);
        return std::make_pair(w, node1);
    }

    Node *del(T const &val) {
        std::pair<Node *, Node *> pair1 = split(root, val);
        Node *t = pair1.second;
        if (t == nullptr) {
            delete pair1.second;
            return pair1.first;
        }
        if (t->left == nullptr) {
            if (t->right == nullptr) {
                delete pair1.second;
                return pair1.first;
            } else {
                delete t;
                return merge(pair1.first, t->right);
            }
        } else {
            Node *r = t;
            while (r->left->left != nullptr) {
                r = r->left;
            }
            Node *y = r->left;

            r->left = r->left->right;
            delete y;
        }
        return merge(pair1.first, t);
    }

    Node *findElem(Node *node, T const &value) const {
        if (node == nullptr)
            return nullptr;
        if (node->val == value)
            return node;
        if (node->val > value)
            return findElem(node->left, value);
        else
            return findElem(node->right, value);
    }

    static Node *next(Node &root, T const &value) {
        Node *a = &root;
        Node *b = nullptr;
        while (a != nullptr) {
            if (a->val > value) {
                b = a;
                a = a->left;
            } else {
                a = a->right;
            }
        }
        return b;
    }

    static Node *prev(Node &root, T const &value) {
        Node *a = &root;
        Node *b = nullptr;
        while (a != nullptr) {
            if (a->val < value) {
                b = a;
                a = a->right;
            } else {
                a = a->left;
            }
        }
        return b;
    }

    Node *find_lower_bound(Node *node1, Node *lb, T const &elem) {
        if (node1 == nullptr) {
            return lb;
        }
        if (node1->val >= elem) {
            lb = node1;
            lb = find_lower_bound(node1->left, lb, elem);
        } else {
            lb = find_lower_bound(node1->right, lb, elem);
        }
        return lb;
    }

    Node *find_upper_bound(Node *node1, Node *ub, T const &elem) {
        if (node1 == nullptr) {
            return ub;
        }
        if (node1->val > elem) {
            ub = node1;
            ub = find_upper_bound(node1->left, ub, elem);
        } else {
            ub = find_upper_bound(node1->right, ub, elem);
        }
        return ub;
    }

    void delTree(Node *node) {
        if (node->left != nullptr) {
            delTree(node->left);
        }
        if (node->right != nullptr) {
            delTree(node->right);
        }
        delete node;
    }

    void dfs(Node *a, Node *b) {
        if (a->left != nullptr) {
            b->left = new Node(*(a->left));
            dfs(a->left, b->left);
        }
        if (a->right != nullptr) {
            b->right = new Node(*(a->right));
            dfs(a->right, b->right);
        }
    }

public:
    Tree() : root(nullptr) {}

    Tree(Tree const &other) {
        root = new Node(*(other.root));
        if (root) {
            dfs(other.root, root);
        }
    }

    void swap(Tree &a) {
        std::swap(root, a.root);
    }

    template <typename S>
    friend void swap(Tree<S> &a, Tree<S> &b);

    Tree &operator=(Tree const &other) {
        if (this->root != other.root) {
            Tree(other).swap(*this);
        }
        return *this;
    }

    const_iterator lower_bound(T const &elem) {
        Node *lb = nullptr;
        lb = find_lower_bound(root, lb, elem);
        return const_iterator(lb, root);
    }

    const_iterator upper_bound(T const &elem) {
        Node *ub = nullptr;
        ub = find_upper_bound(root, ub, elem);
        return const_iterator(ub, root);
    }

    iterator begin() const {
        if (!root) {
            return const_iterator(nullptr, root);
        }
        Node *temp = root;
        while (temp->left != nullptr) {
            temp = temp->left;
        }
        return const_iterator(temp, root);
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    const_iterator end() const {
        return const_iterator(nullptr, root);
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    std::pair<iterator, bool> insert(T const &elem) {
        if (findElem(root, elem) == nullptr) {
            auto smth = add(elem);
            root = smth.first;
            iterator iter = iterator(smth.second, root);
            return std::make_pair(iter, true);
        } else {
            return std::make_pair(iterator(nullptr, nullptr), false);
        }
    }

    iterator erase(iterator iter) {
        auto smth = next(*root, *iter);
        root = del(*iter);
        return iterator(smth, root);
    }

    const_iterator find(const T elem) const {
        return const_iterator(findElem(root, elem), root);
    }

    bool empty() const {
        return root == nullptr;
    }

    void clear() {
        delTree(root);
        root = nullptr;
    }

    ~Tree() {
        clear();
    }

};


template<typename T>
void swap(Tree<T> &a, Tree<T> &b) {
    std::swap(a.root, b.root);
}

#endif //TREE_TREE_H
