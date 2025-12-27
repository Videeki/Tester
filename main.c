#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "includes/parser.h"
#include "includes/sequencer.h"
//#include "includes/memManager.h"
#include "includes/socketClient.h"


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        perror("Few parameters have been specified\n");
        return EXIT_FAILURE;
    }

    PARSED testDescFile;
    testDescFile.seqs = NULL;
    testDescFile.keys = NULL;
    testDescFile.params = NULL;

    int ret = 0;
    ret = parser(&testDescFile, argv[1]);
    
    if(ret)
    {
        perror("Unsuccessfull parsing");
        return EXIT_FAILURE;
    }

    if(testDescFile.seqs == NULL)
        perror("There is no any sequence\n");

    if(testDescFile.keys == NULL)
        perror("There is no any key\n");

    if(testDescFile.params == NULL)
        perror("There is no any parameter\n");

    char startSequence[255] = {0};
    if(argc > 2)
        strncpy(startSequence, argv[2], strlen(argv[2]));
    else
        strncpy(startSequence, testDescFile.seqs->sequence, strlen(testDescFile.seqs->sequence));

    TESTER tester;
    tester.desc = &testDescFile;
    //sequencer(testDescFile.seqs, testDescFile.keys, testDescFile.params, startSequence);
    sequencer(&tester, startSequence);

    socketClientListAll_free(tester.sockList);

    keys_free(testDescFile.keys);
    param_free(testDescFile.params);
    sequence_free(testDescFile.seqs);

    return 0;
}