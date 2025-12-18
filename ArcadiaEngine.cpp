// ArcadiaEngine.cpp - STUDENT TEMPLATE
// TODO: Implement all the functions below according to the assignment requirements

#include "ArcadiaEngine.h"
#include <algorithm>
#include <queue>
#include <numeric>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <random>
#include <map>
#include <set>
#define ll long long

using namespace std;

// =========================================================
// PART A: DATA STRUCTURES (Concrete Implementations)
// =========================================================

// --- 1. PlayerTable (Double Hashing) ---
// Implemented by: Eyad

class ConcretePlayerTable : public PlayerTable {
private:
    static const int TABLE_SIZE = 101;

    struct Entry {
        int playerID;
        string name;
        bool isOccupied;
        bool isDeleted;

        Entry() : playerID(-1), name(""), isOccupied(false), isDeleted(false) {}
    };

    vector<Entry> table;

    // Primary hash function
    int h1(int key) {
        return key % TABLE_SIZE;
    }

    // Secondary hash function for double hashing
    int h2(int key) {
        return 7 - (key % 7);
    }

public:
    ConcretePlayerTable() {
        table.resize(TABLE_SIZE);
    }

    void insert(int playerID, string name) override {
        int index = h1(playerID);
        int step = h2(playerID);
        int i = 0;

        while (i < TABLE_SIZE) {
            int probeIndex = (index + i * step) % TABLE_SIZE;

            // If slot is empty or was deleted, we can insert here
            if (!table[probeIndex].isOccupied || table[probeIndex].isDeleted) {
                table[probeIndex].playerID = playerID;
                table[probeIndex].name = name;
                table[probeIndex].isOccupied = true;
                table[probeIndex].isDeleted = false;
                return;
            }

            // If same playerID exists, update it
            if (table[probeIndex].playerID == playerID && !table[probeIndex].isDeleted) {
                table[probeIndex].name = name;
                return;
            }

            i++;
        }

        // Table is full
        cout << "Table is full" << endl;
    }

    string search(int playerID) override {
        int index = h1(playerID);
        int step = h2(playerID);
        int i = 0;

        while (i < TABLE_SIZE) {
            int probeIndex = (index + i * step) % TABLE_SIZE;

            // If slot was never occupied, player doesn't exist
            if (!table[probeIndex].isOccupied) {
                return "";
            }

            // If slot has the player and is not deleted
            if (table[probeIndex].playerID == playerID && !table[probeIndex].isDeleted) {
                return table[probeIndex].name;
            }

            i++;
        }

        return "";
    }
};

// --- 2. Leaderboard (Skip List) ---


class ConcreteLeaderboard : public Leaderboard {
    // TODO: Define your skip list node structure and necessary variables
    // Hint: You'll need nodes with multiple forward pointers

private:
    // Expected no. of players is 101, so max no. of levels is log(100) = 10
    const int levels = 11;
    struct Node {
        vector <Node*> forward;
        int level;
        int playerID;
        int score;
        string playerName;

        Node() : forward(vector<Node*> (11)), level(0), playerID(0), playerName("") {}
    };

    Node* head;
    int currLevel;



public:
    ConcreteLeaderboard() {
        // TODO: Initialize your skip list
        head = new Node();
        currLevel = 0;
    }

    // Return greatest score node but smaller than the one we're searching for
    Node* searchByScore(int playerID, int score) {
        Node* trav = head;

        for (int lvl = currLevel; lvl >= 0; lvl--) {

            while (trav->forward[lvl] != NULL) {

                Node* next = trav->forward[lvl];

                // Case 1: Next score is greater → keep moving (descending order)
                if (next->score > score) {
                    trav = next;
                }
                // Case 2: Same score: check ID ascending
                else if (next->score == score && next->playerID < playerID) {
                    trav = next;
                }
                // Case 3: next should not be passed → stop
                else {
                    break;
                }
            }
        }

        return trav;
    }

    void insertUpperLevels(Node* newNode) {

        int playerID = newNode->playerID;
        int score = newNode->score;
        int nodeLevel = newNode->level;

        for (int lvl = 1; lvl <= nodeLevel; lvl++) {

            Node* trav = head;

            while (trav->forward[lvl] != NULL &&
                  (trav->forward[lvl]->score > score ||
                  (trav->forward[lvl]->score == score && trav->forward[lvl]->playerID < playerID))) {

                trav = trav->forward[lvl];
                  }

            newNode->forward[lvl] = trav->forward[lvl];
            trav->forward[lvl] = newNode;
        }
    }


    void addScore(int playerID, int score) override {

        // Level 0 predecessor
        Node* prev = searchByScore(playerID, score);

        Node* newNode = new Node();
        newNode->playerID = playerID;
        newNode->score = score;

        // Random height
        int nodeLevel = 0;
        int random = random_device{}() % 2;
        while ((random & 1) && nodeLevel < levels - 1) nodeLevel++;
        newNode->level = nodeLevel;

        // Insert at level 0
        newNode->forward[0] = prev->forward[0];
        prev->forward[0] = newNode;

        // Update skip list max level
        if (nodeLevel > currLevel)
            currLevel = nodeLevel;

        // Insert higher levels using helper
        insertUpperLevels(newNode);
    }


    void removePlayer(int playerID) override {
        // TODO: Implement skip list deletion
        Node* curr = head;
        while (curr->forward[0] != NULL) {
            curr = curr->forward[0];
            if (curr->playerID == playerID) break;
        }

        for (int lvl = 0; lvl <= curr->level; lvl++) {
            Node* trav = head;
            while (trav->forward[lvl] != NULL && trav->forward[lvl] != curr) {
                trav = trav->forward[lvl];
            }
            if (trav->forward[lvl] == curr) {
                trav->forward[lvl] = curr->forward[lvl];
            }
        }

        while (currLevel > 0 && head->forward[currLevel] == nullptr)
            currLevel--;

        delete curr;
    }

    vector<int> getTopN(int n) override {
        // TODO: Return top N player IDs in descending score order
        Node* first = head;

        vector<int> result;
        for (int i = 0; i < n; i++) {
            if (first->forward[0] == NULL) break;
            result.push_back(first->forward[0]->playerID);
            first = first->forward[0];
        }
        return result;
    }
};

// --- 3. AuctionTree (Red-Black Tree) ---
// Implemented by: Eyad

class ConcreteAuctionTree : public AuctionTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        int itemID;
        int price;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(int id, int p) : itemID(id), price(p), color(RED),
                              left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root;
    Node* NIL;

    // Helper function to compare nodes (price first, then itemID for ties)
    bool isLess(Node* a, Node* b) {
        if (a->price != b->price) return a->price < b->price;
        return a->itemID < b->itemID;
    }

    // Left rotation
    void rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        if (y->left != NIL) {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->left) {
            x->parent->left = y;
        } else {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    // Right rotation
    void rotateRight(Node* x) {
        Node* y = x->left;
        x->left = y->right;
        if (y->right != NIL) {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr) {
            root = y;
        } else if (x == x->parent->right) {
            x->parent->right = y;
        } else {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    // Fix Red-Black Tree after insertion
    void insertFixup(Node* z) {
        while (z->parent != nullptr && z->parent->color == RED) {
            if (z->parent->parent != nullptr && z->parent == z->parent->parent->left) {
                Node* y = z->parent->parent->right;
                if (y != NIL && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        rotateLeft(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateRight(z->parent->parent);
                }
            } else if (z->parent->parent != nullptr) {
                Node* y = z->parent->parent->left;
                if (y != NIL && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rotateRight(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rotateLeft(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    // Transplant helper for deletion
    void transplant(Node* u, Node* v) {
        if (u->parent == nullptr) {
            root = v;
        } else if (u == u->parent->left) {
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    // Find minimum node in subtree
    Node* minimum(Node* node) {
        while (node->left != NIL) {
            node = node->left;
        }
        return node;
    }

    // Fix Red-Black Tree after deletion
    void deleteFixup(Node* x) {
        while (x != root && x->color == BLACK) {
            if (x == x->parent->left) {
                Node* w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right->color == BLACK) {
                        w->left->color = BLACK;
                        w->color = RED;
                        rotateRight(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                Node* w = x->parent->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left->color == BLACK) {
                        w->right->color = BLACK;
                        w->color = RED;
                        rotateLeft(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    // Find node by itemID (O(N) traversal)
    Node* findByID(Node* node, int itemID) {
        if (node == NIL) return nullptr;

        if (node->itemID == itemID) return node;

        Node* leftResult = findByID(node->left, itemID);
        if (leftResult != nullptr) return leftResult;

        return findByID(node->right, itemID);
    }

    // Delete a specific node
    void deleteNode(Node* z) {
        if (z == nullptr || z == NIL) return;

        Node* y = z;
        Node* x;
        Color yOriginalColor = y->color;

        if (z->left == NIL) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == NIL) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z) {
                x->parent = y;
            } else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK) {
            deleteFixup(x);
        }
    }

    // Clear tree helper
    void clearTree(Node* node) {
        if (node == NIL) return;
        clearTree(node->left);
        clearTree(node->right);
        delete node;
    }

public:
    ConcreteAuctionTree() {
        NIL = new Node(0, 0);
        NIL->color = BLACK;
        NIL->left = nullptr;
        NIL->right = nullptr;
        NIL->parent = nullptr;
        root = NIL;
    }

    ~ConcreteAuctionTree() {
        clearTree(root);
        delete NIL;
    }

    void insertItem(int itemID, int price) override {
        Node* z = new Node(itemID, price);
        z->left = NIL;
        z->right = NIL;

        Node* y = nullptr;
        Node* x = root;

        while (x != NIL) {
            y = x;
            if (isLess(z, x)) {
                x = x->left;
            } else {
                x = x->right;
            }
        }

        z->parent = y;
        if (y == nullptr) {
            root = z;
        } else if (isLess(z, y)) {
            y->left = z;
        } else {
            y->right = z;
        }

        z->color = RED;
        insertFixup(z);
    }

    void deleteItem(int itemID) override {
        // O(N) search to find node by itemID
        Node* nodeToDelete = findByID(root, itemID);
        if (nodeToDelete != nullptr) {
            deleteNode(nodeToDelete);
        }
    }
};

// =========================================================
// PART B: INVENTORY SYSTEM (Dynamic Programming)
// =========================================================

int InventorySystem::optimizeLootSplit(int n, vector<int>& coins) {
    // TODO: Implement partition problem using DP
    // Goal: Minimize |sum(subset1) - sum(subset2)|
    // Hint: Use subset sum DP to find closest sum to total/2
    int s = 0;
    for(int coin : coins) s += coin;
    int target = s / 2;

    vector<bool> dp(target + 1, false);
    dp[0] = true;
    for (int coin : coins) {
        for (int j = target; j >= coin; j--) {
            if (dp[j - coin]) {
                dp[j] = true;
            }
        }
    }
    for (int i = target; i >= 0; i--) {
        if (dp[i]) {
            return s - 2 * i;
        }
    }
    return s;
}

int InventorySystem::maximizeCarryValue(int capacity, vector<pair<int, int>>& items) {
    // TODO: Implement 0/1 Knapsack using DP
    // items = {weight, value} pairs
    // Return maximum value achievable within capacity
    vector<int> dp(capacity + 1, 0);
    for (const auto& item : items) {
        int weight = item.first;
        int value = item.second;
        for (int w = capacity; w >= weight; w--) {
            dp[w] = max(dp[w], dp[w - weight] + value);
        }
    }
    return dp[capacity];
}
static const long long MOD = 1e9 + 7;
long long InventorySystem::countStringPossibilities(string s) {
    // TODO: Implement string decoding DP
    // Rules: "uu" can be decoded as "w" or "uu"
    //        "nn" can be decoded as "m" or "nn"
    // Count total possible decodings

    for (char c : s) {
        if (c == 'w' || c == 'm')
            return 0;
    }

    int n = s.size();
    vector<long long> dp(n + 1, 0);
    dp[0] = 1; // Empty string
    dp[1] = 1; // Single character

    for (int i = 2; i <= n ; ++i) {
        dp[i] = dp[i - 1]; // 2
        if(s[i - 1] == 'u' && s[i - 2] == 'u') {
            dp[i] = (dp[i] + dp[i - 2]) % MOD;
        }
        if(s[i - 1] == 'n' && s[i - 2] == 'n') {
            dp[i] = (dp[i] + dp[i - 2]) % MOD;
        }
        dp[i] %= MOD;
    }
    return dp[n];

}

// =========================================================
// PART C: WORLD NAVIGATOR (Graphs)
// =========================================================

bool WorldNavigator::pathExists(int n, vector<vector<int>>& edges, int source, int dest) {
    // TODO: Implement path existence check using BFS or DFS
    // edges are bidirectional
    return false;
}

long long WorldNavigator::minBribeCost(int n, int m, long long goldRate, long long silverRate,
                                       vector<vector<int>>& roadData) {
    // TODO: Implement Minimum Spanning Tree (Kruskal's or Prim's)
    // roadData[i] = {u, v, goldCost, silverCost}
    // Total cost = goldCost * goldRate + silverCost * silverRate
    // Return -1 if graph cannot be fully connected
    return -1;
}

string WorldNavigator::sumMinDistancesBinary(int n, vector<vector<int>>& roads) {
    // TODO: Implement All-Pairs Shortest Path (Floyd-Warshall)
    // Sum all shortest distances between unique pairs (i < j)
    // Return the sum as a binary string
    // Hint: Handle large numbers carefully
    return "0";
}

// =========================================================
// PART D: SERVER KERNEL (Greedy)
// =========================================================

int ServerKernel::minIntervals(vector<char>& tasks, int n) {
    // TODO: Implement task scheduler with cooling time
    // Same task must wait 'n' intervals before running again
    // Return minimum total intervals needed (including idle time)
    // Hint: Use greedy approach with frequency counting

    // this solution is O(n)
    vector<int> count(26, 0);
    for (int i = 0; i < tasks.size(); i++) {
        count[tasks[i] - 'A']++;
    }

    int max = 0;
    for (int i = 0; i < 26; i++) {
        if (count[i] > max) max = count[i];
    }

    int rep = -1;
    for (int i = 0; i < 26; i++) {
        if (count[i] == max) rep++;
    }

    int res = count[0] + (count[0] - 1) * n;
    res += rep;

    if (max == 1 || n == 0) res = tasks.size();
    if (tasks.empty()) res = 0;

    return res;
}

// =========================================================
// FACTORY FUNCTIONS (Required for Testing)
// =========================================================

extern "C" {
    PlayerTable* createPlayerTable() { 
        return new ConcretePlayerTable(); 
    }

    Leaderboard* createLeaderboard() { 
        return new ConcreteLeaderboard(); 
    }

    AuctionTree* createAuctionTree() { 
        return new ConcreteAuctionTree(); 
    }
}

