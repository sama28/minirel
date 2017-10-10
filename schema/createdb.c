//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <stdio.h>
//-------------------------------------
//-----additional header---------
//-------------------------------------
//-----for int mkdir(const char *pathname, mode_t mode); 
#include <sys/stat.h> 
#include <sys/types.h> 
//------------------------------------
//for errno and EEXIST
#include<errno.h>
//-----------------------------------
#include<string.h>
//------------------------------------
int CreateDB (argc, argv)
int	argc;
char	**argv;
{

    /* print command line arguments */
    //short	k;		/* iteration counter	    */
	
	/*    
	printf ("%s:\n", argv[0]);
	    for (k = 1 ; k < argc; ++k)
	printf ("\targv[%d] = %s\n", k, argv[k]);

     printf("CreateDB \n");
	*/
	//-------------------------------------------------------
	//finding path of database directory 
	//path =HOME_MINIREL/data/argv[1]

	char path[MAX_PATH_LENGTH],*c;
	int status;

	strcpy(path,HOME_MINIREL);
	c=strcat(path,"/data/");
	strcpy(path,c);
	c=strcat(path,argv[1]);
	//--------------------------------------------------------
	printf(" \n \npath sent as args is  :-  %s  \n\n",c);
	
	status=mkdir(c,S_IRWXU);
	//printf("\nerrno :- %d",errno);
	//printf("\nstautus is :- %d",status);	
	if(status==-1)
	{
		if(errno==EEXIST)
		{
			printf("\n---------------------------------------------------");
			printf("\nDATABASE ALREADY EXISTS....");
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
		printf("\nDATABASE CREATED......");
		printf("\n---------------------------------------------------\n");	
		//.........................................
		//some work is to be done .......

		//printf("creatcat from create db with arg %s",c); //debug print to debug createcats...
		CreateCats(c);
	}
	
return (OK);  /* all's fine */
}

//-----additional notes--------------
//------------------------------------------
//not completed yet...
//
