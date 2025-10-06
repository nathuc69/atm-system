#include "header.h"

void mainMenu(struct User u)
{
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
        break;
    case 2:
        updateAccountInfo(u);
        break;
    case 3:
        checkAccountDetails(u);
        break;
    case 4:
        checkAllAccounts(u);
        break;
    case 5:
        makeTransaction(u);
        break;
    case 6:
        removeAccount(u);
        break;
    case 7:
        transferOwnership(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation! Please choose 1-8.\n");
        mainMenu(u);  // Return to main menu instead of exiting
        break;
    }
};

void initMenu(struct User *u)
{
    int r = 0;
    int option;
    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        switch (option)
        {
        case 1:
            loginMenu(u->name, u->password);
            if (strcmp(u->password, getPassword(*u)) == 0)
            {
                printf("\n\nPassword Match!");
            }
            else
            {
                printf("\nWrong password!! or User Name\n");
                exit(1);
            }
            r = 1;
            break;
        case 2:
           if (registerMenu(u->name, u->password)) {
                r = 1;
           }
            system("clear");
            printf("\n\n\t\t======= ATM =======\n");
            printf("\n\t\t-->> Feel free to login / register :\n");
            printf("\n\t\t[1]- login\n");
            printf("\n\t\t[2]- register\n");
            printf("\n\t\t[3]- exit\n");
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};

int main()
{
    struct User u;
    
    // Initialize user struct
    u.id = -1;
    strcpy(u.name, "");
    strcpy(u.password, "");
    
    initMenu(&u);
    
    // Get user ID from file after successful login
    FILE *fp = fopen("./data/users.txt", "r");
    if (fp != NULL) {
        struct User temp;
        while (fscanf(fp, "%d %s %s", &temp.id, temp.name, temp.password) != EOF) {
            if (strcmp(temp.name, u.name) == 0) {
                u.id = temp.id;
                break;
            }
        }
        fclose(fp);
    }
    
    if (u.id == -1) {
        printf("Error: Could not find user ID\n");
        exit(1); 
    }
    
    mainMenu(u);
    return 0;
}
