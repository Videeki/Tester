#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/parser.h"
#include "includes/sequencer.h"


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

    int ret = 0;
    ret = parser(seqs, keys, params, argv[1]);
    
    if(ret)
    {
        perror("Unsuccessfull parsing");
        return EXIT_FAILURE;
    }

    if(seqs == NULL)
        perror("There is no any sequence\n");

    if(keys == NULL)
        perror("There is no any key\n");

    if(params == NULL)
        perror("There is no any parameter\n");

    sequencer(seqs, keys, params, argv[2]);

    keys_free(keys);
    param_free(params);
    sequence_free(seqs);

    return 0;
}