#ifndef UI_H
#define UI_H

#include "Finance.h"

// Show the main menu loop for a logged-in user
void runMainMenu(User currentUser);

// Internal helpers exposed for clarity (optional to call from Main)
void showSummary();
void listAllTransactionsUI();
void addTransactionUI();
void deleteTransactionUI();
void manageBudgetsUI();
void showReportsUI();

#endif
