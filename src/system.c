#include "header.h"

const char *RECORDS = "./data/records.txt";

int getAccountFromFile(FILE *ptr, char name[50], struct Record *r)
{
    int result = fscanf(ptr, "%d %d %s %d %d/%d/%d %s %d %lf %s",
                  &r->id,
		  &r->userId,
		  name,
                  &r->accountNbr,
                  &r->deposit.month,
                  &r->deposit.day,
                  &r->deposit.year,
                  r->country,
                  &r->phone,
                  &r->amount,
                  r->accountType);
    if (result != EOF && result == 11) {
        strcpy(r->name, name);
        return 1;
    }
    return 0;
}

void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            r.id,
            r.userId,
            u.name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u)
{
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u)
{
    struct Record r;
    struct Record cr;
    char userName[50];
    int maxId = 0;
    FILE *pf = fopen(RECORDS, "a+");
    if (pf == NULL) {
        printf("✖ Error opening records file!\n");
        printf("Enter 1 to return to main menu and 0 to exit: ");
        int option;
        scanf("%d", &option);
        if (option == 1) {
            mainMenu(u);
        } else {
            exit(1);
        }
        return;
    }

    // Find the highest ID
    rewind(pf);
    while (getAccountFromFile(pf, userName, &cr)) {
        if (cr.id > maxId) maxId = cr.id;
    }

noAccount:
    system("clear");
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);

    // Basic date validation
    if (r.deposit.month < 1 || r.deposit.month > 12 ||
        r.deposit.day < 1 || r.deposit.day > 31 ||
        r.deposit.year < 1900 || r.deposit.year > 2100) {
        printf("✖ Invalid date format!\n");
        goto noAccount;
    }

    printf("\nEnter the account number:");
    scanf("%d", &r.accountNbr);

    // Validate account number
    if (r.accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        goto noAccount;
    }

    rewind(pf);
    while (getAccountFromFile(pf, userName, &cr))
    {
        if (strcmp(userName, u.name) == 0 && cr.accountNbr == r.accountNbr)
        {
            printf("✖ This Account already exists for this user\n\n");
            goto noAccount;
        }
    }
    printf("\nEnter the country:");
    scanf("%99s", r.country);  // Prevent buffer overflow
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);

    // Validate phone number
    if (r.phone <= 0) {
        printf("✖ Phone number must be positive!\n");
        goto noAccount;
    }

    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);

    // Validate deposit amount
    if (r.amount <= 0) {
        printf("✖ Deposit amount must be positive!\n");
        goto noAccount;
    }

    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%9s", r.accountType);  // Prevent buffer overflow (accountType is char[10])

    // Validate account type
    if (strcmp(r.accountType, "saving") != 0 &&
        strcmp(r.accountType, "current") != 0 &&
        strcmp(r.accountType, "fixed01") != 0 &&
        strcmp(r.accountType, "fixed02") != 0 &&
        strcmp(r.accountType, "fixed03") != 0) {
        printf("✖ Invalid account type!\n");
        goto noAccount;
    }

    r.id = maxId + 1;
    r.userId = u.id;
    saveAccountToFile(pf, u, r);

    fclose(pf);
    success(u);
}

double calculateInterest(struct Record r)
{
    if (strcmp(r.accountType, "saving") == 0) {
        // 7% annual interest, monthly calculation
        return (r.amount * 0.07) / 12.0;
    } else if (strcmp(r.accountType, "fixed01") == 0) {
        // 4% annual interest for 1 year
        return r.amount * 0.04;
    } else if (strcmp(r.accountType, "fixed02") == 0) {
        // 5% annual interest for 2 years
        return r.amount * 0.05 * 2.0;
    } else if (strcmp(r.accountType, "fixed03") == 0) {
        // 8% annual interest for 3 years
        return r.amount * 0.08 * 3.0;
    }
    return 0.0; // No interest for current accounts
}

void checkAllAccounts(struct User u)
{
    char userName[100];
    struct Record r;
    double interest;

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("✖ Error opening records file!\n");
        stayOrReturn(0, checkAllAccounts, u);
        return;
    }

    system("clear");
    printf("\t\t====== All accounts from user, %s =====\n\n", u.name);
    while (getAccountFromFile(pf, userName, &r))
    {
        if (strcmp(userName, u.name) == 0)
        {
            printf("_____________________\n");
            printf("\nAccount number:%d\nDeposit Date:%d/%d/%d \ncountry:%s \nPhone number:%d \nAmount deposited: $%.2f \nType Of Account:%s\n",
                   r.accountNbr,
                   r.deposit.day,
                   r.deposit.month,
                   r.deposit.year,
                   r.country,
                   r.phone,
                   r.amount,
                   r.accountType);
            
            if (strcmp(r.accountType, "current") == 0) {
                printf("You will not get interests because the account is of type current\n");
            } else {
                interest = calculateInterest(r);
                if (strcmp(r.accountType, "saving") == 0) {
                    printf("Interest: $%.2f on day %d of every month\n", interest, r.deposit.day);
                } else if (strcmp(r.accountType, "fixed01") == 0) {
                    printf("Interest: $%.2f on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 1);
                } else if (strcmp(r.accountType, "fixed02") == 0) {
                    printf("Interest: $%.2f on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 2);
                } else if (strcmp(r.accountType, "fixed03") == 0) {
                    printf("Interest: $%.2f on %d/%d/%d\n", interest, r.deposit.month, r.deposit.day, r.deposit.year + 3);
                }
            }
        }
    }
    fclose(pf);
    success(u);
}

void updateAccountInfo(struct User u) 
{
    int accountNbr, found = 0, choice;
    char userName[50];
    struct Record r;
    struct Record records[100];
    int recordCount = 0;

    system("clear");
    printf("\t\t====== Update Account Information =======\n\n");
    printf("Enter the account number to update: ");
    scanf("%d", &accountNbr);

    // Validate account number
    if (accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }

    // Read all records into memory
    while (getAccountFromFile(pf, userName, &r)) {
        if (recordCount >= 100) {
            printf("✖ Error: Too many records in file!\n");
            fclose(pf);
            stayOrReturn(0, updateAccountInfo, u);
            return;
        }
        records[recordCount] = r;
        strcpy(records[recordCount].name, userName);
        recordCount++;

        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            printf("\nAccount found!\n");
            printf("Current Country: %s\n", r.country);
            printf("Current Phone: %d\n", r.phone);

            printf("\nWhat would you like to update?\n");
            printf("[1] Country\n[2] Phone number\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter new country: ");
                scanf("%99s", records[recordCount-1].country);  // Prevent buffer overflow
            } else if (choice == 2) {
                printf("Enter new phone number: ");
                scanf("%d", &records[recordCount-1].phone);
                // Validate phone number
                if (records[recordCount-1].phone <= 0) {
                    printf("✖ Phone number must be positive!\n");
                    fclose(pf);
                    stayOrReturn(0, updateAccountInfo, u);
                    return;
                }
            } else {
                printf("Invalid Choice!\n");
                fclose(pf);
                stayOrReturn(0, updateAccountInfo, u);
                return;
            }
        }
    }
    fclose(pf);

    if (!found) {
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }

    // Write all records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL) {
        printf("✖ Error: Could not save changes to file!\n");
        stayOrReturn(0, updateAccountInfo, u);
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            records[i].id,
            records[i].userId,
            records[i].name,
            records[i].accountNbr,
            records[i].deposit.month,
            records[i].deposit.day,
            records[i].deposit.year,
            records[i].country,
            records[i].phone,
            records[i].amount,
            records[i].accountType);
    }
    fclose(pf);

    success(u);
}

void makeTransaction(struct User u)
{
    int accountNbr, found = 0, choice, targetIndex = -1;
    double amount;
    char userName[50];
    struct Record r;
    struct Record records[100];
    int recordCount = 0;

    system("clear");
    printf("\t\t====== Make Transaction =======\n\n");
    printf("Enter the account number: ");
    scanf("%d", &accountNbr);

    // Validate account number
    if (accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    // Read all records into memory and find the target account
    while (getAccountFromFile(pf, userName, &r)) {
        if (recordCount >= 100) {
            printf("✖ Error: Too many records in file!\n");
            fclose(pf);
            stayOrReturn(0, makeTransaction, u);
            return;
        }
        records[recordCount] = r;
        strcpy(records[recordCount].name, userName);
        
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            targetIndex = recordCount;
            
            // Check if account is fixed type
            if (strstr(r.accountType, "fixed") != NULL) {
                printf("\n✖ Error: It is not possible to withdraw or deposit for fixed accounts!\n");
                fclose(pf);

                // Custom handling for fixed account error
                int option;
                printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit: ");
                scanf("%d", &option);
                if (option == 0)
                    makeTransaction(u);
                else if (option == 1)
                    mainMenu(u);
                else if (option == 2)
                    exit(0);
                else {
                    printf("Insert a valid operation!\n");
                    makeTransaction(u);
                }
                return;
            }
        }
        recordCount++;
    }
    fclose(pf);

    if (!found) {
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    printf("\nAccount found!\n");
    printf("Current Balance: $%.2f\n", records[targetIndex].amount);
    printf("\nTransaction Type:\n");
    printf("[1] Deposit\n[2] Withdraw\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter amount to deposit: $");
        scanf("%lf", &amount);
        if (amount <= 0) {
            printf("Invalid amount!\n");
            stayOrReturn(0, makeTransaction, u);
            return;
        }
        records[targetIndex].amount += amount;
        printf("\n✔ Deposited $%.2f successfully!\n", amount);
    } else if (choice == 2) {
        printf("Enter amount to withdraw: $");
        scanf("%lf", &amount);
        if (amount <= 0) {
            printf("Invalid amount!\n");
            stayOrReturn(0, makeTransaction, u);
            return;
        }
        if (amount > records[targetIndex].amount) {
            printf("\n✖ Insufficient balance! Available: $%.2f\n", records[targetIndex].amount);
            stayOrReturn(0, makeTransaction, u);
            return;
        }
        records[targetIndex].amount -= amount;
        printf("\n✔ Withdrew $%.2f successfully!\n", amount);
    } else {
        printf("Invalid choice!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }

    // Write all records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL) {
        printf("✖ Error: Could not save transaction to file!\n");
        stayOrReturn(0, makeTransaction, u);
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            records[i].id,
            records[i].userId,
            records[i].name,
            records[i].accountNbr,
            records[i].deposit.month,
            records[i].deposit.day,
            records[i].deposit.year,
            records[i].country,
            records[i].phone,
            records[i].amount,
            records[i].accountType);
    }
    fclose(pf);

    success(u);
}

/**
 * Check and display detailed information for a specific account
 * including balance and interest calculations based on account type
 */
void checkAccountDetails(struct User u)
{
    int accountNbr, found = 0;
    char userName[50];
    struct Record r;
    double interest;

    system("clear");
    printf("\t\t====== Check Account Details =======\n\n");
    printf("Enter the account number to check: ");
    scanf("%d", &accountNbr);

    // Validate account number
    if (accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        stayOrReturn(0, checkAccountDetails, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(0, checkAccountDetails, u);
        return;
    }

    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            printf("\n_____________________\n");
            printf("\nAccount Details:\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Deposit Date: %d/%d/%d\n", r.deposit.month, r.deposit.day, r.deposit.year);
            printf("Country: %s\n", r.country);
            printf("Phone number: %d\n", r.phone);
            printf("Amount deposited: $%.2f\n", r.amount);
            printf("Type Of Account: %s\n", r.accountType);

            if (strcmp(r.accountType, "current") == 0) {
                printf("\nYou will not get interests because the account is of type current\n");
            } else {
                interest = calculateInterest(r);
                if (strcmp(r.accountType, "saving") == 0) {
                    printf("\nYou will get $%.2f as interest on day %d of every month\n",
                           interest, r.deposit.day);
                } else if (strcmp(r.accountType, "fixed01") == 0) {
                    printf("\nYou will get $%.2f as interest on %d/%d/%d\n",
                           interest, r.deposit.month, r.deposit.day, r.deposit.year + 1);
                } else if (strcmp(r.accountType, "fixed02") == 0) {
                    printf("\nYou will get $%.2f as interest on %d/%d/%d\n",
                           interest, r.deposit.month, r.deposit.day, r.deposit.year + 2);
                } else if (strcmp(r.accountType, "fixed03") == 0) {
                    printf("\nYou will get $%.2f as interest on %d/%d/%d\n",
                           interest, r.deposit.month, r.deposit.day, r.deposit.year + 3);
                }
            }
            break;
        }
    }
    fclose(pf);

    if (!found) {
        stayOrReturn(0, checkAccountDetails, u);
        return;
    }

    success(u);
}

/**
 * Remove an account owned by the current user
 * Requires confirmation before deletion and updates the records file
 */
void removeAccount(struct User u)
{
    int accountNbr, found = 0, choice;
    char userName[50];
    struct Record r;
    struct Record records[100];
    int recordCount = 0;

    system("clear");
    printf("\t\t====== Remove Account =======\n\n");
    printf("Enter the account number to remove: ");
    scanf("%d", &accountNbr);

    // Validate account number
    if (accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        stayOrReturn(0, removeAccount, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(0, removeAccount, u);
        return;
    }

    // Read all records into memory
    while (getAccountFromFile(pf, userName, &r)) {
        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            printf("\nAccount found!\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Balance: $%.2f\n", r.amount);
            printf("Account type: %s\n", r.accountType);

            printf("\nAre you sure you want to delete this account? (1 = Yes, 0 = No): ");
            scanf("%d", &choice);

            if (choice != 1) {
                printf("Account deletion cancelled.\n");
                fclose(pf);
                success(u);
                return;
            }
            // Don't add this record to the records array (effectively deleting it)
        } else {
            // Keep all other records
            if (recordCount >= 100) {
                printf("✖ Error: Too many records in file!\n");
                fclose(pf);
                stayOrReturn(0, removeAccount, u);
                return;
            }
            records[recordCount] = r;
            strcpy(records[recordCount].name, userName);
            recordCount++;
        }
    }
    fclose(pf);

    if (!found) {
        stayOrReturn(0, removeAccount, u);
        return;
    }

    // Write all remaining records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL) {
        printf("✖ Error: Could not save changes to file!\n");
        stayOrReturn(0, removeAccount, u);
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            records[i].id,
            records[i].userId,
            records[i].name,
            records[i].accountNbr,
            records[i].deposit.month,
            records[i].deposit.day,
            records[i].deposit.year,
            records[i].country,
            records[i].phone,
            records[i].amount,
            records[i].accountType);
    }
    fclose(pf);

    printf("\n✔ Account deleted successfully!\n");
    success(u);
}

/**
 * Transfer ownership of an account from current user to another user
 * Validates that both the account exists and the target user exists
 */
void transferOwnership(struct User u)
{
    int accountNbr, found = 0, targetUserId = -1;
    char userName[50], targetUsername[50];
    struct Record r;
    struct Record records[100];
    int recordCount = 0;
    struct User targetUser;

    system("clear");
    printf("\t\t====== Transfer Account Ownership =======\n\n");
    printf("Enter the account number to transfer: ");
    scanf("%d", &accountNbr);

    // Validate account number
    if (accountNbr <= 0) {
        printf("✖ Account number must be positive!\n");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    FILE *pf = fopen(RECORDS, "r");
    if (pf == NULL) {
        printf("Error opening file!\n");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    // Read all records into memory and find the account
    while (getAccountFromFile(pf, userName, &r)) {
        if (recordCount >= 100) {
            printf("✖ Error: Too many records in file!\n");
            fclose(pf);
            stayOrReturn(0, transferOwnership, u);
            return;
        }
        records[recordCount] = r;
        strcpy(records[recordCount].name, userName);

        if (strcmp(userName, u.name) == 0 && r.accountNbr == accountNbr) {
            found = 1;
            printf("\nAccount found!\n");
            printf("Account number: %d\n", r.accountNbr);
            printf("Balance: $%.2f\n", r.amount);
            printf("Account type: %s\n", r.accountType);
        }
        recordCount++;
    }
    fclose(pf);

    if (!found) {
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    printf("\nEnter the username to transfer ownership to: ");
    scanf("%49s", targetUsername);  // Prevent buffer overflow

    // Validate that user is not transferring to themselves
    if (strcmp(targetUsername, u.name) == 0) {
        printf("✖ Error: Cannot transfer account to yourself!\n");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    // Check if target user exists
    FILE *userFile = fopen("./data/users.txt", "r");
    if (userFile == NULL) {
        printf("Error opening users file!\n");
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    while (fscanf(userFile, "%d %s %s", &targetUser.id, targetUser.name, targetUser.password) != EOF) {
        if (strcmp(targetUser.name, targetUsername) == 0) {
            targetUserId = targetUser.id;
            break;
        }
    }
    fclose(userFile);

    if (targetUserId == -1) {
        printf("\n✖ Target user '%s' does not exist!\n", targetUsername);
        stayOrReturn(0, transferOwnership, u);
        return;
    }

    // Update the ownership in records array
    for (int i = 0; i < recordCount; i++) {
        if (strcmp(records[i].name, u.name) == 0 && records[i].accountNbr == accountNbr) {
            records[i].userId = targetUserId;
            strcpy(records[i].name, targetUsername);
            break;
        }
    }

    // Write all records back to file
    pf = fopen(RECORDS, "w");
    if (pf == NULL) {
        printf("✖ Error: Could not save changes to file!\n");
        stayOrReturn(0, transferOwnership, u);
        return;
    }
    for (int i = 0; i < recordCount; i++) {
        fprintf(pf, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n",
            records[i].id,
            records[i].userId,
            records[i].name,
            records[i].accountNbr,
            records[i].deposit.month,
            records[i].deposit.day,
            records[i].deposit.year,
            records[i].country,
            records[i].phone,
            records[i].amount,
            records[i].accountType);
    }
    fclose(pf);

    printf("\n✔ Account ownership transferred successfully to %s!\n", targetUsername);
    success(u);
}