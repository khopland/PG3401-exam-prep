#ifndef __TK1104_DATABASE_H__
#define __TK1104_DATABASE_H__
#include"linkedList.h"

typedef struct _DB {
    LIST *pHead;
    int iSze;
} DB;

DB *CreateDatabase();
int AddEntry(DB *db, char *pcValue) ;
int DeleteEntry(DB *db, char *pcValue);
char *Search(DB *db, char *pcValue) ;
void dispose(DB *db);
#endif // __TK1104_DATABASE_H__