#include <stdio.h>
#include <string.h>

#include "UI.h"
#include "Transactions.h"
#include "Limits.h"
#include "Reports.h"
#include "Utils.h"

static void printMainMenu() {
    printf("\n================= MAIN MENU =================\n");
    printf("1. Add Transaction\n");
    printf("2. Delete Transaction\n");
    printf("3. View All Transactions\n");
    printf("4. View Summary (Income/Expense/Balance)\n");
    printf("5. Manage Budgets\n");
    printf("6. Reports\n");
    printf("0. Logout / Exit\n");
    printf("=============================================\n");
    printf("Enter choice: ");
}

void showSummary() {
    double income = getTotalIncome();
    double expense = getTotalExpense();
    double balance = getCurrentBalance();

    printf("\n------- SUMMARY -------\n");
    printf("Total Income : %.2f\n", income);
    printf("Total Expense: %.2f\n", expense);
    printf("Current Balance: %.2f\n", balance);
    printf("------------------------\n");
}

void listAllTransactionsUI() {
    sortTransactionsByDate(); // show in date order
    Transaction* arr = getAllTransactions();
    int n = getTransactionCount();

    if (n == 0) {
        printf("\nNo transactions to display.\n");
        return;
    }

    printf("\n---------------- TRANSACTIONS ----------------\n");
    printf("ID | Date       | Type    | Category   | Amount    | Description\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        Transaction t = arr[i];
        printf("%2d | %02d-%02d-%04d | %-7s | %-10s | %9.2f | %s\n",
               t.id,
               t.date.day, t.date.month, t.date.year,
               t.type,
               t.category,
               t.amount,
               t.des);
    }
    printf("---------------------------------------------------------------\n");
}

void addTransactionUI() {
    Transaction t;
    memset(&t, 0, sizeof(Transaction));

    printf("\n--- Add Transaction ---\n");
    printf("Enter amount: ");
    scanf("%lf", &t.amount);

    printf("Enter type (INCOME/EXPENSE): ");
    scanf("%14s", t.type);

    printf("Enter category (e.g. Food, Rent): ");
    scanf("%19s", t.category);

    printf("Enter description (no commas): ");
    scanf(" %49[^\n]", t.des);

    printf("Enter date (dd mm yyyy): ");
    scanf("%d %d %d", &t.date.day, &t.date.month, &t.date.year);

    if (!isValidDate(t.date)) {
        printf("Invalid date. Transaction cancelled.\n");
        return;
    }

    // If EXPENSE, check against budget
    if (strcmp(t.type, "EXPENSE") == 0) {
        double currentCategoryTotal = getCategoryTotal(t.category);
        double newTotal = currentCategoryTotal + t.amount;

        if (isOverBudget(t.category, newTotal)) {
            printf("WARNING: This expense will exceed the budget for '%s'.\n",
                   t.category);
        }
    }

    if (addTransaction(t)) {
        printf("Transaction added successfully.\n");
    } else {
        printf("Failed to add transaction.\n");
    }
}

void deleteTransactionUI() {
    int id;
    printf("\nEnter transaction ID to delete: ");
    scanf("%d", &id);

    deleteTransaction(id);
}

void manageBudgetsUI() {
    int choice;
    do {
        printf("\n------- BUDGET MENU -------\n");
        printf("1. Set / Update Budget\n");
        printf("2. List Budgets\n");
        printf("0. Back to Main Menu\n");
        printf("---------------------------\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            char category[30];
            double amount;
            printf("Enter category: ");
            scanf("%29s", category);
            printf("Enter max amount: ");
            scanf("%lf", &amount);
            setBudget(category, amount);
            printf("Budget set/updated.\n");
        } else if (choice == 2) {
            Budget* b = getAllBudgets();
            int n = getBudgetCount();
            if (n == 0) {
                printf("No budgets defined.\n");
            } else {
                printf("\nCategory         | Limit\n");
                printf("-----------------------------\n");
                for (int i = 0; i < n; i++) {
                    printf("%-16s | %.2f\n", b[i].category, b[i].maxAmount);
                }
            }
        }
    } while (choice != 0);
}

void showReportsUI() {
    char category[30];
    printf("\nEnter category for report: ");
    scanf("%29s", category);

    double total = getCategoryTotal(category);
    printf("Total expenses in category '%s': %.2f\n", category, total);
}

void runMainMenu(User currentUser) {
    int choice;
    do {
        printMainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTransactionUI();
                break;
            case 2:
                deleteTransactionUI();
                break;
            case 3:
                listAllTransactionsUI();
                break;
            case 4:
                showSummary();
                break;
            case 5:
                manageBudgetsUI();
                break;
            case 6:
                showReportsUI();
                break;
            case 0:
                printf("Logging out %s. Goodbye!\n", currentUser.username);
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 0);
}
