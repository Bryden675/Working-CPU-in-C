/*
 * Assingnment : Project 4
 * Date : 4/28/22
 * Author : Bryden Mollenauer   mollenjb@dukes.jmu.edu
 * File Name : p4-interp.c
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#include "p4-interp.h"

//=======================================================================
// help display
void usage_p4 ()
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
    printf("  -d      Disassemble code contents\n");
    printf("  -D      Disassemble data contents\n");
    printf("  -e      Execute program\n");
    printf("  -E      Execute program (debug trace mode)\n");
	printf("Options must not be repeated neither explicitly nor implicitly.");
}

//=======================================================================

bool parse_command_line_p4 
    (   int argc         , char **argv      ,
        bool *header     , bool *segments   , bool *membrief , 
        bool *memfull    , bool *disas_code , bool *disas_data ,
        bool *exec_normal, bool *exec_debug , char **file 
    )
{
	      //checks all null logic
    if(argc <= 1 ||argv == NULL || header == NULL || 
    membrief == NULL || segments == NULL || memfull == NULL || disas_code == NULL
	|| disas_data == NULL || exec_debug == NULL || exec_normal == NULL ) {
        usage_p4();
        return false;
    }

    *header = false;
    *segments = false;
    *membrief = false;
    *memfull = false;
	*disas_code = false;
    *disas_data = false;
	*exec_debug = false;
	*exec_normal = false;
    char *optionStr = "+hHafsmMDdeE";
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
			 usage_p4();
			 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
		    }
			*header = true; 
			break;
			
            case 'm': 
			if(*membrief) {
				usage_p4();
			 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}
			*membrief = true; 
			break;
			
            case 'M': 
			if(*memfull) {
				usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}
			*memfull = true; 
			break;
			
            case 's': 
			if(*segments) {
				usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}
			*segments = true; 
			break;
			
            case 'a': 
			
			if(*header || *membrief || *segments) {
			 usage_p4();
			 *header = false;
			 *membrief = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
		    }
			*header = true; 
			*membrief = true; 
			*segments = true; 
			break;
			
            case 'f':
			
			 if(*header || *memfull || *segments) {
			 usage_p4();
			 *header = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
				}
			
			*header = true; 
			*memfull = true; 
			*segments = true; 
			break;
			
			case 'd':
			if(*disas_code) {
				usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}
			*disas_code = true;
			break;
			
			case 'D':
			if(*disas_data) {
				usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}
			*disas_data = true;
			break;
			
			case 'E':
            if(*exec_debug) {
			      usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}	
			if (*exec_normal) {
				*exec_normal = false;
			}
			*exec_debug = true; 
			break;
			
            case 'e': 
			if(*exec_normal) {
			     usage_p4();
				 *header = false;
			 *membrief = false;
			 *memfull = false;
			 *segments = false;
			 *disas_code = false;
			 *disas_data = false;
			 *exec_debug = false;
			 *exec_normal = false;
			 return false;
			}			
			*exec_normal = true; 
			break;
			
            default: usage_p4(); 
			return false;
        }
    }
    
    //if printhelp is true prints Usage
    if(printHelp)
    {
        *header = false;
        usage_p4();
        return true;
    }
    else if(*membrief && *memfull)
    {
        usage_p4();
        return false;
    }
    else
    {
        //sets the last command arg to the file char array 
        *file = argv[optind];
        if(*file == NULL || argc > optind + 1)
        {

            usage_p4();
            return false;
        }
    
        return true;
    }
}

//=======================================================================

void dump_cpu( const y86_t *cpu ) 
{
	//dumps the rip register with the flags for the called execution
    printf("dump of Y86 CPU:\n");
	printf("  %%rip: %016lx   flags: SF%d ZF%d OF%d  ", cpu->pc, cpu->sf, cpu->zf, cpu->of);
	switch(cpu->stat) {
		case 1:
		printf("AOK\n");
		break;
		case 2:
		printf("HLT\n");
		break;
		case 3:
		printf("ADR\n");
		break;
		case 4:
		printf("INS\n");
		break;
	}
	// prints all registers with their values 
	printf("  %%rax: %016lx    %%rcx: %016lx\n", cpu->rax, cpu->rcx);
	printf("  %%rdx: %016lx    %%rbx: %016lx\n", cpu->rdx, cpu->rbx);
	printf("  %%rsp: %016lx    %%rbp: %016lx\n", cpu->rsp, cpu->rbp);
	printf("  %%rsi: %016lx    %%rdi: %016lx\n", cpu->rsi, cpu->rdi);
	printf("   %%r8: %016lx     %%r9: %016lx\n", cpu->r8, cpu->r9);
	printf("  %%r10: %016lx    %%r11: %016lx\n", cpu->r10, cpu->r11);
	printf("  %%r12: %016lx    %%r13: %016lx\n", cpu->r12, cpu->r13);
	printf("  %%r14: %016lx\n", cpu->r14);
	printf("\n");
}

//=======================================================================
void setRegister(y86_t *cpu , y86_register_t reg, y86_register_t  valE) {
	// helper method for setting the valE to the 
	//correct register based on the given register
	switch(reg) {
		case 0:
		cpu-> rax = valE;
		break;
		
		case 1:
		cpu-> rcx = valE;
		break;
		
		case 2:
		cpu-> rdx = valE;
		break;
		
		case 3:
		cpu-> rbx = valE;
		break;
		
		case 4:
		cpu-> rsp = valE;
		break;
		
		case 5:
		cpu-> rbp = valE;
		break;
		
		case 6:
		cpu-> rsi = valE;
		break;
		
		case 7:
		cpu-> rdi = valE;
		break;
		
		case 8:
		cpu-> r8 = valE;
		break;
		
		case 9:
		cpu-> r9 = valE;
		break;
		
		case 10:
		cpu-> r10 = valE;
		break;
		
		case 11:
		cpu-> r11 = valE;
		break;
		
		case 12:
		cpu-> r12 = valE;
		break;
		
		case 13:
		cpu-> r13 = valE;
		break;
		
		case 14:
		cpu-> r14 = valE;
		break;
	}
}
y86_register_t getRegVal(y86_t *cpu, y86_register_t reg) {
	//helper method for setting cpu to the given register value 
	switch(reg) {
		case 0:
		return cpu-> rax;
		break;
		
		case 1:
		return cpu-> rcx;
		break;
		
		case 2:
		return cpu-> rdx;
		break;
		
		case 3:
		return cpu-> rbx;
		break;
		
		case 4:
		return cpu-> rsp;
		break;
		
		case 5:
		return cpu-> rbp;
		break;
		
		case 6:
		return cpu-> rsi;
		break;
		
		case 7:
		return cpu-> rdi;
		break;
		
		case 8:
		return cpu-> r8;
		break;
		
		case 9:
		return cpu-> r9;
		break;
		
		case 10:
		return cpu-> r10;
		break;
		
		case 11:
		return cpu-> r11;
		break;
		
		case 12:
		return cpu-> r12;
		break;
		
		case 13:
		return cpu-> r13;
		break;
		
		case 14:
		return cpu-> r14;
		break;
		
		default:
		return 0;
		break;
	}
	return 0;
}
int addOvf(y86_register_t* result, int64_t a, int64_t b)
{ 
// helper method for overflow logic 
//uses the #include limits.h directory
//always returns result with the of set depending on if there is overflow
    if (a >= 0 && b >= 0 && (a > INT_MAX - b)) {
		*result = a + b;
        return -1;
    }
 
    else if (a < 0 && b < 0 && (a < INT_MIN - b)) {
		*result = a + b;
        return -1;
		
    }
    else {
        *result = a + b;
        return 0;
    }
}
y86_register_t decode_execute(  y86_t *cpu , bool *cond , const y86_inst_t *inst ,
                                y86_register_t *valA 
                             )
{
	// execution stage 


    y86_register_t valE;
	int64_t a;
	int64_t b;
	// if null cond or valA it is an invalid instruction
	   if(cond == NULL || valA == NULL)
    {
        cpu->stat = INS;
        return 0;

    }

 
  // switch cases for all inst types
  // assigns valA and valB then assgins the correct flags
	switch (inst->type) {
		case HALT:
		cpu->stat = HLT;
		break;
		
		case NOP:
		break;
		
		case IRMOVQ:
		valE = inst->value;
		break;
		
		case OPQ: ;
		a = getRegVal(cpu, inst->ra);
		b = getRegVal(cpu, inst->rb);
		cpu->sf = 0;
		cpu->zf = 0;
		cpu->of = 0;
		switch (inst->op) {
			case ADD:
			*valA = a;
			
			if (addOvf(&valE, b, *valA) < 0 ) {
			cpu->of = 1;
			}
			if (b + a < 0){
				cpu->sf = 1;
			}
			if (b + a == 0) {
				cpu->zf = 1;
			}
			
		   
			break;
			
			case SUB:
			*valA = a;
		
			if (addOvf(&valE, b, -*valA) < 0 ) {
			cpu->of = 1;
			}
			if (valE > b){
				cpu->sf = 1;
			}
			if (b - a < 0){
				cpu->sf = 1;
			}
			if (b - a == 0) {
				cpu->zf = 1;
			}
			
		
			break;
			
			case AND:
			*valA = a;
			
			valE = b & *valA;
			break;
			
			case XOR:
			*valA = a;
			
			valE = b ^ *valA;
			if (valE == 0) {
				cpu->zf = 1;
			}
			break;
			
			default:
			cpu->stat = INS;
		}
		break;
		
		case PUSHQ:
		a = getRegVal(cpu, inst->ra);
		b = getRegVal(cpu, RSP);
	
		*valA = a;
		valE = b - 8;
		break;
		
		case POPQ:
		a = getRegVal(cpu, RSP);
		b = getRegVal(cpu, RSP);
		
		*valA = a;
		valE = b + 8;
		break;
		
		case RMMOVQ:
		a = getRegVal(cpu, inst->ra);
		b = getRegVal(cpu, inst->rb);
	
		*valA = a;
		
		valE = b + inst->d;
		break;
		
		case MRMOVQ:
		b = getRegVal(cpu, inst->rb);
		
		valE = b + inst->d;
		break;
		
		case CALL:
		b = getRegVal(cpu, RSP);
	
		valE = b - 8;
		break;
		
		case RET:
		a = getRegVal(cpu, RSP);
		b = getRegVal(cpu, RSP);
		
		*valA = a;
		valE = b + 8;
		break;
		
		case CMOV:
		a = getRegVal(cpu, inst->ra);
		b = getRegVal(cpu, inst->rb);
		*valA = a;
	
		valE = *valA;
		switch (inst->cmov) {
			// does the correct the logic for cmov using flags
			case RRMOVQ:
			*cond = true;
			break;
			
			case CMOVLE:
			*cond = ((cpu->sf == 1) != (cpu->of == 1)) || cpu->zf == 1;
			break;
			
			case CMOVL:
			*cond = ((cpu->sf == 1) != (cpu->of == 1));
			break;
			
			case CMOVE:
			*cond = cpu->zf == 1;
			break;
			
			case CMOVNE:
			*cond = cpu->zf == 0;
			break;
			
			case CMOVGE:
			*cond = !((cpu->sf == 1) != (cpu->of == 1));
			break;
			
			case CMOVG:
			*cond = !((cpu->sf == 1) != (cpu->of == 1)) && cpu->zf == 0;
			break;
			
			default:
			cpu->stat = INS;
			*cond = false;
			break;
		}
		break;
		
		case JUMP:
		switch (inst->jump){
			// does correct logic for jump using flags
			case JMP:
			*cond = true;
			break;
			
			case JLE:
			*cond = ((cpu->sf == 1) != (cpu->of == 1)) || cpu->zf == 1;
			break;
			
			case JL:
			*cond = (cpu->sf == 1) != (cpu->of == 1);
			break;
			
			case JE:
			*cond = cpu->zf == 1;
			break;
			
			case JNE:
			*cond = cpu->zf == 0;
			break;
			
			case JGE:
			*cond = !((cpu->sf == 1) != (cpu->of == 1));
			break;
			
			case JG:
			*cond = !((cpu->sf == 1) != (cpu->of == 1)) && cpu->zf == 0;
			break;
				
			default:
			cpu->stat = INS;
			* cond = false;
			break;
			
		}
		break;
		
		default:
		cpu->stat = INS;
		break;
		
		
		
		
		
		
	}
    return valE;
}

//=======================================================================
void writeToMem(memory_t memory, y86_register_t  start, y86_register_t value) {
	// helper method for writing registers to memory
	// bit shift logic using the memory array
    printf("Memory write to 0x%04lx: 0x%lx\n", start, value); 
    for (int i = start; i < start + 8; i++) {
        memory[i] = value >> (8*i) & 0xff;
		
	}
	
    }
	y86_register_t readFromMem( memory_t memory, y86_register_t  start) {
		y86_register_t value;
	// helper method for reading to memory
	// starts at the last bit then converts to little endian
		for (int i = start + 7; i >= start; i--) {
			
			value = value << 8 | memory[i];
		}
		
		return value;
	}
	
void memory_wb_pc(  y86_t *cpu , memory_t memory , bool cond , 
                    const y86_inst_t *inst , y86_register_t  valE , 
                    y86_register_t  valA 
                 )
{
	// memory write back method 
	// checks null logic 
	    if(cpu->pc >= MEMSIZE || cpu->pc < 0 || memory == NULL || valE < 0 || valA < 0)
    {
        cpu->stat = ADR;
        cpu->pc = 0xffffffffffffffff;
        return;
    }
	int64_t valM;
	// switch for memory wb using the inst type 
	// goes through all the cases and sets registers using valE and the correct register
    switch (inst->type) {
		case HALT:
		cpu->pc += 1;
		cpu->sf = 0;
		cpu->zf = 0;
		cpu->of = 0;
		break;
		
		case NOP:
		cpu->pc += 1;
		break;
		
		case IRMOVQ:
		setRegister(cpu, inst->rb, valE);
		cpu->pc += 10;
		break;
		
		case OPQ:
		setRegister(cpu, inst->rb, valE);
		cpu->pc += 2;
		break;
		
		case PUSHQ:
		if(valE >=MEMSIZE)
            { 
                cpu->stat = ADR; 
                cpu->pc += 2;	
                break; 
            }
		writeToMem(memory, valE, valA);
		setRegister(cpu, RSP, valE);
		cpu->pc += 2;	
		break;

        case POPQ:;
		 if(valA >=MEMSIZE)
            { 
                cpu->stat = ADR; 
				cpu->pc += 2;
                break; 
            }
		valM = readFromMem(memory, valA);
		setRegister(cpu, RSP, valE);
		setRegister(cpu, inst->ra, valM);
		cpu->pc += 2;
		break;
		
		case RMMOVQ:
		 if(valE >=MEMSIZE) 
            {
                cpu->stat = ADR; 
                cpu->pc += 10;
                break;
            }
		writeToMem(memory, valE, valA);
		cpu->pc += 10;
		break;
		
		case MRMOVQ:
		 if(valE >=MEMSIZE) 
            {
				
                cpu->stat = ADR; 
				cpu->pc += 10;
                break;
            }
		valM = readFromMem(memory, valE);
		setRegister(cpu, inst->ra, valM);
		cpu->pc += 10;
		break;
		
		case CALL:
		if(valE >=MEMSIZE) 
            {
                cpu->stat = ADR; 
                cpu->pc = readFromMem(memory, cpu->pc + 1);
                break;
            }
		writeToMem(memory, valE, cpu->pc + 9);
		setRegister(cpu, RSP, valE);
		cpu->pc = readFromMem(memory, cpu->pc + 1);
		break;
		
		case RET:
		valM = 0;
		if(valA >=MEMSIZE)
            { 
                cpu->stat = ADR; 
                cpu->pc = valM;
                break; 
            }
		valM = readFromMem(memory, valA);
		setRegister(cpu, RSP, valE);
		cpu->pc = valM;
		break;
		
		case CMOV:
		if (cond){
			setRegister(cpu, inst->rb, valE);
		}
		cpu->pc += 2;
		break;
		
		case(JUMP):
        if(cond){ 
                cpu->pc = inst->dest;
                break;
            }
            cpu->pc += inst->size;
            break;
			
			case(INVALID): 
			cpu->stat = INS;
			break;
		
	}
}


