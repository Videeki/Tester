#ifndef SEQUENCER_H
#define SEQUENCER_H

#include "interface.h"

#define InitPS      792116435
#define InitGateway 993770905
#define SocketINIT  549261615
#define SocketCOMM  3323704766
#define SocketCLEAN 2529897614
#define LogINIT     403310502
#define LogFREE     2255366703


//int CMDProc(Parameters* list, char* cmd);
int CMDProc(TESTER* data, char* cmd);
//void sequencer(Sequences* seqs, Keys* keys, Parameters* params, char* actSeq);
void sequencer(TESTER* self, const char* actSeq);

void saveSequence(TESTER* data, const char* path);

#endif  /* SEQUENCER_H */