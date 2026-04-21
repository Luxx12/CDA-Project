#include "spimcore.h"

// done by Gabriel Barreto Otero
/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch (ALUControl)
    {
        case 0:
            // addition: add A and B together and store the result
            // used by add, addi, lw, and sw for address calculation
            *ALUresult = A + B;
            break;

        case 1:
            // subtraction: subtract B from A
            // used by sub and beq 
            // beq compares by checking if A - B == 0
            *ALUresult = A - B;
            break;

        case 2:
            // signed set-less-than: treats A and B as signed (twos complement)
            // integers. If A is less than B, result is 1, otherwise 0
            //   Used by slt and slti
            *ALUresult = ((int)A < (int)B) ? 1 : 0;
            break;

        case 3:
            // Unsigned set less than: same idea as case 2 but A and B are treated as plain positive numbers with no sign bit
            // Used by sltu and sltiu. 
            *ALUresult = (A < B) ? 1 : 0;
            break;

        case 4:
            // Bitwise AND: compares each pair of bits result bit is 1 only when both input bits are 1
            // used by the and instruction. 
            *ALUresult = A & B;
            break;

        case 5:
            // Bitwise OR: compares each pair of bits result bit is 1 when at least one input bit is 1
            // Used by the or instruction. 
            *ALUresult = A | B;
            break;

        case 6:
            /* 
               shift B left by 16 bits, filling the vacated low bits with 0s
               this is how lui loads a constant into the upper half of a
               register, the  value arrives as the low 16 bits of B
               and gets moved into position by this shift. 
            */
            *ALUresult = B << 16;
            break;

        case 7:
            // Bitwise NOT: flips every bit of A (0 becomes 1, 1 becomes 0)
            // B is ignored here
            *ALUresult = ~A;
            break;

        default:
            // ALUControl value does not match any known operation
            // set result to 0 to avoidd any errors just in case
            *ALUresult = 0;
            break;
    }

    // Zero flag: the rest of the datapath uses this to detect equality 
    //  set it to 1 if the result is zero, 0 otherwise. 
    *Zero = (*ALUresult == 0) ? 1 : 0;
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

}

// done by Gabriel Barreto Otero
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    // shift the instruction right 26 bits to isolate the top 6 bits
    *op = instruction >> 26;

    // shift right 21 bits then mask off everything above bit 4
    *r1 = instruction >> 21;
    *r1 = *r1 & 0x1F;

    // shift right 16 bits, then mask to 5 bits
    *r2 = instruction >> 16;
    *r2 = *r2 & 0x1F;

    // shift right 11 bits, then mask to 5 bits 
    *r3 = instruction >> 11;
    *r3 = *r3 & 0x1F;

    // bottom 6 bits for funct
    *funct = instruction & 0x3F;

    // bottom 16 bits for offset/immediate 
    *offset = instruction & 0xFFFF;

    // bottom 26 bits for jump target 
    *jsec = instruction & 0x3FFFFFF;
}


// Done by Carlos Calimano
/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

    if(op== 0x00){

	controls->RegDst=1;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=0;
	controls->MemWrite=0;
	controls->ALUSrc=0;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=7;
}
else if(op == 0x08){
//addi
	controls->RegDst=0;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=0;
	controls->MemWrite=0;
	controls->ALUSrc=1;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=0;

}
else if(op == 0x23){ // lw


	controls->RegDst=0;
	controls->Branch=0;
	controls->MemRead =1;
	controls->MemtoReg=1;
	controls->MemWrite=0;
	controls->ALUSrc=1;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=0;
}
else if(op ==0x2B){ //sw

	controls->RegDst=2;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=2;
	controls->MemWrite=1;
	controls->ALUSrc=1;
	controls->RegWrite=0;
	controls->Jump=0;
	controls->ALUOp=0;
}
else if(op==0x0F){
//lui

	controls->RegDst=0;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=0;
	controls->MemWrite=0;
	controls->ALUSrc=1;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=6;
}

else if(op == 0x04){
//beq
	controls->RegDst=2;
	controls->Branch=1;
	controls->MemRead =0;
	controls->MemtoReg=2;
	controls->MemWrite=0;
	controls->ALUSrc=0;
	controls->RegWrite=0;
	controls->Jump=0;
	controls->ALUOp=1;
}

//slti
else if(op == 0x0A){
	controls->RegDst=0;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=0;
	controls->MemWrite=0;
	controls->ALUSrc=1;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=2;

}
else if(op == 0x0B){
//sltiu

	controls->RegDst=0;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=0;
	controls->MemWrite=0;
	controls->ALUSrc=1;
	controls->RegWrite=1;
	controls->Jump=0;
	controls->ALUOp=3;
}

else if(op == 0x02){ //jump

	controls->RegDst=2;
	controls->Branch=0;
	controls->MemRead =0;
	controls->MemtoReg=2;
	controls->MemWrite=0;
	controls->ALUSrc=2;
	controls->RegWrite=0;
	controls->Jump=1;
	controls->ALUOp=0;

}
else{
	return 1;
}
return 0;

}

// done by Gabriel Barreto Otero
/* Read Register */
/* 5 Points */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


// done by Gabriel Barreto Otero
/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    // check if bit 15 is a 1
    if ((offset & 0x8000) == 0x8000)
    {
        // if its negative then fill the upper 16 bits with 1's
        *extended_value = offset | 0xFFFF0000;
    }
    else
    {
        // if its positive then just make sure upper bits are 0 
        *extended_value = offset & 0x0000FFFF;
    }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{
    
}

// Done by Carlos Calimano
/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
	if (MemRead == 1 || MemWrite == 1){
if (ALUresult % 4 != 0){
	return 1;
}

	if(ALUresult >= 65536){

return 1;
	}
}
if (MemRead == 1){
*memdata = Mem[ALUresult >> 2];

	}
	if(MemWrite == 1){

		Mem[ALUresult >> 2] = data2;
	}

	return 0;
}


/* Write Register */
/* 10 Points */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{

}

// Done by Carlos Calimano
/* PC update */
/* 10 Points */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
*PC = *PC + 4;

if (Branch == 1 && Zero == 1){

	*PC = *PC + (extended_value << 2);
}


if (Jump == 1 ){

	*PC = (*PC & 0xF0000000) | (jsec << 2);
}
}

