//UMBC - CSEE - CMSC 341 - Fall 2025 - Proj2
#include "booktree.h"
BookTree::BookTree() {
    m_root = nullptr;
}

BookTree::~BookTree() {
    destroyTree(m_root);
    m_root = nullptr;
}

void BookTree::loadData(Book data[], int arrayLength) {
    for (int i = 0; i < arrayLength; i++) {
        insert(data[i].m_title, data[i].m_author, data[i].m_text);
    }
}

bool BookTree::insert(string title, string author, string text) {
    if (!m_root) {
        m_root = new BNode(title, author, text);
        return true;
    }

    // Splay the closest node to root
    m_root = splay(m_root, title);

    // Duplicate title � don�t insert
    if (m_root->m_title == title)
        return false;

    BNode* newNode = new BNode(title, author, text);

    // Insert new node and reattach subtrees
    if (title < m_root->m_title) {
        newNode->m_right = m_root;
        newNode->m_left = m_root->m_left;
        m_root->m_left = nullptr;
    }
    else {
        newNode->m_left = m_root;
        newNode->m_right = m_root->m_right;
        m_root->m_right = nullptr;
    }

    m_root = newNode;
    return true;
}


int BookTree::findFrequency(string title, string word) {
    bool found = false;
    m_root = access(m_root, title, found);

    if (!found)
        return 0;

    Node* node = m_root->m_tree.find(word);
    return node ? node->m_count : 0;
}

int BookTree::searchCount(string title, string word) {
    return 0;
}

int BookTree::getTextTreeHeight(string title) {
    return 0;
}

string BookTree::getRootKey() {
    // implementation is provided
    // do not modify this function
    return m_root->m_title;
}

bool BookTree::removeWord(string title, string word) {
    BNode* curr = m_root;
    while (curr) {
        if (title == curr->m_title)
            return curr->m_tree.remove(word);
        else if (title < curr->m_title)
            curr = curr->m_left;
        else
            curr = curr->m_right;
    }
    return false;
}

void BookTree::dump(bool verbose) const {
    //cout << "m_root is " << m_root->m_title << endl;
    dumpHelper(m_root, verbose);
    cout << endl;
}

void BookTree::dumpHelper(BNode* root, bool verbose) const {
    if (root != nullptr) {
        if (verbose) {
            cout << "(";
            dumpHelper(root->m_left, verbose);
            cout << root->m_title << ":" << root->m_author;
            root->m_tree.dump();
            dumpHelper(root->m_right, verbose);
            cout << ")";
        }
        else {
            cout << "(";
            dumpHelper(root->m_left, verbose);
            cout << root->m_title;
            dumpHelper(root->m_right, verbose);
            cout << ")";
        }
    }
}

// ============================================================
// Private Helper Functions
// ============================================================

// Right rotation (Zig)
BNode* BookTree::rotateRight(BNode* x) {
    BNode* y = x->m_left;
    x->m_left = y->m_right;
    y->m_right = x;
    return y;
}

// Left rotation (Zag)
BNode* BookTree::rotateLeft(BNode* x) {
    BNode* y = x->m_right;
    x->m_right = y->m_left;
    y->m_left = x;
    return y;
}

// root is root and key is title
BNode* BookTree::splay(BNode* root, const string& title) {
    if (!root || root->m_title == title)
        return root;

    // Left subtree
    if (title < root->m_title) {
        if (!root->m_left) return root;

        // Zig-Zig (Left Left)
        if (title < root->m_left->m_title) {
            root->m_left->m_left = splay(root->m_left->m_left, title);
            root = rotateRight(root);
        }
        // Zig-Zag (Left Right)
        else if (title > root->m_left->m_title) {
            root->m_left->m_right = splay(root->m_left->m_right, title);
            if (root->m_left->m_right)
                root->m_left = rotateLeft(root->m_left);
        }

        return (root->m_left == nullptr) ? root : rotateRight(root);
    }
    else { // title > root->m_title
        if (!root->m_right) return root;

        // Zag-Zig (Right Left)
        if (title < root->m_right->m_title) {
            root->m_right->m_left = splay(root->m_right->m_left, title);
            if (root->m_right->m_left)
                root->m_right = rotateRight(root->m_right);
        }
        // Zag-Zag (Right Right)
        else if (title > root->m_right->m_title) {
            root->m_right->m_right = splay(root->m_right->m_right, title);
            root = rotateLeft(root);
        }

        return (root->m_right == nullptr) ? root : rotateLeft(root);
    }
}

// Find and splay target or would-be parent
BNode* BookTree::access(BNode* root, const string& title, bool& found) {
    root = splay(root, title);
    found = (root && root->m_title == title);
    return root;
}

void BookTree::destroyTree(BNode* node) {
    if (!node) return;
    destroyTree(node->m_left);
    destroyTree(node->m_right);
    delete node;
}

///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
WordTree::~WordTree() {
    destroyTree(m_root);
}

Node* WordTree::find(const string& word) {
    return nullptr;
}

bool WordTree::remove(const string& word) {
    bool flag = false;
    m_root = removeRec(m_root, word, flag);
    return flag;
}

void WordTree::insert(const string& word) {
    m_root = insertRec(m_root, word);
}

int WordTree::searchCount(string word) {
    return 0;
}

int WordTree::getTreeHeight() {
    // implementation is provided
    // do not modify this function
    return m_root->m_height;
}

void WordTree::dump(std::ostream& ostr) {
    dump(m_root, ostr);
}

void WordTree::dump(Node* aNode, std::ostream& ostr) {
    if (aNode != nullptr) {
        ostr << "[";
        dump(aNode->m_left, ostr);//first visit the left child
        ostr << aNode->m_word << ":" << aNode->m_count << ":" << aNode->m_height;//second visit the node itself
        dump(aNode->m_right, ostr);//third visit the right child
        ostr << "]";
    }
}

// ============================================================
// Private Helper Functions
// ============================================================

void WordTree::destroyTree(Node* node) {
    if (node == nullptr) return;
    destroyTree(node->m_left);
    destroyTree(node->m_right);
    delete node;
}

Node* WordTree::insertRec(Node* node, const string& word) {
    if (node == nullptr)
        return new Node(word);

    if (word == node->m_word) {
        node->m_count++;
        return node;
    }
    else if (word < node->m_word)
        node->m_left = insertRec(node->m_left, word);
    else
        node->m_right = insertRec(node->m_right, word);

    // Update height
    updateHeight(node);

    // Balance factor
    int balance = getBalance(node);

    // Left Heavy
    if (balance > 1 && word < node->m_left->m_word)
        return rotateRight(node);

    // Right Heavy
    if (balance < -1 && word > node->m_right->m_word)
        return rotateLeft(node);

    // Left Right
    if (balance > 1 && word > node->m_left->m_word) {
        node->m_left = rotateLeft(node->m_left);
        return rotateRight(node);
    }

    // Right Left
    if (balance < -1 && word < node->m_right->m_word) {
        node->m_right = rotateRight(node->m_right);
        return rotateLeft(node);
    }

    return node;
}

Node* WordTree::removeRec(Node* node, const string& word, bool& success) {
    if (node == nullptr)
        return nullptr;

    if (word < node->m_word)
        node->m_left = removeRec(node->m_left, word, success);
    else if (word > node->m_word)
        node->m_right = removeRec(node->m_right, word, success);
    else {
        success = true;
        if (node->m_count > 1) {
            node->m_count--;
            return node;
        }
        if (node->m_left == nullptr || node->m_right == nullptr) {
            Node* temp = node->m_left ? node->m_left : node->m_right;
            delete node;
            return temp;
        }
        Node* temp = node->m_right;
        while (temp->m_left != nullptr)
            temp = temp->m_left;
        node->m_word = temp->m_word;
        node->m_count = temp->m_count;
        temp->m_count = 1;
        node->m_right = removeRec(node->m_right, node->m_word, success);
    }

    // Recalculate height and balance
    node->m_height = 1 + max(getHeight(node->m_left), getHeight(node->m_right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->m_left) >= 0)
        return rotateRight(node);
    if (balance > 1 && getBalance(node->m_left) < 0) {
        node->m_left = rotateLeft(node->m_left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->m_right) <= 0)
        return rotateLeft(node);
    if (balance < -1 && getBalance(node->m_right) > 0) {
        node->m_right = rotateRight(node->m_right);
        return rotateLeft(node);
    }

    return node;
}

int WordTree::getHeight(Node* node) {
    return (node == nullptr) ? -1 : node->m_height;
}

void WordTree::updateHeight(Node* node) {
    if (node == nullptr) return;

    int leftHeight = (node->m_left) ? node->m_left->m_height : -1;
    int rightHeight = (node->m_right) ? node->m_right->m_height : -1;

    node->m_height = 1 + max(leftHeight, rightHeight);
}

int WordTree::getBalance(Node* node) {
    if (node == nullptr) return 0;
    return getHeight(node->m_left) - getHeight(node->m_right);
}

Node* WordTree::rotateRight(Node* y) {
    Node* x = y->m_left;
    Node* T2 = x->m_right;

    x->m_right = y;
    y->m_left = T2;

    y->m_height = 1 + max(getHeight(y->m_left), getHeight(y->m_right));
    x->m_height = 1 + max(getHeight(x->m_left), getHeight(x->m_right));

    return x;
}

Node* WordTree::rotateLeft(Node* x) {
    Node* y = x->m_right;
    Node* T2 = y->m_left;

    y->m_left = x;
    x->m_right = T2;

    x->m_height = 1 + max(getHeight(x->m_left), getHeight(x->m_right));
    y->m_height = 1 + max(getHeight(y->m_left), getHeight(y->m_right));

    return y;
}


////////////////////////////////////////////////////////////
BNode::BNode() {
    m_title = "";
    m_author = "";
    m_left = nullptr;
    m_right = nullptr;
}

BNode::BNode(string title, string author, string text) {
    m_title = title;
    m_author = author;
    m_left = nullptr;
    m_right = nullptr;

    string word;
    for (size_t i = 0; i <= text.size(); i++) {
        if (i == text.size() || text[i] == ' ') {
            if (!word.empty()) {
                m_tree.insert(word);
                word.clear();
            }
        }
        else {
            word += text[i];
        }
    }
}

BNode::~BNode() {
    m_left = nullptr;
    m_right = nullptr;
}

int BNode::findFrequency(string word) {
    return 0;
}

int BNode::searchCount(string word) {
    return 0;
}

int BNode::getTextTreeHeight() {
    return 0;
}
