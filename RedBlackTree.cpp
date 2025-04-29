// Author: Wesley Ihezuo
// Date: 04/24/2025

#include "RedBlackTree.h"
#include <string>
#include <stdexcept>

using namespace std;

// Helper function to recursively destroy all nodes in the tree.
// Frees up memory by deleting left and right children first.
static void DestroyHelper(RBTNode* node) {
    if (node == nullptr) return;
    DestroyHelper(node->left);
    DestroyHelper(node->right);
    delete node;
}

// Default Constructor
// Initializes an empty tree (no nodes yet).
RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

// Constructor that creates a tree with a single black root node.
RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode;
    root->data = newData;
    root->color = COLOR_BLACK;  // Root is always black by property of Red-Black Trees
    root->left = nullptr;
    root->right = nullptr;
    root->parent = nullptr;
    root->IsNullNode = false;
    numItems = 1;
}

// Copy Constructor
// Creates a deep copy of another RedBlackTree (new memory, same structure and values).
RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = CopyOf(rbt.root);
    numItems = rbt.numItems;
}

// Destructor
// Properly cleans up tree nodes to prevent memory leaks.
RedBlackTree::~RedBlackTree() {
    DestroyHelper(root);
}

// Helper function to deep copy a tree starting from a given node.
// This is used by the copy constructor.
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
// Adds a new node into the tree, maintaining Red-Black properties.
void RedBlackTree::Insert(int newData) {
    if (Contains(newData)) {
        throw invalid_argument("Duplicate entry not allowed."); // No duplicate entries allowed
    }

    RBTNode* newNode = new RBTNode;
    newNode->data = newData;
    newNode->color = COLOR_RED; // New nodes are always inserted as red first
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    newNode->IsNullNode = false;

    BasicInsert(newNode);
    InsertFixUp(newNode); // Fix any Red-Black violations after insert
    root->color = COLOR_BLACK; // Ensure root stays black
    numItems++;
}

// BasicInsert
// Standard Binary Search Tree insert (ignores Red-Black rules for now).
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
        root = node;  // New node becomes the root
    } else if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
}

// InsertFixUp
// Fixes Red-Black Tree properties after insertion by rotating/recoloring.
void RedBlackTree::InsertFixUp(RBTNode* node) {
    while (node->parent != nullptr && node->parent->color == COLOR_RED) {
        RBTNode* uncle = GetUncle(node);
        RBTNode* grandparent = node->parent->parent;

        if (uncle != nullptr && uncle->color == COLOR_RED) {
            // Case 1: Parent and Uncle are both red - recolor and move up the tree
            node->parent->color = COLOR_BLACK;
            uncle->color = COLOR_BLACK;
            grandparent->color = COLOR_RED;
            node = grandparent;
        } else {
            // Cases 2 and 3: Rotation cases
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
    root->color = COLOR_BLACK; // Always reassert root is black
}

// LeftRotate
// Performs a left rotation around a given node.
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
// Performs a right rotation around a given node.
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
// Finds and returns the uncle of a node (may return nullptr if no uncle).
RBTNode* RedBlackTree::GetUncle(RBTNode* node) const {
    RBTNode* grandparent = node->parent ? node->parent->parent : nullptr;
    if (grandparent == nullptr) return nullptr;
    if (grandparent->left == node->parent) return grandparent->right;
    return grandparent->left;
}

// IsLeftChild
// Returns true if node is a left child.
bool RedBlackTree::IsLeftChild(RBTNode* node) const {
    return node->parent && node == node->parent->left;
}

// IsRightChild
// Returns true if node is a right child.
bool RedBlackTree::IsRightChild(RBTNode* node) const {
    return node->parent && node == node->parent->right;
}

// Contains
// Returns true if the tree contains a given value.
bool RedBlackTree::Contains(int data) const {
    return (Get(data) != nullptr);
}

// Get
// Returns the node with given data, or nullptr if not found.
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
// Finds and returns the minimum data value in the tree.
int RedBlackTree::GetMin() const {
    if (root == nullptr) throw underflow_error("Tree is empty.");
    RBTNode* curr = root;
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr->data;
}

// GetMax
// Finds and returns the maximum data value in the tree.
int RedBlackTree::GetMax() const {
    if (root == nullptr) throw underflow_error("Tree is empty.");
    RBTNode* curr = root;
    while (curr->right != nullptr) {
        curr = curr->right;
    }
    return curr->data;
}

// Infix Traversal
// Returns a string representing an in-order traversal (left, node, right).
string RedBlackTree::ToInfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToInfixString(n->left) + GetNodeString(n) + ToInfixString(n->right);
}

// Prefix Traversal
// Returns a string representing a pre-order traversal (node, left, right).
string RedBlackTree::ToPrefixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return GetNodeString(n) + ToPrefixString(n->left) + ToPrefixString(n->right);
}

// Postfix Traversal
// Returns a string representing a post-order traversal (left, right, node).
string RedBlackTree::ToPostfixString(const RBTNode* n) {
    if (n == nullptr) return "";
    return ToPostfixString(n->left) + ToPostfixString(n->right) + GetNodeString(n);
}

// GetColorString
// Returns a string representation of a node's color: "R", "B", or "D" (dummy).
string RedBlackTree::GetColorString(const RBTNode* n) {
    if (n->color == COLOR_RED) return "R";
    if (n->color == COLOR_BLACK) return "B";
    return "D";
}

// GetNodeString
// Combines color and data into a formatted string for printing.
string RedBlackTree::GetNodeString(const RBTNode* n) {
    return " " + GetColorString(n) + to_string(n->data) + " ";
}
