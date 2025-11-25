/* reports.h */
#ifndef REPORTS_H
#define REPORTS_H

#include "Finance.h" // <--- This is the key line!

double getCategoryTotal(char* category);
void sortTransactionsByDate();
double getIncomeCategoryTotal(char* category);


#endif