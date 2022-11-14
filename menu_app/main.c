#include "database.h"
#include "debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 200

void print(DB *db);

int main(void) {

    char input, ch, *result;
    char strBuf[BUFSIZE] = {0};
    int int_choice = 0;

    DB *db = CreateDatabase();

    do {
        printf("\nSkriv inn ønsket kommando [1-5]:\n");
        printf("1. Legg til et element i databasen\n");
        printf("2. Finn et element i databasen\n");
        printf("3. Finn antall elementer i databasen\n");
        printf("4. Slett et element fra databasen\n");
        printf("5. Avslutte\n");

        result = NULL;
        input = getchar();
        int_choice = atoi(&input);
        while ((ch = getchar()) != EOF && ch != '\n')
            ;
        switch (int_choice) {
        case 1:
            printf("adding element\n");
            result = fgets(strBuf, sizeof(strBuf), stdin);
            if (result != NULL) {
                AddEntry(db, strBuf);
            }
            memset(strBuf, 0, sizeof(strBuf));
            print(db);
            break;
        case 2:
            printf("find element by value\n");
            result = fgets(strBuf, sizeof(strBuf), stdin);
            if (result != NULL) {
                char *value = Search(db, strBuf);
                if (value == NULL) {
                    printf("could not find entry with value %s\n", strBuf);
                } else {
                    printf("found value %s\n", value);
                }
            }
            memset(strBuf, 0, sizeof(strBuf));
            break;
        case 3:
            printf("count in database is %d\n", db->iSze);
            break;
        case 4:
            printf("fjerner element, skriv in verdien som skal fjernes\n");
            result = fgets(strBuf, sizeof(strBuf), stdin);
            if (result != NULL) {
                int res = DeleteEntry(db, strBuf);
                if (res != 0) {
                    printf("kunne ikke finne noen element med den verdien\n");
                } else {
                    printf("fjernet element med veriden %s\n", strBuf);
                }
            }
            memset(strBuf, 0, sizeof(strBuf));
            break;
        case 5:
            printf("clean up\n");
            dispose(db);
            break;

        default:
            printf("ikke et tall mellom 1-5, prøv igjen\n");
            break;
        }

    } while (int_choice != 5);
}

void print(DB *db) {
    int i = 0;

    LIST *pHead = db->pHead;
    do{
        printf("%d: %s\n", ++i, pHead->cBuf);
        if(pHead->pNext == NULL){break;}
        pHead = pHead->pNext;
    }while (pHead->pNext != NULL);
    printf("\n");
}