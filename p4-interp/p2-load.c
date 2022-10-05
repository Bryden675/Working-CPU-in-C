/*
 * Assingnment : Project 2
 * Date : 4/13/22
 * Author : Bryden Mollenauer   mollenjb@dukes.jmu.edu
 * File Name : p2-load.c
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "p2-load.h"
char * type_set(uint16_t type_var);
void flag_set(uint16_t flag_var);
void usage_p2 ()
{
    printf("Usage: y86 <option(s)> mini-elf-file\n");
    printf(" Options are:\n");
    printf("  -h      Display usage\n");
    printf("  -H      Show the Mini-ELF header\n");
    printf("  -a      Show all with brief memory\n");
    printf("  -f      Show all with full memory\n");
    printf("  -s      Show the program headers\n");
    printf("  -m      Show the memory contents (brief)\n");
    printf("  -M      Show the memory contents (full)\n");
	printf("Options must not be repeated neither explicitly nor implicitly.");

}

bool parse_command_line_p2 (int argc, char **argv,
        bool *header, bool *segments, bool *membrief, bool *memfull,
        char **file)
{
      //checks all null logic
    if(argc <= 1 ||argv == NULL || header == NULL || 
    membrief == NULL || segments == NULL || memfull == NULL)
    {
        usage_p2();
        return false;
    }

    *header = false;
    *segments = false;
    *membrief = false;
    *memfull = false;
    char *optionStr = "+hHafsmM";
    int opt;
    opterr = 0;
    bool printHelp = false;
    
    //getopt logic for all the command arguments 
	// sets everything ot false if argument is put in twice
    while((opt = getopt(argc, argv, optionStr))!= -1) 
    {
        switch(opt)
        {
            case 'h': printHelp = true; 
			break;
			
            case 'H':
			if(*header) {
			 usage_p2();
			 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 return false;
		    }
			*header = true; 
			break;
			
            case 'm': 
			if(*membrief) {
				usage_p2();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 return false;
			}
			*membrief = true; 
			break;
			
            case 'M': 
			if(*memfull) {
				usage_p2();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 return false;
			}
			*memfull = true; 
			break;
			
            case 's': 
			if(*segments) {
				usage_p2();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 return false;
			}
			*segments = true; 
			break;
			
            case 'a': 
			
			if(*header || *membrief || *segments) {
			 usage_p2();
			 *header = false;
			 *membrief = false;
			 *segments = false;
			 return false;
		    }
			*header = true; 
			*membrief = true; 
			*segments = true; 
			break;
			
            case 'f':
			
				if(*header || *memfull || *segments) {
			 usage_p2();
			 *header = false;
			 *memfull = false;
			 *segments = false;
			 return false;
				}
			
			*header = true; 
			*memfull = true; 
			*segments = true; 
			break;
			
            default: usage_p2(); 
			return false;
        }
    }
    
    //if printhelp is true prints Usage
    if(printHelp)
    {
        *header = false;
        usage_p2();
        return true;
    }
    else if(*membrief && *memfull)
    {
        usage_p2();
        return false;
    }
    else
    {
        //sets the last command arg to the file char array 
        *file = argv[optind];
        if(*file == NULL || argc > optind + 1)
        {

            usage_p2();
            return false;
        }
    
        return true;
    }
}

bool read_phdr (FILE *file, uint16_t offset, elf_phdr_t *phdr)
{
     //logic for null arguments
    if(!file || !offset || !phdr) 
    {
        return false;
    }
    //finds the correct file location
    if(fseek(file, offset, SEEK_SET) != 0)
    {
        return false;
    }
    //reads the program header from the file into the struct
    if(fread(phdr, sizeof(elf_phdr_t), 1, file) != 1)
    {
        return false;
    }
    //checks the magic number of the program header
    if(phdr->magic != 0xDEADBEEF)
    {
        return false;
    }
    return true;
}

void dump_phdrs (uint16_t numphdrs, elf_phdr_t phdr[])
{
	 char* type; 
	 //printf for dump header format
    printf("Segment   Offset    VirtAddr  FileSize  Type      Flag\n"); 
    // loops through all the numphdr and prints them 
	
    for(int i = 0; i < numphdrs; i++)
    {
        
        type = type_set(phdr[i].p_type);
        
        
        printf("  %02d      0x%04x    0x%04x    0x%04x    %s", i, 
	phdr[i].p_offset, phdr[i].p_vaddr, phdr[i].p_filesz, type);
        flag_set(phdr[i].p_flag);
		printf("\n");
		
	}
	printf("\n");
}
char * type_set(uint16_t type_var)
{ 
    // all the switch cases for each type
    char* type;
    switch(type_var){
        case 0:  type = "DATA      "; 
		break;
		
        case 1:  type = "CODE      "; 
		break;
		
        case 2:  type = "STACK     "; 
		break;
		
        case 3:  type = "HEAP      "; 
		break;
		
        default: type = "UNKNOWN   "; 
		break;
    }
    return type;

}

void flag_set(uint16_t flag_var)
{  
    // does bit shift logic to find the correct flag
    for(int i = 2; i >= 0; i--)
    {
        if((flag_var & 1 << i) == 4)
        {
            printf("R");
        }
        else if((flag_var & 1 << i) == 2)
        {
            printf("W");
        }
        else if((flag_var & 1 << i) == 1)
        {
            printf("X");
        }
        else
        {
            printf(" ");
        }
    }
   
}

bool load_segment (FILE *file, memory_t memory, elf_phdr_t phdr)
{
	// checks if file of memory is null
       if(!file || !memory || phdr.p_offset < 0)  
    {
        return false;
    }
    //does fseek to find correct progam header location
    if (fseek(file, phdr.p_offset , SEEK_SET) != 0)
    {
        return false;
    }
    // logic for correct size
    if(phdr.p_vaddr > 4096 || phdr.p_vaddr + phdr.p_filesz > 4096
       || phdr.p_vaddr < 0)
    {
        return false;
    }
    //freads program header into memory address
    if (fread(&memory[phdr.p_vaddr], phdr.p_filesz, 1, file) != 1 &&
	 phdr.p_filesz)
    {
        return false;
    }
    return true;
}

void dump_memory (memory_t memory, uint16_t start, uint16_t end)
{
	 int j = 0;
	 // printf for dump memory
    printf("Contents of memory from %04x to %04x:\n",start,end);
    if(start == end)
    {
	    return;
    }
	// loops through the start through end and increments j every loop
    printf("  %04x ", start);
    for(int i = start; i < end; i++)
    {
        if((j % 8) == 0)
        {
            printf(" ");
        }
        if((j % 16) == 0 && j != 0)
        {
            printf("\n");
            printf("  %04x  ", i);
        }
        printf("%02x ", memory[i]);
        j++;
    }
    printf("\n");
	printf("\n");
}
