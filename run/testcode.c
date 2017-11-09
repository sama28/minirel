#include<stdio.h>
#include<string.h>
int main()
{

char a[1024*3];
FILE *fd;
int i ,j;
fd=fopen("/home/samadhan/Desktop/git/minirel/run/tempread.txt","rb+");
/*
char p[]="samadhan";
char q[]="sharma";
printf("%u",&p[0]);
strcpy(p,"whatever");
printf("%s",p);
printf("%u",&p[0]);
*/

if(fd>0)
    {
        char b[1024*3];
        
        for(i=0;i<1024;i++)
        {

            b[i]='a';
        }
        b[1023]='i';
        for(i=1024;i<2048;i++)
        {

            b[i]='b';
        }
        b[2047]='j';
        for(i=2048;i<3*1024;i++)
        {

            b[i]='c';
        }
        fseek(fd,0,SEEK_SET);
        fwrite(b,1024*3,1,fd);
      /*
        char d[1024]="kkkkkkkkkthis is what i really want to test";
        
        fseek(fd,0,SEEK_SET);        
        fwrite(d,1023,1,fd);
        fseek(fd,0,SEEK_SET);
        char ch[]="\0\0\0\0";
        fwrite(ch,4,1,fd);
        /*for (i=0;i<40;i++)
        {
            fputc(d[i],fd);            
        }
        */
        fseek(fd,0,SEEK_SET);
        printf("\n\n\n");
        if(fread(a,3*1024,1,fd)>0)
        {

            //printf("insideread");
        }
      //  a[3*1024]='\0';
      int lwrlm=0;
      int uprlm=3*1024;
      for(i=lwrlm;i<uprlm;i++)//read the content from lower limit to upper limit
      {

        printf("%c",a[i]);
      }

      
       //printf("\n%s",a);

       //printf("\n\n\n%c %c %c",a[1022],a[1023],a[1024]);
        fclose(fd);
    }

}