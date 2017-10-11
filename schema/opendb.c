#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <stdio.h>

#include<errno.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

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
      printf("\n---------------------------------------------------\n");OpenCats();

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
}
