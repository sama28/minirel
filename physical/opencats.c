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

void cachePopulate2(FILE* relcatFile, FILE* attrcatFile){
    int size=fileSize(relcatFile);

    relCacheIndex++;
    fseek(relcatFile,0,SEEK_SET);
    fseek(relcatFile,BITMS_NUM*sizeof(unsigned int),SEEK_SET);
    fseek(relcatFile,58*2,SEEK_CUR);

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
        fseek(attrcatFile,BITMS_NUM*sizeof(unsigned int),SEEK_CUR);//Skip bitmap slot.
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

void cachePopulate1(FILE* relcatFile, FILE* attrcatFile){
    

    fseek(relcatFile,BITMS_NUM*sizeof(unsigned int),SEEK_CUR);
    fread(&relCache[relCacheIndex].relName,32,1,relcatFile);
    fread(&relCache[relCacheIndex].recLength,4,1,relcatFile);
    fread(&relCache[relCacheIndex].recPerPg,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numPgs,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numRecs,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numAttrs,2,1,relcatFile);
    struct recid attrcatRid;
    fread(&attrcatRid.pid,4,1,relcatFile);//(attr0pid)pageid of page in attrcat havinf relation's first attr entry
    fread(&attrcatRid.slotnum,4,1,relcatFile);//(attr0rid)recid in attrcat having relation's first attr entry
    relCache[relCacheIndex].Rid.pid=0;
    relCache[relCacheIndex].Rid.slotnum=0;
    relCache[relCacheIndex].relFile=relcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].attrHead=NULL;
    printf("%s\n%x\n%x\n%x\n%x\n%x\n%x\n%x\n%c\n",relCache[0].relName,relCache[0].recLength,relCache[0].recPerPg,relCache[0].numPgs,relCache[0].numRecs,relCache[0].numAttrs,relCache[0].Rid.pid,relCache[0].Rid.slotnum,relCache[0].dirty);
    

    relCacheIndex++;
    fread(&relCache[relCacheIndex].relName,32,1,relcatFile);
    fread(&relCache[relCacheIndex].recLength,4,1,relcatFile);
    fread(&relCache[relCacheIndex].recPerPg,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numPgs,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numRecs,4,1,relcatFile);
    fread(&relCache[relCacheIndex].numAttrs,2,1,relcatFile);
    relCache[relCacheIndex].Rid.pid=1;
    relCache[relCacheIndex].Rid.slotnum=1;
    relCache[relCacheIndex].relFile=relcatFile;
    relCache[relCacheIndex].dirty='c';
    relCache[relCacheIndex].attrHead=NULL;
    printf("%s\n%x\n%x\n%x\n%x\n%x\n%x\n%x\n%c\n",relCache[1].relName,relCache[1].recLength,relCache[1].recPerPg,relCache[1].numPgs,relCache[1].numRecs,relCache[1].numAttrs,relCache[1].Rid.pid,relCache[1].Rid.slotnum,relCache[1].dirty);

    fseek(attrcatFile,attrcatRid.pid*PAGESIZE,SEEK_SET);//Seek to page where first attribute entry for relation is present.
    fseek(attrcatFile,BITMS_NUM*sizeof(unsigned int),SEEK_CUR);//Skip bitmap slot.
    fseek(attrcatFile,attrcatRid.slotnum*relCache[1].recLength,SEEK_CUR);
    int size=fileSize(attrcatFile);
    
    unsigned int offset,length;
    unsigned short type;
    char attrName[ATTRLEN];
    for(int j=0;j<2;j++){
        struct attrList* attrListHead=NULL;
        for(int i=0;i<relCache[j].numAttrs;i++){
            fread(&attrName,32,1,attrcatFile);
            fread(&offset,4,1,attrcatFile);
            fread(&length,4,1,attrcatFile);
            fread(&type,2,1,attrcatFile);
            printf("/nattrcat dataread is:-%s %u %u %u\n",attrName,offset,length,type);
            attrListHead=addAttrListNode(attrListHead,attrName,offset,length,type);
        }
        relCache[j].attrHead=attrListHead;
    }
    
    
    for(int j=0;j<2;j++){
    for(int i=0;i<relCache[j].numAttrs;i++){
        struct attrList* tmp=returnAttrNode(relCache[j].attrHead,i);
        printf("%s %u %u %u\n",tmp->attrName,tmp->offset,tmp->length,tmp->type);
    }printf("\n");}

    fread(&attrcatRid.pid,4,1,relcatFile);
    fread(&attrcatRid.slotnum,4,1,relcatFile);
    cachePopulate2(relcatFile,attrcatFile);
}

OpenCats()
{
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
