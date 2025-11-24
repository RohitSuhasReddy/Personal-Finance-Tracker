/* test_main.c */
// Compile with: gcc test_main.c transactions.c limits.c reports.c utils.c -o test_app

#include <stdio.h>
#include "Finance.h"       // The Dictionary
#include "transactions.h"  // The Storage
#include "limits.h"        // The Rules
#include "reports.h"       // The Math

int main() {
    printf("=== PERSON 3 LOGIC TEST ===\n\n");

    // 1. SETUP: Create some fake dates
    Date d1 = {10, 10, 2023};
    Date d2 = {15, 10, 2023};

    // 2. TEST TRANSACTIONS: Add Money
    Transaction t1 = {1, 1000.0, "INCOME", "Salary", "Monthly Job", d1};
    if (addTransaction(t1)) {
        printf("[PASS] Income Added. Balance: %.2f\n", getCurrentBalance());
    } else {
        printf("[FAIL] Could not add Income.\n");
    }

    // 3. TEST LIMITS: Set a budget for Food
    setBudget("Food", 50.0);
    printf("[INFO] Set Food Budget to $50.00\n");

    // 4. TEST LOGIC: Spend more than the budget
    Transaction t2 = {2, 60.0, "EXPENSE", "Food", "Burger King", d2};
    
    // Check if we are about to break the limit?
    double currentFood = getCategoryTotal("Food");
    if (isOverBudget("Food", currentFood + t2.amount)) {
        printf("[PASS] Budget Alert Triggered! (Expected)\n");
    } else {
        printf("[FAIL] Budget Alert DID NOT Trigger.\n");
    }

    addTransaction(t2); // Add it anyway
    printf("Current Balance: %.2f\n", getCurrentBalance());

    // 5. TEST REPORTS: Check Totals
    double totalFood = getCategoryTotal("Food");
    if (totalFood == 60.0) {
        printf("[PASS] Reporting Math is Correct.\n");
    } else {
        printf("[FAIL] Reporting Math is Wrong. Got %.2f\n", totalFood);
    }

    return 0;
}