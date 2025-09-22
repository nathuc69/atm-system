#include <termios.h>
#include "header.h"

char *USERS = "./data/users.txt";

void loginMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    scanf("%49s", a);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    scanf("%49s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
};

void registerMenu(char a[50], char pass[50])
{
    struct termios oflags, nflags;
    FILE *fp;
    struct User userChecker = {0};
    int userExists = 0;

    system("clear");
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Registration:\n");
    printf("\n\t\t\t\tEnter username: ");
    scanf("%49s", a);

    // Check if username already exists
    fp = fopen(USERS, "r");
    if (fp != NULL) {
    while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, a) == 0)
        {
            userExists = 1;
            break;
        }
    }
    fclose(fp);
  }

    if (userExists)
    {
        printf("\n\t\t\t\tUsername already exists! Please choose another one.\n");
        printf("\n\t\t\t\tPress any key to continue...");
        getchar();
        getchar();
        return;
    }

    // disabling echo for password input
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }
    printf("\n\t\t\t\tEnter the password: ");
    scanf("%49s", pass);

    // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0)
    {
        perror("tcsetattr");
        return exit(1);
    }

    // Get the next available user ID
    int nextId = 0;
    if ((fp = fopen(USERS, "r")) != NULL)
    {
        while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
        {
            if (userChecker.id >= nextId)
            {
                nextId = userChecker.id + 1;
            }
        }
        fclose(fp);
    }

    // Save the new user
    if ((fp = fopen(USERS, "a")) == NULL)
    {
        printf("Error! opening file");
        exit(1);
    }

    fprintf(fp, "%d %s %s\n", nextId, a, pass);
    fclose(fp);

    printf("\n\t\t\t\tRegistration successful!\n");
    printf("\n\t\t\t\tPress any key to continue...");
    getchar();
    getchar();
};

const char *getPassword(struct User u)
{
    FILE *fp;
    struct User userChecker = {0};
    static char foundPassword[50] = {0};

    if ((fp = fopen(USERS, "r")) == NULL)
    {
        return "no user found";
    }

    while (fscanf(fp, "%d %49s %49s", &userChecker.id, userChecker.name, userChecker.password) != EOF)
    {
        if (strcmp(userChecker.name, u.name) == 0)
        {
            fclose(fp);
            strcpy(foundPassword, userChecker.password);
            return foundPassword;
        }
    }

    fclose(fp);
    return "no user found";
}