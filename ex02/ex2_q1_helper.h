#ifndef EX2_Q1_HELPER_H
#define EX2_Q1_HELPER_H
#include "miscellaneous.h"

// functions
void print_polygon(Polygon polygon);	// print the type of polygon and its vertices
void print_perimeter(Polygon polygon);	// calculate and print the perimeter
void print_diagonals(Polygon polygon);	// calculate and print the length of all diagonals
void print_area(Polygon polygon);		// calculate and print the area

// custom functions
double calculate_distance(Vertex point_1, Vertex point_2);
long long calculate_part(Vertex vertex, Vertex head);
void print_point(Point point);
Polygon createPolygon(POLYGON type);
void convertVertices(long long vertices[], Polygon polygon);
Point createPoint(int pointBits);
void custom_forEach(long long command, long long type);
void activeFunctions(Polygon poly, long long command);
void freeList(GenericList list);
void add(void* data, GenericList list);
GenericList createEmptyList();
int polygonEngine();

#endif /*EX2_Q1_HELPER_H*/