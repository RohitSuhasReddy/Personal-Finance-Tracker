/*
 * File: transactions.h
 * Purpose: Manage financial transactions (Income/Expenses).
 */

#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "Finance.h" // Must contain struct Transaction

// --- 1. Actions ---
// Returns 1 if success, 0 if full or invalid date
int addTransaction(Transaction t);

// Returns 1 if deleted, 0 if ID not found
int deleteTransaction(int id);

// --- 2. Get Data ---
Transaction* getAllTransactions();
int getTransactionCount();

// --- 3. Math Helpers ---
double getTotalIncome();
double getTotalExpense();
double getCurrentBalance();

// --- 4. Helper for Storage (Person 2 needs this) ---
// Converts a transaction into a string: "1,500,Food,INCOME,..."
void transactionToString(Transaction t, char* buffer);

#endif