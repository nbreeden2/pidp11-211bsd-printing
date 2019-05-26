#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define  PHEADER      80   /* total characters in the page header */
#define  LINESPERPAGE 56   /* We're printing 56 lines per page    */

char  ph[PHEADER];         /* Holds the page number and filename  */

/**************************************************************************/
void initHeader(fname,pn)  /* prepare the text for the header row in ph[] */
char *fname;
int  pn;
{
  int i;
  for (i=0; i<(PHEADER); ++i)  /* clear the header row to spaces */
  {
    ph[i]=' ';  
  }
  for (i=0; i<strlen(fname);i++)  /* Add the file name somewhat centered */
  {
    ph[40-(strlen(fname)-2)+i] = fname[i];
  }
  sprintf(&ph[PHEADER-9],"Page:%3d",pn);  /* Add the page number */
}

/**************************************************************************/
void printcr(cdos) /* Are we add an <cr> for each <lf> found? */
short cdos;
{
  if (cdos)        /* Adding <cr> to the output stream */
  {
    printf("\r");  /* Actually print the <cr>          */
  }
}

/**************************************************************************/
short indentLine(idnt)  /* Are we indenting the lines? */
short idnt;
{
  if (idnt)             /* Are we indenting the lines? */
  {
    printf("        "); /* Indent the line 8 spaces    */
    return(8);
  }
  return(0);
}

/**************************************************************************/
short lineNumbers(plns,ln) /* Are we printing line numbers?             */
short plns;
int   *ln;
{
  if (plns)                /* Are we printing line numbers?             */
  {
    printf("%5d ",*ln);    /* Send the line number to the output stream */
    *ln = *ln + 1;
    return(6);
  }
  return(0);
}

/**************************************************************************/
void checkHeader(plc,pn,fname,cdos)  /* Time to print the page header? */
short *plc;   /* Count of lines printed to the page    */
int   *pn;    /* The current page number               */
char  *fname; /* The name of the file we're processing */
short cdos;   /* Are we converting to DOS format       */
{
  if (*plc == LINESPERPAGE)
  {
    *plc=0;                 /* Reset the page line count                   */
    *pn = *pn + 1;          /* Increment the page number                   */
    printf("\f");           /* Send a Formfeed so we go onto the next page */
    initHeader(fname,*pn);  /* Format the header text                      */
    printf("%s\n",ph);      /*  and print it                               */
    printcr(cdos);          /* Adding <cr> to the output stream?           */
    printf("\n");           /* Add a blank line after the header row       */
    printcr(cdos);          /* Adding <cr> to the output stream?           */
  }
}

/**************************************************************************/
int main(argc, argv) /* Here we go.... */
int argc;
char *argv[];
{
   int   c;          /* The character being read from the input file  */
   int   cc   = 0;   /* Characters printed in to row so we can wrap   */
   int   i;          /* General purpose integer variable              */
   short plns = 0;   /* -l Print line number from 1..99999            */
   int   ln   = 1;   /*    Current line number being printed          */
   short cdos = 1;   /* -u Don't convert to DOS format                */
   short idnt = 0;   /* -i Indent the file 8 spaces                   */
   short pnum = 0;   /* -p Print with file name and page numbers      */
   int   pn   = 1;   /*    Current page being printed                 */
   short plc  = 0;   /*    Number of lines printed on a page          */
   FILE  *fptr;
   
   if (argc > 1)  /* Then at least one command line parm was specified    */
   {
      for (i=2; i<argc; ++i)  /* Walk over the command parms to set flags */
      {
        if (!strcmp(argv[i],"-l")) plns = 1;  /* Add line numbers */
        if (!strcmp(argv[i],"-u")) cdos = 0;  /* Don't add <cr>s  */
        if (!strcmp(argv[i],"-i")) idnt = 1;  /* Indent 8 spaces  */
        if (!strcmp(argv[i],"-p")) pnum = 1;  /* Print headers    */
      }
      
      fptr = fopen(argv[1], "r+");   /* Try to open the input file for read */ 
      if (fptr)                      /* If not NULL it opened so process it */
      {
         if (pnum) /* Add filename and page number to the first page */
         {
             initHeader(argv[1],pn);
             printf("%s\n",ph); 
             printcr(cdos);   /* Adding <cr> to the output stream?     */
             printf("\n");    /* Add a blank line after the header row */
             printcr(cdos);   /* Adding <cr> to the output stream?     */
         }
         
         cc += indentLine(idnt);      /* Indent the line?   */
         cc += lineNumbers(plns,&ln); /* Print line number? */

         while ((c=fgetc(fptr))!=EOF)  /* Read the input file one     */
         {                             /*  character at a time        */
           printf("%c",c);    /* Print the character                  */
           cc++;
           if (cc == 79)      /* Do we need to wrap to the next line? */
           {
             cc = 0;
             ln--;            /* Keep the line number the same        */
             printf("\n");    /* Start printing on the next line      */
             printcr(cdos);   /* Adding <cr> to the output stream?    */
             c  = '\n';       /* Inject a newline into the stream     */
           }
           if (c=='\n')       /* Is the character a <lf>?             */
           {
             cc = 0;
             printcr(cdos);   /* Adding <cr> to the output stream?    */
             if (pnum)        /* We're printing the page header       */
             {
               plc++;         /* Count the lines on the page          */
               checkHeader(&plc,&pn,argv[1],cdos);
             }            
             cc += indentLine(idnt);      /* Indent the line?       */       
             cc += lineNumbers(plns,&ln); /* print the line number? */
           }
         }
      }
	  else
	  {
		  fprintf(stderr,"Unable to open file %s\n",argv[1]);
		  exit(1);
	  }
      fclose(fptr);   /* Close the input file              */
      printf("\n");   /* Send a final <lf> out             */
      printcr(cdos);  /* Adding <cr> to the output stream? */     
   }
   exit(0);
}
