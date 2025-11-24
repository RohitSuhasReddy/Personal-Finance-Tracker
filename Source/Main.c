#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Finance.h"
#include "UI.h"

#define USERS_FILE "users.txt"

// Get next user ID by scanning file
static int getNextUserId() {
    FILE* fp = fopen(USERS_FILE, "r");
    if (!fp) {
        // No file yet
        return 1;
    }

    int maxId = 0;
    int id;
    char uname[30], pwd[30];

    while (fscanf(fp, "%d %29s %29s", &id, uname, pwd) == 3) {
        if (id > maxId) maxId = id;
    }

    fclose(fp);
    return maxId + 1;
}

static int registerNewUser(User* outUser) {
    FILE* fp = fopen(USERS_FILE, "a");
    if (!fp) {
        printf("Error opening users file for writing.\n");
        return 0;
    }

    User u;
    u.userId = getNextUserId();

    printf("\n--- New User Registration ---\n");
    printf("Enter username (no spaces): ");
    scanf("%29s", u.username);
    printf("Enter password (no spaces): ");
    scanf("%29s", u.password);

    // Save to file
    fprintf(fp, "%d %s %s\n", u.userId, u.username, u.password);
    fclose(fp);

    *outUser = u;

    printf("User created successfully! Your user ID is %d.\n", u.userId);
    return 1;
}

static int loginExistingUser(User* outUser) {
    FILE* fp = fopen(USERS_FILE, "r");
    if (!fp) {
        printf("No users found. Please create a new user first.\n");
        return 0;
    }

    char uname[30], pwd[30];
    printf("\n--- Existing User Login ---\n");
    printf("Enter username: ");
    scanf("%29s", uname);
    printf("Enter password: ");
    scanf("%29s", pwd);

    int id;
    char fileUser[30], filePass[30];
    int found = 0;

    while (fscanf(fp, "%d %29s %29s", &id, fileUser, filePass) == 3) {
        if (strcmp(uname, fileUser) == 0 && strcmp(pwd, filePass) == 0) {
            found = 1;
            outUser->userId = id;
            strcpy(outUser->username, fileUser);
            strcpy(outUser->password, filePass);
            break;
        }
    }

    fclose(fp);

    if (!found) {
        printf("Invalid username or password.\n");
        return 0;
    }

    printf("Login successful! Welcome back, %s (ID: %d).\n",
           outUser->username, outUser->userId);
    return 1;
}

int main() {
    User currentUser;
    int choice;

    printf("=====================================\n");
    printf("      SIMPLE PERSONAL FINANCE APP    \n");
    printf("=====================================\n");

    while (1) {
        printf("\n1. New User\n");
        printf("2. Existing User\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting.\n");
            return 0;
        }

        if (choice == 1) {
            if (registerNewUser(&currentUser)) {
                break; // go to main menu
            }
        } else if (choice == 2) {
            if (loginExistingUser(&currentUser)) {
                break; // go to main menu
            }
        } else if (choice == 0) {
            printf("Exiting program.\n");
            return 0;
        } else {
            printf("Invalid choice. Try again.\n");
        }
    }

    // Logged-in user -> run UI
    runMainMenu(currentUser);

    return 0;
}
