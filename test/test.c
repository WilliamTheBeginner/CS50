#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  if (argc != 2)
    {
        fprintf(stderr, "Usage: resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    //char *infile = argv[2];
    //char *outfile = argv[3];
    //char *n = argv[1];

    int *n;

    int x = atoi(argv[1]);

    n = (int*) malloc(sizeof(int));



    //int factor = &argv[1] - '0';

    n = &x;

    printf("%d\n", *n);

    *n = *n * 5;

    printf("%d\n", *n);

}