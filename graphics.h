#ifndef GRA_H
#define GRA_H

void addPlotToLatexDirect(tree_t* tree, VariableTable* table, FILE* texFile, double xMin, double xMax, double step);
void calculateYRangeSimple(const tree_t* tree, VariableTable* table, int indexX, double xMin, double xMax, double step, double* yMin, double* yMax);
void writePlotHeaderSimple(FILE* texFile, double xMin, double xMax, double yMin, double yMax);
void writePlotPoints(const tree_t* tree, VariableTable* table, FILE* texFile, int indexX, double xMin, double xMax, double step);



#endif


