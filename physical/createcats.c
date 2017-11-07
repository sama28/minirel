//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include "../include/mrdtypes.h"
#include <stdio.h>
//-------------------------------------
//-----additional header---------
//-------------------------------------
//-----for int mkdir(const char *pathname, mode_t mode),lseek(); 
#include <sys/stat.h> 
#include <sys/types.h> 
//------------------------------------
//for errno and EEXIST
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
CreateCats(char *d)//d should be d=HOME/data/[dbname]/data
{	
		//chmod("./testsbit",S_IRWXU| S_ISVTX|O_DIRECTORY);
	
	char path[MAX_PATH_LENGTH],path1[MAX_PATH_LENGTH],*c,*ca,catpath[MAX_PATH_LENGTH];
	int status;
	FILE *fd,*fda;
	long long offset;
	//long int temp;
	//-------------------------------------------------------------------------------------------------------------------------
	//-------------------------------------------------------------------------------------------------------------------------
		//---------------------------CATALOG SPECIFIC--------------------------------------------------------
		//part1 FOR RELCAT
	//-------------------------------------------------------------------------------------------------------------------------

		char relName[RELNAME]="relcat";//size=s[0]
		char readrel[RELNAME];
		unsigned recLength;//size=s[1]
		unsigned recPerPg;//size=s[2]
		unsigned short numAttrs;//size=s[3]
		unsigned numpages;//size=s[8]
		unsigned numrecs;//size=s[9]
		unsigned  s[10];
		int i;
		Rid recid;//s[5]//corresponding to ,the rid of 1st attribute of relcat, in attrCats	
		PageRelCat pg;//s[4]
		PageAttrCat pg1;

		unsigned int bitmssize;

	//size of relcat attribute------------------
		s[0]=sizeof(relName);
		s[1]=sizeof(recLength);
		s[2]=sizeof(recPerPg);
		s[3]=sizeof(numAttrs);
		//--------------------
		s[5]=sizeof(recid.pid);
		s[6]=sizeof(recid.slotnum);
		//--------------------------
		s[8]=sizeof(numpages);
		s[9]=sizeof(numrecs);
	//-------------------------------------------------
		//s[4]=sizeof(pg.slotmap);//bitmap size priveously used
		bitmssize=MR_RELCAT_BITMS_NUM;//(PAGESIZE-PGTAIL_SPACE)/(8*MR_RELCAT_REC_SIZE+1);
		s[4]=bitmssize;
		//----------------this memory space wastage (in array s[7]) could be saved -------
		
		s[7]=sizeof(pg.slotmap[0]);//size of slot of bitmap(which nothing but unsigned char)
	//----------------------------------------------	
		
		//previously used
		//for(int i =0 ;i<bitmssize;i++)
		for(int i =0 ;i<MR_RELCAT_BITMS_NUM;i++) 
		{
			pg.slotmap[i]=0x00;
		}
		pg.slotmap[0]=0xc0;//setting 1st 2 bit of bitmap slot to 1, 
		//for record slot num1 =1(correspnds relcat)
		//for record slot num2 =1(correspnds attrcat)
		printf("\nvalue of 1st bitmap slot is in decimal %d in octal %o",pg.slotmap[0],pg.slotmap[0]);
		numpages=1;
		numrecs=2;//attrcat+relcat
		numAttrs=8;
		recid.pid=0;		//this shows 1st atrribute of this relation(relcat) is at page 0; 
		recid.slotnum=0;	////this shows 1s(t atrribute of this relation(relcat) is at slotnum=0; in attrcat 
		recLength=s[0]+s[1]+s[2]+s[3]+s[5]+s[6]+s[8]+s[9];		
		recPerPg=(PAGESIZE-PGTAIL_SPACE-s[4])/recLength;//s[4]*8;
		//recPerPg=(PAGESIZE - (space intentionally left blank in ech page) -bitmap size)/recLemgth
		
		//printf("\n s0 %d s1 %d s2 %d s3 %d s4 %d recLength = %d,recPerPg=%d",s[0],s[1],s[2],s[3],s[4],recLength,recPerPg);
		//------------------------------------------------------------------------------------------------------------------
		//-------------------------------------------------------------------------------------------------------------------------
		//part2 FOR ATTRCAT
		char name[ATTRLEN];
		unsigned ofst;
		unsigned attrl=ATTRLEN;
		short type;

		unsigned arecLength;
		unsigned arecPerPg;
		unsigned short anumAttrs;
		unsigned  as[8];
		Rid arecid;

		//----------------------------
		as[0]=sizeof(name);
		as[1]=sizeof(ofst);
		as[2]=sizeof(attrl);
		as[3]=sizeof(type);
		//------Storing values, to be write in relcat, for attrcat relation
		strcpy(name,"attrcat");
		anumAttrs=4;
		arecid.pid=0;		//this shows 1st atrribute of this relation(attrcat) is at page 0; 
		arecid.slotnum=numAttrs;	////this shows that 
		//1st atrribute of this relation(attrcat) is at which record in attrcat
		//(in this case it is(8-numAttrs in relcat) beacuse relcat have 8 attribute) 
		arecLength=as[0]+as[1]+as[2]+as[3];//two entries ar 's'	
		arecPerPg=(PAGESIZE-PGTAIL_SPACE-s[4])/arecLength;//note -> s[4]==bitmap size

		//--------------------------------------------------------------------
	
	strcpy(path,d);
	c=strcat(path,"/catalog");//C=HOME_MINIREL/[DB1]/data/catalog
	strcpy(catpath,c);
//--------------------------------------------------------
//	printf(" \n \npath comes in cats is   :-  %s  \n\n",d);
//	printf(" \n \npath in cats is   :-  %s  \n\n",c);
	

	status=mkdir(c,S_IRWXU);//making C=HOME_MINIREL/[DB1]/data/catalog
	//printf("\nerrno :- %d",errno);
	//printf("\nstautus is :- %d",status);	
	if(status==-1)
	{
		if(errno==EEXIST)
		{
			printf("\n---------------------------------------------------");
			printf("\nCATLOG DIRECTORY ALREADY EXISTS....");
			printf("\n---------------------------------------------------\n");
		}
		else
		{
			printf("\n---------------------------------------------------");	
			printf("\nSOMETHING GOES WRONG MAY BE PERMISSION,PATH OR SPACE PROBLEM......");
			printf("\n---------------------------------------------------\n");	
		}
     	}
	else if(status==0)
	{
		printf("\n---------------------------------------------------");	
		printf("\nCATALOG DIRECTORY CREATED......");
		strcpy(path,c);
		strcpy(path1,c);
		c=strcat(path,"/relcat");		
		ca=strcat(path1,"/attrcat");

		printf("%s",ca);
		fd=fopen(c,"rb");//C=HOME_MINIREL/data/[DB1]/data/catalog/relcat
		fda=fopen(ca,"rb");//C=HOME_MINIREL/data/[DB1]/data/catalog/attrcat
		printf("fd=%d fda=%d",fd,fda);
	if(fd<=0 && fda<=0)
	{
			//fd=open(c,O_RDWR|O_CREAT,S_IRWXU);
		fd=fopen(c,"wb+");//C=HOME_MINIREL/[DB1]/data/catalog/relcat
		fda=fopen(ca,"wb+");//C=HOME_MINIREL/[DB1]/data/catalog/attrcat
		//printf(" \n \npath is  :-  %s  \n\n",c);
		if(fd > 0 && fda > 0)
		{
			//----------------------------------------------------------
			//----------WRITTING data of relcat relation TO relcat(its own data to itself)-----
			//relcat(relName,recLength,rcPerPg,numPgs,numRecs,numAttrs,pid,rid)
			
			//fwrite(buf,4*MR_RELCAT_BITMS_NUM,1,fd);
			//printf("file opened successfully fd= %d,offset=%d",fd,offset);//debug code	
					
			//
			//fseek(fd,offset,SEEK_SET); //setting pointer to 1st Record slot (just one byte after bitmap)		
			
			for(int i = 0 ;i<MR_RELCAT_BITMS_NUM;i++)
			{
				fwrite(&(&pg)->slotmap[i],s[7],1,fd);	//writing bitmap
			}

			offset=s[4];//size of bitmapslot;	
			fseek(fd,offset,SEEK_SET); //setting pointer to 1st Record slot (just one byte after bitmap)		
			
			fwrite(relName,s[0],1,fd);
			//fread(readrel,32,1,fd);
			//printf("\n%d data written to file and read back is %s",len,readrel);
		
			fwrite(&recLength,s[1],1,fd);
			//printf("\n%d NOF data written to file reclength   ",len);	
				
			fwrite(&recPerPg,s[2],1,fd);
			fwrite(&numpages,s[8],1,fd);
			fwrite(&numrecs,s[9],1,fd);
			fwrite(&numAttrs,s[3],1,fd);
			fwrite(&(&recid)->pid,s[5],1,fd);
			fwrite(&(&recid)->slotnum,s[6],1,fd);
			
			
			//-----------------------------------------------------
			//printf("file opened successfully fd= %d,offset=%d",fd,offset);//debug code, can be removed
			//----------------------------------------------------------
			//---------- : WRITTING data of attrcat relation TO relcat : -----
			
			fseek(fd,s[4]+recLength,SEEK_SET);//offset to 2nd record slot of page
			
			fwrite(name,s[0],1,fd);
			fwrite(&arecLength,s[1],1,fd);	
			fwrite(&arecPerPg,s[2],1,fd);
			numpages=1;//total pagerequired to store atrributes of relcat+attrcat
			fwrite(&numpages,s[8],1,fd);
			numrecs=12;//total #of atributes in relacat+attrcat
			fwrite(&numrecs,s[9],1,fd);
			fwrite(&anumAttrs,s[3],1,fd);
			fwrite(&(&arecid)->pid,s[5],1,fd);
			fwrite(&(&arecid)->slotnum,s[6],1,fd);
			//-------------------------------------------
			fflush(fd);
			fclose(fd);
			chmod(c,S_IRUSR| S_ISVTX|O_DIRECTORY);
			//chmod(catpath,S_IRUSR|S_IXUSR|O_DIRECTORY);
			
			//.........................................
			//----WRITTING INTO ATRRCAT RELATION------------
			
			//-------------------------------------------------
			//setting bitmapslot
			//since total 12 records are going to be added in the attrcat hence
			//setting the 1st 12 bit of bitmap to 1;  

			pg1.slotmap[0]=0xff;//hex=1111 1111 
			pg1.slotmap[1]=0xf0;//1111 0000
			fwrite(&(&pg)->slotmap[0],s[7],1,fda);	//writing bitmap
			fwrite(&(&pg)->slotmap[1],s[7],1,fda);	//writing bitmap

			for(i = 2 ;i<MR_ATTRCAT_BITMS_NUM;i++)
			{
				pg1.slotmap[i]=0x00;
				fwrite(&(&pg)->slotmap[i],s[7],1,fda);	//writing bitmap
			}

			//--------------------------------------------------
			//writing 1st attribute to attrcat
			//1st 8 attributes are from relcat relation since relcat point attribute pointer point  to pid=0 rid=0 
			//relcat(relName,recLength,rcPerPg,numPgs,numRecs,numAttrs,pid,rid)
			strcpy(name,"relName");
			//printf("\n\n it shoul be relName %s",name);
			ofst=0;
			attrl=7;
			type=DTSTRING;

			//-----------------------------------------------------------
			s[4]=MR_ATTRCAT_BITMS_NUM;//(PAGESIZE-PGTAIL_SPACE)/(8*MR_ATTRCAT_REC_SIZE+1);//bitmap size in byte
			//----------------------------------------------------------------
			fseek(fda,s[4],SEEK_SET);//setting fd to 1st record slot of attrcat
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//second attribute
			strcpy(name,"recLength");
			ofst=1;
			attrl=9;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//3rd attribute
			strcpy(name,"recPerPg");
			ofst=2;
			attrl=8;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+2*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//4th attribute
			strcpy(name,"numPgs");
			ofst=3;
			attrl=6;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+3*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//5th attribute
			strcpy(name,"numRecs");
			ofst=4;
			attrl=7;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+4*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//6th attribute
			strcpy(name,"numAttrs");
			ofst=5;
			attrl=8;
			type=DTUNSIGNED_SHORT;

			fseek(fda,s[4]+5*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//7th attribute
			strcpy(name,"attr0Pid");
			ofst=6;
			attrl=8;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+6*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//8th attribute
			strcpy(name,"attr0Slotnum");
			ofst=7;
			attrl=12;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+7*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//-------------------------------
			//writting attribute of attribute catalog into itsel
			//1st attribute of attrcat
			strcpy(name,"attrName");
			ofst=0;
			attrl=4;
			type=DTSTRING;

			fseek(fda,s[4]+8*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//2nd attribute
			strcpy(name,"offset");
			ofst=1;
			attrl=6;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+9*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);
			
			//3rd attribute
			strcpy(name,"length");
			ofst=2;
			attrl=6;
			type=DTUNSIGNED_INT;

			fseek(fda,s[4]+10*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			//4th attribute
			strcpy(name,"type");
			ofst=3;
			attrl=4;
			type=DTSHORT;

			fseek(fda,s[4]+11*arecLength,SEEK_SET);//adjusting bit boundary;
			fwrite(name,as[0],1,fda);
			fwrite(&ofst,as[1],1,fda);	
			fwrite(&attrl,as[2],1,fda);
			fwrite(&type,as[3],1,fda);

			fflush(fda);
			fclose(fda);
						
			chmod(ca,S_IRUSR| S_ISVTX|O_DIRECTORY);
			chmod(catpath,S_IRUSR|S_IXUSR|O_DIRECTORY);
			
			//changeing the permission so that people can not change catalog

		}
		else
		{
			printf("atleast one catlog can not be created relcat fd %d attrcat fd %d errno %d",fd,fda,errno);//debug code
		}

	}
	else
	{

		printf("\n some catalog already exists...%d %d",fd,fda);
	}		
		
	}
//--------------------------------------------------------------------
}
