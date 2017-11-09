/****************************************************************
		GLOBAL VARIABLE DECLARATIONS
****************************************************************/

//CacheEntry	catcache[MAXOPEN];	/* catalog cache */
//Buffer		buffer[MAXOPEN];        /* buffer pool */
char MR_CURR_DB[MR_MAX_FILENAME_SIZE];
//pageBuffer PGBUFF[MR_RELCACHE_SIZE];
//pageNode gPgTable[MR_MAXBUFPG];
//buffCat gtCat[MR_MAXBUFPG];
GtPage gPgTable[MAXOPEN];//global page table just collection of pages
//hash_map<std::string,int> gtCat;
//unordered_map< string,unordered_map<unsigned,buffCat> > gtCat;
unsigned int relCacheIndex;
struct relList relCache[NUM_RELCACHE_ENTRY];
unsigned char relcat_page[PAGESIZE+1];
unsigned char attrcat_page[PAGESIZE+1];