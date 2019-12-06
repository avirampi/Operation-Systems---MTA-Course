#ifndef EX2_Q1_H
#define EX2_Q1_H
#include "miscellaneous.h"

void start();
void addToAllCommandLists(long long command, GenericList lists[], GenericList except);
void freeCommandList(GenericList commands);
void add(void* data, GenericList list);
int createChildern();
int findByPID(int pids[], int pid);
GenericList createEmptyList();

#endif /*EX2_Q1_H*/