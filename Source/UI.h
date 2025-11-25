#ifndef UI_H
#define UI_H

#include "Finance.h"

//  ANSI COLOR CODES
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"
#define COLOR_PINK    "\x1b[95m"
#define COLOR_BOLD    "\x1b[1m"

// Clears the console window
void clearScreen();

// The main loop that shows the menu
void runMainMenu();

// Individual Screens
void showAddTransactionForm();
void showAllTransactions();
void showSummary();
void showBudgetMenu();

#endif