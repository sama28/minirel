
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <stdio.h>


Destroy (argc, argv)
int	argc;
char	**argv;

{
    unsigned char a[59]="absdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghj";
    InsertRec(0,&a);
    struct recid rid;
    rid.pid=0;
    rid.slotnum=2;
    DeleteRec(0,&rid);
    unsigned char b[59]="aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    rid.slotnum=3;
    WriteRec(0,&b,&rid);
}


