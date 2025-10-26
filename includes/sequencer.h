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


#define InitTPS     792116435
#define InitGateway 993770905



int CMDProc(Parameters* list, char* cmd);
void sequencer(Sequences* seqs, Keys* keys, Parameters* params, char* actSeq);

#endif  /* SEQUENCER_H */