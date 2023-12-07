#ifndef ASSIGNMENT_MODEL_H
#define ASSIGNMENT_MODEL_H

#include "defs.h"

// Stack structure used for handling operands
typedef struct {
    int top;
    int capacity;
    int* array;
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

// Used to fully evaluate an expression
float evaluateExpression(char* expression);


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
