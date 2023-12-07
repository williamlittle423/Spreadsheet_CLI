#include "model.h"
#include "interface.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stddef.h>


#define MAX_ROWS 10
#define MAX_COLS 10

// DEFINE THE SPREADSHEET
char* spreadsheet[MAX_ROWS][MAX_COLS];

void model_init() {
    int rows = MAX_ROWS;
    int cols = MAX_COLS;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Allocate memory for each cell and initialize to an empty string
            spreadsheet[i][j] = strdup("");
        }
    }
}

char* spreadsheet[10][10];

int getCellValue(char col, int row) {
    int colIndex = col - 'A';
    char* valueAsString = spreadsheet[row - 1][colIndex];
    return atoi(valueAsString);
}



void set_cell_value(ROW row, COL col, char *text) {
    // TODO: implement this.
    // Free the existing memory in the cell
    free(spreadsheet[row][col]);

    // Allocate new memory for the text and set the cell value
    spreadsheet[row][col] = strdup(text);

    // Update cell display
    update_cell_display(row, col, text);
}

void clear_cell(ROW row, COL col) {
    // Free the memory of the cell value
    free(spreadsheet[row][col]);

    // Allocate new memory for an empty string
    spreadsheet[row][col] = strdup("");

    // Update cell display
    update_cell_display(row, col, "");
}

char *get_textual_value(ROW row, COL col) {
    // TODO: implement this.
    return spreadsheet[row][col];
}