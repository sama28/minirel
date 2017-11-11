#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <stdio.h>

#include<errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
int returnAttrNode(struct attrList* attrListHead,int i,struct attrList* source){
  struct attrList* tmp,*tmp2;
  int count=0,numNode=0;
  tmp=attrListHead;
  tmp2=attrListHead;
  while(tmp2!=NULL){
    tmp2=tmp2->next;numNode++;
  }
  //printf("numnodes%d\n",numNode);
  while(tmp!=NULL && count<i){
    tmp=tmp->next;count++;
  }
  if(i<numNode){
    strcpy(source->attrName,tmp->attrName);
    source->length=tmp->length;
    source->offset=tmp->offset;
    source->type=tmp->type;
    source->next=tmp->next;
    return 1;
  }
  else{
    return 0;
  }
  
}
int OpenDB (argc, argv)
int	argc;
char	**argv;

{
  
  int flag=1,p;
  struct dirent *dp;
  char path[MAX_PATH_LENGTH],*c;

	strcpy(path,HOME_MINIREL);
	c=strcat(path,"/data/");
	strcpy(path,c);
  DIR *dir= opendir(path);
  if (dir){
    while((dp =readdir(dir))!=NULL && flag){
      if(!(p=strcmp(dp->d_name,argv[1])))
        flag=0;
    }
    if(flag==0){
      strcpy(MR_CURR_DB,argv[1]);
      printf("\n---------------------------------------------------\n");
      printf("Database %s is opened successfully.---------------------\n",MR_CURR_DB);
      printf("\n---------------------------------------------------\n");
      
      OpenCats();
      //unsigned char a[59]="absdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghjklabsdfghj";
      //InsertRec(0,&a);
    }
    else{
      printf("\n---------------------------------------------------\n");
      printf("Database not found.----------------------------------\n");
      printf("\n---------------------------------------------------\n");
    }
  }
  else if (errno){
      printf("\n---------------------------------------------------\n");
      printf("Error opening database.\n%s---------------------------\n",strerror(errno));
      printf("\n---------------------------------------------------\n");
  }
  return 0;
  printf("");
  return 0;
}
