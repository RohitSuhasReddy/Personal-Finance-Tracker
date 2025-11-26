#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UI.h"
#include "Transactions.h"
#include "Limits.h"
#include "Reports.h"
#include "Utils.h" 
#include "Storage.h"


typedef struct {
    char category[30];
    double amount;
    int isIncome;
} CategoryTotal;

/* Add this to Source/UI.c */

void waitForInput() {
    printf("\nPress [Enter] to return to MENU...");
    
    // 1. Consume the leftover "Enter" key from the previous menu selection
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    // 2. Wait for the user to press Enter again
    getchar(); 
}

int compareByAmountDesc(const void* a, const void* b) {
    CategoryTotal* ca = (CategoryTotal*)a;
    CategoryTotal* cb = (CategoryTotal*)b;
    return (cb->amount > ca->amount) - (cb->amount < ca->amount);
}



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


// HELPER FUNCTIONS

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void printHeader(char* title) {
    printf("\n"COLOR_PINK COLOR_BOLD "======= %s ======" COLOR_RESET "\n", title);
}


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

    // 4. Get Description
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

    // B. Check Budget
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

    if (addTransaction(t)) {
        saveAllData();
        printf(COLOR_GREEN "\n[SUCCESS] Transaction added successfully!\n" COLOR_RESET);
    } else {
        printf(COLOR_RED "\n[ERROR] Database is full.\n" COLOR_RESET);
    }
}

void showAllTransactions() {
    sortTransactionsByDate();
    Node* current = getAllTransactions(); // Get Head
    printHeader("HISTORY of Transactions");

    if (current == NULL) {
        printf(COLOR_YELLOW "No transactions found.\n" COLOR_RESET);
    }

    while (current != NULL) {
        Transaction t = current->data; // Access data for cleaner code

        char* color = (strcmp(t.type, "INCOME") == 0) ? COLOR_GREEN : COLOR_RED;

        printf("%-4d | %02d-%02d-%04d | %s%-8s%s | %-12s | %s%9.2f%s | %s\n",
            t.id,
            t.date.day, t.date.month, t.date.year,
            color, t.type, COLOR_RESET,
            t.category,
            color, t.amount, COLOR_RESET,
            t.des
        );

        current = current->next; // Move to next node
    }
    printf("--------------------------------------------------------------------------\n");
    waitForInput();
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

    waitForInput();
}


void showCategoryDistribution() {
    printHeader("CATEGORY-WISE DISTRIBUTION");

    CategoryTotal arr[50];
    int n = 0;

    // 1. Collect EXPENSE Totals
    for (int i = 0; i < EXP_CAT_COUNT; i++) {
        double val = getCategoryTotal((char*)EXPENSE_CATEGORIES[i]);
        if (val > 0) {
            strcpy(arr[n].category, EXPENSE_CATEGORIES[i]);
            arr[n].amount = val;
            arr[n].isIncome = 0; // Mark for RED color
            n++;
        }
    }

    // 2. Collect INCOME Totals
    for (int i = 0; i < INC_CAT_COUNT; i++) {
        double val = getCategoryTotal((char*)INCOME_CATEGORIES[i]);
        if (val > 0) {
            strcpy(arr[n].category, INCOME_CATEGORIES[i]);
            arr[n].amount = val;
            arr[n].isIncome = 1; // Mark for GREEN color
            n++;
        }
    }

    if (n == 0) {
        printf(COLOR_YELLOW "No data available.\n" COLOR_RESET);
        return;
    }

    // 3. AUTO-SORT (Highest Amount First)
    qsort(arr, n, sizeof(CategoryTotal), compareByAmountDesc);

    // 4. Print Table
    printf("\n" COLOR_CYAN "%-20s | %-10s\n" COLOR_RESET, "Category", "Amount");
    printf("----------------------------------------\n");

    for (int i = 0; i < n; i++) {
        // Use the isIncome flag to decide color
        char* color = (arr[i].isIncome == 1) ? COLOR_GREEN : COLOR_RED;
        
        printf("%-20s | %s%.2f%s\n",
               arr[i].category, color, arr[i].amount, COLOR_RESET);
    }
    printf("----------------------------------------\n");
    waitForInput();
}




void showBudgetMenu() {
    printHeader("SET BUDGET LIMITS");
    
    // 1. Display Categories (Numbered List) 
    printf("\nSelect Expense Category to Limit:\n");
    for(int i = 0; i < EXP_CAT_COUNT; i++) {
        printf("%d. %s\n", i+1, EXPENSE_CATEGORIES[i]);
    }

    // 2. Get User Selection
    int opt;
    printf("Enter option: ");
    scanf("%d", &opt);

    char category[30];

    // 3. Validate & Map Selection to String 
    if(opt < 1 || opt > EXP_CAT_COUNT) {
        printf(COLOR_RED "Invalid option. Defaulting to 'Other Expenses'.\n" COLOR_RESET);
        strcpy(category, "Other Expenses");
    } else {
        // Automatically picks the correct name (e.g., "Food") based on the number
        strcpy(category, EXPENSE_CATEGORIES[opt - 1]);
    }

    // 4. Get Limit Amount 
    double limit;
    printf("Enter Max Limit Amount for '%s': ", category);
    scanf("%lf", &limit);

    // 5. Save to Backend 
    setBudget(category, limit);
    printf(COLOR_GREEN "\n[SUCCESS] Budget set for %s at %.2f\n" COLOR_RESET, category, limit);
}



void searchTransactions() {
    printHeader("SEARCH TRANSACTIONS");

    printf("Search by:\n");
    printf("1. Category\n");
    printf("2. Date (DD MM YYYY)\n");
    printf("3. Month (MM)\n");
    printf("4. Year (YYYY)\n");
    printf("Enter choice: ");

    int ch;
    scanf("%d", &ch);

    Node* current = getAllTransactions(); 
    int found = 0;

    // --- HELPER MACRO FOR PRINTING COLORED ROW ---
    // This makes sure search results look just like the History view
    #define PRINT_SEARCH_ROW(t) { \
        char* color = (strcmp(t.type, "INCOME") == 0) ? COLOR_GREEN : COLOR_RED; \
        printf("%-4d | %02d-%02d-%04d | %s%-8s%s | %-12s | %s%9.2f%s | %s\n", \
            t.id, \
            t.date.day, t.date.month, t.date.year, \
            color, t.type, COLOR_RESET, \
            t.category, \
            color, t.amount, COLOR_RESET, \
            t.des \
        ); \
    }

    // --- HELPER MACRO FOR TABLE HEADER ---
    #define PRINT_TABLE_HEADER() { \
        printf("\n" COLOR_CYAN "%-4s | %-10s | %-8s | %-12s | %-9s | %s\n" COLOR_RESET, "ID", "Date", "Type", "Category", "Amount", "Description"); \
        printf("--------------------------------------------------------------------------\n"); \
    }

    // ---------------------------
    // 1. SEARCH BY CATEGORY
    // ---------------------------
    if (ch == 1) {
        printHeader("SEARCH BY CATEGORY");
        int idx = 1;
        printf("\n" COLOR_GREEN "Income Categories:" COLOR_RESET "\n");
        for (int i = 0; i < INC_CAT_COUNT; i++) printf("%d. %s\n", idx++, INCOME_CATEGORIES[i]);
        printf("\n" COLOR_RED "Expense Categories:" COLOR_RESET "\n");
        for (int i = 0; i < EXP_CAT_COUNT; i++) printf("%d. %s\n", idx++, EXPENSE_CATEGORIES[i]);

        int totalOptions = INC_CAT_COUNT + EXP_CAT_COUNT;
        printf("\nEnter choice: ");
        int opt;
        scanf("%d", &opt);

        char selected[30];
        if (opt <= 0 || opt > totalOptions) {
            printf(COLOR_RED "Invalid option.\n" COLOR_RESET);
            return;
        }

        if (opt <= INC_CAT_COUNT) strcpy(selected, INCOME_CATEGORIES[opt - 1]);
        else strcpy(selected, EXPENSE_CATEGORIES[opt - INC_CAT_COUNT - 1]);

        PRINT_TABLE_HEADER();

        while (current != NULL) {
            if (strcmp(current->data.category, selected) == 0) {
                PRINT_SEARCH_ROW(current->data);
                found = 1;
            }
            current = current->next;
        }
    }

    // ---------------------------
    // 2. SEARCH BY FULL DATE
    // ---------------------------
    else if (ch == 2) {
        int d, m, y;
        printf("Enter date (DD MM YYYY): ");
        scanf("%d %d %d", &d, &m, &y);

        // --- FIX: CHECK IF DATE IS VALID ---
        Date checkDate = {d, m, y};
        if (!isValidDate(checkDate)) {
            printf(COLOR_RED "\n[ERROR] Invalid Date entered! Please check days/months.\n" COLOR_RESET);
            waitForInput();
            return;
        }

        PRINT_TABLE_HEADER();

        while (current != NULL) {
            if (current->data.date.day == d &&
                current->data.date.month == m &&
                current->data.date.year == y) {
                
                PRINT_SEARCH_ROW(current->data);
                found = 1;
            }
            current = current->next;
        }
    }

    // ---------------------------
    // 3. SEARCH BY MONTH
    // ---------------------------
    else if (ch == 3) {
        int m;
        printf("Enter month (1-12): ");
        scanf("%d", &m);

        if (m < 1 || m > 12) {
            printf(COLOR_RED "Invalid month.\n" COLOR_RESET);
            waitForInput();
            return;
        }

        PRINT_TABLE_HEADER();

        while (current != NULL) {
            if (current->data.date.month == m) {
                PRINT_SEARCH_ROW(current->data);
                found = 1;
            }
            current = current->next;
        }
    }

    // ---------------------------
    // 4. SEARCH BY YEAR
    // ---------------------------
    else if (ch == 4) {
        int y;
        printf("Enter year: ");
        scanf("%d", &y);

        PRINT_TABLE_HEADER();

        while (current != NULL) {
            if (current->data.date.year == y) {
                PRINT_SEARCH_ROW(current->data);
                found = 1;
            }
            current = current->next;
        }
    }

    else {
        printf(COLOR_RED "Invalid choice.\n" COLOR_RESET);
        waitForInput();
        return;
    }

    if (!found)
        printf(COLOR_YELLOW "No matching transactions found.\n" COLOR_RESET);

    waitForInput();
    
    // Clean up macros
    #undef PRINT_SEARCH_ROW
    #undef PRINT_TABLE_HEADER
}

// MAIN MENU LOOP 

void runMainMenu() {
    int choice;
    while(1) {
        printf("\n" COLOR_PINK COLOR_BOLD "====== PERSONAL FINANCE TRACKER ======" COLOR_RESET "\n");
        printf("1. Add Transaction\n");
        printf("2. View History\n");
        printf("3. View Financial Report\n");
        printf("4. Category-wise Distribution\n");
        printf("5. Set Budget Limit\n");
        printf("6. Search Transactions\n");
        printf("7. Exit\n");
        printf("Enter choice: ");


        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }

        switch(choice) {
            case 1: showAddTransactionForm(); break;
            case 2: showAllTransactions(); break;
            case 3: showSummary(); break;
            case 4: showCategoryDistribution(); break;
            case 5: showBudgetMenu(); break;
            case 6: searchTransactions(); break;
            case 7:
                saveAllData();
                printf(COLOR_GREEN "Data saved. Exiting...\n" COLOR_RESET);
                return;
            default:
                printf(COLOR_RED "Invalid Option\n" COLOR_RESET);
        }
    }
}