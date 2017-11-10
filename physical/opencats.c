#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>

struct attrList* addAttrListNode(struct attrList* attrListHead,char attrName[],unsigned offset,unsigned length,unsigned short type){
    struct attrList* new;
    new=(struct attrList*) malloc(sizeof(struct attrList));
    strcpy(new->attrName,attrName);
    new->offset=offset;
    new->length=length;
    new->type=type;
    new->next=NULL;
    if(attrListHead==NULL){
        attrListHead=new;
    }   
    else{
        struct attrList* tmp;
        tmp=attrListHead;
        while(tmp->next!=NULL){
            tmp=tmp->next;
          }
          tmp->next=new;
          new->next=NULL;
    }
    return attrListHead;
}

struct attrList* returnAttrNode(struct attrList* attrListHead,int i){
    struct attrList* tmp;
    int count=0;
    tmp=attrListHead;
    while(tmp!=NULL && count<i){
      tmp=tmp->next;count++;
    }
    return tmp;
}
/*
void cachePopulate2(FILE* relcatFile, FILE* attrcatFile){
    int size=fileSize(relcatFile);

    relCacheIndex++;
    fseek(relcatFile,0,SEEK_SET);
    fseek(relcatFile,MR_RELCAT_BITMS_NUM,SEEK_SET);
    fseek(relcatFile,MR_RELCAT_REC_SIZE*2,SEEK_CUR);

    fseek(attrcatFile,0,SEEK_SET);
    int smallFile=(size-PGTAIL_SPACE-BITMS_NUM*sizeof(unsigned int))/sizeof(struct relList);
    if(smallFile>NUM_RELCACHE_ENTRY)
        smallFile=NUM_RELCACHE_ENTRY;
    for(int i=relCacheIndex;i<smallFile;i++){
        fread(&relCache[i].relName,32,1,relcatFile);
        fread(&relCache[i].recLength,4,1,relcatFile);
        fread(&relCache[i].recPerPg,4,1,relcatFile);
        fread(&relCache[i].numPgs,4,1,relcatFile);
        fread(&relCache[i].numRecs,4,1,relcatFile);
        fread(&relCache[i].numAttrs,2,1,relcatFile);
        struct recid attrcatRid;
        fread(&attrcatRid.pid,4,1,relcatFile);//(attr0pid)pageid of page in attrcat havinf relation's first attr entry
        fread(&attrcatRid.slotnum,4,1,relcatFile);//(attr0rid)recid in attrcat having relation's first attr entry
        
        fseek(attrcatFile,attrcatRid.pid*PAGESIZE,SEEK_SET);//Seek to page where first attribute entry for relation is present.
        fseek(attrcatFile,MR_ATTRCAT_BITMS_NUM,SEEK_CUR);//Skip bitmap slot.
        fseek(attrcatFile,attrcatRid.slotnum*relCache[1].recLength,SEEK_CUR);

        unsigned int offset,length;
        unsigned short type;
        char attrName[ATTRLEN];
        struct attrList* attrListHead=NULL;int j=0;
        for(int i=0;i<relCache[j].numAttrs;i++){
            fread(&attrName,32,1,attrcatFile);
            fread(&offset,4,1,attrcatFile);
            fread(&length,4,1,attrcatFile);
            fread(&type,2,1,attrcatFile);
            printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            attrListHead=addAttrListNode(attrListHead,attrName,offset,length,type);j++;
        }
        relCache[j].attrHead=attrListHead;
        relCache[i].Rid.pid=0;
        relCache[i].Rid.slotnum=0;
        relCache[i].relFile=relcatFile;
        relCache[i].dirty='c';
        
    }
    printf("%d",relCacheIndex);
}
*/


void cachePopulate1(FILE* relcatFile, FILE* attrcatFile){
    int relcat_index=0,attrcat_index=0,fsize=fileSize(relcatFile);;
    unsigned int offset,length;
    unsigned short type;
    char attrName[ATTRLEN];
    fread(&relcat_page,sizeof(relcat_page),1,relcatFile);
    //for (int i=0;i<PAGESIZE/4;i++)
      //  printf("%x",relcat_page[i]);
    relcat_index+=MR_RELCAT_BITMS_NUM;
    unsigned char tmp[32];
    bread_string(relcat_page,32,&relcat_index,tmp);
    strcpy(relCache[relCacheIndex].relName,tmp);
    relCache[relCacheIndex].recLength=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].recPerPg=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numPgs=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numRecs=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numAttrs=bread_int(relcat_page,2,&relcat_index);
    struct recid attrcatRid;
    attrcatRid.pid=bread_int(relcat_page,4,&relcat_index);
    attrcatRid.slotnum=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].Rid.pid=0;
    relCache[relCacheIndex].Rid.slotnum=0;
    relCache[relCacheIndex].relFile=relcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].attrHead=NULL;
    relCache[relCacheIndex].valid='v';
    printf("%s\n%x\n%x\n%x\n%x\n%x\n%x\n%x\n%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty);
    relCacheIndex++;

    bread_string(relcat_page,32,&relcat_index,tmp);
    strcpy(relCache[relCacheIndex].relName,tmp);
    relCache[relCacheIndex].recLength=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].recPerPg=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numPgs=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numRecs=bread_int(relcat_page,4,&relcat_index);
    relCache[relCacheIndex].numAttrs=bread_int(relcat_page,2,&relcat_index);
    relCache[relCacheIndex].Rid.pid=0;
    relCache[relCacheIndex].Rid.slotnum=1;
    relCache[relCacheIndex].relFile=attrcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].attrHead=NULL;
    relCache[relCacheIndex].valid='v';
    printf("%s\n%x\n%x\n%x\n%x\n%x\n%x\n%x\n%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty);
    
    fseek(attrcatFile,attrcatRid.pid*PAGESIZE,SEEK_SET);
    fread(&attrcat_page,sizeof(attrcat_page),1,attrcatFile);
    attrcat_index+=MR_ATTRCAT_BITMS_NUM+attrcatRid.slotnum*relCache[1].recLength;
    for(int j=0;j<2;j++){
        struct attrList* attrListHead=NULL;
        for(int i=0;i<relCache[j].numAttrs;i++){
            bread_string(attrcat_page,32,&attrcat_index,tmp);
            strcpy(attrName,tmp);
            offset=bread_int(attrcat_page,4,&attrcat_index);
            length=bread_int(attrcat_page,4,&attrcat_index);
            type=bread_int(attrcat_page,2,&attrcat_index);
            printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            attrListHead=addAttrListNode(attrListHead,attrName,offset,length,type);
        }
        relCache[j].attrHead=attrListHead;
    }
    /* Print attribute cache
    for(int j=0;j<2;j++){
        for(int i=0;i<relCache[j].numAttrs;i++){
            struct attrList* tmp=returnAttrNode(relCache[j].attrHead,i);
            printf("%s %u %u %u\n",tmp->attrName,tmp->offset,tmp->length,tmp->type);
        }printf("\n");}
    */
    attrcatRid.pid=bread_int(relcat_page,4,&relcat_index);
    attrcatRid.slotnum=bread_int(relcat_page,4,&relcat_index);
    printf("%d",relcat_index);
    int howmuchRec;
    if(relCache[0].numRecs<20){
        howmuchRec=relCache[0].numRecs;
    }
    else{
        howmuchRec=20;
    }
    for (int i=relCacheIndex;i<howmuchRec;i++){
        relCacheIndex++;
        bread_string(relcat_page,32,&relcat_index,tmp);
        strcpy(relCache[relCacheIndex].relName,tmp);
        relCache[relCacheIndex].recLength=bread_int(relcat_page,4,&relcat_index);
        relCache[relCacheIndex].recPerPg=bread_int(relcat_page,4,&relcat_index);
        relCache[relCacheIndex].numPgs=bread_int(relcat_page,4,&relcat_index);
        relCache[relCacheIndex].numRecs=bread_int(relcat_page,4,&relcat_index);
        relCache[relCacheIndex].numAttrs=bread_int(relcat_page,2,&relcat_index);
        attrcatRid.pid=bread_int(relcat_page,4,&relcat_index);
        attrcatRid.slotnum=bread_int(relcat_page,4,&relcat_index);
        attrcat_index=0;
        attrcat_index+=attrcatRid.pid*PAGESIZE;
        fread(&attrcat_page,sizeof(attrcat_page),1,attrcatFile);
        attrcat_index+=MR_ATTRCAT_BITMS_NUM+attrcatRid.slotnum*relCache[1].recLength;
        struct attrList* attrListHead=NULL;
        for(int j=0;j<relCache[relCacheIndex].numAttrs;i++){
            bread_string(attrcat_page,32,&attrcat_index,tmp);
            strcpy(attrName,tmp);
            offset=bread_int(attrcat_page,4,&attrcat_index);
            length=bread_int(attrcat_page,4,&attrcat_index);
            type=bread_int(attrcat_page,2,&attrcat_index);
            //printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            attrListHead=addAttrListNode(attrListHead,attrName,offset,length,type);
        }
        relCache[i].attrHead=attrListHead;
        relCache[i].Rid.pid=0;
        relCache[i].Rid.slotnum=0;
        relCache[i].relFile=NULL;
        relCache[i].dirty='c';
        relCache[relCacheIndex].valid='i';
    }
}

OpenCats()
{
    printf("reached opencat");
    int flag=1,size; 
    unsigned int slot_buffer[BITMS_NUM*sizeof(unsigned int)],*buffer;
    char path1[MAX_PATH_LENGTH],*c,*d,path2[MAX_PATH_LENGTH];
    

    strcpy(path1,HOME_MINIREL);
    strcpy(path2,HOME_MINIREL);
    c=strcat(path1,"/data/");
    d=strcat(path2,"/data/");
    c=strcat(c,MR_CURR_DB);
    d=strcat(d,MR_CURR_DB);
    c=strcat(c,"/catalog/relcat");
    d=strcat(d,"/catalog/attrcat");
    FILE *relcatFile=fopen(c,"rb");
    FILE *attrcatFile=fopen(d,"rb");
    if(relcatFile!=NULL && attrcatFile!=NULL){
        /*buffer=(unsigned int*)malloc(PAGESIZE/sizeof(unsigned int));
        fread(buffer,sizeof(unsigned int),PAGESIZE/sizeof(unsigned int),relcatFile);
        size=fileSize(relcatFile);
        for(int i=0;i<size/(sizeof(unsigned int));i++)
            printf("%x\n",buffer[i]);*/
        
        //fread(&slot_buffer,BITMS_NUM*sizeof(unsigned int),1,relcatFile);
        
        cachePopulate1(relcatFile,attrcatFile);
    }
    else if(errno){
        printf("%s",strerror(errno));
    }
    
    /*if (dir){
        while((dp =readdir(dir))!=NULL && flag){
          if(!(p=strcmp(dp->d_name,argv[1])))
            flag=0;
        }
        
      }
      else if (errno){
          printf("\n---------------------------------------------------\n");
          printf("Error opening database.\n%s---------------------------\n",strerror(errno));
          printf("\n---------------------------------------------------\n");
      }*/
}