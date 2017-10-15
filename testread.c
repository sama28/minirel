#include "include/defs.h"
#include "include/error.h"
#include "include/globals.h"
#include "include/error.h"
#include "include/globals.h"
#include <sys/stat.h> 
//------------------------------------
//for errno and EEXIST
#include <sys/types.h> 
#include<errno.h>
//-----------------------------------
#include<string.h>
//------------------------------------
//for file and directory permission ----------
//for :- open(),chmod(),lseek()
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
 // ssize_t write(int fd, const void *buf, size_t count);
#include <stdio.h>


int main()
{
		PageRelCat pg;
		char path[MAX_PATH_LENGTH]="/home/samadhan/Desktop/git/minirel/data/cat7/catalog/relcat";
		char path1[MAX_PATH_LENGTH]="/home/samadhan/Desktop/git/minirel/data/cat7/catalog/attrcat";
		//chmod(path,S_IRUSR|S_IWUSR|S_IRUSR|S_IXUSR|O_DIRECTORY);			
		chmod(path,S_IRUSR|S_IWUSR| S_ISVTX|O_DIRECTORY);
		chmod(path1,S_IRUSR|S_IWUSR| S_ISVTX|O_DIRECTORY);
		FILE*fd ,*fda; 
		fd =fopen(path,"rb+");
		fda =fopen(path1,"rb+");

		long offset=sizeof(pg.slotmap);
		char c[RELNAME]="reName";
		char d[RELNAME],b[32];
		int atcatofst;
		Rid recid;

		unsigned int e,f,x=5,y=6,nr,np;
		unsigned short g,z=7;
		unsigned char e1;
		int i;
		if(fd>0)
		{
		//	offset=s[4];//size of bitmapslot;		
		/*	fseek(fd,offset,SEEK_SET);
			fwrite(c,32,1,fd);
			fwrite(&x,4,1,fd);
			fwrite(&y,4,1,fd);
			fwrite(&z,2,1,fd);										
		*/		
		//printf("\nfile opened successfully fd= %d,offset=%d",fd,offset);//debug code			
		//	fseek(fd,offset,SEEK_SET);			
			printf("\ndirect 4 byte result");				
			fseek(fd,0,SEEK_SET);
			for(i=0;i<30;i++)
			{
				fread(&e1,1,1,fd);
				printf("\n%x",e1);
			}
			printf("\n------------");
			//fread(b,32,1,fd);
			
			printf("\nbitmap of relcat");
			fseek(fd,0,SEEK_SET);
			for(i=0;i<MR_RELCAT_BITMS_NUM;i++)
			{
				fread(&e1,1,1,fd);
				printf("\n%x",e1);
			}

			fread(d,32,1,fd);
			fread(&e,4,1,fd);
			fread(&f,4,1,fd);
			fread(&np,4,1,fd);
			fread(&nr,4,1,fd);
			fread(&g,2,1,fd);
			fread(&(&recid)->pid,4,1,fd);
			fread(&(&recid)->slotnum,4,1,fd);

			
			//printf("\nfile opened successfully fd= %d,offset=%d",fd,offset);//debug code, can be removed
			
			printf("/ndataread is:-%s %u %u %u %u %u %u %u\n",d,e,f,g,np,nr,recid.pid,recid.slotnum);


			fread(d,32,1,fd);
			fread(&e,4,1,fd);
			fread(&f,4,1,fd);
		
			fread(&np,4,1,fd);
			fread(&nr,4,1,fd);
			fread(&g,2,1,fd);
			fread(&(&recid)->pid,4,1,fd);
			fread(&(&recid)->slotnum,4,1,fd);
			printf("/ndataread is:-%s %u %u %u %u %u %u %u\n",d,e,f,g,np,nr,recid.pid,recid.slotnum);
			//------------------------------------------------------------------------------------
			printf("\n Bitmap of attrcat is :- ");
			//reading attribute catalog
			//---------------------------
			/*fseek(fda,0,SEEK_SET);
			for(i=0;i<48;i++)
			{
				fread(&e,4,1,fda);
				printf("\n%x",e);
			}
			//-----------------------
			*/
			fseek(fda,0,SEEK_SET);
				for(i=0;i<MR_ATTRCAT_BITMS_NUM;i++)
				{
					fread(&e1,1,1,fda);
					printf("\n%x",e1);
				}
				atcatofst=(PAGESIZE-PGTAIL_SPACE)/(8*MR_ATTRCAT_REC_SIZE+1);
				fseek(fda,atcatofst,SEEK_SET);
			for(i=0;i<12;i++)
			{
										
				fread(d,32,1,fda);
				fread(&e,4,1,fda);
				fread(&f,4,1,fda);
				fread(&g,2,1,fda);
				printf("/nattrcat dataread is:-%s %u %u %u\n",d,e,f,g);
			}
			

		}
		else
		{
			printf("file can not be oppened fd %d errno %d",fd,errno);//debug code	
		}


}
