#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "Finance.h"

typedef struct Node {
    Transaction data;
    struct Node* next;
} Node;

// Actions
int addTransaction(Transaction t);

// Get Data
Node* getAllTransactions(); 
int getTransactionCount();

// Math Helpers
double getTotalIncome();
double getTotalExpense();
double getCurrentBalance();

// Helper for Storage
void transactionToString(Transaction t, char* buffer);

#endif