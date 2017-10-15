/*
//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"

ReadPage(int relNum,unsigned pid)
{
    //dependent code that comes here
     if(relNum < relCacheIndex)//relNum is available in relCache)       
        {
            findPgInBuff(relNum,pid)//find pid in PGBUFF[relNum])
            if (pid availble in PGBUFF)
            {
                do nothing..........
            }
            else
            {
                read pid in PGBUFF[relNum];
                replace page if necessary
            */
            /*
            }            }
        }
        else(relNum is not in cache)
        {

        }

 printf("ReadPage \n ");
}

//--------------------------------
//--buffer management related-----
int findPgInBuff(int relNum,unsigned pid )
{
    if(relNum < relCacheIndex)
    {
        
    }
}

*/