#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <functional>
#include "ArcadiaEngine.h"
#include <algorithm>

using namespace std;

extern "C" {
    PlayerTable* createPlayerTable();
    Leaderboard* createLeaderboard();
    AuctionTree* createAuctionTree();
}

class StudentTestRunner {
    int count = 0;
    int passed = 0;
    int failed = 0;

public:
    void runTest(string testName, bool condition) {
        count++;
        cout << "TEST: " << left << setw(90) << testName;
        if (condition) {
            cout << "[ PASS ]";
            passed++;
        } else {
            cout << "[ FAIL ]";
            failed++;
        }
        cout << endl;
    }

    void printSummary() {
        cout << "\n==========================================" << endl;
        cout << "SUMMARY: Passed: " << passed << " | Failed: " << failed << endl;
        cout << "==========================================" << endl;
        cout << "TOTAL TESTS: " << count << endl;
        if (failed == 0) {
            cout << "Excellent! All tests passed!" << endl;
        }
    }
};

StudentTestRunner runner;

void test_PartA_DataStructures_Extended() {
    cout << "\n========== Part A: Extended Tests ==========" << endl;
    
    // PlayerTable edge cases
    cout << "\n--- PlayerTable Edge Cases ---" << endl;
    PlayerTable* table = createPlayerTable();
    
    runner.runTest("PlayerTable: Search empty table", [&]() {
        return table->search(1) == "";
    }());
    
    runner.runTest("PlayerTable: Update same ID multiple times", [&]() {
        table->insert(1, "First");
        table->insert(1, "Second");
        table->insert(1, "Third");
        return table->search(1) == "Third";
    }());
    
    runner.runTest("PlayerTable: Insert 101 players (stress test)", [&]() {
        for(int i = 1; i <= 101; i++) {
            table->insert(i, "Player" + to_string(i));
        }
        return table->search(50) == "Player50";
    }());
    
    delete table;
    
    // Leaderboard edge cases
    cout << "\n--- Leaderboard Edge Cases ---" << endl;
    Leaderboard* board = createLeaderboard();
    
    runner.runTest("Leaderboard: Remove from empty board", [&]() {
        board->removePlayer(999);
        return board->getTopN(1).empty();
    }());
    
    runner.runTest("Leaderboard: Multiple updates same player", [&]() {
        board->addScore(1, 100);
        board->addScore(1, 200);
        board->addScore(1, 50);
        vector<int> top = board->getTopN(1);
        return top[0] == 1;
    }());
    
    runner.runTest("Leaderboard: All zeros scores", [&]() {
        for(int i = 10; i < 15; i++) {
            board->addScore(i, 0);
        }
        vector<int> top = board->getTopN(3);
        return top[0] == 10 && top[1] == 11 && top[2] == 12;
    }());
    
    runner.runTest("Leaderboard: Negative scores handling", [&]() {
        board->addScore(100, -50);
        board->addScore(101, -100);
        vector<int> top = board->getTopN(10);
        // 100 (-50) should come before 101 (-100)
        bool found100First = false;
        for(size_t i = 0; i < top.size(); i++) {
            if(top[i] == 100) {
                found100First = true;
                break;
            }
            if(top[i] == 101) break;
        }
        return found100First;
    }());
    
    delete board;
    
    // AuctionTree stress tests
    cout << "\n--- AuctionTree Stress Tests ---" << endl;
    AuctionTree* tree = createAuctionTree();
    
    runner.runTest("AuctionTree: Insert descending order", [&]() {
        for(int i = 1000; i > 0; i--) {
            tree->insertItem(i, i);
        }
        return true;
    }());
    
    runner.runTest("AuctionTree: Insert ascending order", [&]() {
        for(int i = 2000; i < 3000; i++) {
            tree->insertItem(i, i);
        }
        return true;
    }());
    
    runner.runTest("AuctionTree: Delete all items", [&]() {
        for(int i = 1; i <= 1000; i++) {
            tree->deleteItem(i);
        }
        return true;
    }());
    
    delete tree;
}

void test_PartB_Inventory_Extended() {
    cout << "\n========== Part B: Extended Tests ==========" << endl;
    
    cout << "\n--- Loot Split Extended ---" << endl;
    
    runner.runTest("LootSplit: Two coins {1, 1} -> 0", [&]() {
        vector<int> coins = {1, 1};
        return InventorySystem::optimizeLootSplit(2, coins) == 0;
    }());
    
    runner.runTest("LootSplit: Powers of 2 {1,2,4,8,16} -> 1", [&]() {
        vector<int> coins = {1, 2, 4, 8, 16};
        return InventorySystem::optimizeLootSplit(5, coins) == 1;
    }());
    
    runner.runTest("LootSplit: Large equal values {1000,1000,1000} -> 1000", [&]() {
        vector<int> coins = {1000, 1000, 1000};
        return InventorySystem::optimizeLootSplit(3, coins) == 1000;
    }());
    
    cout << "\n--- Knapsack Extended ---" << endl;
    
    runner.runTest("Knapsack: Single item that fits", [&]() {
        vector<pair<int,int>> items = {{5, 100}};
        return InventorySystem::maximizeCarryValue(5, items) == 100;
    }());
    
    runner.runTest("Knapsack: Single item too heavy", [&]() {
        vector<pair<int,int>> items = {{10, 100}};
        return InventorySystem::maximizeCarryValue(5, items) == 0;
    }());
    
    runner.runTest("Knapsack: Many small items", [&]() {
        vector<pair<int,int>> items = {{1,5},{1,10},{1,15},{1,20}};
        return InventorySystem::maximizeCarryValue(3, items) == 45;
    }());
    
    runner.runTest("Knapsack: Large capacity", [&]() {
        vector<pair<int,int>> items = {{10,60},{20,100},{30,120}};
        return InventorySystem::maximizeCarryValue(50, items) == 220;
    }());
    
    cout << "\n--- String Decoder Extended ---" << endl;
    
    runner.runTest("ChatDecoder: 'a' (single non-pair char) -> 1", [&]() {
        return InventorySystem::countStringPossibilities("a") == 1;
    }());
    
    runner.runTest("ChatDecoder: 'un' (no pairs) -> 1", [&]() {
        return InventorySystem::countStringPossibilities("un") == 1;
    }());
    
    runner.runTest("ChatDecoder: 'uuuuuu' (6 u's) -> 8", [&]() {
        return InventorySystem::countStringPossibilities("uuuuuu") == 8;
    }());
    
    runner.runTest("ChatDecoder: 'nnnn' (4 n's) -> 5", [&]() {
        return InventorySystem::countStringPossibilities("nnnn") == 5;
    }());
    
    runner.runTest("ChatDecoder: 'uunnuunn' -> 16", [&]() {
        return InventorySystem::countStringPossibilities("uunnuunn") == 16;
    }());
}

void test_PartC_Navigator_Extended() {
    cout << "\n========== Part C: Extended Tests ==========" << endl;
    
    cout << "\n--- Path Exists Extended ---" << endl;
    
    runner.runTest("PathExists: Star topology center to leaf", [&]() {
        vector<vector<int>> edges = {{0,1},{0,2},{0,3},{0,4}};
        return WorldNavigator::pathExists(5, edges, 0, 4) == true;
    }());
    
    runner.runTest("PathExists: Star topology leaf to leaf", [&]() {
        vector<vector<int>> edges = {{0,1},{0,2},{0,3},{0,4}};
        return WorldNavigator::pathExists(5, edges, 1, 4) == true;
    }());
    
    runner.runTest("PathExists: Long chain 0->10", [&]() {
        vector<vector<int>> edges;
        for(int i = 0; i < 10; i++) {
            edges.push_back({i, i+1});
        }
        return WorldNavigator::pathExists(11, edges, 0, 10) == true;
    }());
    
    runner.runTest("PathExists: Two separate triangles", [&]() {
        vector<vector<int>> edges = {{0,1},{1,2},{2,0},{3,4},{4,5},{5,3}};
        return WorldNavigator::pathExists(6, edges, 0, 5) == false;
    }());
    
    cout << "\n--- MST Extended ---" << endl;
    
    runner.runTest("MST: Star graph", [&]() {
        vector<vector<int>> roads = {{0,1,1,0},{0,2,2,0},{0,3,3,0}};
        return WorldNavigator::minBribeCost(4, 3, 1, 1, roads) == 6;
    }());
    
    runner.runTest("MST: All edges same weight", [&]() {
        vector<vector<int>> roads = {{0,1,5,0},{1,2,5,0},{2,3,5,0}};
        return WorldNavigator::minBribeCost(4, 3, 1, 1, roads) == 15;
    }());
    
    runner.runTest("MST: Complex silver/gold mix", [&]() {
        vector<vector<int>> roads = {{0,1,10,5},{1,2,0,20}};
        // 0-1: 10*2 + 5*1 = 25
        // 1-2: 0*2 + 20*1 = 20
        return WorldNavigator::minBribeCost(3, 2, 2, 1, roads) == 45;
    }());
    
    cout << "\n--- APSP Extended ---" << endl;
    
    runner.runTest("APSP: Square graph", [&]() {
        vector<vector<int>> roads = {{0,1,1},{1,2,1},{2,3,1},{3,0,1}};
        // Distances: 0-1=1, 0-2=2, 0-3=1, 1-2=1, 1-3=2, 2-3=1
        // Sum = 1+2+1+1+2+1 = 8 = "1000"
        return WorldNavigator::sumMinDistancesBinary(4, roads) == "1000";
    }());
    
    runner.runTest("APSP: Two nodes far apart", [&]() {
        vector<vector<int>> roads = {{0,1,1000}};
        return WorldNavigator::sumMinDistancesBinary(2, roads) == "1111101000";
    }());
    
    runner.runTest("APSP: Multiple paths - shortest wins", [&]() {
        vector<vector<int>> roads = {{0,1,1},{1,2,1},{0,2,10}};
        // 0-2 should be 2 (via 1), not 10
        // Sum = 1 + 2 + 1 = 4 = "100"
        return WorldNavigator::sumMinDistancesBinary(3, roads) == "100";
    }());
}

void test_PartD_Kernel_Extended() {
    cout << "\n========== Part D: Extended Tests ==========" << endl;
    
    cout << "\n--- Task Scheduler Extended ---" << endl;
    
    runner.runTest("Scheduler: {A,A,A,A,A}, n=1 -> 9", [&]() {
        vector<char> tasks = {'A','A','A','A','A'};
        return ServerKernel::minIntervals(tasks, 1) == 9;
    }());
    
    runner.runTest("Scheduler: All different tasks", [&]() {
        vector<char> tasks = {'A','B','C','D','E'};
        return ServerKernel::minIntervals(tasks, 10) == 5;
    }());
    
    runner.runTest("Scheduler: {A,A,B,B}, n=0", [&]() {
        vector<char> tasks = {'A','A','B','B'};
        return ServerKernel::minIntervals(tasks, 0) == 4;
    }());
    
    runner.runTest("Scheduler: Large n, few tasks", [&]() {
        vector<char> tasks = {'A','A'};
        return ServerKernel::minIntervals(tasks, 100) == 102;
    }());
    
    runner.runTest("Scheduler: Many tasks, small n", [&]() {
        vector<char> tasks;
        for(int i = 0; i < 26; i++) {
            tasks.push_back('A' + i);
            tasks.push_back('A' + i);
        }
        return ServerKernel::minIntervals(tasks, 1) >= 52;
    }());
}

int main() {
    cout << "===================================================" << endl;
    cout << "     ARCADIA ENGINE - COMPREHENSIVE TEST SUITE    " << endl;
    cout << "===================================================" << endl;

    test_PartA_DataStructures_Extended();
    test_PartB_Inventory_Extended();
    test_PartC_Navigator_Extended();
    test_PartD_Kernel_Extended();

    runner.printSummary();

    return 0;
}