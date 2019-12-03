#pragma once
#include "miscellaneous.h"

void start();
void addToAllCommandLists(long long command, GenericList lists[]);
void freeCommandList(GenericList commands);
void add(void* data, GenericList list);
int monsterOn();
int findByPID(int pids[], int pid);
GenericList createEmptyList();