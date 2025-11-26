#include "Utils.h"

int isValidDate(Date d) {
    // 1. Basic Range Checks
    if (d.year < 2000 || d.year > 2100) return 0; // Year out of range
    if (d.month < 1 || d.month > 12) return 0;    // Month out of range
    if (d.day < 1) return 0;                      // Day cannot be 0 or negative

    // 2. Define days in each month (Index 0 is empty, Index 1 is Jan, etc.)
    // Jan, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 3. Leap Year Logic (Updates Feb to 29 days)
    // Rule: Divisible by 4 AND (NOT divisible by 100 OR divisible by 400)
    if ((d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0)) {
        daysInMonth[2] = 29;
    }

    // 4. Final Check: Does the day exceed the max days for that month?
    if (d.day > daysInMonth[d.month]) {
        return 0; // Invalid (e.g., Nov 31st or Feb 30th)
    }

    return 1; // Date is Valid
}

int compareDates(Date d1, Date d2) {
    // If years are different, return the difference
    if (d1.year != d2.year) return d1.year - d2.year;

    // If months are different...
    if (d1.month != d2.month) return d1.month - d2.month;

    // If days are different...
    return d1.day - d2.day;
}