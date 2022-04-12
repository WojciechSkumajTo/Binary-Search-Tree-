#include <iostream>
#include <utility>
#include <vector>
#include <fstream>

struct Node {
    Node *left;
    Node *right;
    int data;
};

struct Print {
    Print *prev;
    std::string str;

    Print(Print *prev, std::string str) {
        this->prev = prev;
        this->str = std::move(str);
    }
};

Node *getMinimumKey(Node *curr) {
    while (curr->left != nullptr) {
        curr = curr->left;
    }
    return curr;
}

void searchKey(Node *&curr, int key, Node *&parent) {
    while (curr != nullptr && curr->data != key) {
        parent = curr;
        if (key < curr->data) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
}

void insert(Node **root, int val) {
    Node *temp;
    if (!(*root)) {
        temp = (Node *) malloc(sizeof(Node));
        temp->left = temp->right = nullptr;
        temp->data = val;
        *root = temp;
        return;
    }
    if (val < (*root)->data) {
        insert(&(*root)->left, val);
    } else {
        insert(&(*root)->right, val);
    }
}

void deleteNode(Node **root, int key) {
    Node *parent = nullptr;
    Node *curr = *root;
    searchKey(curr, key, parent);
    if (curr == nullptr) {
        return;
    }
    // Case 1: node to be deleted has no children, i.e., it is a leaf node
    if (curr->left == nullptr && curr->right == nullptr) {
        if (curr != *root) {
            if (parent->left == curr) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
        }
        /*else {
            root = nullptr;
        }*/
        free(curr);
    }
        // Case 2: node to be deleted has two children
    else if (curr->left && curr->right) {
        Node *successor = getMinimumKey(curr->right);
        int val = successor->data;
        deleteNode(root, successor->data);
        curr->data = val;
    }
        // Case 3: node to be deleted has only one child
    else {
        Node *child = (curr->left) ? curr->left : curr->right;

        if (curr != *root) {
            if (curr == parent->left) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        } else {
            *root = child;
        }
        free(curr);
    }
}

void helpShow(Print *p) {
    if (p == nullptr) {
        return;
    }
    helpShow(p->prev);
    std::cout << p->str;
}

void showTree(Node *root, Print *prev, bool isLeft) {
    if (root == nullptr) {
        return;
    }
    std::string prev_str = "    ";
    auto *trunk = new Print(prev, prev_str);
    showTree(root->right, trunk, true);
    if (!prev) {
        trunk->str = "---";
    } else if (isLeft) {
        trunk->str = ".---";
        prev_str = "   |";
    } else {
        trunk->str = "`---";
        prev->str = prev_str;
    }
    helpShow(trunk);
    std::cout << root->data << '\n';
    if (prev) {
        prev->str = prev_str;
    }
    trunk->str = "   |";
    showTree(root->left, trunk, false);
}

std::vector<int> loadNumbers(const std::string &filename) {
    std::ifstream file;
    file.open(filename);
    if (!file.is_open()) {
        throw std::runtime_error("FILE NOT FOUND\n");
    }
    int number;
    std::vector<int> result;
    while (!file.eof()) {
        file >> number;
        file.get();
        result.emplace_back(number);
    }
    return result;
}

//DSW ALGORITHM

void storeBSTNodes(Node *root, std::vector<Node *> &nodes) {
    if (root == nullptr) {
        return;
    }

    storeBSTNodes(root->left, nodes);
    nodes.emplace_back(root);
    storeBSTNodes(root->right, nodes);
}

Node *buildTreeUtil(std::vector<Node *> &nodes, int start, int end) {
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) / 2;
    Node *root = nodes[mid];

    root->left = buildTreeUtil(nodes, start, mid - 1);
    root->right = buildTreeUtil(nodes, mid + 1, end);

    return root;
}

Node *buildTree(Node **root) {
    std::vector<Node *> nodes;
    storeBSTNodes(*root, nodes);
    int n = nodes.size();
    return buildTreeUtil(nodes, 0, n - 1);
}

int main() {
    Node *root = nullptr;
    std::vector<int> vec = loadNumbers("numbers.txt");

    for (const auto &i: vec) {
        insert(&root, i);
    }

    showTree(root, nullptr, false);
    std::cout << "\n";
    root = buildTree(&root);
    showTree(root, nullptr, false);
      
    return 0;
}