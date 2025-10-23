#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "stringUtils.h"

int main(int argc, char* argv[])
{
    int ret = string_trim_before(argv[1], 0);
    return ret == atoi(argv[2]) ? EXIT_SUCCESS : EXIT_FAILURE;
}

