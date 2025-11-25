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

