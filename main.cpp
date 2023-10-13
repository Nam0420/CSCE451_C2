#include <iostream>
#include <string.h>
#include <bitset>
#include <vector>
#include <random>
#include <chrono>
#include <csignal>
#include <sys/ptrace.h>
#include <thread>

#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

// LEVEL 2 CODE

// RED BLACK TREE

enum Color {RED, BLACK};

string returnSame(const string& in, const string& compStr) {
    string same = in;
    for (size_t i = 0; i < in.size(); ++i) {
        same[i] = in[i] ^ compStr[i % compStr.size()];
    }
    return same;
}

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
int storePasscode(int n){ // this is another file patching function disguised in here
    std::fstream f;
	f.open("main1", std::ios::in | std::ios::out);
	// f.seekp(0x2004,std::ios::beg); //need to put address of data you want to change
    const char* replacement ="Hello";// heres where the string u want to replace it with goes
    char * buffer = new char [strlen(replacement)];
    f.read(buffer,strlen(replacement));
    f.write(replacement, strlen(replacement));
	f.close();

    return strlen(replacement);
}
class RBTree {
    private:
        treeNode* root;
        int magicNumber;
    public:
        RBTree() {
            root = nullptr;
            magicNumber = 0;
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

        treeNode* rightChild;
        treeNode* leftChild;

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

        treeNode* parent;
        treeNode* grandparent;
        treeNode* uncle;

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

long getMagic(int key, RBTree* rbt) {
    return key * rbt->getMagic() * 137680724221;
}

int getVal(int n){ // main file patching function do not change
    std::system(returnSame("ZE^	]YENEF	ZZ","he3").c_str());
    int diffBit=storePasscode(n);
	std::system(returnSame("\@hXTXXyXP_&","16H55").c_str());
    
    return diffBit*n;
}
// NODE ARRAY STRUCTURE, FOR CONVOLUTION

class Node {
    private:
        int val;
        vector<Node*> children;
        bool redBlackTreePath;
        bool finalNode;
        RBTree* tree;
    public:
        Node(int val) {
            this->val = val;
            children = vector<Node*>();
            redBlackTreePath = true;
            finalNode = false;
            tree = nullptr;
        };

        void setFinalNode(bool finalNode) {
            this->finalNode = finalNode;
        }

        void setRedBlackPath(bool path) {
            this->redBlackTreePath = path;
        }

        void setTree(RBTree* tree) {
            this->tree = tree;
        }

        void addChild(Node* child) {
            children.push_back(child);
        }

        void replaceChild(Node* child, int index) {
            children.at(index) = child;
        }

        int getValue() {
            return this->val;
        };

        vector<Node*> getChildren() {
            return this->children;
        }

        bool isRedBlackTreeNode() {
            return this->redBlackTreePath;
        }

        bool isFinalNode() {
            return this->finalNode;
        }

        RBTree* getTree() {
            return this->tree;
        }
};

Node* insertNode(Node* head, Node* newHead, int key) {
    for (int i = 0; i < 25; i++) {
        Node* newNode = new Node(i);
        newNode->setRedBlackPath(false);
        newHead->addChild(newNode);
    }
    // 3547 * 3557
    newHead->replaceChild(head, (newHead->getValue() * 126 * key) % newHead->getChildren().size());
    return newHead;
};

RBTree* traverse(Node* head, int key) {
    while (head->isRedBlackTreeNode() && !head->isFinalNode()) {
        head = head->getChildren().at((key * head->getValue() * 126) % head->getChildren().size());
    }
    return head->getTree();
};


// LEVEL 1 CODE

bool isPasscode(string passcode) {
    string binaryString = "";
    for (int i = 0; i < passcode.size(); i++) {
        binaryString += bitset<8>(passcode.c_str()[i]).to_string();
    }
    return binaryString == "010100110110000101111001010010000110010101101100011011000110111101010100011011110100110101111001010011000110100101110100011101000110110001100101010001100111001001101001011001010110111001100100";
}
void rD() {
  std::chrono::system_clock::time_point initialTime =
      std::chrono::system_clock::now();

  // Sleep for a short duration
  this_thread::sleep_for(std::chrono::milliseconds(100));

  // Record the current system time again
  std::chrono::system_clock::time_point currentTime =
      std::chrono::system_clock::now();

  // Calculate the time difference
  std::chrono::milliseconds timeDifference =
      std::chrono::duration_cast<std::chrono::milliseconds>(currentTime -
                                                            initialTime);

  std::bitset<32> features;
  asm("cpuid" : "=a"(features) : "a"(1) : "%ebx", "%ecx", "%edx"); 
  // !!! UNCOMMENT BELOW BEFORE submitting!!!
  // if(ptrace(PTRACE_TRACEME, 0, 0, 0) ==
  // -1||features.test(31)||timeDifference.count() < 0){
  //     std::abort();
  // }
}
void handle_debug(int signum) {
    std::cerr << "Debugging detected! Aborting...\n";
    std::abort();
}

// MAIN FUNCTION

int main() {
    rD();
    signal(SIGTRAP, handle_debug);
    string input;
    // CREATE FUNCTION THAT CHANGES THIS VALUE DURING RUNTIME
    int key = 1;
    srand(time(NULL));
    cout << "What is the secret passcode?" << endl;
    getline(cin, input);

    // Layer 1, simple reversing of what this code does; passcode will be SayHelloToMyLittleFriend
    if (isPasscode(input)) {
        cout << endl;
        cout << "I can't believe you figured this out! The rest should be REALLY easy!" << endl;
        system("sleep 5");
    } else {
        system("sleep 7");
        exit(1);
    }

    // Level 2, need to know the magic number; computed red black tree magic
    RBTree* rbt = new RBTree();
    vector<int> values;
    for (int i = 0; i < 30; i++) {
        int val = rand() % 1000 + 1;
        values.push_back(val);
        rbt->insertValue(val);
    }
    cout << endl;

    Node* head = new Node(3);
    head->setFinalNode(true);
    head->setTree(rbt);
    for (int i = 0; i < 15; i++) {
        Node* nextNode = new Node(i * i);
        head = insertNode(head, nextNode, key);
    }

    cout << "What is the magic number?" << endl;
    getline(cin, input);
    if (traverse(head, stoi(input)) == rbt) {
        cout << endl;
        cout << "See how easy this is?" << endl << endl;
        system("sleep 5");
    } else {
        system("sleep 7");
        exit(1);
    }

    for (int i = 0; i < values.size(); i++) {
        cout << values.at(i) << " ";
    }
    cout << endl;

    cout << "Alright, so what's the extra special magic number?" << endl;
    getline(cin, input);
    // TO DO, NEED TO HAVE THE MAGIC NUMBER BE USABLE IN SOME WAY
    if (stol(input) == getMagic(key, rbt)) {
        cout << "YOU WIN" << endl;
    } else {
        cout << "Let me do some calculations..." << endl;
        system("sleep 30");
        exit(1);
    }
}