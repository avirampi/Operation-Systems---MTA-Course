#include "ex2_q1.h"

String inFiles[] = { HEX_IN , HEPT_IN , OCT_IN }, outFiles[] = { HEX_OUT , HEPT_OUT , OCT_OUT };
const char* polygonsNames[] = { POLYGON_HEXAGON , POLYGON_HEPTAGON, POLYGON_OCTAGON };

void add(void* data, GenericList list) {
	GenericNode node = (GenericNode)calloc(1, SIZE_OF_GENERIC_NODE);
	if (node) node->data = data;

	if (list->head == NULL) { // list is empty
		list->head = list->tail = node;
	}
	else {
		list->tail->next = node;
		list->tail = node;
	}
}

GenericList createEmptyList() {
	return (GenericList)calloc(1, SIZE_OF_GENERIC_LIST);
}

void printToFile(GenericList commands, String in) {
	File inFile = fopen(in, "w+");
	for (GenericNode current = commands->head; current != NULL; current = current->next) {
		fprintf(inFile, "%llx\n", CONVERT_TO_LL(current->data));
	}

	fprintf(inFile, END_OF_FILE);
	fclose(inFile);
}

void freeCommandList(GenericList commands) {
	GenericNode temp, current = commands->head;
	while (current != NULL) {
		temp = current->next;
		free(current->data);
		free(current);
		current = temp;
	}

	free(commands);
}

void addToAllCommandLists(long long command, GenericList lists[]) {
	for (size_t i = 0; i < POLYGON_TYPES_ARRAY_SIZE; ++i) {
		Command newCommand = NEW_COMMAND;
		if (newCommand) {
			PTR(newCommand) = MASK_GET_JUST_WHAT_TO_PRINT(command);
			add(newCommand, lists[i]);
		}
	}
}

int findByPID(int pids[], int pid) {
	for (size_t i = 0; i < POLYGON_TYPES_ARRAY_SIZE; ++i) {
		if (pids[i] == pid) {
			return i;
		}
	}
	
	return -1;
}

int monsterOn() {
	int save_stdin = dup(STDIN_FILENO), save_stdout = dup(STDOUT_FILENO),
		pids[POLYGON_TYPES_ARRAY_SIZE], status, wpid;
	char *args[2];
	args[0] = HELPER_PROG;
	args[1] = NULL;
	for (size_t i = 0; i < POLYGON_TYPES_ARRAY_SIZE; ++i) {
		int fd[2];
		if (pipe(fd)) {
			return -1;
		}
		pids[i] = fork();
		switch (pids[i]) {
		case -1:
			return -1;
		case 0: // child
			fd[INPUT_END] = open(inFiles[i], OPEN_FLAGS, S_IWUSR | S_IRUSR);
			dup2(fd[INPUT_END], STDIN_FILENO);
			close(fd[INPUT_END]);
			fd[OUTPUT_END] = open(outFiles[i], OPEN_FLAGS, S_IWUSR | S_IRUSR);
			dup2(fd[OUTPUT_END], STDOUT_FILENO);
			close(fd[OUTPUT_END]);
			execve(HELPER_PROG, args, NULL);
		default:
			close(fd[INPUT_END]);
			close(fd[OUTPUT_END]);
		}
	}
	
	dup(save_stdin);
	dup(save_stdout);
	while ((wpid = wait(&status)) > 0) {
		int typeIndex = findByPID(pids, wpid);
		if (typeIndex >= 0) {
			printf("child terminated – created %d %s\n",WEXITSTATUS(status), polygonsNames[typeIndex]);
		}
	}
	
	printf("all child processes terminated");
	return 0;
}

void start() {
	GenericList commandsArray[POLYGON_TYPES_ARRAY_SIZE] = { createEmptyList(), createEmptyList(),createEmptyList() };
	GenericList commandsList;
	long long command;
	bool finishInput = false;
	Command vertices[VERTICES_ARRAY_SIZE];

	while (!finishInput) {
		scanf("%llx", &command);
		finishInput = command & 1;
		Command newCommand = NEW_COMMAND;
		if (newCommand) {
			PTR(newCommand) = finishInput;
			PTR(newCommand) |= (MASK_NEW_POLYGON(command)) << 1;
			int polygonType = MASK_POLYGON_TYPE(command);
			if (polygonType >= HEXAGON && polygonType <= OCTAGON)
				commandsList = commandsArray[polygonType - POLYGON_BASE_INDEX];
			else {
				printf(INVALID_POLYGON_TYPE);
				return;
			}
			PTR(newCommand) |= MASK_GET_POLYGON_TYPE(command);
			PTR(newCommand) |= MASK_GET_WHAT_TO_DO(command);
			if (MASK_PRINT_TYPE(command) == MASK_ALL_TYPES) {
				addToAllCommandLists(command, commandsArray);
			}
			if ((MASK_PRINT_TYPE(command) - POLYGON_BASE_INDEX) == polygonType) {
				PTR(newCommand) |= MASK_PRINT_TYPE(command) << 10;
			}
			add(newCommand, commandsList);
			if (MASK_NEW_POLYGON(PTR(newCommand))) {
				for (size_t i = 0; i < VERTICES_ARRAY_SIZE; ++i) {
					vertices[i] = NEW_COMMAND;
					scanf("%llx", vertices[i]);
					add(vertices[i], commandsList);
				}
			}
		}
	}

	if (monsterOn() < 0) {
		fprintf(stderr,"error while forking childers\n");
	}

	for (size_t i = 0; i < POLYGON_TYPES_ARRAY_SIZE; ++i) {
		printToFile(commandsArray[i], inFiles[i]);
		freeCommandList(commandsArray[i]);
	}
}