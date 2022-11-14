#ifndef _linkedList_H_
#define _linkedList_H_

typedef struct _LIST {
    struct _LIST *pNext;
    int iSze;
    char cBuf[1];
} LIST;

int InsertInList(LIST **ppHead, void *pvData, int iSze);
int AppendToList(LIST **ppHead, void *pvData, int iSze);
int RemoveFromList(LIST **ppHead, LIST *pDelete);
LIST *FindFirstByValue(LIST **ppHead, void *pvData);

#endif