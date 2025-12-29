#ifndef MEMMANAGER_H
#define MEMMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "sequencer.h"
#include "stringUtils.h"
#include "socketClient.h"
#include "serialClient.h"


static void seqListFree(void** seqList);
static void parListFree(void** parList);
static void keyListFree(void** keyList);
static void sockClientListFree(void** sockClientList);

#define SEQLIST(buffer) Sequences* buffer __attribute__((cleanup(seqListFree))) = NULL;
#define PARLIST(buffer) Parameters* buffer __attribute__((cleanup(parListFree))) = NULL;
#define KEYLIST(buffer) Keys* buffer __attribute__((cleanup(keyListFree))) = NULL;

#define SOCKCLIENTLIST(buffer) SOCKETCLIENTLIST* buffer __attribute__((cleanup(sockClientListFree))) = NULL;

void memoryFree(void* mem);

#endif  /* MEMMANAGER_H */