#include "RedBlackTree.h"
#include <string>
#include <stdexcept>

using namespace std;

// Static Destroy Helper//
static void DestroyHelper(RBTNode* node) {
    if (node == nullptr) return;
    DestroyHelper(node->left);
    DestroyHelper(node->right);
    delete node;
}



// Default Constructor
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

// Constructor with one node
RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode;
    root->data = newData;
    root->color = COLOR_BLACK;
    root->left = nullptr;
    root->right = nullptr;
    root->parent = nullptr;
    root->IsNullNode = false;
    numItems = 1;
}

// Copy Constructor
RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// Destructor
RedBlackTree::~RedBlackTree() {
    DestroyHelper(root);
}


// Copy Helper
RBTNode* RedBlackTree::CopyOf(const RBTNode* node) {
    if (node == nullptr) return nullptr;
    RBTNode* copy = new RBTNode;
    copy->data = node->data;
    copy->color = node->color;
    copy->IsNullNode = node->IsNullNode;
    copy->left = CopyOf(node->left);
    copy->right = CopyOf(node->right);
    if (copy->left) copy->left->parent = copy;
    if (copy->right) copy->right->parent = copy;
    return copy;
}

// Insert
void RedBlackTree::Insert(int newData) {
    if (Contains(newData)) {
        throw invalid_argument("Duplicate entry not allowed.");
    }

    RBTNode* newNode = new RBTNode;
    newNode->data = newData;
    newNode->color = COLOR_RED;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    newNode->IsNullNode = false;

    BasicInsert(newNode);
    InsertFixUp(newNode);
    root->color = COLOR_BLACK;
    numItems++;
}

// Basic BST Insert
void RedBlackTree::BasicInsert(RBTNode* node) {
    RBTNode* curr = root;
    RBTNode* parent = nullptr;

    while (curr != nullptr) {
        parent = curr;
        if (node->data < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    node->parent = parent;
    if (parent == nullptr) {
        root = node;
    } else if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

// InsertFixUp
void RedBlackTree::InsertFixUp(RBTNode* node) {
    while (node->parent != nullptr && node->parent->color == COLOR_RED) {
        RBTNode* uncle = GetUncle(node);
        RBTNode* grandparent = node->parent->parent;

        if (uncle != nullptr && uncle->color == COLOR_RED) {
            node->parent->color = COLOR_BLACK;
            uncle->color = COLOR_BLACK;
            grandparent->color = COLOR_RED;
            node = grandparent;
        } else {
            if (IsLeftChild(node) && IsLeftChild(node->parent)) {
                RightRotate(grandparent);
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
            } else if (IsRightChild(node) && IsRightChild(node->parent)) {
                LeftRotate(grandparent);
                node->parent->color = COLOR_BLACK;
                grandparent->color = COLOR_RED;
            } else if (IsLeftChild(node) && IsRightChild(node->parent)) {
                RightRotate(node->parent);
                node = node->right;
            } else {
                LeftRotate(node->parent);
                node = node->left;
            }
        }
    }
    root->color = COLOR_BLACK;
}

// LeftRotate
void RedBlackTree::LeftRotate(RBTNode* node) {
    RBTNode* rightChild = node->right;
    node->right = rightChild->left;
    if (rightChild->left != nullptr) {
        rightChild->left->parent = node;
    }
    rightChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = rightChild;
    } else if (node == node->parent->left) {
        node->parent->left = rightChild;
    } else {
        node->parent->right = rightChild;
    }
    rightChild->left = node;
    node->parent = rightChild;
}

// RightRotate
void RedBlackTree::RightRotate(RBTNode* node) {
    RBTNode* leftChild = node->left;
    node->left = leftChild->right;
    if (leftChild->right != nullptr) {
        leftChild->right->parent = node;
    }
    leftChild->parent = node->parent;
    if (node->parent == nullptr) {
        root = leftChild;
    } else if (node == node->parent->left) {
        node->parent->left = leftChild;
    } else {
        node->parent->right = leftChild;
    }
    leftChild->right = node;
    node->parent = leftChild;
}

// GetUncle
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    RBTNode* grandparent = node->parent ? node->parent->parent : nullptr;
    if (grandparent == nullptr) return nullptr;
    if (grandparent->left == node->parent) return grandparent->right;
    return grandparent->left;
}

// IsLeftChild
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node->parent && node == node->parent->left;
}

// IsRightChild
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node->parent && node == node->parent->right;
}

// Contains
bool RedBlackTree::Contains(int data) const {
    return (Get(data) != nullptr);
}

// Get
RBTNode* RedBlackTree::Get(int data) const {
    RBTNode* curr = root;
    while (curr != nullptr) {
        if (data == curr->data) {
            return curr;
        } else if (data < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return nullptr;
}

// GetMin
int RedBlackTree::GetMin() const {
    if (root == nullptr) throw underflow_error("Tree is empty.");
    RBTNode* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->data;
}

// GetMax
int RedBlackTree::GetMax() const {
    if (root == nullptr) throw underflow_error("Tree is empty.");
    RBTNode* curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->data;
}

// Infix traversal
string RedBlackTree::ToInfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToInfixString(n->left) + " " + GetNodeString(n) + " " + ToInfixString(n->right);
}

// Prefix traversal
string RedBlackTree::ToPrefixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return GetNodeString(n) + " " + ToPrefixString(n->left) + ToPrefixString(n->right);
}

// Postfix traversal
string RedBlackTree::ToPostfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToPostfixString(n->left) + ToPostfixString(n->right) + " " + GetNodeString(n);
}

// GetColorString
string RedBlackTree::GetColorString(const RBTNode* n) {
    if (n->color == COLOR_RED) return "R";
    if (n->color == COLOR_BLACK) return "B";
    return "D";
}


// GetNodeString
string RedBlackTree::GetNodeString(const RBTNode* n) {
    return " " + GetColorString(n) + to_string(n->data) + " ";
}

