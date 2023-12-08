#ifndef ASSIGNMENT_MODEL_H
#define ASSIGNMENT_MODEL_H

#include "defs.h"

#define MAX_ROWS 10
#define MAX_COLS 7

// Define the spreadsheet cell structure
// text contains the textual value of the cell if it is an expression. i.e. '=A4+B3+3.6'
// cellResult is the actual float value of the cell
struct CellData {
    float cellResult;
    char* text;
};

// A directed graph structure is implemented to handle cell dependencies
struct Graph {
    int V;      // Number of vertices
    int** matrix; // Adjacency matrix
};

// Initialize a directed graph with all zeros for each edge
struct Graph* createDirectedGraph(int V);

// Calculates a unique index the cell
int calculateGraphIndex(char letter, char digit);

// Function to add a direction edge from a source cell and a dependency cell
void addDirectedEdge(struct Graph* graph, char sourceLetter, char sourceDigit, char depLetter, char depDigit);

// Initialize an empty cell
struct CellData* createCell();

// The spreadsheet data structure is a MAX_ROWS x MAX_COLS 2d array
// Each piece of data in the array is a pointer to a CellData struct
struct CellData* spreadsheet[MAX_ROWS][MAX_COLS];

struct Graph* dependencies;

// Stack structure used for handling operands
typedef struct {
    int top;
    int capacity;
    float* array;
} Stack;

// Initializes the data structure.
//
// This is called once, at program start.
void model_init();

// Retrieve the value of a cell
float getCellValue(char col, int row);

// Determine precedence of operators to conform to BEDMAS order of operations
int precedence(char op);

// Initialize the stack
Stack* createStack(int capacity);

// Check if a stack is empty
int isEmpty(Stack* stack);

// Push a float to the stack
void push(Stack* stack, float item);

// Pop a number form the stack
float pop(Stack* stack);

// Performs an operation on two operands based on the character operator op
float performOperation(float operand1, float operand2, char op);

// Used to fully evaluate an expression and add/remove cell dependencies
float evaluateExpression(ROW r, COL c, char* expression);

// USED FOR TESTING PURPOSES
void printDirectedGraph(struct Graph* graph);

// Sets the value of a cell based on user input.
//
// The string referred to by 'text' is now owned by this function and/or the
// cell contents data structure; it is its responsibility to ensure it is freed
// once it is no longer needed.
void set_cell_value(ROW row, COL col, char *text);

// Clears the value of a cell.
void clear_cell(ROW row, COL col);

// Gets a textual representation of the value of a cell, for editing.
//
// The returned string must have been allocated using 'malloc' and is now owned
// by the interface. The cell contents data structure must not modify it or
// retain any reference to it after the function returns.
char *get_textual_value(ROW row, COL col);

#endif //ASSIGNMENT_MODEL_H
