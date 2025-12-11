#include "diff.h"
#include "proizv.h"
#include "simply.h"
#include "toLatex.h"
#include "graphics.h"
#include <stdio.h>
#include <stdbool.h>


//frac{}{} - дроби
// ^{} - создает верхний индекс степень
// \\begin{dmath}\n - формулы с автоматическими переносами

bool needsParenthesesForOp(node_t* node, int parentPriority)                // нужно ли ставить скобки вокруг оператора при выводе в латех
{
    if (node == NULL || node->type != OP)                                   // узел сущ и явл оператором
    {
        return false;
    }

    int currentPriority = 0;

    switch (node->object.operation) {
        case ADD:
        case SUB:
            currentPriority = 1;                // низкий приоритет
            break;
        case MUL:
        case DIV:
            currentPriority = 2;
            break;
        case RAIZE:
            currentPriority = 3;
            break;
        case SIN:
        case COS:
        case TG:
        case CTG:
        case ARCSIN:
        case ARCCOS:
        case ARCTG:
        case ARCCTG:
        case LN:
        case SQRT:
        case SH:
        case CH:
        case TH:
        case CTH:
            currentPriority = 4;                // самый высокий приоритет
            break;
        default:
            return false;
    }
    return currentPriority < parentPriority;    // скобки нужны - если приоритет текущей операции ниже приоритета родительской операции
}


void nodeToLatexWithPriority(node_t* node, FILE* texFile, int parentPriority)      // дерево -> laTex с уч приоритетов
{
    if (node == NULL)
    {
        return;
    }

    switch (node->type) {
        case NUM:
            fprintf(texFile, "%g", node->object.constant);                         // просто печатаем числа и переменные напрямую
            break;
        case VAR:
            fprintf(texFile, "%s", node->object.var);
            break;
        case OP:

            bool needsParens = needsParenthesesForOp(node, parentPriority);

            if (needsParens)
            {
                fprintf(texFile, "\\left(");
            }

            switch (node->object.operation) {                                       // рекурсивно все делаем как обычно
                case ADD:
                    nodeToLatexWithPriority(node->left, texFile, 1);               // для +  выводим рекурсивно и левую и правую часть с приоритетом 1
                    fprintf(texFile, " + ");
                    nodeToLatexWithPriority(node->right, texFile, 1);
                    break;

                case SUB:
                    nodeToLatexWithPriority(node->left, texFile, 1);
                    fprintf(texFile, " - ");
                    nodeToLatexWithPriority(node->right, texFile, 1);
                    break;

                case MUL:
                    nodeToLatexWithPriority(node->left, texFile, 2);
                    fprintf(texFile, " \\cdot ");
                    nodeToLatexWithPriority(node->right, texFile, 2);
                    break;

                case DIV:
                    fprintf(texFile, "\\frac{");
                    nodeToLatexWithPriority(node->left, texFile, 0);              // приоритет 0 => всегда без скобок
                    fprintf(texFile, "}{");
                    nodeToLatexWithPriority(node->right, texFile, 0);
                    fprintf(texFile, "}");
                    break;

                case SIN:
                    fprintf(texFile, "\\sin\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case COS:
                    fprintf(texFile, "\\cos\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case TG:
                    fprintf(texFile, "\\tan\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case CTG:
                    fprintf(texFile, "\\ctg\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case ARCSIN:
                    fprintf(texFile, "\\arcsin\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case ARCCOS:
                    fprintf(texFile, "\\arccos\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case ARCTG:
                    fprintf(texFile, "\\arctg\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case ARCCTG:
                    fprintf(texFile, "\\arcctg\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case LN:
                    fprintf(texFile, "\\ln\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case RAIZE:
                    if (node->left && (node->left->type == VAR || (node->left->type == NUM && node->left->object.constant >= 0)))
                    {
                        nodeToLatexWithPriority(node->left, texFile, 3);
                    }
                    else
                    {
                        fprintf(texFile, "\\left(");
                        nodeToLatexWithPriority(node->left, texFile, 0);
                        fprintf(texFile, "\\right)");
                    }
                    fprintf(texFile, "^{");
                    nodeToLatexWithPriority(node->right, texFile, 0);
                    fprintf(texFile, "}");
                    break;

                case SQRT:
                    fprintf(texFile, "\\sqrt{");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "}");
                    break;

                case SH:
                    fprintf(texFile, "\\sinh\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case CH:
                    fprintf(texFile, "\\cosh\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case TH:
                    fprintf(texFile, "\\tanh\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case CTH:
                    fprintf(texFile, "\\coth\\left(");
                    nodeToLatexWithPriority(node->left, texFile, 0);
                    fprintf(texFile, "\\right)");
                    break;

                case HZ_OPERATION:
                default:
                    fprintf(texFile, "?");
                    break;
            }

            if (needsParens) fprintf(texFile, "\\right)");             // если была открывающая то нужна и закрывающая
            break;
    }
}


void nodeToLatex(node_t* node, FILE* texFile)
{
    nodeToLatexWithPriority(node, texFile, 0);                     // начало с приоритетом 0
}


void formulaToLatex(tree_t* tree, FILE* texFile, const char* title)    // просто формула
{
    if (title && title[0] != '\0')
    {
        fprintf(texFile, "\\subsection*{%s}\n", title);
    }
    fprintf(texFile, "\\begin{dmath}\n");
    nodeToLatex(tree->root, texFile);
    fprintf(texFile, "\n\\end{dmath}\n\n");
}


void formulaWithComputationToLatex(tree_t* tree, VariableTable* table, FILE* texFile, const char* title)   // формула и численное значение
{
    formulaToLatex(tree, texFile, title);

    double result = countingTree(tree->root, table);
    fprintf(texFile, "Результат: \\[%.4f\\]\n\n", result);
}


void variablesTableToLatex(VariableTable* table, FILE* texFile)
{
    fprintf(texFile,
        "\\begin{tabular}{|c|c|}\n"                 // таблица с двумя колонками и вертик линиями
        "\\hline\n"
        "Переменная & Значение \\\\\n"
        "\\hline\n");

    for (int i = 0; i < table->count; i++)
    {
        if (table->variables[i].isDefined)                          // если определена переменная и имеет значение
        {
            fprintf(texFile, "$%s$ & $%.2f$ \\\\\n", table->variables[i].name, table->variables[i].value);
        }
    }
    fprintf(texFile,
        "\\hline\n"
        "\\end{tabular}\n\n");
}



void createComprehensiveReport(tree_t* original, VariableTable* table, const char* filename,  float plotMinX, float plotMaxX)
{
    FILE* texFile = fopen(filename, "w");

    if (!texFile)
    {
        printf("не сделали файл %s\n", filename);
        return;
    }

    tree_t* simplified = NULL;
    tree_t* derivative = NULL;

    double originalValue = 0;
    double simplifiedValue = 0;
    double derivativeValue = 0;

    writeLaTeXPreamble(texFile);                                            // заголовочек
    writeTitleAndTOC(texFile);

    originalValue = writeOriginalDataSection(original, table, texFile);     // исходные данные

    simplified = writeSimplificationSection(original, table, texFile, &simplifiedValue, plotMinX , plotMaxX);  // упрощаем исходное

    if (simplified != NULL && simplified->root != NULL)                     // если упростили - берем производную
    {
        derivative = writeDerivativeSection(simplified, table, texFile, &derivativeValue, plotMinX, plotMaxX);
    }

    if (simplified != NULL && derivative != NULL)                           // полученные результаты
    {
        writeFinalResultsSection(simplified, derivative, table, texFile, simplifiedValue, derivativeValue);
    }

    writeSummaryTable(original, simplified, derivative, table, texFile, originalValue, simplifiedValue, derivativeValue);       // итоговая таблица значений

    writeLaTeXFooter(texFile);                                              // закрываем документ
    fclose(texFile);

    cleanupTrees(simplified, derivative);
}


void writeLaTeXPreamble(FILE* texFile)
{
    fprintf(texFile,
        "\\documentclass{article}\n"
        "\\usepackage[utf8]{inputenc}\n"
        "\\usepackage{amsmath}\n"
        "\\usepackage{breqn}\n"
        "\\usepackage[russian]{babel}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{pgfplots}\n"
        "\\pgfplotsset{compat=1.18}\n"
        "\\geometry{a4paper, left=20mm, right=20mm, top=20mm, bottom=20mm}\n"     // поля со всех сторон по 20мм
        "\\setlength{\\parindent}{0pt}\n"                                         // без отступа у -красной- строки
        "\\setlength{\\parskip}{1em}\n");                                           // отступ между абзацами
}

void writeTitleAndTOC(FILE* texFile)
{
    fprintf(texFile,
        "\\title{Математический отчет}\n"
        "\\author{ЯРИК}\n"
        "\\date{\\today}\n"
        "\\begin{document}\n"
        "\\maketitle\n"                        // генерит титульную страницу
        "\\tableofcontents\n"
        "\\newpage\n");                           // нов страница
}

double writeOriginalDataSection(tree_t* original, VariableTable* table, FILE* texFile)
{
    fprintf(texFile,
        "\\section{Исходные данные}\n\n"                                        // section раздел автоматически нумеруется
        "\\subsection{Исходное выражение}\n"
        "\\begin{dmath}\n");
    nodeToLatex(original->root, texFile);

    fprintf(texFile,
        "\n\\end{dmath}\n\n"
        "\\subsection{Таблица переменных}\n\n");

    variablesTableToLatex(table, texFile);

    double originalResult = countingTree(original->root, table);

    fprintf(texFile, "Значение исходного выражения: \\[%.4f\\]\n\n", originalResult);

    return originalResult;
}

tree_t* writeSimplificationSection(tree_t* original, VariableTable* table, FILE* texFile, double* simplifiedValue, float plotMinX, float plotMaxX)
{
    fprintf(texFile, "\\section{Упрощение выражения}\n\n");

    tree_t* simplified = treeCtor();
    if (!simplified)
    {
        fprintf(texFile, "\\textbf{не получилось сделать дерево для упрощения}\n\n");
        return NULL;
    }

    simplified->root = copyNode(original->root);
    if (!simplified->root)
    {
        fprintf(texFile, "\\textbf{не получилось скопировать дерево}\n\n");
        free(simplified);
        return NULL;
    }

    fprintf(texFile, "\\subsection{Поэтапное упрощение}\n\n");
    simplified->root = simplification(simplified->root, simplified, texFile);

    fprintf(texFile,
        "\\subsection{Финальное упрощенное выражение}\n"
        "\\begin{dmath}\n");

    nodeToLatex(simplified->root, texFile);
    fprintf(texFile, "\n\\end{dmath}\n\n");

    *simplifiedValue = countingTree(simplified->root, table);

    fprintf(texFile,
        "Значение упрощенного выражения: \\[%.4f\\]\n\n"
        "\\subsection{График упрощенной функции}\n",
        *simplifiedValue);

    float step = (plotMaxX - plotMinX) / 1000.0;

    addPlotToLatexDirect(simplified, table, texFile, plotMinX, plotMaxX, step);                      // снова графичек - теперь упрощенной функции

    return simplified;
}

tree_t* writeDerivativeSection(tree_t* simplified, VariableTable* table, FILE* texFile, double* derivativeValue, float plotMinX, float plotMaxX)
{
    fprintf(texFile, "\\section{Производная по переменной $x$}\n\n");

    tree_t* derivative = diffTree(simplified, table, "x");

    if (derivative == NULL || derivative->root == NULL)
    {
        fprintf(texFile, "\\textbf{не получилось вычислить производную}\n\n");
        return NULL;
    }

    fprintf(texFile,
        "\\subsection{Исходная производная}\n"
        "\\begin{dmath}\n");

    nodeToLatex(derivative->root, texFile);

    fprintf(texFile,
        "\n\\end{dmath}\n\n"
        "\\subsection{Упрощенная производная}\n");

    derivative->root = simplification(derivative->root, derivative, texFile);
    fprintf(texFile, "\\begin{dmath}\n");
    nodeToLatex(derivative->root, texFile);
    fprintf(texFile, "\n\\end{dmath}\n\n");

    *derivativeValue = countingTree(derivative->root, table);
    fprintf(texFile,
        "Значение производной: \\[%.4f\\]\n\n"
        "\\subsection{График производной}\n",
        *derivativeValue);

    float step = (plotMaxX - plotMinX) / 1000.0;
    addPlotToLatexDirect(derivative, table, texFile, plotMinX, plotMaxX, step);                       // графичек производной

    return derivative;
}

void writeFinalResultsSection(tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* texFile, double simplifiedValue, double derivativeValue)
{
    double xValue = 0.0;
    if (table->count > 0 && table->variables[0].isDefined)
    {
        xValue = table->variables[0].value;
    }

    fprintf(texFile,
        "\\begin{itemize}\n"
        "\\item Значение упрощенного выражения: $f(%.2f) = %.4f$\n"
        "\\item Значение производной: $f'(%.2f) = %.4f$\n"
        "\\end{itemize}\n\n",
        xValue, simplifiedValue, xValue, derivativeValue);

    fprintf(texFile,
        "\\section{Финальные результаты}\n\n"
        "\\subsection{Упрощенное выражение и его производная}\n"
        "\\begin{align*}\n"
        "f(x) &= ");
    nodeToLatex(simplified->root, texFile);
}

void writeSummaryTable(tree_t* original, tree_t* simplified, tree_t* derivative, VariableTable* table, FILE* texFile, double originalValue, double simplifiedValue, double derivativeValue)
{
    fprintf(texFile,
        "\\section{Итоговая таблица}\n\n"
        "\\begin{tabular}{|l|c|}\n"
        "\\hline\n"
        "Параметр & Значение \\\\\n"
        "\\hline\n"
        "Исходное выражение & $%.4f$ \\\\\n"
        "Упрощенное выражение & $%.4f$ \\\\\n",
        originalValue, simplifiedValue);

    if (derivative != NULL && derivative->root != NULL)
    {
        fprintf(texFile, "Производная по $x$ & $%.4f$ \\\\\n", derivativeValue);
    }
    else
    {
        fprintf(texFile, "Производная по $x$ & не вычислена \\\\\n");
    }

    fprintf(texFile, "Размер исходного дерева & %d узлов \\\\\n", countTreeSize(original->root));

    if (simplified != NULL && simplified->root != NULL)
    {
        fprintf(texFile, "Размер упрощенного дерева & %d узлов \\\\\n", countTreeSize(simplified->root));
    }
    else
    {
        fprintf(texFile, "Размер упрощенного дерева & не определен \\\\\n");
    }

    fprintf(texFile,
        "\\hline\n"
        "\\end{tabular}\n");
}

void cleanupTrees(tree_t* simplified, tree_t* derivative)
{
    if (simplified != NULL)
    {
        if (simplified->root != NULL)
        {
            treeRecursiveDelete(simplified->root);
        }
        free(simplified);
    }

    if (derivative != NULL)
    {
        if (derivative->root != NULL)
        {
            treeRecursiveDelete(derivative->root);
        }
        free(derivative);
    }
}

void writeLaTeXFooter(FILE* texFile)
{
    fprintf(texFile, "\\end{document}\n");
}
