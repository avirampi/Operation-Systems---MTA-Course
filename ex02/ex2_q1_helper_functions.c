#include "ex2_q1_helper.h"

GenericList polygons;
const char* polygonsNames[] = { POLYGON_HEXAGON , POLYGON_HEPTAGON, POLYGON_OCTAGON };
void (*functions[])(Polygon) = { &print_polygon, &print_perimeter, &print_area , &print_diagonals };

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

void print_polygon(Polygon polygon) {
	printf("%s ", polygonsNames[polygon->type - POLYGON_BASE_INDEX]);

	for (GenericNode current = polygon->vertices; current != NULL; current = current->next, printf(SPACE)) {
		print_point((Point)current->data);
	}

	printf("\n");
}

void print_point(Point point) {
	printf(POLYGON_POINT_STRING, point->x, point->y);
}

void print_perimeter(Polygon polygon) {
	float perimeter = 0;

	for (GenericNode current = polygon->vertices; current != NULL; current = current->next)
		perimeter += (float)calculate_distance(current, current->next == NULL ? polygon->vertices : current->next);

	printf(POLYGON_PERMITER_STRING, perimeter);
}

void print_diagonals(Polygon polygon) {
	double diagonalsLenghts = 0;

	for (GenericNode current = polygon->vertices; current->next->next != NULL; current = current->next) {
		for (GenericNode innerNode = current->next->next; innerNode != NULL; innerNode = innerNode->next)
			diagonalsLenghts += calculate_distance(current, innerNode);
	}

	diagonalsLenghts -= calculate_distance(polygon->vertices, polygon->vertices->prev);
	printf(POLYGON_DIAGONALS_STRING, (float)diagonalsLenghts);
}

void print_area(Polygon polygon) {
	long long area = 0;

	for (GenericNode current = polygon->vertices; current != NULL; current = current->next) {
		area += calculate_part(current, current->next == NULL ? polygon->vertices : NULL);
	}

	printf(POLYGON_AREA_STRING, fabsf((float)area) / 2);
}

long long calculate_part(Vertex vertex, Vertex head) {
	return (((Point)vertex->data)->x * (head == NULL ? ((Point)vertex->next->data)->y : ((Point)head->data)->y)) - (((Point)vertex->data)->x * ((Point)vertex->prev->data)->y);
}

double calculate_distance(Vertex point_1, Vertex point_2) {
	return sqrt(pow((double)((Point)point_1->data)->x - (double)((Point)point_2->data)->x, 2) +
		pow((double)((Point)point_1->data)->y - (double)((Point)point_2->data)->y, 2));
}

int polygonEngine() {
	long long command, vertex[2];
	int polygonsCounter = 0;
	Polygon current;
	polygons = (GenericList)calloc(1, SIZE_OF_GENERIC_LIST);

	while (true) {
		scanf("%llx", &command);
		if (command == -1) {
			break;
		}
		current = createPolygon((POLYGON)MASK_POLYGON_TYPE(command));
		if (MASK_NEW_POLYGON(command)) {
			polygonsCounter++;
			scanf("%llx\n%llx", &vertex[0], &vertex[1]);
			convertVertices(vertex, current);
			add(current, polygons);
		}
		if (!MASK_PRINT_TYPE(command)) {
			activeFunctions(current, (command >> GET_ACTIONS)& MASK_COMMAND);
		}
		else {
			custom_forEach((command >> GET_ACTIONS)& MASK_COMMAND, command >> 10);
		}
	}
	
	freeList(polygons);
	return polygonsCounter;
}

void freeList(GenericList list) {
	for (GenericNode current = list->head, temp = current->next; current != NULL; current = temp, temp = (temp != NULL ? temp->next : temp)) {
		for (Vertex vertex = ((Polygon)current->data)->vertices, tempo = vertex->next; vertex != NULL; vertex = tempo, tempo = (tempo != NULL ? tempo->next : tempo)) {
			free(vertex->data);
			free(vertex);
		}
		free(current);
	}

	free(list);
}

GenericList createEmptyList() {
	return (GenericList)calloc(1, SIZE_OF_GENERIC_LIST);
}

Polygon createPolygon(POLYGON type) {
	Polygon newPolygon = (Polygon)malloc(SIZE_OF_POLYGON);
	if (newPolygon) newPolygon->type = type;
	return newPolygon;
}

Point createPoint(int pointBits) {
	Point point = (Point)malloc(SIZE_OF_POINT);
	char x = pointBits & MASK_COMMAND, y = (pointBits >> MOVE_TO_Y_POINT) & MASK_COMMAND;
	if (point) {
		point->x = x;
		point->y = y;
		return point;
	}
	return NULL;
}

void convertVertices(long long vertices[], Polygon polygon) {
	polygon->vertices = (Vertex)malloc(SIZE_OF_VERTEX);
	if (polygon->vertices) polygon->vertices->data = createPoint(vertices[0] & MASK_POLYGON_POINT);
	vertices[0] = vertices[0] >> SHIFT_TO_NEXT_POINT;
	Vertex current = polygon->vertices, nextNode;

	for (int i = 0, innerLoop = 3; i < VERTICES_ARRAY_SIZE; i++, innerLoop = polygon->type - FIRST_VERTICES_ARRAY) {
		for (int j = 0; j < innerLoop; j++, vertices[i] = vertices[i] >> SHIFT_TO_NEXT_POINT, current = nextNode) {
			nextNode = (Vertex)malloc(SIZE_OF_VERTEX);
			if (nextNode) {
				nextNode->data = createPoint(vertices[i] & MASK_POLYGON_POINT);
				if (current) current->next = nextNode;
				nextNode->prev = current;
			}
		}
	}
	if (polygon->vertices) polygon->vertices->prev = current;
	current->next = NULL;
}


void activeFunctions(Polygon poly, long long command) {
	for (int i = 0, bit = command & 1; i < 4; i++, command = command >> 1, bit = command & 1) {
		if (bit) functions[i](poly);
	}
}

void custom_forEach(long long command, long long type) {
	for (GenericNode current = polygons->head; current != NULL; current = current->next) {
		if (type == MASK_ALL_TYPES || ((Polygon)current->data)->type == (POLYGON)(type))
			activeFunctions(current->data, command);
	}
}