#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu -> ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu -> ram[address] = value;
}
/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *path)
{
  // DAY 1:
  // char data[DATA_LEN] = {
  //   // From print8.ls8
  //   0b10000010, // LDI R0,8
  //   0b00000000,
  //   0b00001000,
  //   0b01000111, // PRN R0
  //   0b00000000,
  //   0b00000001  // HLT
  // };
  // //
  // int address = 0;
  //
  // for (int i = 0; i < DATA_LEN; i++) {
  //   cpu->ram[address++] = data[i];
  // }

  FILE *f = fopen(path, "r");
  char line[1024];
  unsigned char ram_address = 0x00;

  if (f == NULL)
  {
    fprintf(stderr, "Couldn't open file\n");
    exit(1);
  }

  while (fgets(line, sizeof(line), f) != NULL)
  {
    char *end_ptr;
    unsigned char new_line = strtoul(line, &end_ptr, 2);
    if (line == end_ptr)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, new_line);
  }
  fclose(f);

  // TODO: Replace this with something less hard-coded
}


/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  // (void)cpu;
  // (void)regA;
  // (void)regB;

  switch (op) {
    case ALU_MUL:
      // TODO
      cpu -> reg[regA] *= cpu -> reg[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
    cpu -> reg[regA] += cpu -> reg[regB];
    break;
  }
}


/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction
  unsigned char IR;
  unsigned char opA;
  unsigned char opb;

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    IR = cpu_ram_read(cpu, cpu -> PC);
    // 2. Figure out how many operands this next instruction requires
    // unsigned char operands = (IR >> 6) + 1;
    // 3. Get the appropriate value(s) of the operands following this instruction
    opA = cpu_ram_read(cpu, cpu -> PC + 1 & 0xff);
    opB = cpu_ram_read(cpu, cpu -> PC + 2 & 0xff);
    // 4. switch() over it to decide on a course of action.

    int next = (IR >> 6) + 1;

    address = cpu -> PC;

    printf("TRACE: %02X | %02X %02X %02X |", cpu->PC, IR, opA, opB);

    switch(IR)
    {
      // 5. Do whatever the instruction should do according to the spec.
      case LDI:
      cpu -> reg[opA] = opB;
      break;

      case PRN:
      printf("%d\n", cpu -> reg[opA]);
      break;

      case HLT:
      running = 0;
      break;

      case MUL:
      alu(cpu, ALU_MUL, opA, opB);
      break;

      case ADD:
      alu(cpu, ALU_ADD, opA, opB);
      break;

      default:
      printf("I don't know this: 0x%02x at 0x%02x\n", IR, cpu -> PC);
      running = 0;
      break;
    }

    // 6. Move the PC to the next instruction.
    cpu -> PC += next;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu -> PC = 0;
  memset(cpu->reg, 0, sizeof(cpu->reg));
  memset(cpu->ram, 0, sizeof(cpu->ram));
}
