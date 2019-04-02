#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define LR 7
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

  FILE *f;
  char buffer[512];
  f = fopen(path, "r");
  unsigned char address = 0;

  while (fgets(buffer, sizeof(buffer), f) != NULL)
  {
    char *end;
    unsigned char string = strtol(buffer, &end, 2);
    if (buffer == end)
    {
      continue;
    }
    cpu -> ram[address++] = string;
  }
  fclose(f);

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char address)
{
  return cpu -> ram[address];
}

void cpu_ram_write(struct cpu *cpu, unsigned char address, unsigned char value)
{
  cpu -> ram[address] = value;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  (void)cpu;
  (void)regA;
  (void)regB;

  switch (op) {
    case ALU_MUL:
      // TODO
      cpu -> registers[regA] *= cpu -> registers[regB];
      break;

    // TODO: implement more ALU ops
  }
}

void push(struct cpu *cpu, unsigned char value)
{
  cpu -> registers[LR] = cpu -> registers[LR] - 1;
  cpu_ram_write(cpu, cpu -> registers[LR], value);
}

void pop(struct cpu *cpu)
{
  unsigned char _ram = cpu -> ram[cpu -> registers[LR]];
  cpu -> registers[LR] = cpu -> registers[LR] + 1;
  return _ram;
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running) {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    unsigned char IR = cpu_ram_read(cpu, cpu -> PC);
    // 2. Figure out how many operands this next instruction requires
    unsigned char operands = IR >> 6;
    // 3. Get the appropriate value(s) of the operands following this instruction
    unsigned char op0 = cpu_ram_read(cpu, cpu -> PC + 1);
    unsigned char op1 = cpu_ram_read(cpu, cpu -> PC + 2);
    // 4. switch() over it to decide on a course of action.
    switch(IR)
    {
      // 5. Do whatever the instruction should do according to the spec.
      case LDI:
      cpu -> registers[op0] = op1;
      break;

      case PRN:
      printf("%d\n", cpu -> registers[op0]);
      break;

      case HLT:
      running = 0;
      break;

      case MUL:
      alu(cpu, ALU_MUL, op0, op1);
      break;

      case PUSH:
      push(cpu, cpu -> registers[op0]);
      break;

      case POP:
      cpu -> registers[op0] = pop(cpu);
      break;

      default:
      printf("I don't know this: %d\n", IR);
      running = 0;
      break;
    }

    // 6. Move the PC to the next instruction.
    cpu -> PC += (operands + 1);
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu -> PC = 0;
  memset(cpu -> registers, 0, 8);
  memset(cpu -> ram, 0, 256);
}
