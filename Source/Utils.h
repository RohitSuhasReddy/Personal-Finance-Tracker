#ifndef UTILS_H
#define UTILS_H
#include "Finance.h" // Needs your struct Date

// Returns 1 if valid, 0 if invalid
int isValidDate(Date d); 

// Returns negative if d1 is older, positive if d1 is newer, 0 if same
int compareDates(Date d1, Date d2); 

#endif