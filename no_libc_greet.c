#include <stdio.h>

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("I don't know to greet you\n");
        return 1;
    }
    printf("Hello, dear %s\n", argv[1]);
    return 0;
}