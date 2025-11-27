#ifndef FINANCE_H
#define FINANCE_H

#include <stdio.h>
#include <stdlib.h>

// 1. Define the Date struct
typedef struct {
    int day;
    int month;
    int year;
} Date;

// 2. Define the Transaction struct
typedef struct {
    int id;
    double amount;     
    char type[15];      // "INCOME" or "EXPENSE"
    char category[20];  // "Food", etc.
    char des[50];       // Description
    Date date;          
} Transaction;


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