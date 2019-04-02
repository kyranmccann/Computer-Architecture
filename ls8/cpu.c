#include "cpu.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DATA_LEN 6
#define LR 7

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

  FILE *f;

  f = fopen(path, "r");
  if (f == NULL)
  {
    fprintf(stderr, "Couldn't open file\n");
    exit(1);
  }
  unsigned char address = 0;
  unsigned char line[1024];

  while (fgets(line, sizeof(line), f) != NULL)
  {
    char *end;
    unsigned char new_line = strtol(line, &end, 2);
    if (line == end)
    {
      continue;
    }
    cpu_ram_write(cpu, address++, new_line);
    cpu -> ram[address++] = new_line;
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
      cpu -> registers[regA] *= cpu -> registers[regB];
      break;

    // TODO: implement more ALU ops
    case ALU_ADD:
    cpu -> registers[regA] += cpu -> registers[regB];
    break;
  }
}

// void push(struct cpu *cpu, unsigned char value)
// {
//   cpu -> registers[LR] = cpu -> registers[LR] - 1;
//   cpu_ram_write(cpu, cpu -> registers[LR], value);
// }
//
// void pop(struct cpu *cpu)
// {
//   unsigned char _ram = cpu -> ram[cpu -> registers[LR]];
//   cpu -> registers[LR] = cpu -> registers[LR] + 1;
//   return _ram;
// }

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
    unsigned char operands = (IR >> 6) + 1;
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

      case ADD:
      alu(cpu, ALU_ADD, op0, op1);
      break;

      case PUSH:
      cpu -> registers[op0] = cpu -> ram[cpu -> registers[7]];
      cpu -> registers[7]++;
      break;

      case POP:
      cpu -> registers[7]--;
      cpu -> ram[cpu -> registers[7]] = cpu -> registers[op0];
      break;

      case CALL:
      cpu -> registers[7]--;
      cpu_ram_write(cpu, cpu -> registers[7], cpu -> PC + (operands - 1));
      cpu -> PC = cpu -> registers[op0] - operands;
      break;

      case RET:
      cpu -> PC = cpu_ram_read(cpu, cpu -> registers[7]);
      cpu -> registers[7]++;
      break;

      default:
      printf("I don't know this: %d\n", IR);
      running = 0;
      break;
    }

    // 6. Move the PC to the next instruction.
    cpu -> PC += operands;
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
