#include <stdio.h>
#include "cpu.h"
#include <stdlib.h>

/**
 * Main
 */
int main(int argc, char **argv)
{
  struct cpu cpu;
  char *path;

  if (argc == 2)
  {
    path = argv[1];
  }
  else
  {
    fprintf(stderr, "got a ./ls8 filename? \n");
    exit(1);
  }

  cpu_init(&cpu);
  cpu_load(&cpu, path);
  cpu_run(&cpu);

  return 0;
}
