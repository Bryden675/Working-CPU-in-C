/*
 * Assingnment : Project 3
 * Date : 4/13/22
 * Author : Bryden Mollenauer   mollenjb@dukes.jmu.edu
 * File Name : p3-disas.c
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "p3-disas.h"

//============================================================================
void usage_p3 ()
{
	// prints all the different commands that the user can do 
printf("Usage: y86 <option(s)> mini-elf-file\n");
    printf(" Options are:\n");
    printf("  -h      Display usage\n");
    printf("  -H      Show the Mini-ELF header\n");
    printf("  -a      Show all with brief memory\n");
    printf("  -f      Show all with full memory\n");
    printf("  -s      Show the program headers\n");
    printf("  -m      Show the memory contents (brief)\n");
    printf("  -M      Show the memory contents (full)\n");
    printf("  -d      Disassemble code contents\n");
    printf("  -D      Disassemble data contents\n");
	printf("Options must not be repeated neither explicitly nor implicitly.");
}

//============================================================================
bool parse_command_line_p3 (int argc, char **argv,
        bool *header, bool *segments, bool *membrief, bool *memfull,
        bool *disas_code, bool *disas_data, char **file) {
       //checks all null logic
    if(argc <= 1 ||argv == NULL || header == NULL || 
    membrief == NULL || segments == NULL || memfull == NULL || disas_code == NULL
	|| disas_data == NULL) {
        usage_p3();
        return false;
    }

    *header = false;
    *segments = false;
    *membrief = false;
    *memfull = false;
	*disas_code = false;
    *disas_data = false;
    char *optionStr = "+hHafsmMDd";
    int opt;
    opterr = 0;
    bool printHelp = false;
    
    //getopt logic for all the command arguments 
	// sets everything ot false if argument is put in twice
    while((opt = getopt(argc, argv, optionStr))!= -1) 
    {
        switch(opt)
        {
            case 'h': 
			printHelp = true; 
			break;
			
            case 'H':
			if(*header) {
			 usage_p3();
			 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
		    }
			*header = true; 
			break;
			
            case 'm': 
			if(*membrief) {
				usage_p3();
			 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
			}
			*membrief = true; 
			break;
			
            case 'M': 
			if(*memfull) {
				usage_p3();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
			}
			*memfull = true; 
			break;
			
            case 's': 
			if(*segments) {
				usage_p3();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
			}
			*segments = true; 
			break;
			
            case 'a': 
			
			if(*header || *membrief || *segments) {
			 usage_p3();
			 *header = false;
			 *membrief = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
		    }
			*header = true; 
			*membrief = true; 
			*segments = true; 
			break;
			
            case 'f':
			
			 if(*header || *memfull || *segments) {
			 usage_p3();
			 *header = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
				}
			
			*header = true; 
			*memfull = true; 
			*segments = true; 
			break;
			
			case 'd':
			if(*disas_code) {
				usage_p3();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
			}
			*disas_code = true;
			break;
			
			case 'D':
			if(*disas_data) {
				usage_p3();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 return false;
			}
			*disas_data = true;
			break;
			
			
            default: usage_p3(); 
			return false;
        }
    }
    
    //if printhelp is true prints Usage
    if(printHelp)
    {
        *header = false;
        usage_p3();
        return true;
    }
    else if(*membrief && *memfull)
    {
        usage_p3();
        return false;
    }
    else
    {
        //sets the last command arg to the file char array 
        *file = argv[optind];
        if(*file == NULL || argc > optind + 1)
        {

            usage_p3();
            return false;
        }
    
        return true;
    }
}

//============================================================================
y86_inst_t fetch (y86_t *cpu, memory_t memory)
{
    y86_inst_t ins;

    // Initialize the instruction
    memset( &ins , 0 , sizeof(y86_inst_t) );  // Clear all fields i=on instr.    
    // Invalid instruction until proven otherwise
	// if given any null arguments automatically return and sets stat to ADR
 if(memory == NULL || cpu->pc >= MEMSIZE || cpu->pc < 0)
   {
        ins.type = INVALID;
		cpu->stat = ADR;
        return ins;
   } 
   // sets the opcode and gets the first byte using bit wise shifts 
   ins.opcode = memory[cpu->pc];
   uint8_t parentCode = memory[cpu->pc] & 0xF0;
   uint8_t registers = memory[cpu->pc + 1];
   // switch statment for determining case the parent code given is.
   switch(parentCode) 
   {
	   // for each case the switch sets the type to the type specified by the y86 doc and then 
	   // also sets the size and depending on which case sets specific registers and opcode 
        case (0x00) : 
		if (ins.opcode != 0x00) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		ins.type = HALT; 
		ins.size = 1; 
		if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                return ins;
            }
		  
		break;
		
		case (0x10) : 
		if (ins.opcode != 0x10) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		ins.type = NOP; 
		ins.size = 1; 
		if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                return ins;
            }
		break;
		
		case (0x20):
		ins.type = CMOV;
		ins.size = 2;
				 if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
			// bit wise logic for putting the right bits into ra and rb 
		ins.ra = registers >> 4;
		ins.rb = registers & 0x0F;
		//checks out of bounds for ra and rb 
		if (ins.ra > 14 || ins.ra < 0 || ins.rb > 14 || ins.rb < 0) {
			ins.type = INVALID;
			cpu->stat = INS;
		}
		// sets specific cmov depending on the opcode 
		switch (ins.opcode) {
			case (0x20):
			ins.cmov = RRMOVQ;
			break;
			
			case (0x21):
			ins.cmov = CMOVLE;
			break;
			
			case (0x22):
			ins.cmov = CMOVL;
			break;
			
			case (0x23):
			ins.cmov = CMOVE;
			break;
			
			case (0x24):
			ins.cmov = CMOVNE;
			break;
			
			case (0x25):
			ins.cmov = CMOVGE;
			break;
			
			case (0x26):
			ins.cmov = CMOVG;
			break;
			
			default: 
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		break;
		
		 case (0x30) : 
		 if (ins.opcode != 0x30) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		 }
		 ins.type = IRMOVQ;
		 ins.size = 10; 
		  if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
	     ins.ra = registers >> 4;
		 ins.rb = registers & 0x0F;
		 if (ins.ra != 0x0F || ins.rb > 14 || ins.rb < 0) {
			ins.type = INVALID;
			cpu->stat = INS;
		}
		 // big endian to little endian in memory logiv 
		 ins.value = memory[cpu->pc + 9];
         for (int i = cpu->pc + 8; i > cpu->pc + 1; i--) {
         ins.value = (ins.value << 8) | memory[i];
         }
		 break;
		 
		  case (0x40) : 
		  if (ins.opcode != 0x40) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		  ins.type = RMMOVQ; 
		  ins.size = 10;
		   if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		  ins.ra = registers >> 4;
		  ins.rb = registers & 0x0F;
		  ins.d = memory[cpu->pc + 9];
          for (int i = cpu->pc + 8; i > cpu->pc + 1; i--) {
          ins.d = (ins.d << 8) | memory[i];
          }
		  break;
		  
		   case (0x50) : 
		   if (ins.opcode != 0x50) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		   ins.type = MRMOVQ; 
		   ins.size = 10; 
		    if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		   ins.ra = registers >> 4;
		   ins.rb = registers & 0x0F;
		   ins.d = memory[cpu->pc + 9];
           for (int i = cpu->pc + 8; i > cpu->pc + 1; i--) {
           ins.d = (ins.d << 8) | memory[i];
           }
		   break;
		 
		 case(0x60):
		 ins.type = OPQ; 
		 ins.size = 2;
		 if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		 ins.ra = registers >> 4;
		 ins.rb = registers & 0x0F;
		 if (ins.ra > 14 || ins.ra < 0 || ins.rb > 14 || ins.rb < 0) {
			ins.type = INVALID;
			cpu->stat = INS;
		}
		 switch (ins.opcode) {
			// switch for setting correct .op bases on opcode 
			case (0x60):
			ins.op = ADD;
			break;
			
			case (0x61):
			ins.op = SUB;
			break;
			
			case (0x62):
			ins.op = AND;
			break;
			
			case (0x63):
			ins.op = XOR;
			break;
			
			default:
			ins.type = INVALID;
			cpu->stat = INS;
		 }
		 break;
		 
		 case(0x70):
		 ins.type = JUMP;
		 ins.size = 9;
		  if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		 ins.dest = memory[cpu->pc + 8];
         for (int i = cpu->pc + 7; i > cpu->pc; i--) {
         ins.dest = (ins.dest << 8) | memory[i];
         }
		 switch (ins.opcode) {
			 // switch for setting correct .jump based on opcode
			 case (0x70):
			ins.jump = JMP;
			break;
			
			case (0x71):
			ins.jump = JLE;
			break;
			
			case (0x72):
			ins.jump = JL;
			break;
			
			case (0x73):
			ins.jump = JE;
			break;
			
			case (0x74):
			ins.jump = JNE;
			break;
			
			case (0x75):
			ins.jump = JGE;
			break;
			
			case (0x76):
			ins.jump = JG;
			break;
			
			default:
			ins.type = INVALID;
			cpu->stat = INS;
		 }
		 break;
		 
		 case (0x80) : 
		 if (ins.opcode != 0x80) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		 ins.type = CALL;
		 ins.size = 9;
		 if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		 ins.dest = memory[cpu->pc + 8];
         for (int i = cpu->pc + 7; i > cpu->pc; i--) {
         ins.dest = (ins.dest << 8) | memory[i];
		 }
		 break;
		 
		case (0x90) :
        if (ins.opcode != 0x90) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}		
		ins.type = RET;
		ins.size = 1;
		if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		break;
		
		case (0xA0) : 
		if (ins.opcode != 0xA0) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}
		ins.type = PUSHQ;
		ins.size = 2;
		if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		ins.ra = registers >> 4;
		ins.rb = registers & 0x0F;
		if (ins.ra > 14 || ins.ra < 0 || ins.rb != 0x0F) {
			ins.type = INVALID;
			cpu->stat = INS;
		}
		break;

		case (0xB0):
        if (ins.opcode != 0xB0) {
			ins.type = INVALID;
			cpu->stat = INS;
			break;
		}		
		ins.type = POPQ; 
		ins.size = 2;
		if(cpu->pc + ins.size >= MEMSIZE)
            {
                ins.type = INVALID;
				cpu->stat = ADR;
                break;
            }
		ins.ra = (registers >> 4);
		ins.rb = registers & 0x0F;
		if (ins.ra > 14 || ins.ra < 0 || ins.rb != 0x0F) {
			ins.type = INVALID;
			cpu->stat = INS;
		}
		break;
		
		default:
		// sets type to invalid if no case is hit 
		ins.type = INVALID;
		cpu->stat = INS;
		
		break;
   }
    return ins;
}

char* getRegister(y86_rnum_t reg) {
	// helper method to help dissasembl() for printing the correct register 
	switch (reg) {
		case 0:
		return "%rax";
		
		case 1:
		return "%rcx";
		
		case 2:
		return "%rdx";
		
		case 3:
		return "%rbx";
		
		case 4:
		return "%rsp";
		
		case 5:
		return "%rbp";
		
		case 6:
		return "%rsi";
		
		case 7:
		return "%rdi";
		
		case 8:
		return "%r8";
		
		case 9:
		return "%r9";
		
		case 10:
		return "%r10";
		
		case 11:
		return "%r11";
		
		case 12:
		return "%r12";
		
		case 13:
		return "%r13";
		
		case 14:
		return "%r14";
		
		default:
		return "badReg";
	}
}
//============================================================================
void disassemble (y86_inst_t inst)
{
	// switch statement depending on the inst.type to determine what is printed
	switch(inst.type) {
		case HALT:
		printf("halt");
		break;
		
		case NOP:
		printf("nop");
		break;
		
		case CMOV:
		switch(inst.cmov){
			
			case RRMOVQ:
			printf("rrmovq");
			break;
			
			case CMOVLE:
			printf("cmovle");
			break;
			
			case CMOVL:
			printf("cmovl");
			break;
			
			case CMOVE:
			printf("cmove");
			break;
			
			case CMOVNE:
			printf("cmovne");
			break;
			
			case CMOVGE:
			printf("cmovge");
			break;
			
			case CMOVG:
			printf("cmovg");
			break;
			
			case BADCMOV:
			return;
			
		}
		// prints the registers if the type in the y86 has registers 
		printf(" %s, %s", getRegister(inst.ra), getRegister(inst.rb)); 
		break;
		
		case IRMOVQ:
		printf("irmovq");
		printf(" 0x%lx, %s", inst.value, getRegister(inst.rb));
		break;
		
		case RMMOVQ:
		printf("rmmovq");
		// since rmmovq doesnt have to have a rb register if its invalid you print nothing 
		if (inst.rb < 0 || inst.rb > 14) {
		printf(" %s, 0x%lx", getRegister(inst.ra), inst.d);
		} else {
		printf(" %s, 0x%lx(%s)", getRegister(inst.ra), inst.d, getRegister(inst.rb));
		}		
		break;
		
		case MRMOVQ:
		printf("mrmovq");
		if (inst.rb < 0 || inst.rb > 14) {
		printf(" 0x%lx, %s", inst.d, getRegister(inst.ra) );
		} else {
		
		printf(" 0x%lx(%s), %s", inst.d, getRegister(inst.rb), getRegister(inst.ra));
		}
		break;
		
		case OPQ:
			switch(inst.op) {
				case ADD:
				printf("addq");
			    break;
				
				case SUB:
				printf("subq");
			    break;
				
				case AND:
				printf("andq");
			    break;
				
				case XOR:
				printf("xorq");
			    break;
				
				case BADOP:
				return;
				
			}
			printf(" %s, %s", getRegister(inst.ra), getRegister(inst.rb)); 
			break;
			
			case JUMP:
			switch(inst.jump) {
				
				case JMP:
				printf("jmp");
			    break;
				
				case JLE:
				printf("jle");
			    break;
				
				case JL:
				printf("jl");
			    break;
				
				case JE:
				printf("je");
			    break;
				
				case JNE:
				printf("jne");
			    break;
				
				case JGE:
				printf("jge");
			    break;
				
				case JG:
				printf("jg");
			    break;
				
				case BADJUMP:
				return;
			}
			printf(" 0x%lx", inst.dest); 
			break;
			
			case CALL:
			printf("call");
			printf(" 0x%lx", inst.dest);
			break;
			
			case RET:
			printf("ret");
			break;
			
			case PUSHQ:
			printf("pushq");
			printf(" %s", getRegister(inst.ra));
			break;
			
			case POPQ:
			printf("popq");
			printf(" %s", getRegister(inst.ra));
			break;
			
			case INVALID:
			break;
			}
}

//============================================================================
void disassemble_code (memory_t memory, elf_phdr_t *phdr, elf_hdr_t *hdr)
{
	// if given null arguments return
	if(memory == NULL || phdr == NULL || hdr == NULL) 
    {
        return;
    }
    
    //assigns cpu program counter
    y86_t cpu;
    y86_inst_t ins;
    uint32_t addr = phdr->p_vaddr;
	addr -= addr % 16;
    cpu.stat = AOK;
    cpu.pc = addr;
	printf("  0x%03x:                      | .pos 0x%03x code\n", addr, addr);
	// while cpu is smallet then the vaddr + the file size loop through the print logiv 
   while (cpu.pc < addr + phdr->p_filesz) {
	   ins = fetch(&cpu, memory);
	   if (cpu.pc == hdr->e_entry) {
		printf("  0x%03lx:                      | _start:\n", cpu.pc);
	}
	   if (ins.type == INVALID) {
		   // invalid opcode print logic 
		   printf("Invalid opcode: 0x%x\n", ins.opcode);
		   cpu.pc += ins.size;
		   return;
	   }
	   
	   printf("  0x%03lx: ", cpu.pc);
	   // prints the bytes until i > ins.size and then prints spaces to print the proper output
	   for(int i = cpu.pc; i < cpu.pc + 10; i++){
		   if (i < cpu.pc + ins.size){
		   printf("%02x", memory[i]);
		   } else {
			   printf("  ");
	   }
	   }
	   printf(" |   ");
	   disassemble(ins);
	   cpu.pc += ins.size;
	   printf("\n");
   }
    
}

//============================================================================
void disassemble_data (memory_t memory, elf_phdr_t *phdr)
{
	// if given null arguments return 
	if(memory == NULL || phdr == NULL) 
    {
        return;
    }
	int numofSegments = phdr->p_filesz / 8;
	uint32_t addr = phdr->p_vaddr;
	addr -= addr % 16;
	
	printf("  0x%03x:                      | .pos 0x%03x data\n", addr, addr);
	// printing however number segments of data are represented by the Pheader
	for (int i = 0; i < numofSegments; i++) {
		// logic for switching the segments into big endian
	     uint64_t big = memory[(addr + 8 * i) + 7];
		 printf("  0x%03x: ", addr + 8 * i);
         for (int j = (addr + 8 * i) + 6; j > (addr + 8 * i) - 1; j--) {
         big = (big << 8) | memory[j];
		 }
		 // prints the data in little endian and makes sure there is 20 spaces to fit the expected output 
	   for(int k = addr + 8 * i; k < (addr + 8 * i) + 10; k++){
		   if (k < (addr + 8 * i) + 8){
		   printf("%02x", memory[k]);
		   } else {
			   printf("  ");
	   }
	   }
	   // print the bar and the big endian representation that was built earlier
	   printf(" |   ");
	   printf(".quad 0x%lx\n", big);
	}
        
}

//============================================================================
void disassemble_rodata (memory_t memory, elf_phdr_t *phdr)
{
	// if given null arguments return 
		if(memory == NULL || phdr == NULL) 
    {
        return;
    }
	uint32_t addr = phdr->p_vaddr;
	addr -= addr % 16;
	
	uint8_t bytes[phdr->p_filesz];
	int index = 0;
	printf("  0x%03x:                      | .pos 0x%03x rodata\n", addr, addr);
	memset(bytes, 0, phdr->p_filesz);
	// loops through the file 
	for (int i = addr; i < addr + phdr->p_filesz; i++) {
		// memory isnt 0 it builds the string 
	    if (memory[i] != 0) {
		bytes[index] = memory[i];
		index++;
		} else {
			// when memory hits 0 memcopy the string 
			unsigned char curString[index];
			memcpy(curString, &bytes, sizeof(bytes));
			// loops through curstring 
	for (int j = 0; j < sizeof(curString); j++) {
		if (j % 10 == 0) {
			// prints all the data
			printf ("  0x%03lx: ", i - sizeof(curString) + j);
		}
		printf("%02x", curString[j]);
		// when the string is full print the bar and the actual string 
		if (j % 10 == 9 && j != 0) {
			printf(" |");
			if (j == 9) {
				printf("   .string \"%s\"", curString);
			}
			printf("\n");
		}
		
        if (j == sizeof(curString) - 1) {
			if (j % 10 == 9) {
				printf ("  0x%03lx: ", i - sizeof(curString) + j + 1);
				printf("00                  ");
			} else {
				// prints extra 00
			printf("00");
			}
			// makes sure there is enough spaces for expected output 
			for (int k = 0; k < (20 - (2 * j) % 20) - 4; k++) {
				printf(" ");
			}
			printf(" |");
			if(j < 9) {
				printf("   .string \"%s\"", curString);
			}
			
		}
    }
	// resets index and depending on how big filesz is sets bytes to that many 0s 
	index = 0;
    printf("\n");	
	memset(bytes, 0, phdr->p_filesz);
	}
}
}
//============================================================================

