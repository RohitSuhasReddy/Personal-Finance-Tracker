#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "transactions.h"
#include "utils.h" 

// FUNCTION 1: Calculate Category Total


double getCategoryTotal(char* category) {
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();
    
    double sum = 0.0;

    for (int i = 0; i < count; i++) {
        // Now it sums up anything that matches the category name.
        if (strcmp(list[i].category, category) == 0) {
            sum += list[i].amount;
        }
    }
    return sum;
}




//FUNCTION 2: Sort by Date (Bubble Sort)

void sortTransactionsByDate() {
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();
    
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