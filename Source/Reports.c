#include <stdio.h>
#include <string.h>
#include "reports.h"
#include "transactions.h"
#include "utils.h" 

// Calculate Category Total

double getCategoryTotal(char* category) {
    Node* current = getAllTransactions();
    double sum = 0.0;

    while (current != NULL) {
        if (strcmp(current->data.category, category) == 0) {
            sum += current->data.amount;
        }
        current = current->next;
    }
    return sum;
}

void sortTransactionsByDate() {
    int count = getTransactionCount();
    if (count < 2) return;

    // Bubble Sort on Linked List (Swapping Data)
    for (int i = 0; i < count - 1; i++) {
        Node* current = getAllTransactions();
        
        for (int j = 0; j < count - i - 1; j++) {
            // Compare current vs current->next
            if (current->next != NULL) {
                if (compareDates(current->data.date, current->next->data.date) < 0) {
                    
                    // SWAP DATA
                    Transaction temp = current->data;
                    current->data = current->next->data;
                    current->next->data = temp;
                }
                current = current->next;
            }
        }
    }
}