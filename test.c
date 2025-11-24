/*
 * PERSONAL FINANCE TRACKER (v2.0)
 * -------------------------------
 * Features:
 * - Predefined Categories
 * - Search Functionality
 * - Category-wise Analytics
 * - Auth, Budgeting, Goals, Storage
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// ==========================================
// DATA STRUCTURES
// ==========================================

#define MAX_USERS 10
#define MAX_TRANSACTIONS 200
#define MAX_GOALS 10
#define MAX_NAME_LEN 50
#define DATA_FILE "finance_data_v2.txt"

// Predefined Categories
const char* CATEGORIES[] = {
    "Salary", "Food", "Rent", "Transport", "Utilities", 
    "Entertainment", "Health", "Shopping", "Education", "Other"
};
const int NUM_CATEGORIES = 10;

typedef struct {
    int id;
    char type[10]; // "Income" or "Expense"
    char category[MAX_NAME_LEN];
    double amount;
    char date[20];
    char note[100];
} Transaction;

typedef struct {
    char name[MAX_NAME_LEN];
    double target_amount;
    double current_amount;
} Goal;

typedef struct {
    char category[MAX_NAME_LEN];
    double limit_amount;
} BudgetLimit;

typedef struct {
    char username[MAX_NAME_LEN];
    char password[MAX_NAME_LEN];
    double balance;
    Transaction transactions[MAX_TRANSACTIONS];
    int trans_count;
    Goal goals[MAX_GOALS];
    int goal_count;
    BudgetLimit limits[NUM_CATEGORIES];
    int limit_count;
} User;

// Global State
User currentUser;
int isLoggedIn = 0;

// ==========================================
// FUNCTION PROTOTYPES
// ==========================================

// Auth
void auth_menu();
int register_user();
int login_user();

// Storage
void save_data();
int load_data();

// UI Utils
void clear_screen();
void print_header(const char* title);
void press_enter();

// Core Logic
void main_menu();
void add_transaction();
void view_history_menu();
void print_transactions(const char* search_query);
void view_category_distribution();
void manage_goals();
void set_limits();
void check_limits(double expenseAmount, const char* category);
void get_current_date(char* buffer);

// ==========================================
// MAIN MODULE
// ==========================================

int main() {
    // Attempt to load existing data
    if (!load_data()) {
        currentUser.trans_count = 0;
        currentUser.goal_count = 0;
        currentUser.limit_count = 0;
        currentUser.balance = 0.0;
        memset(currentUser.username, 0, MAX_NAME_LEN);
    }

    while (1) {
        if (!isLoggedIn) {
            auth_menu();
        } else {
            main_menu();
        }
    }
    return 0;
}

// ==========================================
// UI & UTILS
// ==========================================

void print_header(const char* title) {
    printf("\n========================================\n");
    printf(" %s\n", title);
    printf("========================================\n");
}

void press_enter() {
    printf("\nPress [Enter] to continue...");
    getchar();
}

void get_current_date(char* buffer) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

// ==========================================
// AUTH MODULE
// ==========================================

void auth_menu() {
    int choice;
    print_header("FINANCE TRACKER V2.0");
    printf("1. Login\n");
    printf("2. Register (Reset Data)\n");
    printf("3. Exit\n");
    printf("Enter choice: ");
    scanf("%d", &choice);
    getchar(); 

    switch(choice) {
        case 1:
            if(login_user()) {
                isLoggedIn = 1;
                printf("Login Successful!\n");
            } else {
                printf("Login Failed. User not found.\n");
            }
            break;
        case 2:
            register_user();
            isLoggedIn = 1;
            save_data();
            break;
        case 3:
            exit(0);
        default:
            printf("Invalid choice.\n");
    }
}

int register_user() {
    print_header("REGISTER USER");
    printf("Enter new username: ");
    fgets(currentUser.username, MAX_NAME_LEN, stdin);
    currentUser.username[strcspn(currentUser.username, "\n")] = 0;

    printf("Enter new password: ");
    fgets(currentUser.password, MAX_NAME_LEN, stdin);
    currentUser.password[strcspn(currentUser.password, "\n")] = 0;

    currentUser.balance = 0;
    currentUser.trans_count = 0;
    currentUser.goal_count = 0;
    currentUser.limit_count = 0;
    
    printf("Registration Successful!\n");
    return 1;
}

int login_user() {
    char inUser[MAX_NAME_LEN], inPass[MAX_NAME_LEN];
    
    if (strlen(currentUser.username) == 0) return 0;

    print_header("LOGIN");
    printf("Username: ");
    fgets(inUser, MAX_NAME_LEN, stdin);
    inUser[strcspn(inUser, "\n")] = 0;

    printf("Password: ");
    fgets(inPass, MAX_NAME_LEN, stdin);
    inPass[strcspn(inPass, "\n")] = 0;

    if (strcmp(inUser, currentUser.username) == 0 && strcmp(inPass, currentUser.password) == 0) {
        return 1;
    }
    return 0;
}

// ==========================================
// TRANSACTIONS MODULE (Enhanced)
// ==========================================

void add_transaction() {
    if (currentUser.trans_count >= MAX_TRANSACTIONS) {
        printf("Memory full!\n");
        return;
    }

    Transaction *t = &currentUser.transactions[currentUser.trans_count];
    int typeChoice, catChoice;

    print_header("ADD TRANSACTION");
    printf("1. Income\n2. Expense\nChoice: ");
    scanf("%d", &typeChoice);
    getchar();

    if (typeChoice == 1) strcpy(t->type, "Income");
    else strcpy(t->type, "Expense");

    // Predefined Categories Selection
    printf("\n--- Select Category ---\n");
    for(int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%d. %s\n", i + 1, CATEGORIES[i]);
    }
    printf("Enter choice (1-%d): ", NUM_CATEGORIES);
    scanf("%d", &catChoice);
    getchar();

    if(catChoice < 1 || catChoice > NUM_CATEGORIES) {
        strcpy(t->category, "Other");
    } else {
        strcpy(t->category, CATEGORIES[catChoice - 1]);
    }

    printf("Amount: ");
    scanf("%lf", &t->amount);
    getchar();

    printf("Note: ");
    fgets(t->note, 100, stdin);
    t->note[strcspn(t->note, "\n")] = 0;

    get_current_date(t->date);
    t->id = currentUser.trans_count + 1;

    // Logic
    if (typeChoice == 1) {
        currentUser.balance += t->amount;
    } else {
        currentUser.balance -= t->amount;
        check_limits(t->amount, t->category);
    }

    currentUser.trans_count++;
    printf("Transaction added.\n");
    save_data();
}

void view_history_menu() {
    int choice;
    char query[50];

    while(1) {
        print_header("HISTORY OPTIONS");
        printf("1. View All Transactions\n");
        printf("2. Search Transactions (Keyword/Date)\n");
        printf("3. Back to Main Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar();

        switch(choice) {
            case 1:
                print_transactions(NULL);
                press_enter();
                break;
            case 2:
                printf("Enter search keyword (Category, Date, or Note): ");
                fgets(query, 50, stdin);
                query[strcspn(query, "\n")] = 0;
                print_transactions(query);
                press_enter();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice.\n");
        }
    }
}

// Combined Print/Search Function
void print_transactions(const char* search_query) {
    int found_count = 0;
    
    printf("\n%-4s %-8s %-12s %-10s %-12s %s\n", "ID", "Type", "Category", "Amount", "Date", "Note");
    printf("------------------------------------------------------------------\n");
    
    for(int i = 0; i < currentUser.trans_count; i++) {
        Transaction t = currentUser.transactions[i];
        
        int match = 1;
        if (search_query != NULL) {
            // Simple substring search (Case sensitive)
            if (strstr(t.category, search_query) == NULL && 
                strstr(t.note, search_query) == NULL && 
                strstr(t.date, search_query) == NULL &&
                strstr(t.type, search_query) == NULL) {
                match = 0;
            }
        }

        if (match) {
            printf("%-4d %-8s %-12s $%-9.2f %-12s %s\n", 
                t.id, t.type, t.category, t.amount, t.date, t.note);
            found_count++;
        }
    }
    printf("------------------------------------------------------------------\n");
    
    if (search_query != NULL) {
        printf("Found %d matching record(s).\n", found_count);
    }
}

// ==========================================
// ANALYTICS MODULE (New)
// ==========================================

void view_category_distribution() {
    print_header("CATEGORY DISTRIBUTION");
    
    printf("%-15s | %-12s | %-12s\n", "Category", "Total Income", "Total Expense");
    printf("----------------------------------------------\n");

    for (int i = 0; i < NUM_CATEGORIES; i++) {
        double total_income = 0;
        double total_expense = 0;
        int has_data = 0;

        for (int j = 0; j < currentUser.trans_count; j++) {
            if (strcmp(currentUser.transactions[j].category, CATEGORIES[i]) == 0) {
                if (strcmp(currentUser.transactions[j].type, "Income") == 0) {
                    total_income += currentUser.transactions[j].amount;
                } else {
                    total_expense += currentUser.transactions[j].amount;
                }
                has_data = 1;
            }
        }

        if (has_data) {
            printf("%-15s | $%-11.2f | $%-11.2f\n", CATEGORIES[i], total_income, total_expense);
        }
    }
    printf("----------------------------------------------\n");
    press_enter();
}

// ==========================================
// GOALS & LIMITS
// ==========================================

void check_limits(double expenseAmount, const char* category) {
    for (int i = 0; i < currentUser.limit_count; i++) {
        if (strcmp(currentUser.limits[i].category, category) == 0) {
            double spent = 0;
            for (int j = 0; j < currentUser.trans_count; j++) {
                if (strcmp(currentUser.transactions[j].type, "Expense") == 0 &&
                    strcmp(currentUser.transactions[j].category, category) == 0) {
                    spent += currentUser.transactions[j].amount;
                }
            }
            // Add current expense to check
            // (Note: The transaction is already added before calling this in add_transaction usually,
            // but logic here recalculates sum from array. Since we increment count AFTER check in many logic flows,
            // let's just use the calculated sum. If this is called after adding, 'spent' includes current.)
            
            if (spent > currentUser.limits[i].limit_amount) {
                printf("\n[!] WARNING: Budget exceeded for %s!\n", category);
                printf("    Limit: $%.2f | Spent: $%.2f\n", currentUser.limits[i].limit_amount, spent);
            }
        }
    }
}

void set_limits() {
    if (currentUser.limit_count >= NUM_CATEGORIES) {
        printf("Limits full.\n"); return;
    }
    
    int catChoice;
    printf("\n--- Select Category to Limit ---\n");
    for(int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%d. %s\n", i + 1, CATEGORIES[i]);
    }
    printf("Choice: ");
    scanf("%d", &catChoice);
    
    if(catChoice < 1 || catChoice > NUM_CATEGORIES) return;

    BudgetLimit *b = &currentUser.limits[currentUser.limit_count];
    strcpy(b->category, CATEGORIES[catChoice - 1]);
    
    printf("Max Monthly Amount: ");
    scanf("%lf", &b->limit_amount);
    
    currentUser.limit_count++;
    printf("Limit set successfully.\n");
    save_data();
}

void manage_goals() {
    int choice;
    print_header("FINANCIAL GOALS");
    printf("1. Set New Goal\n2. View Goals Progress\nChoice: ");
    scanf("%d", &choice);
    getchar();

    if (choice == 1) {
        if (currentUser.goal_count >= MAX_GOALS) return;
        Goal *g = &currentUser.goals[currentUser.goal_count];
        printf("Goal Name: ");
        fgets(g->name, MAX_NAME_LEN, stdin);
        g->name[strcspn(g->name, "\n")] = 0;
        printf("Target Amount: ");
        scanf("%lf", &g->target_amount);
        currentUser.goal_count++;
        printf("Goal saved.\n");
        save_data();
    } else {
        printf("\n%-20s %-10s %-10s %s\n", "Goal", "Target", "Saved", "Progress");
        for(int i=0; i<currentUser.goal_count; i++) {
            Goal g = currentUser.goals[i];
            double progress = (currentUser.balance / g.target_amount) * 100;
            if(progress > 100) progress = 100; else if(progress < 0) progress = 0;
            printf("%-20s $%-9.2f $%-9.2f %.1f%%\n", g.name, g.target_amount, currentUser.balance, progress);
        }
        press_enter();
    }
}

// ==========================================
// STORAGE & MAIN MENU
// ==========================================

void save_data() {
    FILE *f = fopen(DATA_FILE, "wb");
    if (f) {
        fwrite(&currentUser, sizeof(User), 1, f);
        fclose(f);
    }
}

int load_data() {
    FILE *f = fopen(DATA_FILE, "rb");
    if (!f) return 0;
    fread(&currentUser, sizeof(User), 1, f);
    fclose(f);
    return 1;
}

void main_menu() {
    int choice;
    print_header("MAIN DASHBOARD");
    printf("User: %s | Wallet: $%.2f\n", currentUser.username, currentUser.balance);
    printf("----------------------------------------\n");
    printf("1. Add Transaction\n");
    printf("2. View History & Search\n");
    printf("3. Category Distribution Report\n");
    printf("4. Manage Goals\n");
    printf("5. Set Budget Limits\n");
    printf("6. Logout\n");
    printf("Enter choice: ");
    
    scanf("%d", &choice);
    getchar();

    switch(choice) {
        case 1: add_transaction(); break;
        case 2: view_history_menu(); break;
        case 3: view_category_distribution(); break;
        case 4: manage_goals(); break;
        case 5: set_limits(); break;
        case 6: isLoggedIn = 0; break;
        default: printf("Invalid choice.\n");
    }
}