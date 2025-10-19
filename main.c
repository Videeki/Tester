#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "sequencer.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        perror("Few parameters have been specified\n");
        return EXIT_FAILURE;
    }

    Sequences* seqs = NULL;
    Keys* keys = NULL;
    Parameters* params = NULL;

    parser(seqs, keys, params, argv[1]);

    sequencer(seqs, keys, params, argv[2]);

    keys_free(keys);
    param_free(params);
    sequence_free(seqs);

    return 0;
}