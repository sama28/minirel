//test file to check mkdir() ......................
//----------------------------------------------------
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

extern int errno;

int main()
{
int e;
struct stat sb;
char *name = "/home/samadhan/Desktop/customminirel/data/sam1";

e = stat(name, &sb);
printf("e=%d errno=%d\n",e,errno);
if (e == 0)
{
if (sb.st_mode & S_IFDIR)
printf("%s is a directory.\n",name);
if (sb.st_mode & S_IFREG)
printf("%s is a regular file.\n",name);
// etc.
}
else
{
printf("stat failed.\n");
if (errno = ENOENT)
{
printf("The directory does not exist. Creating new directory...\n");
// Add more flags to the mode if necessary.
e = mkdir(name, S_IRWXU);
if (e != 0)
{
printf("mkdir failed; errno=%d\n",errno);
}
else
{
printf("created the directory %s\n",name);
}
}
}
return 0;
}
