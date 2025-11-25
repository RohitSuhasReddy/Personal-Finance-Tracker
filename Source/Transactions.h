#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "Finance.h" 

// 1. Actions
// Returns 1 if success, 0 if full or invalid date
int addTransaction(Transaction t);


// 2. Get Data
Transaction* getAllTransactions();
int getTransactionCount();

// 3. Math Helpers
double getTotalIncome();
double getTotalExpense();
double getCurrentBalance();

// 4. Helper for Storage (Person 2 needs this)
// Converts a transaction into a string: "1,500,Food,INCOME"
void transactionToString(Transaction t, char* buffer);

#endif