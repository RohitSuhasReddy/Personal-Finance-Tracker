/*
 * File: reports.c
 * Purpose: Logic for calculating totals and sorting.
 */

#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "transactions.h"
#include "utils.h" 
// Note: Finance.h is included inside transactions.h, so we are safe.

// --- FUNCTION 1: Calculate Category Total ---

double getCategoryTotal(char* category) {
    // FIX 1: Get access to the private array from transactions.c
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();
    
    double sum = 0.0;

    for (int i = 0; i < count; i++) {
        // Only sum up EXPENSES
        if (strcmp(list[i].type, "EXPENSE") == 0) {
            // Check if the category matches (e.g., "Food" == "Food")
            if (strcmp(list[i].category, category) == 0) {
                sum += list[i].amount;
            }
        }
    }
    return sum;
}

double getIncomeCategoryTotal(char* category) {
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();

    double sum = 0.0;

    for (int i = 0; i < count; i++) {
        if (strcmp(list[i].type, "INCOME") == 0) {
            if (strcmp(list[i].category, category) == 0) {
                sum += list[i].amount;
            }
        }
    }
    return sum;
}


// --- FUNCTION 2: Sort by Date (Bubble Sort) ---

void sortTransactionsByDate() {
    // FIX 2: Get access to the private array
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();
    
    // FIX 3: Declare the temporary variable for swapping
    Transaction temp;

    // Bubble Sort Algorithm
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            
            // compareDates returns < 0 if the first date is OLDER.
            // We want NEWEST first, so if list[j] is older than list[j+1], swap them.
            if (compareDates(list[j].date, list[j + 1].date) < 0) {
                
                // Swap logic
                temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}