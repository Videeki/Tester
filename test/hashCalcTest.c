#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hashCalc.h"

int main(int argc, char* argv[])
{
    int ret = SuperFastHash(argv[1], strlen(argv[1]));
    return ret == atoi(argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;
}