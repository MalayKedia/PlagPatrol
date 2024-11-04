#include <iostream>

struct Node {
    int key;
    Node* left;
    Node* right;

    Node(int data) : key(data), left(nullptr), right(nullptr) {}
};

class BST {
    Node* root;

    Node* insert(Node* node, int key) {
        if (!node) return new Node(key);
        if (key < node->key)
            node->left = insert(node->left, key);
        else
            node->right = insert(node->right, key);
        return node;
    }

    Node* findMin(Node* node) {
        while (node && node->left) node = node->left;
        return node;
    }

    Node* deleteNode(Node* node, int key) {
        if (!node) return node;
        if (key < node->key)
            node->left = deleteNode(node->left, key);
        else if (key > node->key)
            node->right = deleteNode(node->right, key);
        else {
            if (!node->left) {
                Node* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            Node* temp = findMin(node->right);
            node->key = temp->key;
            node->right = deleteNode(node->right, temp->key);
        }
        return node;
    }

public:
    BST() : root(nullptr) {}

    void insert(int key) { root = insert(root, key); }

    void deleteNode(int key) { root = deleteNode(root, key); }

    void inorder(Node* node) {
        if (!node) return;
        inorder(node->left);
        std::cout << node->key << " ";
        inorder(node->right);
    }

    void printInorder() {
        inorder(root);
        std::cout << std::endl;
    }
};

int main() {
    BST tree;
    tree.insert(50);
    tree.insert(30);
    tree.insert(20);
    tree.insert(40);
    tree.insert(70);
    tree.insert(60);
    tree.insert(80);

    std::cout << "Inorder traversal of the BST: ";
    tree.printInorder();

    std::cout << "\nDeleting 20\n";
    tree.deleteNode(20);
    tree.printInorder();

    std::cout << "\nDeleting 30\n";
    tree.deleteNode(30);
    tree.printInorder();

    std::cout << "\nDeleting 50\n";
    tree.deleteNode(50);
    tree.printInorder();

    return 0;
}
