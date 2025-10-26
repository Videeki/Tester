#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "hashCalc.h"

int main(int argc, char* argv[])
{
    uint32_t ret = fastHash(argv[1], strlen(argv[1]));
    uint32_t expected = (uint32_t)atoi(argv[2]);

    return ret == expected ? EXIT_SUCCESS : EXIT_FAILURE;
}