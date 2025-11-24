#include <string.h>
#include "limits.h"

// Private memory
static Budget budgets[20]; // Max 20 categories allowed
static int count = 0;

void setBudget(char* category, double amount) {
    // 1. Try to find existing category to update
    for (int i = 0; i < count; i++) {
        if (strcmp(budgets[i].category, category) == 0) {
            budgets[i].maxAmount = amount;
            return; // Done
        }
    }

    // 2. If not found, add new one (if space exists)
    if (count < 20) {
        strcpy(budgets[count].category, category);
        budgets[count].maxAmount = amount;
        count++;
    }
}

int isOverBudget(char* category, double currentSpending) {
    // Find the limit for this category
    for (int i = 0; i < count; i++) {
        if (strcmp(budgets[i].category, category) == 0) {
            // Check the math
            if (currentSpending > budgets[i].maxAmount) {
                return 1; // WARNING: Over Budget
            }
        }
    }
    return 0; // Safe (or no limit set for this category)
}

// Getters for UI
Budget* getAllBudgets() { return budgets; }
int getBudgetCount() { return count; }