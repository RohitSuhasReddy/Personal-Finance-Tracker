/*
 * File: transactions.c
 * Purpose: Logic for storing and calculating money.
 */

#include <stdio.h>
#include <string.h>
#include "transactions.h"
#include "utils.h" // For isValidDate()

#define MAX_TRANSACTIONS 1000

// Private Memory
static Transaction list[MAX_TRANSACTIONS];
static int count = 0;

// --- ACTIONS ---

int addTransaction(Transaction t) {
    // 1. Check if full
    if (count >= MAX_TRANSACTIONS) return 0;

    // 2. Check if Date is valid (using your Utils file)
    if (!isValidDate(t.date)) return 0; 

    // 3. Add to list
    list[count] = t;
    count++;
    return 1; // Success
}

int deleteTransaction(int id) {
    int index = -1;
    
    // Find the item
    for (int i = 0; i < count; i++) {
        if (list[i].id == id) {
            index = i;
            break;
        }
    }

    if (index == -1) return 0; // Not found

    // Shift everything left to fill the gap
    for (int i = index; i < count - 1; i++) {
        list[i] = list[i + 1];
    }
    
    count--;
    return 1;
}

// --- GETTERS ---

Transaction* getAllTransactions() {
    return list;
}

int getTransactionCount() {
    return count;
}

// --- MATH ---

double getTotalIncome() {
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].type, "INCOME") == 0) {
            total += list[i].amount;
        }
    }
    return total;
}

double getTotalExpense() {
    double total = 0.0;
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].type, "EXPENSE") == 0) {
            total += list[i].amount;
        }
    }
    return total;
}

double getCurrentBalance() {
    return getTotalIncome() - getTotalExpense();
}

// --- HELPER FOR STORAGE ---

void transactionToString(Transaction t, char* buffer) {
    // Formats data as CSV so Person 2 can write it to a text file easily
    // Order: ID, Amount, Type, Category, Day, Month, Year
    sprintf(buffer, "%d,%.2f,%s,%s,%d,%d,%d",
            t.id, t.amount, t.type, t.category, 
            t.date.day, t.date.month, t.date.year);
}