    /* 
	This is the basic definition file.
    */
/*************************************************************
		CONSTANTS
*************************************************************/
#include<stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<errno.h>
#define PAGESIZE			2048				/* number of bytes in a page */
#define BITMS_NUM           (PAGESIZE/128)		//assuming a record is atleat 4 byte and one bitmap 												slot is of 4byte 
#define	MAXRECORD			(32*BITMS_NUM) 	//since one bitmap slot can corresponds to 32 records
#define PGTAIL_SPACE		4 				//sapce always left blanks for safety

#define RELNAME		32	/* max length of a relation name */
#define MAXOPEN		20  /* max number of files that can be open
										   at the same time */
										   
//#define MR_MAXBUFPG 1024	

#define MR_PGPERREL 10
#define	OK			0		/* return codes */
#define NOTOK		-1

#define MR_RELCAT_BITMS_NUM   (((PAGESIZE-PGTAIL_SPACE)/(8*MR_RELCAT_REC_SIZE+1))+1) 
#define MR_ATTRCAT_BITMS_NUM  (((PAGESIZE-PGTAIL_SPACE)/(8*MR_ATTRCAT_REC_SIZE+1))+1)
#define MR_RELCAT_REC_SIZE 58
#define	MR_ATTRCAT_REC_SIZE 42
#define MR_RELCATENTRYSIZE 58
#define RELCAT		"relcat"   /* name of the relation catalog file */
#define ATTRCAT		"attrcat"  /* name of the attribute catalog file */

#define NUM_RELCACHE_ENTRY ((PAGESIZE-PGTAIL_SPACE-(BITMS_NUM*sizeof(unsigned int)))/MR_RELCATENTRYSIZE)

//************************************************************
//----------this part depends on where you deploy this project
//***********************************************************
#define HOME_MINIREL "/home/ameya/minirel"//location of project root directory
#define MAX_PATH_LENGTH 255 						//length of absolute address of any file
#define ATTRLEN		32
#define MR_MAX_FILENAME_SIZE 255
//------------------------------------------------------------------------------------------------
//------------bit map slot specific----------------------------------------------------------


/*************************************************************
		TYPE DEFINITIONS 
*************************************************************/

/* Rid structure */
typedef struct recid {
	unsigned 	pid;
	unsigned 	slotnum;
} Rid;

/* Page Structure */
typedef struct ps {
//	unsigned  char slotmap[BITMS_NUM];
	char contents [PAGESIZE];
	} Page;

typedef struct gtps {
		//	unsigned  char slotmap[BITMS_NUM];
			unsigned 	pid;
			unsigned char contents [PAGESIZE+1];
			} GtPage;

typedef struct psrelcat {
		
			unsigned char slotmap[MR_RELCAT_BITMS_NUM];//
			char contents [MAXRECORD];
			} PageRelCat;

typedef struct psattrcat {
				
					unsigned char slotmap[MR_ATTRCAT_BITMS_NUM];//
					char contents [MAXRECORD];
} PageAttrCat;
/*****************************************************************/

typedef struct relList{
	char valid;
	char relName[RELNAME];
	unsigned int recLength;
	unsigned int recPerPg;
	unsigned int numPgs;
	unsigned int numRecs;
	unsigned short numAttrs;
	struct recid Rid;
	FILE *relFile;
	char dirty;
	struct attrList* attrHead;
} relCacheEntry;

typedef struct attrList{
	char attrName[ATTRLEN];
	unsigned length;
	unsigned offset;
	unsigned short type;
	struct attrList* next;
};
/*
struct pageBuffer
{
	int lastptr=0;
	int dirty[MR_PGPERREL];
	unsigned page[MR_PGPERREL];//-1 will show no page
	ps[MR_PGPERREL];
}
*/
/*
struct buffCat
{
	//char relName[RELNAME];//implicitly known
	//unsigned pid;			//implicitly know
	unsigned gtpage;
	int dirty;
};
*/