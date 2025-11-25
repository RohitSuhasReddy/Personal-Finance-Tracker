/* Source/Transactions.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Transactions.h"
#include "Utils.h"

// Private Memory: HEAD pointer instead of Array
static Node* head = NULL;
static int count = 0;

// --- ACTIONS ---

int addTransaction(Transaction t) {
    // 1. Validate Date
    if (!isValidDate(t.date)) return 0; 

    // 2. Auto-increment ID
    // Note: In a linked list, we can't just use 'count + 1' if we delete items, 
    // but for simplicity, we will stick to the logic of the original code 
    // or find the max ID. Here we stick to count + 1 to match your logic.
    t.id = count + 1; 

    // 3. Create New Node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return 0; // Memory full

    newNode->data = t;
    newNode->next = NULL;

    // 4. Append to End of List
    if (head == NULL) {
        head = newNode;
    } else {
        Node* current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }

    count++;
    return 1;
}

int deleteTransaction(int id) {
    Node *current = head;
    Node *prev = NULL;

    // Search for the node
    while (current != NULL && current->data.id != id) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) return 0; // Not found

    // Unlink the node
    if (prev == NULL) {
        // Deleting the head
        head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current); // Free memory
    count--;
    return 1;
}

// --- GETTERS ---

Node* getAllTransactions() {
    return head;
}

int getTransactionCount() {
    return count;
}

// --- MATH ---

double getTotalIncome() {
    double total = 0.0;
    Node* current = head;
    
    while (current != NULL) {
        if (strcmp(current->data.type, "INCOME") == 0) {
            total += current->data.amount;
        }
        current = current->next;
    }
    return total;
}

double getTotalExpense() {
    double total = 0.0;
    Node* current = head;
    
    while (current != NULL) {
        if (strcmp(current->data.type, "EXPENSE") == 0) {
            total += current->data.amount;
        }
        current = current->next;
    }
    return total;
}

double getCurrentBalance() {
    return getTotalIncome() - getTotalExpense();
}

void transactionToString(Transaction t, char* buffer) {
    sprintf(buffer, "%d,%.2f,%s,%s,%d,%d,%d,%s",
            t.id, t.amount, t.type, t.category, 
            t.date.day, t.date.month, t.date.year, t.des);
}