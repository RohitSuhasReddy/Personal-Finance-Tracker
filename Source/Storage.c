#include <stdio.h>
#include <string.h>
#include "Storage.h"
#include "Transactions.h"
#include "Finance.h"

static char fileName[50];

void setActiveUserStorage(int userId) {
    sprintf(fileName, "transactions_user%d.txt", userId);
}

void saveAllData() {
    // 1. Open the file using the static 'fileName' variable
    FILE *file = fopen(fileName, "w"); 
    
    if (file == NULL) {
        printf("Error: Could not save data!\n");
        return;
    }

    // 2. Get the head of the Linked List
    Node* current = getAllTransactions(); 
    char buffer[200]; 

    // 3. Iterate through the list
    while (current != NULL) {
        // Convert the transaction data to a string
        transactionToString(current->data, buffer);
        
        // Write to file
        fprintf(file, "%s\n", buffer);
        
        // Move to the next node
        current = current->next;
    }

    // 4. Close the file
    fclose(file);
}

void loadAllData() {
    FILE *file = fopen(fileName, "r");
    if (!file) return;

    Transaction t;

    while (1) {
        int fields = fscanf(file,
            "%d,%lf,%[^,],%[^,],%d,%d,%d,%[^\n]",
            &t.id,
            &t.amount,
            t.type,
            t.category,
            &t.date.day,
            &t.date.month,
            &t.date.year,
            t.des
        );

        if (fields == 8) {
            addTransaction(t);
        }
        else if (fields == 7) {
            strcpy(t.des, "");
            addTransaction(t);
            int c;
            while ((c = fgetc(file)) != '\n' && c != EOF);
        }
        else {
            break;
        }
    }

    fclose(file);
}

