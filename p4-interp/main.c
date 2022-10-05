/*
 * Assingnment : Project 4
 * Date : 4/13/22
 * Author : Bryden Mollenauer   mollenjb@dukes.jmu.edu
 * File Name : main.c\
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "p1-check.h"
#include "p2-load.h"
#include "p3-disas.h"
#include "p4-interp.h"

int main (int argc, char **argv)
{
    bool isheader = false;
    char *file = NULL;
    bool segments = false;
    bool membrief = false;
    bool memfull = false;
	bool disas_code = false;
	bool disas_data = false;
	bool exec_normal = false;
    bool exec_debug = false;
    // updated parse p4 logic
    if (!parse_command_line_p4(argc, argv, &isheader, 
	&segments, &membrief, &memfull,&disas_code, &disas_data, &exec_normal, &exec_debug, &file))
		exit(EXIT_FAILURE);
	
	
	if(!file) {
		exit(EXIT_SUCCESS);
		puts("");
	}
	FILE *input = fopen(file, "r");
	
    struct elf hdr;
   
	
	if(!input)
	{
		puts("Failed to open File");
		exit(EXIT_FAILURE);
	}
	
	// if read_header is invalid print error message
	if (!read_header(input, &hdr))
    {
	puts("Failed to Read ELF Header");
	exit(EXIT_FAILURE);
    }
    // sets up the struct for reading the phdr
    struct elf_phdr phdrs[hdr.e_num_phdr]; 
    int offset;
    //read phdr information from the file into array of program headers
    for(int i = 0; i < hdr.e_num_phdr; i++) 
    {
        offset = hdr.e_phdr_start + (i * sizeof(elf_phdr_t));   
        if(!(read_phdr(input, offset, &phdrs[i])))
        {
	        printf("Failed to Read Program Header");
            return EXIT_FAILURE;
        }
   
    }

   
   
    
    if(isheader)
    {
        // if header is true dump header
        dump_header(hdr);
		puts("");
    }  
	 memory_t memory = (memory_t) malloc(MEMSIZE);
    if (memory == NULL) {
		puts("Failed to Allocate Dynamic Memory");
		exit(EXIT_FAILURE);
	}
    // loops through all hdr numbers
    for(int k = 0; k < hdr.e_num_phdr; k++)
    {
        if(!(load_segment(input, memory, phdrs[k])))
        {
	        printf("Failed to Load Segment");
            free(memory);
            return EXIT_FAILURE;
        }
   
    }
    if(segments)
    {
        // if segments tur dump phdrs
        dump_phdrs(hdr.e_num_phdr, phdrs);
    }
    if(memfull)
    {
        //if memfull is true dump memeroy
        dump_memory(memory, 0, MEMSIZE); 
    }
    else if(membrief)
    {
        // if membrief full and not memfull dump the virtual space
        for(int j = 0; j < hdr.e_num_phdr; j++)
        {
		if (!phdrs[j].p_filesz) {
			continue;
		}
            int vspace = phdrs[j].p_vaddr;
			int fileSize = phdrs[j].p_filesz;
	        dump_memory(memory, vspace - (vspace % 16), vspace + fileSize);
        }
	}
	// if the disas_code flag  is true 
    if (disas_code) {
		// prints dissassembly of the code 
		printf("Disassembly of executable contents:\n");
		for (int i = 0; i < hdr.e_num_phdr; i++) {
			if(phdrs[i].p_type == CODE) {
			   disassemble_code(memory, &phdrs[i], &hdr);
			   
			   printf("\n");
			   
			}
		}
	}
	// if the disas_data flag is true 
	if (disas_data) {
		// print dissassembly data
		printf("Disassembly of data contents:\n");
		for (int i = 0; i < hdr.e_num_phdr; i++) {
			if (phdrs[i].p_type == DATA) {
				switch(phdrs[i].p_flag) {
					//dependint on the case print just data or rodata
					case RW:
					disassemble_data(memory, &phdrs[i]);
					break;
					case RO:
					disassemble_rodata(memory, &phdrs[i]);
					break;
				}
				printf("\n");
			}
		}
				
	}
	y86_t cpu;
	memset(&cpu, 0x00, sizeof(cpu));
	cpu.stat = AOK;
	y86_register_t valE;
	y86_register_t valA;
    bool cond = false;	
	y86_inst_t inst;
	int loopTimes;
	
	cpu.pc = hdr.e_entry;
	
	// if exec normal 
	if (exec_normal) {
	printf("Entry execution point at 0x%04x\n", hdr.e_entry);
	printf("Initial ");
	// dumps cpu 
	dump_cpu(&cpu);
	// while cpu is AOK fetches cpu and mem then decodes and calls writeback 
		while(cpu.stat == AOK)
			{
				inst = fetch(&cpu, memory);
				if (cpu.stat == INS) {
					printf("Corrupt Instruction (opcode 0x%x) at address 0x%04lx\n", inst.opcode, cpu.pc);
					printf("Post-Fetch ");
					dump_cpu(&cpu);
					loopTimes += 1;
                    break;
				}
				valE = decode_execute(&cpu, &cond, &inst, &valA);
				memory_wb_pc(&cpu, memory, cond, &inst, valE, valA);
				loopTimes++;
				if(cpu.pc >= MEMSIZE)
            {
                cpu.stat = ADR;
                cpu.pc = 0xffffffffffffffff;
			}
			
	}
	//when it hits hlt dumps cpu
	if (cpu.stat == HLT) {
	printf("Post-Exec ");
	dump_cpu(&cpu);
	}
	printf("Total execution count: %d instructions\n", loopTimes);
	printf("\n");
	}

	if (exec_debug) {
		printf("Entry execution point at 0x%04x\n", hdr.e_entry);
		printf("Initial ");
		// dumps cpu
		dump_cpu(&cpu);
		// while cpu AOK disassemble then decodes then writes back to mem and dumps again 
		while(cpu.stat == AOK)
			{
				
				inst = fetch(&cpu, memory);
				printf("Executing: ");  
				disassemble(inst);
				printf("\n");
				valE = decode_execute(&cpu, &cond, &inst, &valA);
				memory_wb_pc(&cpu, memory, cond, &inst, valE, valA);
				printf("Post-Exec ");
				dump_cpu(&cpu);
				loopTimes++;
			
		}
		// dumps mem again 
        printf("Total execution count: %d instructions\n\n", loopTimes);	
		dump_memory(memory, 0, MEMSIZE); 
				
	}
    free(memory);
	return EXIT_SUCCESS;
    }
	

	
	

