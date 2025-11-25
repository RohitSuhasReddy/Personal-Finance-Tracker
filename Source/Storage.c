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
    FILE *file = fopen(fileName, "w");
 // "w" overwrites the file with new data
    if (file == NULL) {
        printf("Error: Could not save data!\n");
        return;
    }

    Transaction* list = getAllTransactions();
    int count = getTransactionCount();
    char buffer[200]; // temporary holder for the text

    for (int i = 0; i < count; i++) {
        // Use the helper we wrote in Transactions.c
        transactionToString(list[i], buffer);
        
        // Write it to the file
        fprintf(file, "%s\n", buffer);
    }

    fclose(file);
}

void loadAllData() {
    FILE *file = fopen(fileName, "r"); // "r" is read mode
    if (file == NULL) return; // File doesn't exist yet (first run)

    Transaction t;
    // We scan the CSV format: ID,Amount,Type,Category,Day,Month,Year
    // Note: This needs to match the format in transactionToString
    while (fscanf(file, "%d,%lf,%[^,],%[^,],%d,%d,%d", 
                  &t.id, &t.amount, t.type, t.category, 
                  &t.date.day, &t.date.month, &t.date.year) != EOF) {
        
        // Add description (since it was the last item, we handle it carefully)
        // Note: For simple CSV, description handling can be tricky if it has commas.
        // For now, let's keep it simple or set a default.
        strcpy(t.des, "Loaded"); 
        
        addTransaction(t);
    }
    fclose(file);
}
