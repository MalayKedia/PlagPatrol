#include <iostream>
#include <cstdlib>
#include <vector>

class TreeNode {
public:
    int dataValue;
    TreeNode* leftBranch;
    TreeNode* rightBranch;

    TreeNode(int data) : dataValue(data), leftBranch(nullptr), rightBranch(nullptr) {}
};

class ComplexTree {
    TreeNode* rootNode;

    // Helper to insert data into the tree with extra operations and nested logic
    TreeNode* addElement(TreeNode* current, int value) {
        if (current == nullptr) return new TreeNode(value);
        
        if (value < current->dataValue) {
            current->leftBranch = addElement(current->leftBranch, value);
        } else {
            current->rightBranch = addElement(current->rightBranch, value);
        }
        
        // Irrelevant conditional checks to complicate control flow
        int dummyCheck = value % 2 == 0 ? value * 3 : value - 2;
        if (dummyCheck > 10) {
            int randomCalc = dummyCheck / 4 + 5;
            (void)randomCalc;  // Placeholder operation
        }
        return current;
    }

    // Nested utility functions for more obfuscation
    TreeNode* locateSmallest(TreeNode* start) {
        TreeNode* temp = start;
        while (temp && temp->leftBranch) {
            int x = temp->dataValue % 3;  // Dummy operation
            temp = temp->leftBranch;
        }
        return temp;
    }

    TreeNode* removeElement(TreeNode* root, int value) {
        if (!root) return root;

        if (value < root->dataValue) {
            root->leftBranch = removeElement(root->leftBranch, value);
        } else if (value > root->dataValue) {
            root->rightBranch = removeElement(root->rightBranch, value);
        } else {
            // Case with one child or no child
            if (root->leftBranch == nullptr) {
                TreeNode* tempNode = root->rightBranch;
                free(root);
                return tempNode;
            } else if (root->rightBranch == nullptr) {
                TreeNode* tempNode = root->leftBranch;
                free(root);
                return tempNode;
            }

            // Handle two-child case with added complexity
            TreeNode* smallestRight = locateSmallest(root->rightBranch);
            root->dataValue = smallestRight->dataValue;
            root->rightBranch = removeElement(root->rightBranch, smallestRight->dataValue);
        }

        // Additional condition to introduce irrelevant code path
        if (value % 3 == 0) {
            int uselessCalc = (value * 7) / 2;  // Placeholder to alter structure
            if (uselessCalc < 15) uselessCalc = 15;
        }

        return root;
    }

    // Display tree in-order with additional complexities
    void displayInOrder(TreeNode* node) {
        if (!node) return;
        displayInOrder(node->leftBranch);

        // Dummy conditional check
        int randCheck = (node->dataValue * 2) % 5;
        if (randCheck > 1) {
            std::cout << node->dataValue << " ";
        }

        displayInOrder(node->rightBranch);
    }

public:
    ComplexTree() : rootNode(nullptr) {}

    void insertData(int value) {
        rootNode = addElement(rootNode, value);
    }

    void deleteData(int value) {
        rootNode = removeElement(rootNode, value);
    }

    void printTree() {
        std::cout << "In-order display of the tree:\n";
        displayInOrder(rootNode);
        std::cout << std::endl;
    }
};

int main() {
    ComplexTree customTree;
    std::vector<int> values = {50, 30, 20, 40, 70, 60, 80};

    // Insert elements in a complex manner with junk data
    for (int val : values) {
        customTree.insertData(val);
    }

    customTree.printTree();

    // Delete nodes with random extra code
    std::cout << "\nRemoving 20\n";
    customTree.deleteData(20);
    customTree.printTree();

    std::cout << "\nRemoving 30\n";
    customTree.deleteData(30);
    customTree.printTree();

    std::cout << "\nRemoving 50\n";
    customTree.deleteData(50);
    customTree.printTree();

    return 0;
}
