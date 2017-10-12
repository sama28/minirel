/****************************************************************
		GLOBAL VARIABLE DECLARATIONS
****************************************************************/

//CacheEntry	catcache[MAXOPEN];	/* catalog cache */
//Buffer		buffer[MAXOPEN];        /* buffer pool */
char MR_CURR_DB[MR_MAX_FILENAME_SIZE];
pageBuffer PGBUFF[MR_RELCACHE_SIZE];