#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Transactions.h"
#include "Utils.h"

static Node* head = NULL;
static int count = 0;

// --- ACTIONS ---

int addTransaction(Transaction t) {
    // Validate Date
    if (!isValidDate(t.date)) return 0; 
    t.id = count + 1; 

    //Create New Node
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) return 0; // Memory full

    newNode->data = t;
    newNode->next = NULL;

    //Append to End of List
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

//GETTERS

Node* getAllTransactions() {
    return head;
}

int getTransactionCount() {
    return count;
}

// MATH 

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