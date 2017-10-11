    /* 
	This is the basic definition file.
    */
/*************************************************************
		CONSTANTS
*************************************************************/
#include<stdio.h>
#define PAGESIZE			2048				/* number of bytes in a page */
#define BITMS_NUM           (PAGESIZE/128)		//assuming a record is atleat 4 byte and one bitmap 												slot is of 4byte 
#define	MAXRECORD			(32*BITMS_NUM) 	//since one bitmap slot can corresponds to 32 records
#define   PGTAIL_SPACE		4 				//sapce always left blanks for safety

#define RELNAME		32	/* max length of a relation name */
#define MAXOPEN		20  	/* max number of files that can be open
                                   		at the same time */

#define	OK			0		/* return codes */
#define NOTOK		-1

#define RELCAT		"relcat"   /* name of the relation catalog file */
#define ATTRCAT		"attrcat"  /* name of the attribute catalog file */
#define MR_RELCACHE_SIZE (2*PAGESIZE/sizeof(struct relList))

//************************************************************
//----------this part depends on where you deploy this project
//***********************************************************
#define HOME_MINIREL "/home/ameya/Desktop/minirel"//location of project root directory
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

	unsigned slotmap[BITMS_NUM];
	char contents [MAXRECORD];
	} Page;

/*****************************************************************/

typedef struct relList{
	char valid;
	char relName[RELNAME];
	unsigned int recLength;
	unsigned int recPerPg;
	unsigned int numPgs;
	unsigned int numRecs;
	unsigned short numAttrs;
	FILE *relFile;
	char dirty;
	struct attrList* attrHead;
}relCacheEntry;

typedef struct attrList{
	char attrName[ATTRLEN];
	unsigned length;
	unsigned short type;
	struct attrList* next;
}