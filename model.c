#include "model.h"
#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>


struct CellData* createCell() {
    struct CellData* cd = malloc(sizeof (struct CellData));
    return cd;
}

// Function to create a directed graph with V vertices
struct Graph* createDirectedGraph(int V) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;

    // Allocate memory for the adjacency matrix
    graph->matrix = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; ++i) {
        graph->matrix[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; ++j) {
            graph->matrix[i][j] = 0;  // Initialize all entries to 0
        }
    }

    return graph;
}

void addDirectedEdge(struct Graph* graph, char sourceLetter, char sourceDigit, char depLetter, char depDigit) {

    int indexSource = calculateGraphIndex(sourceLetter, sourceDigit);
    int indexDep = calculateGraphIndex(depLetter, depDigit);

    // Initializes a dependency between the source and the dependency cell
    graph->matrix[indexDep][indexSource] = 1;
}

int calculateGraphIndex(char letter, char digit) {
    // Assuming letters are A-G and digits are 1-9
    if (letter < 'A' || letter > 'G' || digit < '1' || digit > '9') {
        printf("Invalid input\n");
        return -1;  // Return -1 for invalid input
    }

    int index = (letter - 'A') * 7 + (digit - '0') - 1;
    return index;
}

void model_init() {
    int rows = NUM_ROWS;
    int cols = NUM_COLS;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Allocate memory for each cell and initialize to an empty string and 0.0 float value
            struct CellData *cd = createCell();
            cd->cellResult = 0.0;
            cd->text = strdup("");
            update_cell_display(i, j, cd->text);
            spreadsheet[i][j] = cd;
        }
    }

    // Create a directed graph with the number (NUM_COLS * NUM_ROWS) x (NUM_COLS * NUM_ROWS) dimensions
    dependencies = createDirectedGraph(NUM_COLS * NUM_ROWS);
}

void printDirectedGraph(struct Graph* graph) {
    for (int i = 0; i < graph->V; ++i) {
        for (int j = 0; j < graph->V; ++j) {
            printf("%d ", graph->matrix[i][j]);
        }
        printf("\n");
    }
}

float getCellValue(char col, int row) {
    int colIndex = col - 'A';
    return spreadsheet[row - 1][colIndex]->cellResult;
}

int precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    } else if (op == '*' || op == '/') {
        return 2;
    }
    return 0; // For '('
}

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = -1;
    stack->capacity = capacity;
    stack->array = (float*)malloc(stack->capacity * sizeof(float));
    return stack;
}

// Function to check if the stack is empty
int isEmpty(Stack* stack) {
    return stack->top == -1;
}

// Function to push an element onto the stack
void push(Stack* stack, float item) {
    stack->array[++stack->top] = item;
}

// Function to pop an element from the stack
float pop(Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->array[stack->top--];
    }
    return -1;
}

float performOperation(float operand1, float operand2, char op) {
    switch (op) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': return operand1 / operand2;
        default: return -1; // Invalid operator
    }
}

float evaluateExpression(ROW r, COL c, char* expression) {
    Stack* operands = createStack(strlen(expression));
    Stack* operators = createStack(strlen(expression));

    for (int i = 0; i < strlen(expression); i++) {
        if (expression[i] == ' ') {
            continue; // Skip spaces
        }

        if (isdigit(expression[i]) || (expression[i] == '-' && i + 1 < strlen(expression) && (isdigit(expression[i + 1]) || expression[i + 1] == '.'))) {
            float operand = 0;
            int isFractional = 0;
            float fractionalMultiplier = 0.1;

            while (i < strlen(expression) && (isdigit(expression[i]) || expression[i] == '.' || (expression[i] == '-' && isFractional == 0))) {
                if (expression[i] == '.') {
                    isFractional = 1;
                } else {
                    if (isFractional) {
                        operand = operand + (expression[i] - '0') * fractionalMultiplier;
                        fractionalMultiplier *= 0.1;
                    } else {
                        operand = operand * 10 + (expression[i] - '0');
                    }
                }
                i++;
            }
            i--; // Move back one position to handle non-digit character
            push(operands, operand);
        } else if (isalpha(expression[i])) {
            // Handle cell references like 'A4'
            char col = toupper(expression[i]);
            i++; // Move to the next character (skip the column character)
            int row = expression[i] - '0'; // Assuming the row number is a single digit

            // Dependency graph indices
            int sourceIndex = c * 7 + r;
            int depIndex = calculateGraphIndex(col, expression[i]);
            // Check if the there is a circular dependency
            if (dependencies->matrix[sourceIndex][depIndex] == 1) {
                printf("Error: Circular dependency formed");
                return 0.0;
            } else {
                // Initialize a dependency
                dependencies->matrix[depIndex][sourceIndex] = 1;
            }

            push(operands, getCellValue(col, row));
        } else {
            char currentOperator = expression[i];
            if (currentOperator == '+' || currentOperator == '-') {
                while (!isEmpty(operators) && precedence(operators->array[operators->top]) >= precedence(currentOperator)) {
                    float operand2 = pop(operands);
                    float operand1 = pop(operands);
                    char op = pop(operators);
                    push(operands, performOperation(operand1, operand2, op));
                }
                push(operators, currentOperator);
            } else if (currentOperator == '*' || currentOperator == '/') {
                while (!isEmpty(operators) && precedence(operators->array[operators->top]) >= precedence(currentOperator)) {
                    float operand2 = pop(operands);
                    float operand1 = pop(operands);
                    char op = pop(operators);
                    push(operands, performOperation(operand1, operand2, op));
                }
                push(operators, currentOperator);
            }
        }
    }

    while (!isEmpty(operators)) {
        float operand2 = pop(operands);
        float operand1 = pop(operands);
        char op = pop(operators);
        push(operands, performOperation(operand1, operand2, op));
    }

    return pop(operands);
}

void set_cell_value(ROW row, COL col, char *t) {

//     Check if the input is an expression
    if (t[0] == '=') {
        char* expression = t;
        expression++;

        float result = evaluateExpression(row, col, expression);

        clear_cell(row, col);

        struct CellData* cd = createCell();

        cd->text = strdup(t);
        cd->cellResult = result;

        spreadsheet[row][col] = cd;

        // Update cell display
        char resultString[10];  // Adjust the size as needed
        snprintf(resultString, sizeof(resultString), "%.1f", result);
        update_cell_display(row, col, resultString);

    } else {
        // Entering a numerical value only

        // Attempt to convert the text to a float
        char* endptr;
        float value = strtof(t, &endptr);

        // Check if conversion was successful
        if (*endptr != '\0') {
            printf("Error: '%s' is not a valid floating-point number.\n", t);
            return;
        } else {
            // Free the existing memory in the cell
            clear_cell(row, col);

            struct CellData* cd = createCell();
            cd->text = strdup(t);
            cd->cellResult = value;

            spreadsheet[row][col] = cd;

            int rowIndex = col * 7 + row;
            // Update any cells where this cell is a dependency
            for (int i = 0; i < NUM_COLS * NUM_ROWS; i++) {
                if (dependencies->matrix[rowIndex][i] == 1) {
                    int newCol = i / 7;
                    int newRow = i % 7;
                    set_cell_value(newRow, newCol, spreadsheet[newRow][newCol]->text);
                }
            }

            update_cell_display(row, col, t);
        }
    }
}

void clear_cell(ROW row, COL col) {
    // Free the memory of the cell value
    if (spreadsheet[row][col] != NULL) {
        free(spreadsheet[row][col]->text);
        free(spreadsheet[row][col]);
    } else {
        // Handle the case where the pointer is NULL
        printf("Warning: Attempting to clear an already cleared cell.\n");
    }

    // Allocate new memory for an empty string
    struct CellData* cd = createCell();
    cd->text = strdup("");
    cd->cellResult = 0.0;

    spreadsheet[row][col] = cd;

    // Update cell display
    update_cell_display(row, col, strdup(""));
}

char *get_textual_value(ROW row, COL col) {
    // Return a copy of the textual value of the cell
    return strdup(spreadsheet[row][col]->text);
}