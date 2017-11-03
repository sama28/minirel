//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include <string.h>
#include <stdio.h>
//---------test stub---------------
int relCacheIndex=2;//temporary definition remove it ones ameya changes reflect
int relNum =1;
//int pid=1;
int pid=0;
relCacheEntry relCache[10];

main()
{
//-------------------------------------
//temporary---
char tempbuff[3*PAGESIZE];

//-------------------------------------------
int i;
gPgTable[relNum].pid=1;
relCache[1].dirty=0;

//gPgTable[relNum].pid=0;
//relCache[1].dirty=1;
strcpy(gPgTable[relNum].contents,"this is the old content of page 1 of relation relnum 1");
relCache[1].valid=1;
relCache[1].relFile=fopen("/home/samadhan/Desktop/git/minirel/run/tempread.txt","rb+");
if(relCache[1].relFile<=0)
{
    printf("unble to upen stub file for wb+");
}

/*`
relCache[1].relName[RELNAME];
relCache[1].recLength;
relCache[1].recPerPg;
relCache[1].numPgs;
relCache[1].numRecs;
relCache[1].numAttrs;
relCache[1].Rid;
relCache[1].relFile;
relCache[1].dirty=0;
relCache[1].attrHead;
*/
//----------------------------------------




//----------------------------code part-----------------------------

    //dependent code that comes here
     if(relNum < relCacheIndex && relCache[relNum].valid==1)//relNum is available in relCache)       
        {
            
           // findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
            if (!isPgInBuff(relNum,pid))//pid is not availble in PGBUFF)
            {
                //check whether dirty bit is set or not
                if(relCache[relNum].dirty == 1)
                {
                    //insure that file is open in wb+ mode
                    //write the dirty page to the disk 1st
                    FlushPage(relNum,gPgTable[relNum].pid);
                }
                //read pid in gPgTable[relNum];
                //replace page if necessary
    
                //-------------------------------------------
                //--------------------------------------------
                //fseek(relCache[relNum].relFile,0,SEEK_SET);
                fseek(relCache[relNum].relFile,PAGESIZE*pid,SEEK_SET);
                //if(fread(tempbuff,1,PAGESIZE*3,relCache[relNum].relFile) > 0)
                if(fread(gPgTable[relNum].contents,PAGESIZE,1,relCache[relNum].relFile) > 0)
                {
                    gPgTable[relNum].pid=pid;
                    //printf("%s",gPgTable[relNum].contents);
                    
                  int lwrlm=0;
                  int uprlm=PAGESIZE;
                  printf("\n\ncontent of the whole file\n");
                  for(i=lwrlm;i<uprlm;i++)//read the content from lower limit to upper limit
                  {
                    printf("%c",gPgTable[relNum].contents[i]);
                    //printf("%c",tempbuff[i]);
                  }
                }
                else{

                    printf("\nunable to read........");
                }
           
            }        
        }
        else//(relNum is not in cache)
        {
            printf("\n\nOPEN THE RELATION FIRST.....");
        }
fclose(relCache[relNum].relFile);
// printf("ReadPage \n ");
}


//--------------------------------
//--buffer management related-----
//--prototypical instances and implementation so that buffermanagement could be decoupeled 
//--from design perspective
int isPgInBuff(int relNum,unsigned pgid )
{
    if(relNum < relCacheIndex && relCache[relNum].valid==1)
    {
        if(gPgTable[relNum].pid==pgid)
        {
            printf("page in gt");
            
            return 1;
        }
        
    }
    printf("page not in gt");
    
    return 0;
}

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