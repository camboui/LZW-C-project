#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

bool le = false;	// little endian
bool rb = false;        // reverse binary
bool da = false;        // decimal address

FILE *input;
char *filename = NULL;

int line [4];
int linesize;

unsigned long begin_address;
unsigned long end_address;

void print_address (unsigned long adr)
{
  if (da)
    printf ("  %08lu",adr);
  else
    printf ("0x%08lx",adr);
}    


void print_char (int byte)
{
  switch (byte) {
    case '\n' :  printf ("%8s","      \\n"); break;
    case '\r' :  printf ("%8s","      \\r"); break;
    case '\t' :  printf ("%8s","      \\t"); break;
    case '\0' :  printf ("%8s","      \\0"); break;
    case ' ' :   printf ("%8s","   space"); break;
    case '\\' :  printf ("%8s","      \\\\"); break;
    default   :  if (isprint (byte)) 
                   printf ("       %c",byte);
                 else
                   printf ("    \\%03o",byte);
  }
}

void print_hexa (int byte)
{
  printf ("      %02x",byte);
}

void print_bin (int byte)
{
  int bit;
  int i;
  for (i=0;i<=7;i++)
    {
    bit = (byte >> (rb ? i : 7-i)) & 1;
    printf ("%c", '0'+bit);
    }
}

typedef void byte_func_t (int byte);

void print_line_content (byte_func_t f)
{
  int i;
  int byte;

  for (i = 0; i <= 3; i++)
    {
    byte = line [le ? (3-i) : i];
    if (byte >=0) 
      f (byte); 
    else
      printf ("        ");
    printf (" ");
    }
}

void print_line (byte_func_t f, bool write_address)
{
  if (write_address)
    print_address (le ? end_address : begin_address);
  else
    printf ("          ");
  printf ("   ");
  print_line_content (f);
  printf ("   ");
  if (write_address)
    print_address (le ? begin_address : end_address);
  else
    printf ("          ");
  printf ("\n");
} 
     
void read_bytes (void)
{
  int i;
  linesize = 0;
  int new;
  for (i=0;i<=3;i++)
    {
    new = fgetc(input);
    line [i] = new;
    if (new == EOF)
      while (i<=3) line[i++] = -1;
    else
      linesize++;
    }
}

void read_file (void)
{
  void arrow (bool reverse)
    {
    printf ("%s", reverse ? "<-" : "->");
    }
  end_address = 0;
  printf ("   address" "   ");
  arrow (le);
  printf (" byte order  ");
  arrow (le);
  printf ("   ");
  arrow (rb);
  printf (" bit order ");
  arrow (rb);
  printf ("    ""   address");
  printf  ("\n");

  while(1)
    {
    read_bytes ();
    if (linesize == 0) break;
    begin_address = end_address;
    end_address += linesize;
    print_line (print_hexa,true);
    print_line (print_bin,false);
    print_line (print_char,false);
    }
}

void usage (void)
{
  fprintf (stderr, "Usage : bitdump [-le] [-rb] [-da] [inputfile]\n");
  fprintf (stderr, "        dump each byte in both hexa and binary\n");
  fprintf (stderr, "        dump either inputfile arg or stdin\n");
  fprintf (stderr, "        -le : increase byte address form right to left\n");
  fprintf (stderr, "        -rb : reverse bit order within byte\n");
  fprintf (stderr, "        -da : use decimal address\n");
  
  exit (EXIT_FAILURE);
}


void check_args (int argc, char *argv[])
{
  int arg;

  input = stdin;
  for (arg=1;arg <argc; arg++)
    {
    if (!strcmp(argv[arg],"-le")) {
         if (le) 
           {fprintf (stderr,"Redondant option -le\n"); usage ();}
         le = true;
      } else if (!strcmp(argv[arg],"-rb")) {
         if (rb) 
           {fprintf (stderr,"Redondant option -b\n"); usage ();};
         rb = true;
      } else if (!strcmp(argv[arg],"-da")) {
         if (da) 
           {fprintf (stderr,"Redondant option -da\n"); usage ();};
         da = true;
      } else {
         filename = argv[arg];
         if ((input = fopen (filename, "r")) == NULL) {
            fprintf (stderr,
                     "Error : cannot open file %s for reading\n",filename);
            usage ();
         } 
      } 
    }
}


  
int main (int argc, char *argv[], char*envp[])
{
  check_args (argc,argv);
  read_file();
  fclose (input);
  return 0;
}
