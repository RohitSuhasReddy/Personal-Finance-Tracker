/* limits.h */
#ifndef LIMITS_H
#define LIMITS_H

#include "Finance.h" // <--- This imports the Budget struct from Finance.h

// DO NOT define 'typedef struct { ... } Budget' here again.
// It is already inside Finance.h

// --- Function Prototypes ---

// 1. Create or Update a limit
void setBudget(char* category, double amount);

// 2. Check if a specific amount breaks the limit
// Returns: 1 if OVER budget, 0 if SAFE.
int isOverBudget(char* category, double currentSpending);

// 3. For the UI to show the list
Budget* getAllBudgets();
int getBudgetCount();

#endif