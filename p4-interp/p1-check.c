/*
 * Assingnment : Project 1
 * Date : 2/21/22
 * Author : Bryden Mollenauer   mollenjb@dukes.jmu.edu
 * File Name : p1-check.c
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "p1-check.h"

void usage_p1 ()
{
    printf("Usage: y86 <option(s)> mini-elf-file\n");
    printf(" Options are:\n");
    printf("  -h      Display usage\n");
    printf("  -H      Show the Mini-ELF header\n");
}

bool parse_command_line_p1 (int argc, char **argv, bool *header, char **file)
{
    // Implement this function
	if( argc <= 1 || argv == NULL || header == NULL )
	{
	usage_p1();
    return false;
	}
	int opt;
	*header = false;
	char *optionStr = "+hH";
	opterr = 0;
    
	// getopt logic and all the cases 
	while (( opt = getopt(argc, argv, optionStr)) != -1)
	{
	     switch(opt)
		 {
		 
		 case 'h': 
		 *header = false;
	     usage_p1();
	     return true;
		 break;
		 
		 case 'H':
		 if(*header) {
			 usage_p1();
			 *header = false;
			 return false;
		 }
			 
		 *header = true;
		 break;
		 
		 default: usage_p1();
		 *header = false;
		 return false; 
		 }
	}
	
	// makes sure its only 1 file name 
	 if( optind != argc - 1)  
    {
        usage_p1(); 
        return false;
    }
	     *file = argv[optind];
		 if(*file == NULL)
		 {
		     usage_p1();
			 return false;
		 }
		 return true;
		 
	
		 
		 
}

bool read_header (FILE *file, elf_hdr_t *hdr)
{
    // Implement this function
	// reads the file and puts in logic for null file and hdr
	if(!file || !hdr || (fread(hdr, 16, 1, file) != 1) || hdr -> magic != 4607045)
	{
	    
		return false;
    }
	return true;
}

void dump_header (elf_hdr_t hdr)
{
    // Implement this function
	unsigned char *pointer_to_byte = (unsigned char *) &hdr;
	int i;
	
	// prints first 16 bits
	printf("00000000 ");
	for(i = 0; i < 16; i++)
	{
	     if(i == 8)
		{
		    printf(" ");
		}
		printf(" %02x", pointer_to_byte[i]);
	}
	
	printf("\n");
	printf("Mini-ELF version %d\n", hdr.e_version);
	printf("Entry point %#x\n", hdr.e_entry);
	printf("There are %d program headers, starting at offset %d (%#x)\n",
    hdr.e_num_phdr, hdr.e_phdr_start, hdr.e_phdr_start);
	
    if (hdr.e_symtab == 0)
    {
         printf("There is no symbol table present\n");
    }
	else
	{
	    printf("There is a symbol table starting at offset %d (%#x)\n",
		hdr.e_symtab, hdr.e_symtab);
	}
	if (hdr.e_symtab == 0)
	{
	    printf("There is no string table present\n");
	}
	else
	{
	    printf("There is a string table starting at offset %d (%#x)\n", hdr.e_strtab, hdr.e_strtab);
	}
}

