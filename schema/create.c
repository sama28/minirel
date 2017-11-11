
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
void getSizedBin(int* z,int x,int size){
    for(int i=0;i<8*size;i++)
    z[i]=0;
	int c=size*8-1;
	while(x!=0){
		z[c]=x%2;
		x=x/2;
		c--;
	}
    
}

int doesFileExist(char* path){
    if(access(path,0)==0){
        //printf("File Exists");
        return 1;
    }
    else{
        //printf("File does not exist.");
        return 0;
    }
    }

void bwrite_int(unsigned char* tmp,int num,int size,int* init){
    int binArray[size*8],base,sum;
    getSizedBin(&binArray,num,size);
    /*for(int i=0;i<size;i++){
    	for(int j=0;j<8;j++){
    		printf("%d",binArray[j+i*8]);
    	}
    	printf("\t");
    }*/
    for(int i=size-1;i>-1;i--){
    	sum=0;
    	base=1;
    	for(int j=7;j>-1;j--){
    		sum=sum+binArray[j+i*8]*base;
    		base=base*2;
    	}
    	tmp[*init+size-1-i]=sum;
        //printf("\n%d",sum);
        
    }
    *init+=size;
}

void sanitize(char* a,int num){
    for(int i=0;i<num;i++){
        a[i]=0;
    }
    //for(int i=0;i<num;i++)
     //   printf("%x",a[i]);
}


int isInputValid(int num,char **string){
    //printf("%s",string[0]);
    int valid=1,attrCount=(num-2)/2;
    char seen[num][ATTRLEN];
    //strcpy(seen,string);
    for(int i=0;i<num;i=i+1)
    strcpy(seen[i],string[i]);
    char path[MAX_PATH_LENGTH];
    getPath(path,string[1]);
    if(doesFileExist(path)){
        printf("Relation File Already Exists.\n");
        return;
    }
    if(strlen(string[1])<=RELNAME && !doesFileExist(path)){//Add provision for checking if relation already exists.
        valid=valid & 1;
    }
    else{
        valid=valid & 0;
    }
    for(int i=2;i<num;i=i+2){
        if(strlen(string[i])<=ATTRLEN){
            if((string[i+1][0]=='i' && strlen(string[i+1])) || (string[i+1][0]=='f' && strlen(string[i+1]))){
                valid=valid & 1;
            }
            else if(string[i+1][0]=='S' || string[i+1][0]=='s'){
                if(strlen(string[i+1])<=3 && string[i+1][1]<58 && string[i+1][1]>48 && (string[i+1][2]==0 || (string[i+1][2]>47 && string[i+1][2]<58))){
                    valid=valid & 1;
                }
                else{
                    valid=valid & 0;
                }
            }
            else{
                valid=valid & 0;
            }
        }
        else{
            valid=valid & 0;
        }
    }
    if(valid){
        for(int i=2;i<num;i=i+2){
            for(int j=2;j<num;j=j+2){
                if(i!=j && !strcmp(seen[i],string[j])){
                    valid=valid & 0;
                }
                else{
                    valid=valid & 1;
                }
            }
            //printf("%s\n",seen[i]);
        }
    }
    //printf("%d",strlen(string[2]));
    if(valid){
        FILE* fp=fopen(path,"ab");
        if(fp){
            //printf("ban gayi");
            fclose(fp);
        }
        else if(errno){
            printf("%s",strerror(errno));
        }
    }
    return valid;
}

Create (argc, argv)
int	argc;
char	**argv;
{
    //printf("%d%s\n",argc,argv[1]);
    int attrCount=(argc-2)/2,offset=0,count=0,recLength=0;
    unsigned char record[relCache[1].recLength+1];
    //sanitize(&record,relCache[1].recLength+1);
    struct recidArray RidArray[attrCount];
    //printf("%d\n",attrCount);
    
    if(isInputValid(argc,argv)){
        //printf("Valid");
        GetSlots(&RidArray,attrCount,1);
        struct attrList* tmp=(struct attrList*) malloc(sizeof(struct attrList)) ;
        for(int i=2;i<argc;i=i+2){
            offset=0;
            sanitize(record,relCache[1].recLength+1);
            //printf("rids%d\t%d\n",(RidArray+i)->Rid.slotnum,(RidArray+i)->Rid.pid);
            //printf("%s\t%d\t%d\n",argv[2],strlen(argv[3]),strlen(argv[2]));
    
            for(int k=0;k<strlen(argv[i]);k++){
                record[k]=argv[i][k];
            }
            offset=offset+ATTRLEN;
            
            bwrite_int(record,count,4,&offset);
            
            if(argv[i+1][0]=='i'){
                bwrite_int(record,sizeof(int),sizeof(int),&offset);
                bwrite_int(record,DTINT,sizeof(short),&offset);
                recLength+=sizeof(int);
                //printf("int");
            }
            else if(argv[i+1][0]=='f'){
                bwrite_int(record,sizeof(float),sizeof(float),&offset);
                bwrite_int(record,DTFLOAT,sizeof(short),&offset);
                recLength+=sizeof(float);
                //printf("float");
            }
            else if(argv[i+1][0]=='s'){
                int sum=0;
                if(argv[i+1][2]!=0){
                    sum=argv[i+1][2]-48+10*(argv[i+1][1]-48);
                }
                else{
                    sum=argv[i+1][1]-48;
                }
                bwrite_int(record,sum,sizeof(int),&offset);
                bwrite_int(record,DTSTRING,sizeof(short),&offset);
                recLength+=sum;
                //printf("string");
            }
            //for(int k=0;k<42;k++)
            //printf("%x",record[k]);
            //int t=0;
            //unsigned char q[32];
            //bread_string(record,32,&t,q);
            //int a1=bread_int(record,4,&t);int a2=bread_int(record,4,&t);int a3=bread_int(record,2,&t);
            //printf("\n%s\n%d\n%d\n%d",q,a1,a2,a3);
            //printf("\nrids\t%d\t%d\n",(RidArray+count)->Rid.slotnum,(RidArray+count)->Rid.pid);
            WriteRec(1,record,(RidArray+count));
            
            count++;
        }
        int NUM_SLOTS=(((PAGESIZE-PGTAIL_SPACE)/(8*recLength+1))+1);
        int recPerPg=(PAGESIZE-PGTAIL_SPACE-NUM_SLOTS)/recLength,numPgs=0,numRec=0;
        unsigned char record2[relCache[0].recLength+1];
        //printf("reclen%d\t%d\n",recLength,recPerPg);
        //printf("\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d",argv[1],recLength,recPerPg,numPgs,numRec,attrCount,((RidArray)->Rid.pid),((RidArray)->Rid.slotnum));
        sanitize(record2,relCache[0].recLength);
        offset=0;
        //printf("argv[1]::%s\t%d\n",argv[1],strlen(argv[1]));
        for(int k=0;k<strlen(argv[1]);k++){
            record2[k]=argv[1][k];
        }
        offset=offset+RELNAME;
        bwrite_int(record2,recLength,sizeof(int),&offset);
        bwrite_int(record2,recPerPg,sizeof(int),&offset);
        bwrite_int(record2,numPgs,sizeof(int),&offset);
        bwrite_int(record2,numRec,sizeof(int),&offset);
        bwrite_int(record2,attrCount,sizeof(short),&offset);
        bwrite_int(record2,((RidArray+0)->Rid.pid),sizeof(int),&offset);
        bwrite_int(record2,((RidArray+0)->Rid.slotnum),sizeof(int),&offset);
        //for(int k=0;k<relCache[0].recLength;k++)
       // printf("%x",record2[k]);

        int t=0;
        unsigned char q[32];
        bread_string(record2,32,&t,q);
        //int a1=bread_int(record2,4,&t);int a2=bread_int(record2,4,&t);int a3=bread_int(record2,4,&t);
        //int a4=bread_int(record2,4,&t);int a5=bread_int(record2,2,&t);int a6=bread_int(record2,4,&t);
        //int a7=bread_int(record2,4,&t);
        //printf("\n%s\n%d\n%d\n%d\n%d\n%d\n%d\n%d\n",q,a1,a2,a3,a4,a5,a6,a7);
        InsertRec(0,record2);
        free(tmp);
        printf("Relation Successfully Created\n");
    }
    else{
        printf("Invalid parameters for create function.");
    }

    return;
}


