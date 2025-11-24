#include "utils.h"

int isValidDate(Date d) {
    // Simple check: Month 1-12, Day 1-31
    if (d.month < 1 || d.month > 12) return 0;
    if (d.day < 1 || d.day > 31) return 0;
    if (d.year < 2000 || d.year > 2100) return 0;
    return 1;
}

int compareDates(Date d1, Date d2) {
    // Shortcut logic: 
    // If years are different, return the difference
    if (d1.year != d2.year) return d1.year - d2.year;
    
    // If months are different...
    if (d1.month != d2.month) return d1.month - d2.month;
    
    // If days are different...
    return d1.day - d2.day;
}