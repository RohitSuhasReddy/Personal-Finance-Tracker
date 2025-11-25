/* UI.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UI.h"
#include "Transactions.h"
#include "Limits.h"
#include "Reports.h"
#include "Utils.h"
#include "Storage.h"

/* ---------- FIXED CATEGORIES (UPDATED) ---------- */

// Expense Categories
const char* EXPENSE_CATEGORIES[] = {
    "Food", "Transport", "Health", "Education", "Bills",
    "Shopping", "Entertainment", "Other Expenses"
};
int EXP_CAT_COUNT = 8;

// Income Categories
const char* INCOME_CATEGORIES[] = {
    "Salary", "Business", "Investment", "Gift", "Freelancing",
    "Other Income"
};
int INC_CAT_COUNT = 6;


// --- HELPER FUNCTIONS ---

void clearScreen() {
    // Uses "cls" for Windows, "clear" for Mac/Linux
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader(char* title) {
    printf("\n" COLOR_BOLD "=== %s ===" COLOR_RESET "\n", title);
}

// --- FEATURE SCREENS ---

void showAddTransactionForm() {
    Transaction t;
    printHeader("ADD NEW TRANSACTION");

    // 1. Get Type
    printf("Type (1=INCOME, 2=EXPENSE): ");
    int typeChoice;
    scanf("%d", &typeChoice);
    if (typeChoice == 1) strcpy(t.type, "INCOME");
    else strcpy(t.type, "EXPENSE");

    // 2. Get Category
    int opt;
    if (strcmp(t.type, "INCOME") == 0) {
        printf("\nSelect Income Category:\n");
        for(int i = 0; i < INC_CAT_COUNT; i++)
            printf("%d. %s\n", i+1, INCOME_CATEGORIES[i]);
        printf("Enter option: ");
        scanf("%d", &opt);

        if(opt < 1 || opt > INC_CAT_COUNT)
            strcpy(t.category, "Other Income");
        else
            strcpy(t.category, INCOME_CATEGORIES[opt - 1]);

    } else { // EXPENSE
        printf("\nSelect Expense Category:\n");
        for(int i = 0; i < EXP_CAT_COUNT; i++)
            printf("%d. %s\n", i+1, EXPENSE_CATEGORIES[i]);
        printf("Enter option: ");
        scanf("%d", &opt);

        if(opt < 1 || opt > EXP_CAT_COUNT)
            strcpy(t.category, "Other Expenses");
        else
            strcpy(t.category, EXPENSE_CATEGORIES[opt - 1]);
    }


    // 3. Get Amount
    printf("Amount: ");
    scanf("%lf", &t.amount);

    // 4. Get Description (Allows spaces)
    printf("Description: ");
    // The space before % says "skip whitespace"
    scanf(" %[^\n]s", t.des); 

    // 5. Get Date
    printf("Date (DD MM YYYY): ");
    scanf("%d %d %d", &t.date.day, &t.date.month, &t.date.year);

    // --- LOGIC CHECKS ---

    // A. Check Date Validity
    if (!isValidDate(t.date)) {
        printf(COLOR_RED "\n[ERROR] Invalid Date entered! Transaction cancelled.\n" COLOR_RESET);
        return;
    }

    // B. Check Budget (Only for Expenses)
    if (typeChoice == 2) {
        double currentSpent = getCategoryTotal(t.category);
        if (isOverBudget(t.category, currentSpent + t.amount)) {
            printf(COLOR_RED COLOR_BOLD "\n[WARNING] This expense exceeds your budget for '%s'!\n" COLOR_RESET, t.category);
            printf("Proceed anyway? (1=Yes, 0=No): ");
            int proceed;
            scanf("%d", &proceed);
            if (proceed == 0) return;
        }
    }

    // C. Add to Backend
    if (addTransaction(t)) {
        saveAllData();
        printf(COLOR_GREEN "\n[SUCCESS] Transaction added successfully!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "\n[ERROR] Database is full.\n" COLOR_RESET);
    }
}

void showAllTransactions() {
    Transaction* list = getAllTransactions();
    int count = getTransactionCount();

    printHeader("TRANSACTION HISTORY");
    
    // Table Header
    printf(COLOR_CYAN "%-4s | %-12s | %-8s | %-12s | %-10s | %s\n" COLOR_RESET, 
           "ID", "Date", "Type", "Category", "Amount", "Description");
    printf("--------------------------------------------------------------------------\n");

    if (count == 0) {
        printf(COLOR_YELLOW "No transactions found.\n" COLOR_RESET);
    }

    for (int i = 0; i < count; i++) {
        // Determine Color based on Type
        char* color = (strcmp(list[i].type, "INCOME") == 0) ? COLOR_GREEN : COLOR_RED;

        printf("%-4d | %02d-%02d-%04d | %s%-8s%s | %-12s | %s%9.2f%s | %s\n",
            list[i].id,
            list[i].date.day, list[i].date.month, list[i].date.year,
            color, list[i].type, COLOR_RESET, // Colored Type
            list[i].category,
            color, list[i].amount, COLOR_RESET, // Colored Amount
            list[i].des
        );
    }
    printf("--------------------------------------------------------------------------\n");
}

void showSummary() {
    printHeader("FINANCIAL REPORT");
    
    double income = getTotalIncome();
    double expense = getTotalExpense();
    double balance = getCurrentBalance();

    printf("Total Income:   " COLOR_GREEN "%.2f\n" COLOR_RESET, income);
    printf("Total Expense:  " COLOR_RED   "%.2f\n" COLOR_RESET, expense);
    printf("---------------------------\n");
    
    if (balance >= 0)
        printf("Net Balance:    " COLOR_GREEN "%.2f\n" COLOR_RESET, balance);
    else
        printf("Net Balance:    " COLOR_RED   "%.2f\n" COLOR_RESET, balance);
}

void showBudgetMenu() {
    printHeader("MANAGE BUDGETS");
    char category[30];
    double limit;

    printf("Enter Category to limit (Food/Shopping/): ");
    scanf("%s", category);
    printf("Enter Max Limit Amount: ");
    scanf("%lf", &limit);

    setBudget(category, limit);
    printf(COLOR_GREEN "\n[SUCCESS] Budget set for %s at %.2f\n" COLOR_RESET, category, limit);
}

// --- MAIN MENU LOOP ---

void runMainMenu() {
    int choice;
    while(1) {
        // clearScreen(); // Uncomment if you want screen to wipe every time
        printf("\n" COLOR_BOLD "=== PERSONAL FINANCE TRACKER ===" COLOR_RESET "\n");
        printf("1. Add Transaction\n");
        printf("2. View History\n");
        printf("3. View Financial Report\n");
        printf("4. Set Budget Limit\n");
        printf("5. Exit\n");
        printf("Enter Choice: ");
        
        if (scanf("%d", &choice) != 1) {
            // Fix infinite loop if user enters text instead of number
            while(getchar() != '\n'); 
            continue;
        }

        switch(choice) {
            case 1: showAddTransactionForm(); break;
            case 2: showAllTransactions(); break;
            case 3: showSummary(); break;
            case 4: showBudgetMenu(); break;
            case 5: 
                saveAllData();
                printf(COLOR_GREEN "Data saved. Exiting...\n" COLOR_RESET);
                // Note: If you implemented Storage.c, call saveAllData() here!
                return;
            default: printf(COLOR_RED "Invalid Option\n" COLOR_RESET);
        }
    }
}