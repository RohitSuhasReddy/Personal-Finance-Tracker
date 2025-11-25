/* Source/Transactions.h */
#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "Finance.h"

// NEW: Define the Linked List Node
typedef struct Node {
    Transaction data;
    struct Node* next;
} Node;

// --- 1. Actions ---
int addTransaction(Transaction t);
int deleteTransaction(int id);

// --- 2. Get Data ---
// CHANGED: Returns the head of the linked list instead of an array
Node* getAllTransactions(); 
int getTransactionCount();

// --- 3. Math Helpers ---
double getTotalIncome();
double getTotalExpense();
double getCurrentBalance();

// --- 4. Helper for Storage ---
void transactionToString(Transaction t, char* buffer);

#endif