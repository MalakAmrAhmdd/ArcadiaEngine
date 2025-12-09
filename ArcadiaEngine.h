// ArcadiaEngine.h
#ifndef ARCADIA_ENGINE_H
#define ARCADIA_ENGINE_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

// PART A: DATA STRUCTURES
class PlayerTable {
public:
    // Use Double Hashing logic
    //Eyad
    virtual void insert(int playerID, string name) = 0; 
    virtual string search(int playerID) = 0;
};

class Leaderboard {
public:
    // Skip List Implementation
    //Malouk
    virtual void addScore(int playerID, int score) = 0;
    virtual void removePlayer(int playerID) = 0;
    virtual vector<int> getTopN(int n) = 0; // Return IDs of top N
};

class AuctionTree {
public:
    // Red-Black Tree Implementation
    //Eyad
    virtual void insertItem(int itemID, int price) = 0;
    virtual void deleteItem(int itemID) = 0;
};

// PART B: DYNAMIC PROGRAMMING
class InventorySystem {
public:
    // Minimizes difference between two coin stacks
    //Yassin
    static int optimizeLootSplit(int n, vector<int>& coins);

    // 0/1 Knapsack logic
    //Yassin
    static int maximizeCarryValue(int capacity, vector<pair<int, int>>& items);

    // String decoding possibilities
    //Yassin
    static long long countStringPossibilities(string s);
};

// PART C: GRAPHS

class WorldNavigator {
public:
    // Return true if path exists
    //Pedro
    static bool pathExists(int n, vector<vector<int>>& edges, int source, int dest);

    // Min cost to satisfy bandit demands
    // Return -1 if impossible
    //Pedro
    static long long minBribeCost(int n, int m, long long goldRate, long long silverRate, 
                                  vector<vector<int>>& roadData);
                                  
    // Sum of min distances in binary
    //Pedro
    static string sumMinDistancesBinary(int n, vector<vector<int>>& roads);
};

// PART D: GREEDY
class ServerKernel {
public:
    // Task Scheduler
    //Malouk
    static int minIntervals(vector<char>& tasks, int n);
};

#endif