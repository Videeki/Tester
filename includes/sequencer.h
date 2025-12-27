#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "parser.h"
#include "hashCalc.h"
#include "stringUtils.h"
#include "socketClient.h"
#include "serialClient.h"
#include "logger.h"


#define InitPS      792116435
#define InitGateway 993770905
#define SocketINIT  549261615
#define SocketCOMM  3323704766
#define SocketCLEAN 2529897614
#define LogINIT     403310502
#define LogFREE     2255366703


typedef struct tester
{
    PARSED* desc;
    SOCKETCLIENTLIST* sockList;
}TESTER;

//int CMDProc(Parameters* list, char* cmd);
int CMDProc(TESTER* data, char* cmd);
//void sequencer(Sequences* seqs, Keys* keys, Parameters* params, char* actSeq);
void sequencer(TESTER* data, const char* actSeq);

#endif  /* SEQUENCER_H */