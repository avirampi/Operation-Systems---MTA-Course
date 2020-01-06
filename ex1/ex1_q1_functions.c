#include <math.h> // using for Sqrt
#include <stdio.h>
#include <stdlib.h>
#include "ex1_q1.h"

void (*functions[])(struct polygon*) = { &print_polygon, &print_perimeter, &print_area , &print_diagonals };
const char* polygonsNames[] = { POLYGON_HEXAGON , POLYGON_HEPTAGON, POLYGON_OCTAGON };
List *polygons;

void add_polygon(struct polygon* polygon) {
	Node* node = (Node*)calloc(1,sizeof(Node));
	if (node) node->poly = polygon; 

	if (polygons->head == NULL) { // list is empty
		polygons->head = polygons->tail = node;
	} else {
		polygons->tail->next = node;
		polygons->tail = node;
	}
}

void print_polygon(struct polygon* polygon) {
	printf("%s ", polygonsNames[polygon->type - POLYGON_BASE_INDEX]);

	for (VertexNode* current = polygon->verticesPoints_head; current != NULL; PRINT_SPACE_EXCEPT_LAST_ONE, current = current->next) {
		print_point(current->point);
	}

	//printf("\n");
}

void print_point(Point point) {
	printf(POLYGON_POINT_STRING, point.x, point.y);
}

void print_perimeter(struct polygon* polygon) {
	float perimeter = 0;

	for (VertexNode* current = polygon->verticesPoints_head; current != NULL; current = current->next)
		perimeter += (float)calculate_distance(current, current->next == NULL ? polygon->verticesPoints_head : current->next);

	printf(POLYGON_PERMITER_STRING, perimeter);
}

void print_diagonals(struct polygon* polygon) {
	double diagonalsLenghts = 0;

	for (VertexNode* current = polygon->verticesPoints_head; current->next->next != NULL; current = current->next) {
		for (VertexNode* innerNode = current->next->next; innerNode != NULL; innerNode = innerNode->next)
			diagonalsLenghts += calculate_distance(current, innerNode);
	}

	diagonalsLenghts -= calculate_distance(polygon->verticesPoints_head, polygon->verticesPoints_head->prev);
	printf(POLYGON_DIAGONALS_STRING, (float)diagonalsLenghts);
}

void print_area(struct polygon* polygon) {
	long long area = 0;

	for (VertexNode* current = polygon->verticesPoints_head; current != NULL; current = current->next) {
		area += calculate_part(current, current->next == NULL ? polygon->verticesPoints_head : NULL);
	}

	printf(POLYGON_AREA_STRING, fabsf((float)area)/2);
}

long long calculate_part(VertexNode* vertex, VertexNode* head) {
	return (vertex->point.x * (head == NULL ? vertex->next->point.y : head->point.y)) - (vertex->point.x * vertex->prev->point.y);
}

double calculate_distance(VertexNode* point_1, VertexNode* point_2) {
	return sqrt( pow((double)point_1->point.x - (double)point_2->point.x, 2) +
		pow((double)point_1->point.y - (double)point_2->point.y, 2));
}

void polygonEngine() {
	long long command, vertex[2];
	int keepTake = FALSE;
	polygon* current;
	polygons = (List*)calloc(1,sizeof(List));

	while (!keepTake) {
		scanf("%llx", &command);
		keepTake = command & 1;
		current = createPolygon((POLYGON)((command >> 2) & MASK_TAKE_FIRST_FOUR));
		if ((command >> 1) & 1) {
			scanf("%llx\n%llx", &vertex[0], &vertex[1]);
			convertVertices(vertex, current);
			add_polygon(current);
		}
		if (((command >> 10) & MASK_COMMAND) == FALSE) {
			activeFunctions(current, (command >> GET_ACTIONS) & MASK_COMMAND);
		} else {
			custom_forEach((command >> GET_ACTIONS) & MASK_COMMAND, command >> 10);
		}
	}
}

void freeList() {
	for (Node* current = polygons->head, *temp = current->next; current != NULL; current = temp, temp = (temp != NULL ? temp->next : temp)) {
		for (VertexNode* vertex = current->poly->verticesPoints_head, *tempo = vertex->next; vertex != NULL; vertex = tempo, tempo = (tempo != NULL ? tempo->next : tempo))
			free(vertex);
		free(current);
	}

	free(polygons);
}

polygon* createPolygon(POLYGON type) {
	polygon* newPolygon = (polygon*)malloc(sizeof(polygon));
	if (newPolygon) newPolygon->type = type;
	return newPolygon;
}

Point createPoint(int pointBits) {
	char x = pointBits & MASK_COMMAND, y = (pointBits >> MOVE_TO_Y_POINT) & MASK_COMMAND;
	return (Point){ .x = (int)x,.y =(int)y  };
}

void convertVertices(long long vertices[], polygon* polygon) {
	polygon->verticesPoints_head = (VertexNode*)malloc(sizeof(VertexNode));
	if (polygon->verticesPoints_head) polygon->verticesPoints_head->point = createPoint(vertices[0] & MASK_POLYGON_POINT);
	vertices[0] = vertices[0] >> SHIFT_TO_NEXT_POINT;
	VertexNode* current = polygon->verticesPoints_head, * nextNode;

	for (int i = 0, innerLoop = 3; i < VERTICES_ARRAY_SIZE; i++, innerLoop = polygon->type - FIRST_VERTICES_ARRAY) {
		for (int j = 0; j < innerLoop; j++, vertices[i] = vertices[i] >> SHIFT_TO_NEXT_POINT, current = nextNode) {
			nextNode = (VertexNode*)malloc(sizeof(VertexNode));
			if (nextNode) {
				nextNode->point = createPoint(vertices[i] & MASK_POLYGON_POINT);
				if (current) current->next = nextNode;
				nextNode->prev = current;
			}
		}
	}
	if(polygon->verticesPoints_head) polygon->verticesPoints_head->prev = current;
	current->next = NULL;
}


void activeFunctions(struct polygon* poly, int command) {
	for (int i = 0, bit = command & 1; i < 4; i++, command = command >> 1, bit = command & 1) {
		if (bit) functions[i](poly);
	}
}

void custom_forEach(int command, int type) {
	for (Node* current = polygons->head; current != NULL; current = current->next) {
		if (type == MASK_ALL_TYPES || current->poly->type == (POLYGON)(type))
			activeFunctions(current->poly, command);
	}
}