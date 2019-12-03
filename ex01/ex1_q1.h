#pragma once
typedef enum POLYGON { HEXAGON = 6, HEPTAGON = 7, OCTAGON = 8 } POLYGON;

#define POLYGON_BASE_INDEX 6
#define POLYGON_HEXAGON "hexagon"
#define POLYGON_HEPTAGON "heptagon"
#define POLYGON_OCTAGON "octagon"
#define POLYGON_PERMITER_STRING "perimeter = %.1f\n"
#define POLYGON_DIAGONALS_STRING "diagonals = %.1f\n"
#define POLYGON_AREA_STRING "area = %.1f\n"
#define POLYGON_POINT_STRING "{%lld, %lld}"
#define SPACE " "
#define PRINT_SPACE_EXCEPT_LAST_ONE ((current->next != NULL) ? printf(SPACE) : printf("\n"))
#define GET_ACTIONS 6
#define FIRST_VERTICES_ARRAY 4 
#define SHIFT_TO_NEXT_POINT 16
#define VERTICES_ARRAY_SIZE 2
#define MOVE_TO_Y_POINT 8

//Masks
#define MASK_TAKE_FIRST_FOUR 0xF
#define MASK_COMMAND 0xFF
#define MASK_POLYGON_POINT 0xFFFF
#define MASK_ALL_TYPES 0xF

#define TRUE 1
#define FALSE 0

typedef struct polygon {
	POLYGON type;
	struct VertexNode* verticesPoints_head;
} polygon;

typedef struct Point {
	long long x, y;
} Point;

typedef struct VertexNode {
	struct Point point;
	struct VertexNode* next, * prev;
} VertexNode;

typedef struct Node {
	struct polygon* poly;
	struct Node* next;
} Node;

typedef struct List {
	struct Node* head, * tail;
} List;

// functions
void add_polygon(struct polygon* polygon);		// add new polygon to the list
void print_polygon(struct polygon* polygon);	// print the type of polygon and its vertices
void print_perimeter(struct polygon* polygon);	// calculate and print the perimeter
void print_diagonals(struct polygon* polygon);	// calculate and print the length of all diagonals
void print_area(struct polygon* polygon);		// calculate and print the area

// custom functions
double calculate_distance(VertexNode* point_1, VertexNode* point_2);
long long calculate_part(VertexNode* vertex, VertexNode* head);
void print_point(Point point);
void polygonEngine();
struct polygon* createPolygon(POLYGON type);
void convertVertices(long long vertices[], polygon* polygon);
Point createPoint(int pointBits);
void custom_forEach(int command, int type);
void activeFunctions(struct polygon* poly, int command);
void freeList();