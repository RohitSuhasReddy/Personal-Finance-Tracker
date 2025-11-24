/* Finance.h */
#ifndef FINANCE_H
#define FINANCE_H

#include <stdio.h>
#include <stdlib.h>

// 1. Define the Date struct
// We need this so Utils.c can validate days/months
typedef struct {
    int day;
    int month;
    int year;
} Date;

// 2. Define the Transaction struct
// It MUST be named "Transaction" (Big T) to match Reports.c
typedef struct {
    int id;
    double amount;      // Renamed from 'amt' to match logic
    char type[15];      // "INCOME" or "EXPENSE"
    char category[20];  // "Food", etc.
    char des[50];       // Description
    Date date;          // Uses the struct above, NOT a char array
} Transaction;


// 3. Define Limits (If you are using the Limits module)
typedef struct {
    char category[30];
    double maxAmount;
} Budget;

typedef struct {
    int userId;
    char username[30];
    char password[30];
} User;

#endif