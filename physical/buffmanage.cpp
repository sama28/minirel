//------cumpulsory header--------------
#include "../include/defs.h"
#include "../include/error.h"
#include "../include/globals.h"
#include <string.h>
//----------------------------
#include <unordered_map>
#include <iostream>
#include <cassert>
using namespace std;
//-------------------------------------
//---------BUFFER MANAGEMENT-----------
//typedef pair<string,unsigned> keypair;
using namespace std;
//typedef pair<char ,struct buffCat> key_val_Pair;
extern "C" int allocatPg(char[] ,unsigned);
unsigned allocatePg(char relName[],unsigned pid)
{
    /*
    unsigned freePage1=512;
    //find 1st free page suppose index of 1st free page in globle table is freePage1;
    buffCat gPgEntry;
    gPgEntry.gtPage=freePage1;
    gPgEntry.dirty=0;
    gPgEntry.pid=pid;
    strcpy(gPgEntry.relName,relName);
    //buffCat
    gtCat.insert(key_val_Pair(relName,gPgEntry));
    print
    printf("the page to which relation %s belong is %u",relName,gtCat[relName]);
*/

//Test Code Not Useful Written By samadhan

unordered_map< string, unordered_map <unsigned,buffCat > > ht;
unordered_map<unsigned,buffCat> htbucket1,htbucket2,*h1,*h2;
buffCat b1,b2;
char c[10]="samadhan",d[10]="samadhan";
string s(c);
b1.gtPage=445;
b1.dirty=0;
b2.gtPage=999;
b2.dirty=1;

htbucket1[4]=b1;
htbucket1[5]=b2;
//h1=&htbucket1;
ht[c]=htbucket1;
//	h2=&htbucket2;
//strcpy(s,c);
//	ht[c]=h1;
//ht[d]=h2;
   printf("content is :-%s  4 %u %d",c,ht[c][4].gtPage,ht[c][4].dirty);
   printf("content is :-%s  4 %u %d",c,ht[c][5].gtPage,ht[c][5].dirty);
}
//initFreePgList()