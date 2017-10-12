/****************************************************************
		GLOBAL VARIABLE DECLARATIONS
****************************************************************/

//CacheEntry	catcache[MAXOPEN];	/* catalog cache */
//Buffer		buffer[MAXOPEN];        /* buffer pool */
<<<<<<< HEAD
char MR_CURR_DB[MR_MAX_FILENAME_SIZE];
pageBuffer PGBUFF[MR_RELCACHE_SIZE];
=======
;char MR_CURR_DB[MR_MAX_FILENAME_SIZE];
unsigned int relCacheIndex;
struct relList relCache[NUM_RELCACHE_ENTRY];
>>>>>>> ac69ead8193bdd60c61036d82812ee403f44b307
