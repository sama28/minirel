/*
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
//relCache=2;//just for testing remove it compulsorily

FlushPage(int relNum,unsigned pgid)
{
    int len;
    //assuming file is opened in ab+ mode directly write 
    //printf("insigth flush...");
    fseek(relCache[relNum].relFile,PAGESIZE*pgid,SEEK_SET);
    //fseek(relCache[relNum].relFile,0,SEEK_SET);
    len=fwrite(gPgTable[relNum].contents,1,PAGESIZE,relCache[relNum].relFile);
    printf("\n\nlen:-%d\n\n",len);
    if(len>0)//actually condition shloud be ==PAGESIZE
    {
        fflush(relCache[relNum].relFile);
        relCache[relNum].dirty=0;
        printf("\nflushing old page:-\n\n%s",gPgTable[relNum].contents);
    }
    else{

        printf("\n\nflushpage():->there is problem in flushing the page");
    }
}
*/