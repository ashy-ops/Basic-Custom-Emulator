#include <stdio.h>
#include <stdint.h>

#define op_no    0
#define op_put   1
#define op_add   2
#define op_jumpf 3
#define op_jump  4
#define op_swap  7

//8 bits 0 0 0 0 0 0 0 0


// 00100001 01000001 00000001 00101011 01000001 00001001
//  4        8         0     jump to pc=5  dummy      1  
/*
OPCODES-

00000000--Nothinig                                                                  op_no
00000001--Put a number on stack                                                     op_put
00000010--Add the top number to another                                             op_add
00000011--Jump to another place if top number is 0                                  op_jumpf
00000100--Jump to another place no matter what                                      op_jump
00000111--Swap the top number on the stack                                          op_swap

*/


void OP_NO(){
    return;
}

void OP_PUT(uint8_t code, uint32_t stack[],uint8_t *sp){
    uint8_t num = code>>3;
    stack[*sp] = num;
    (*sp)++;
}

void OP_ADD(uint8_t code, uint32_t stack[],uint8_t *sp){
    uint8_t addrs = code>>3;
    stack[addrs] += stack[*sp - 1];

}

void OP_JUMPF(uint8_t code, uint32_t stack[],uint8_t *pc,uint8_t *sp){
    uint8_t addrs = code>>3;
    if(stack[*sp - 1]==0) *pc = addrs;
}

void OP_JUMP(uint8_t code,uint8_t *pc){
    uint8_t addrs = code>>3;
    *pc = addrs;
}

void OP_SWAP(uint8_t code, uint32_t stack[],uint8_t *sp){
    uint8_t addrs = code>>3;
    uint8_t temp = stack[addrs];
    stack[addrs] = stack[*sp - 1];
    stack[*sp - 1] = temp;
}



int main(){

    uint32_t stack[32] = {}; //using 32 because on addition the values can exceed 16 bit or 8 bit(tho for 16 bit you will have to do it a looot of times but in case of 8 bit it will exceed real quick) 
    uint8_t rom[32] = {}; //do not write to it only read

    uint8_t sp = 0; //PRE INCREMENT STACK POINTER
    uint8_t pc = 0;
    uint8_t opcode;
    

    FILE *fp = fopen("rom.bin","rb");

    if(fp == NULL){
        printf("ERROR IN LOADING THE FILE!");
        return 0 ;
    }

    uint8_t size = fread(rom,sizeof(uint8_t),32,fp); //storing into the buffer + fetching the size (aka no of instructions into size)
    fclose(fp);

    if(size>32){
        return 0;
    }
    while(1){

        opcode = rom[pc]; //FETCH
        uint8_t prev_pc = pc;
        //DECODE, EXECUTE
        switch( opcode & 0x07){ //0x07- 0000 0111
                case op_no: OP_NO(); break;
                case op_put: OP_PUT(opcode,stack,&sp); break;
                case op_add: OP_ADD(opcode,stack,&sp); break;
                case op_jumpf: OP_JUMPF(opcode,stack,&pc,&sp); break;
                case op_jump: OP_JUMP(opcode,&pc); break;
                case op_swap: OP_SWAP(opcode,stack,&sp); break;

            }
        
        //INCREMENTATION OF PROGRAM COUNTER
        if(pc==prev_pc) pc++;
        if(pc==32 || pc==size) break; //can loopback as well here but meh
        printf("%d",stack[sp-1]);
        printf("\n");
    }
    printf("FINAL VALUE AT THE TOP OF THE STACK IS:\n");
    printf("%d",stack[sp-1]);
    
}