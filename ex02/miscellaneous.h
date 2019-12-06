#ifndef MISCELLANEOUS_H
#define MISCELLANEOUS_H
#include <math.h> // using for Sqrt
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>     // for exit()
#include <sys/types.h>
#include <unistd.h>    // for sleep(), execvp(), pipe()
#include <sys/wait.h>   // for wait()
#include <termios.h> 
#include <fcntl.h>                                                                                                                                             
#include <sys/stat.h>                                                      

typedef enum POLYGON { HEXAGON = 6, HEPTAGON = 7, OCTAGON = 8 } POLYGON;

//Ex02 In\Out files
#define HEX_IN								"hex_in.tmp"
#define HEX_OUT								"hex_out.log"
#define HEPT_IN								"hept_in.tmp"
#define HEPT_OUT							"hept_out.log"
#define OCT_IN								"oct_in.tmp"
#define OCT_OUT								"oct_out.log"
#define HELPER_PROG							"./ex2_q1_helper"

//Arrays sizes
#define VERTICES_ARRAY_SIZE					2
#define	POLYGON_TYPES_ARRAY_SIZE			3
#define POLYGON_BASE_INDEX					HEXAGON

#define POLYGON_HEXAGON						"hexagon"
#define POLYGON_HEPTAGON					"heptagon"
#define POLYGON_OCTAGON						"octagon"
#define POLYGON_PERMITER_STRING				"perimeter = %.1f \n"
#define POLYGON_DIAGONALS_STRING			"diagonals = %.1f \n"
#define POLYGON_AREA_STRING					"area = %.1f \n"
#define POLYGON_POINT_STRING				"{%lld, %lld}"
#define END_OF_FILE							"-1"
#define SPACE								" "

//pipes, redirections
#define INPUT_END							0                             
#define OUTPUT_END							1    
#define OPEN_FLAGS							O_CREAT | O_RDWR

//Errors
#define INVALID_POLYGON_TYPE				"Invalid polygon type entered, application failed!"


//Masks
#define GET_ACTIONS							6
#define FIRST_VERTICES_ARRAY				4
#define SHIFT_TO_NEXT_POINT					16
#define VERTICES_ARRAY_SIZE					2
#define MOVE_TO_Y_POINT						8
#define MASK_TAKE_FIRST_FOUR				0xF
#define MASK_COMMAND						0xFF
#define MASK_POLYGON_POINT					0xFFFF
#define MASK_ALL_TYPES						0xF
#define MASK_NEW_POLYGON_OFF				0xFFFD
#define PRINT_CURRENT_POLYGON_ONLY			0
#define MASK_NEW_POLYGON(command)			(command >> 1) & 1
#define MASK_POLYGON_TYPE(command)			((command >> 2)& MASK_TAKE_FIRST_FOUR)
#define MASK_PRINT_TYPE(command)			((command >> 10) & MASK_COMMAND)
#define MASK_GET_POLYGON_TYPE(c)			(c & 0x3c)
#define MASK_GET_WHAT_TO_DO(c)				(c & 0x3c0)
#define MASK_GET_JUST_WHAT_TO_PRINT(c)		(c & 0x3FC0)

struct genericNode {
	void* data;
	struct genericNode* next, * prev;
};

struct genericList {
	struct genericNode* head, * tail;
};

struct polygon {
	POLYGON type;
	struct genericNode* vertices;
};

struct point {
	long long x, y;
};

typedef struct polygon* Polygon;
typedef struct point* Point;
typedef struct genericList* GenericList;
typedef struct genericNode* GenericNode;
typedef struct genericNode* Vertex;
typedef char* String;
typedef long long* Command;
typedef FILE* File;

#define SIZE_OF_GENERIC_NODE	sizeof(struct genericNode)
#define SIZE_OF_GENERIC_LIST	sizeof(struct genericList)
#define SIZE_OF_POLYGON			sizeof(struct polygon)
#define SIZE_OF_POINT			sizeof(struct point)
#define SIZE_OF_VERTEX			SIZE_OF_GENERIC_NODE
#define SIZE_OF_COMMAND			sizeof(long long)
#define NEW_COMMAND				(Command)calloc(1,SIZE_OF_COMMAND)
#define CONVERT_TO_LL(c)		*((long long*)(c))		
#define PTR(c)					*c

#endif /*MISCELLANEOUS_H*/