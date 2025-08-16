#include <stdio.h>

int main(int argc, char *argv[]) {

  for (int i = 0; i < argc; ++i) {
    printf("%s \n", argv[i]);
  }

  if (argc != 2) {
    printf("Wrong number of args\n");
    return 1;
  }

  printf("Do something I guess...\n");

  return 0;
}
