#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"

void getBinary(unsigned int* z,unsigned int x){
    for(int i=0;i<8;i++)
    z[i]=0;
	int c=7;
	while(x!=0){
		z[c]=x%2;
		x=x/2;
		c--;
	}
}

int getDecimal(unsigned int* z){
    int sum=0,base=1;
    for (int i=7;i>-1;i--){
        sum=sum+base*z[i];
        base=base*2;
    }
    return sum;
}

void getPath(char* path,char* filename){
    char* c;
    if(!strcmp("relcat",filename) || !strcmp("attrcat",filename)){
        strcpy(path,HOME_MINIREL);
        c=strcat(path,"/data/");
        c=strcat(c,MR_CURR_DB);
        c=strcat(c,"/catalog/");
        c=strcat(c,filename);
        strcpy(path,c);
        chmod(path,S_IWUSR|S_IRUSR);
    }
    else{
        strcpy(path,HOME_MINIREL);
        c=strcat(path,"/data/");
        c=strcat(c,MR_CURR_DB);
        c=strcat(c,filename);
        strcpy(path,c);
    }
}

InsertRec(int relNum, char *rec){
    //printf("reached insertrec");
    FILE *fp;
    char path[255];
    getPath(path,relCache[relNum].relName);
    if(!strcmp("relcat",relCache[relNum].relName) || !strcmp("attrcat",relCache[relNum].relName)){
        fp=fopen(path,"rb+");
    }
    else{
        fp=relCache[relNum].relFile;
    }
    
    if(fp!=NULL){
        
        unsigned int slotArray[8];
        unsigned char write_this_slot;
        int isNewpage=0,maxRec,newslot,offset=0,slotIndex=-1,slot,tmp=0,slotFound=0,NUM_SLOTS=(((PAGESIZE-PGTAIL_SPACE)/(8*relCache[relNum].recLength+1))+1) ,pagenum=0,slotByte=-1;
        maxRec=(PAGESIZE-PGTAIL_SPACE-NUM_SLOTS)/relCache[relNum].recLength;
        //printf("max%d\n",maxRec);
        
        while(!slotFound){
            //printf("while\t%d\n",pagenum);
            //printf("%d\n",relCache[relNum].numPgs);
            slotIndex=-1;
            slotByte=-1;
            if(pagenum+1>relCache[relNum].numPgs){
                relCache[relNum].numPgs++;
                isNewpage=1;
                for(int i=0;i<sizeof(gPgTable[relNum].contents);i++)
                gPgTable[relNum].contents[i]=0;
                gPgTable[relNum].pid=relCache[relNum].numPgs;   
            }
            else{
                fseek(fp,PAGESIZE*pagenum,SEEK_SET);
                fread(&gPgTable[relNum].contents,PAGESIZE,1,fp);
                gPgTable[relNum].pid=pagenum;   
            }
            //for(int j=0;j<PAGESIZE;j++)
            //printf("%02x",gPgTable[relNum].contents[j]);
            for (int i=0;i<NUM_SLOTS && slotByte==-1;i++){
                //printf("for1\t%d\n",slotByte);
                if(gPgTable[relNum].contents[i]!=0xff){
                    slotByte=i;   
                    tmp=slotByte;
                    slot=bread_int(&gPgTable[relNum].contents,1,&tmp);
                    getBinary(&slotArray,slot);
                    //printf("%d",slot);
                    for(int j=0;j<8 && slotIndex==-1;j++){
                        //makeprintf("for2%d\n",slotByte*8+j);
                        if(slotArray[j]==0){
                            //printf("if1\n");
                            if(slotByte*8+j<maxRec){
                                //printf("else2\n");
                                slotIndex=j;
                                slotArray[j]=1;
                                slotFound=1;
                            }
                        }
                        else{
                            //printf("else1");
                        }
                    }
                }
            }
            if(!slotFound){
                pagenum++;
            }
        }
        
        //printf("pagenum%d\nslotByte%d\nslotIndex%d\nrecordnum%d\n",pagenum,slotByte,slotIndex,slotByte*8+slotIndex);
        
        //printf("%d\n",NUM_SLOTS+(slotByte*8+slotIndex)*relCache[relNum].recLength);
        newslot=getDecimal(&slotArray);
        write_this_slot=newslot;
        //printf("%d",write_this_slot);
        offset=slotByte;
        gPgTable[relNum].contents[slotByte]=write_this_slot;
        offset=NUM_SLOTS+(slotByte*8+slotIndex)*relCache[relNum].recLength;
        //printf("offset%d\n",offset+relCache[relNum].recLength);
        for(int i=0;i<relCache[relNum].recLength;i++)
            gPgTable[relNum].contents[i+offset]=rec[i];
        //for(int j=0;j<PAGESIZE;j++)
        //printf("%x",gPgTable[relNum].contents[j]);
        //printf("pagenum%d",pagenum);
        
        fseek(fp,PAGESIZE*pagenum,SEEK_SET);
        fwrite(&gPgTable[relNum].contents,PAGESIZE,1,fp);
        if(isNewpage){
            //relCache[relNum].numPgs++;
        }
        fflush(fp);
        if(!strcmp("relcat",relCache[relNum].relName) || !strcmp("attrcat",relCache[relNum].relName)){
            struct stat st;
            mode_t mode;
            stat(path, &st);
            mode = st.st_mode & 07777;
        
            // modify mode
            mode &= ~(S_IWUSR);    // Clear this bit 
            chmod(path, mode);
        }
        
    }
    else if(errno){
        printf("%s",strerror(errno));
    }
}