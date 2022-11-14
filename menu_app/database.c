#include "database.h"
#include "debugger.h"
#include "linkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

DB *CreateDatabase() {
    debug("creating db");
    DB *db = malloc(sizeof(DB));
    if (db == NULL) {
        error("failed to create Phead");
        return NULL;
    }
    memset(db, 0, sizeof(db) + 0);
    db->pHead = NULL;
    db->iSze = 0;
    InsertInList(& db->pHead, "",1);

    debug("created db");
    return db;
}

int AddEntry(DB *db, char *pcValue) {
    debug("adding %s to db", pcValue);
    LIST *pHead = db->pHead;

    int res = AppendToList(&pHead, pcValue, strlen(pcValue) + 1);
    if (res != 0) {
        error("failed to create entry");
        return res;
    }
    db->pHead = pHead;
    db->iSze++;
    return 0;
}

int DeleteEntry(DB *db, char *pcValue) {
    LIST *pHead = db->pHead;
    LIST *node = FindFirstByValue(&pHead, pcValue);
    if (node == NULL) {
        error("failed to find node with value %s", pcValue);
        return 1;
    }
    int res = RemoveFromList(&pHead, node);
    if (res != 0) {
        error("failed to create entry");
        return res;
    }
    db->iSze--;
    return 0;
}

char *Search(DB *db, char *pcValue) {
    debug("Search for %s", pcValue);
    LIST *pHead = db->pHead;

    LIST *node = FindFirstByValue(&pHead, pcValue);
    if (node == NULL) {
        debug("did not find node with value %s", pcValue);
        return NULL;
    }
    debug("found node");
    return node->cBuf;
}

void dispose(DB *db) {
    LIST *pHead = db->pHead;
    while (pHead != NULL) {
        LIST *pTemp = pHead->pNext;
        debug("freeing data %s", pHead->cBuf);
        free(pHead);
        pHead = pTemp;
    }
    free(db);
    debug("disposed");
}