#include <string>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

enum Color {RED, BLACK};

class treeNode {
    private:
        int data;
        treeNode* left;
        treeNode* right;
        treeNode* parent;
        int color;
    public:
        treeNode(int data) {
            this->data = data;
            color = RED;
            left = nullptr;
            right = nullptr;
            parent = nullptr;
        };
        
        int getData() {
            return this->data;
        }

        treeNode* getLeft() {
            return this->left;
        }

        treeNode* getRight() {
            return this->right;
        }

        treeNode* getParent() {
            return this->parent;
        }

        int getColor() {
            return this->color;
        }

        void setColor(int color) {
            this->color = color;
        }

        void setRight(treeNode* treeNode) {
            this->right = treeNode;
        }

        void setLeft(treeNode* treeNode) {
            this->left = treeNode;
        }

        void setParent(treeNode* treeNode) {
            this->parent = treeNode;
        }
};

class RBTree {
    private:
        treeNode* root;
        int magicNumber = 0;
    public:
        RBTree() {
            root = nullptr;
        };

        int getMagic() {
            return this->magicNumber;
        }

        int getColor(treeNode* treeNode) {
            if (treeNode == nullptr) {
                return BLACK;
            }
            return treeNode->getColor();
        };

        void setColor(treeNode* treeNode, int color) {
            if (treeNode == nullptr) {
                return;
            }
            treeNode->setColor(color);
        };

        treeNode* insert(treeNode* root, treeNode* insertNode) {
            if (root == nullptr) {
                return insertNode;
            }
            if (insertNode->getData() < root->getData()) {
                root->setLeft(insert(root->getLeft(), insertNode));
                root->getLeft()->setParent(root);
            } else if (insertNode->getData() > root->getData()) {
                root->setRight(insert(root->getRight(), insertNode));
                root->getRight()->setParent(root);
            }
            return root;
        };

        void insertValue(int value) {
            treeNode* newTreeNode = new treeNode(value);
            root = insert(root, newTreeNode);
            fixTree(newTreeNode);
        }

        treeNode* rightChild = nullptr;
        treeNode* leftChild = nullptr;

        void rotateLeft(treeNode* treeNode) {
            rightChild = treeNode->getRight();
            treeNode->setRight(rightChild->getLeft());
            magicNumber += 1;

            if (treeNode->getRight() != nullptr) {
                treeNode->getRight()->setParent(treeNode);
            }

            rightChild->setParent(treeNode->getParent());

            if (treeNode->getParent() == nullptr) {
                root = rightChild;
            } else if (treeNode == treeNode->getParent()->getLeft()) {
                treeNode->getParent()->setLeft(rightChild);
            } else {
                treeNode->getParent()->setRight(rightChild);
            }

            rightChild->setLeft(treeNode);
            treeNode->setParent(rightChild);
        };

        void rotateRight(treeNode* treeNode) {
            leftChild = treeNode->getLeft();
            treeNode->setLeft(leftChild->getRight());
            magicNumber += 1;

            if (treeNode->getLeft() != nullptr) {
                treeNode->getLeft()->setParent(treeNode);
            }

            leftChild->setParent(treeNode->getParent());

            if (treeNode->getParent() == nullptr) {
                root = leftChild;
            } else if (treeNode == treeNode->getParent()->getLeft()) {
                treeNode->getParent()->setLeft(leftChild);
            } else {
                treeNode->getParent()->setRight(leftChild);
            }

            leftChild->setRight(treeNode);
            treeNode->setParent(leftChild);
        };

        treeNode* parent = nullptr;
        treeNode* grandparent = nullptr;
        treeNode* uncle = nullptr;

        void fixTree(treeNode* treeNode) {
            while (treeNode != root && getColor(treeNode) == RED && getColor(treeNode->getParent()) == RED) {
                parent = treeNode->getParent();
                grandparent = parent->getParent();
                if (parent == grandparent->getLeft()) {
                    uncle = grandparent->getRight();
                    if (getColor(uncle) == RED) {
                        setColor(uncle, BLACK);
                        setColor(parent, BLACK);
                        setColor(grandparent, RED);
                        treeNode = grandparent;
                    } else {
                        if (treeNode == parent->getRight()) {
                            rotateLeft(parent);
                            treeNode = parent;
                            parent = treeNode->getParent();
                        }
                        rotateRight(grandparent);
                        int parentColor = getColor(parent);
                        int grandparentColor = getColor(grandparent);
                        setColor(parent, grandparentColor);
                        setColor(grandparent, parentColor);
                        treeNode = parent;
                    }
                } else {
                    uncle = grandparent->getLeft();
                    if (getColor(uncle) == RED) {
                        setColor(uncle, BLACK);
                        setColor(parent, BLACK);
                        setColor(grandparent, RED);
                        treeNode = grandparent;
                    } else {
                        if (treeNode == parent->getLeft()) {
                            rotateRight(parent);
                            treeNode = parent;
                            parent = treeNode->getParent();
                        }
                        rotateLeft(grandparent);
                        int parentColor = getColor(parent);
                        int grandparentColor = getColor(grandparent);
                        setColor(parent, grandparentColor);
                        setColor(grandparent, parentColor);
                        treeNode = parent;
                    }
                }
            }    
            setColor(root, BLACK);
        }
};

long solution(int key, vector<int>& vals) {
    RBTree* rbt = new RBTree();
    for (int i = 0; i < vals.size(); i++) {
        rbt->insertValue(vals.at(i));
    }
    return rbt->getMagic() * key * 137680724221; // 5147 * 5153 * 5167, multiplication of large primes
}

int main() {
    vector<int> vals;
    string key;
    string input;

    cout << "Enter the key value:" << endl;
    try {
        getline(cin, key);
    } catch (exception e) {
        cout << "Bad arguments" << endl;
        exit(1);
    }

    cout << "Enter the values of the nodes one at a time, type 'exit' to finish" << endl;
    getline(cin, input);
    while (input != "exit") {
        try {
            vals.push_back(stoi(input));
        } catch (exception e) {
            cout << "Bad arguments" << endl;
            exit(1);
        }
        getline(cin, input);
    }

    cout << solution(stoi(key), vals) << endl;
}