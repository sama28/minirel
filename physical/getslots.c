#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"

int isSlotEmpty(unsigned char* buffer,int index,int relNum){
    int slotIndex,slotByte,tmp,slotVal,slotArray[8],NUM_SLOTS=(((PAGESIZE-PGTAIL_SPACE)/(8*relCache[relNum].recLength+1))+1);
    slotByte=index/8;
    slotIndex=index%8;
    tmp=slotByte;
    slotVal=bread_int(buffer,1,&tmp);
    getBinary(&slotArray,slotVal);
    if(slotArray[slotIndex]==0)
        return 1;
    else    
        return 0;
}

GetSlots(struct recidArray* ridArray,int count,int relNum){
    char path[255];
    FILE* fp;
    int slotsDiscovered=0,zeroCount=0,pagenum=0;
    //printf("%d\n",(ridArray+22)->Rid.pid);
    getPath(path,relCache[relNum].relName);
    if(!strcmp("relcat",relCache[relNum].relName)){
        fp=fopen(path,"rb+");
    }
    else if(!strcmp("attrcat",relCache[relNum].relName)){
        fp=fopen(path,"rb+");
    }
    else{
        fp=relCache[relNum].relFile;
    }
    while(!slotsDiscovered){
        //printf("while\n");
        if(pagenum+1>relCache[relNum].numPgs){
            //printf("if new page\n");
            relCache[relNum].numPgs++;
            for(int i=0;i<sizeof(gPgTable[relNum].contents);i++)
                    gPgTable[relNum].contents[i]=0;
                    gPgTable[relNum].pid=pagenum;printf("p\n");
        }
        else{
            //printf("if old page\n");
            fseek(fp,pagenum*PAGESIZE,SEEK_SET);
                fread(&gPgTable[relNum].contents,PAGESIZE,1,fp);
        }
        
        for(int i=0;i<relCache[relNum].recPerPg && zeroCount!=count;i++){
            //printf("%d",isSlotEmpty(gPgTable[relNum].contents,i,relNum));
            if(isSlotEmpty(gPgTable[relNum].contents,i,relNum)){
                (ridArray+zeroCount)->Rid.slotnum=i;
                (ridArray+zeroCount)->Rid.pid=pagenum;
                zeroCount++;
            }
            else{
                zeroCount=0;
            }
            //printf("\t%d\t%d\n",zeroCount,i);
        }
        if(zeroCount!=count){
            pagenum++;
        }
        else{
            slotsDiscovered=1;
        }
    }
    
    //for(int i=0;i<count;i++)
    //printf("a\t%d\t%d\n",(ridArray+i)->Rid.slotnum,(ridArray+i)->Rid.pid);
    return;
}