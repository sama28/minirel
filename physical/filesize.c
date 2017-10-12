#include <sys/stat.h>
#include<stdio.h>

int fileSize(FILE *fp){
    int prev=ftell(fp);
    fseek(fp, 0L, SEEK_END);
    int size=ftell(fp);
    fseek(fp,prev,SEEK_SET); 
    return size;
}