#ifndef TOLATEX_FILE_H
#define TOLATEX_FILE_H

#include <stdio.h>
#include "diff.h"


void nodeToLatex(node_t* node, FILE* texFile);
void formulaToLatex(tree_t* tree, FILE* texFile, const char* title);
void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* texFile, const char* title);
void variablesTableToLatex(VariableTable* table, FILE* texFile);

void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename, float plotMinX, float plotMaxX);

void writeLaTeXPreamble(FILE* texFile);
void writeTitleAndTOC(FILE* texFile);
double writeOriginalDataSection(tree_t* original, VariableTable* table, FILE* texFile);
tree_t* writeSimplificationSection(tree_t* original, VariableTable* table, FILE* texFile, double* simplifiedValue, float plotMinX, float plotMaxX);
tree_t* writeDerivativeSection(tree_t* simplified, VariableTable* table, FILE* texFile, double* derivativeValue, float plotMinX, float plotMaxX);
void writeFinalResultsSection(tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* texFile, double simplifiedValue, double derivativeValue);
void writeSummaryTable(tree_t* original, tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* texFile, double originalValue, double simplifiedValue, double derivativeValue);
void cleanupTrees(tree_t* simplified, tree_t* derivative);
void writeLaTeXFooter(FILE* texFile);


#endif
