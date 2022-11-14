#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedList.h"

static LIST *CreateNode(void *pvData, int iSze) {
    LIST *pThis = malloc(sizeof(LIST) + iSze);
    if (pThis != NULL) {
        memset(pThis, 0, sizeof(LIST) + iSze);
        pThis->iSze = iSze;
        memcpy(pThis->cBuf, pvData, iSze);
    }
    return pThis;
}

int InsertInList(LIST **ppHead, void *pvData, int iSze) {
    LIST *pThis = CreateNode(pvData, iSze);
    if (pThis == NULL)
        return 1;

    if (*ppHead == NULL) {
        *ppHead = pThis;
        return 0;
    }

    pThis->pNext = *ppHead;
    *ppHead = pThis;
    return 0;
}

int AppendToList(LIST **ppHead, void *pvData, int iSze) {
    LIST *pThis = CreateNode(pvData, iSze);
    if (pThis == NULL)
        return 1;

    if (*ppHead == NULL) {
        *ppHead = pThis;
        return 0;
    }

    LIST *pTemp = *ppHead;
    while (pTemp->pNext != NULL) {
        pTemp = pTemp->pNext;
    }
    pTemp->pNext = pThis;
    return 0;
}

LIST *FindFirstByValue(LIST **ppHead, void *pvData) {
    LIST *pTemp = *ppHead;
    do {
        if (strcmp(pTemp->cBuf, pvData) == 0) {
            return pTemp;
        }
        if (pTemp->pNext != NULL) {
            break;
        }
        pTemp = pTemp->pNext;
    } while (pTemp->pNext != NULL);
    return NULL;
}

int RemoveFromList(LIST **ppHead, LIST *pDelete) {
    if (*ppHead == NULL)
        return 1;

    if (*ppHead == pDelete) {
        *ppHead = pDelete->pNext;
        free(pDelete);
        return 0;
    }

    LIST *pTemp = *ppHead;
    while (pTemp->pNext != NULL) {
        if (pTemp->pNext == pDelete) {
            pTemp->pNext = pDelete->pNext;
            free(pDelete);
            return 0;
        }
        pTemp = pTemp->pNext;
    }
    return 1;
}