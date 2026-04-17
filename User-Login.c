#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkForUser(char username[])
{
    FILE *credentials = fopen("Credentials.txt", "r");
    int check = 0;
    if(fgetc(credentials)!=EOF){
        rewind(credentials);
    while (!feof(credentials))
    {
        char buffer[100] = "";
        char user[50] = "";
        fgets(buffer, 100, credentials);
        int i = 0;
        while ((buffer[i]) != ' ')
        {
            user[i] = buffer[i];
            i++;
        }
        if (!strncmp(user, username, i))
        {
            check = 1;
            break;
        }
    }
}
    fclose(credentials);
    return check;
}
void addUser()
{
    FILE *credentials = fopen("Credentials.txt", "a");

    char bufferU[50];
    char bufferP[50];
    char bufferUCheck[50];
    char bufferPCheck[50];

    printf("\n");
    printf("Sign Up:");
    printf("\n");
    while (1)
    {
        printf("Please type new username:");
        scanf("%s", bufferU);
        printf("Please verify username:");
        scanf("%s", bufferUCheck);

        if (strcmp(bufferU, bufferUCheck))
        {
            printf("Usernames do not match.\n");
        }
        else if (checkForUser(bufferU))
        {
            printf("This username is taken.\n");
        }
        else
        {
            break;
        }
    }
    while (1)
    {
        printf("Please type new password:");
        scanf("%s", bufferP);
        printf("Please verify password:");
        scanf("%s", bufferPCheck);

        if (strcmp(bufferP, bufferPCheck))
        {
            printf("Passwords do not match.\n");
        }
        else
        {
            break;
        }
    }
    if (credentials == NULL)
    {

        fprintf(credentials, "%s %s", bufferU, bufferP);
    }
    else
    {
        fprintf(credentials, "\n%s %s", bufferU, bufferP);
    }
    fclose(credentials);
}
int checkPassword(char username[], char password[])
{
    FILE *credentials = fopen("Credentials.txt", "r");
    int check = 0;
    while (!feof(credentials))
    {
        char buffer[100] = "";
        char user[50] = "";
        char pass[50] = "";
        fgets(buffer, 100, credentials);
        int i = 0;
        while ((buffer[i]) != ' ')
        {
            user[i] = buffer[i];
            i++;
        }
        int k = i;
        i++;
        int j = 0;
        while (((buffer[i]) != '\n')&&(!feof(credentials)))
        {
            pass[j] = buffer[i];
            i++;
            j++;
        }
        if (!strncmp(user, username, k))
        {
            if (!strncmp(pass, password, j))
            {
                check = 1;
                break;
            }
            else
            {
                break;
            }
        }
    }
    fclose(credentials);
    return check;
}
void authenticateUser()
{
    char username[50] = "";
    char password[50] = "";
    int willaddUser = 0;
    int authenticate = 0;
    printf("Enter \"SIGNUP\" at anytime to sign up\n");
    printf("\n");
    while (!authenticate)
    {
        if (willaddUser)
        {
            addUser();
            willaddUser = 0;
        }
        printf("Login:\n");
        while (!willaddUser)
        {
            char buffer[50] = "";
            printf("Please enter your username:");
            scanf("%s", buffer);
            if (!strcmp(buffer, "SIGNUP"))
            {
                willaddUser = 1;
                break;
            }
            else if (!checkForUser(buffer))
            {
                printf("Username not found.\n");
            }
            else
            {
                strcpy(username, buffer);
                break;
            }
        }
        while (!willaddUser)
        {
            char buffer[50] = "";
            printf("Please enter your password:");
            scanf("%s", buffer);
            if (!strcmp(buffer, "SIGNUP"))
            {
                willaddUser = 1;
                break;
            }
            else if (!checkPassword(username, buffer))
            {
                printf("Incorrect Password.\n");
            }
            else
            {
                strcpy(password, buffer);
                printf("Success!!\n");
                authenticate = 1;
                break;
            }
        }
    }
}

// int main()
// {
//     authenticateUser();
//     return 0;
// }