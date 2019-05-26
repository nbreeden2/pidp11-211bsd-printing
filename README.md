# pidp11-211bsd-printing
Print UNIX formatted files on printers that expect DOS formatting.

Note: You will need to have previously configured your printer in BSD before the following program will work. You may need to edit the scipt "print" to change the | redirect to the name of your defined printer.

In UNIX a "line feed character" is used to move the print carrage to the beginning of the next line to print

In DOS a "line feed character" is used to move the print carrage to the next line WITHOUT moving it to the beginning of the line. A "carrage return character" is then used to move the print carrage to the beginning of the line.

In c programming a "\n" is used to move the print carrage to the next line. As UNIX and DOS use different commands as shown above the c compiler on the platform takes care of sending the right characters to the printer.

   printf("Values: %d %d %d\n",a,b,c);
   
Our issue is that UNIX (BSD 2.11 in our case) is only sending the "line feed character" to the printer. My Brother printer expects both a "line feed character" and "carrage return character" to get to the beginning of the next line as it was designed for use with DOS/Windows machines.

This is a small c program under BSD to add the missing "carrage return character" to the output stream so we can print correctly. It can also add line and page numbers to the printed output.

The script "print" and compiled program "u2d" should be copied to the \bin folder in your BSD 2.11 install. This way they can be ran from any folder on your BSD install.

You can compile the program using the provided Makefile
  #make

You can test the local compiled version of the program using. 
  #make test1
    ...
  #make test6
'cat' the makefile to see what the various tests do.

You can install an updated version of the program
  #make install
  
Command line switches for u2c (and print)
  -l   Will print line numbers from 1..99999 
  -u   Don't convert the printed file to DOS format, default is to convert
  -i   Indent the printed line 8 spaces
  -p   Print the file name and page number on the top of each page
You can mix the command line variables as needed.  

The script "print" calls compiled program "u2d" and will pipe the printable output directly to a printer, lpd by default.
  print myfile.txt -l -i -p
  
The "u2c" program outputs the formatted text to stdout. You can redirect this to a file or output device using pipes.
  u2c myfile.txt -l -i -p | newfile.txt
  u2c myfile.txt -l -i -p | lpr
  
