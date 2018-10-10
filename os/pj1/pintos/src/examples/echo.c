#include <stdio.h>
#include <syscall.h>

int
main (int argc, char **argv)
{
  int i;
  printf("1111\n");
  for (i = 0; i < argc; i++)
    printf ("%s ", argv[i]);
  printf ("\n");

  return EXIT_SUCCESS;
}
