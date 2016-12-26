#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

// if/else counter node struct
struct n {
	int count;
	struct n* next;
};

typedef struct n node;
node *head;

// push if counter onto stack
void push(int i ) {
	if (head == NULL) {
		head = malloc(sizeof(node));
		head->count = i;
		head->next = NULL;
	} else {
		node *temp;
		temp = malloc(sizeof(node));
		temp->next = head;
		temp->count = i;
		head = temp;
	}
}

// pop if counter from stack (endif)
int pop() {
	node* holder;
	int temp = head->count;
	holder = head;
	if (head->next != NULL) {
		head = head->next;
	} 
	free(holder);
	return temp;
}

// peek at if counter stack (else)
int peek() {
	return head->count;
}

int main(int argc, char const *argv[]) {
	// open .j source file
	FILE *j_source;
	j_source = fopen(argv[1], "r"); 

	// store name of file for .asm purposes
	char fileName[200];
	strcpy(fileName, argv[1]);
	
	// if and comp counts for writing
	int if_count = 0; 
	int comp_count = 0;

	// properly format file name
	for (int i = 0; i < 200; i++) {
		if (fileName[i] == '.' && fileName[i + 1] == 'j') {
			// null terminate
			fileName[i] = '\0'; 
			fileName[i + 1] = '\0';
			break;
		}
	}

	// store name for assembly file to be generated
	char assemblyName[200];
	strcpy(assemblyName, fileName);
	for (int i = 0; i < 200; i++) {
		if (assemblyName[i] == '\0') {
			assemblyName[i] = '.';
			assemblyName[i + 1] = 'a';
			assemblyName[i + 2] = 's';
			assemblyName[i + 3] = 'm';
			assemblyName[i + 4] = '\0';
			break;
		}
	}
	
	// generate assembly file
	FILE *output; 
	output = fopen(assemblyName, "w");

	token *tok;
	tok = malloc(sizeof(token));

	fputs(".CODE\n", output);

	// parse through .j file
	while (!feof(j_source)) {
		int b = read_token(tok, j_source);

		if (b == 0 || b == 1) {
			if (tok->type == 0) {
				// number literal
				fputs("\tADD R6, R6, #-1\n", output);
				fputs("\tCONST R0, #", output);
				fprintf(output, "%d", tok->literal_value); 
				fputs("\n\tHICONST R0, #0\n", output);
				fputs("\tSTR R0, R6, #0\n", output); 
			} 

			if (tok->type == 1) {
				// plus
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tADD R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 2) {
				// minus
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tSUB R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 3) {
				// multiply
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tMUL R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 4) {
				// divide
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tDIV R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 5) {
				// mod
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tMOD R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 6) {
				// lt
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRn ", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #1\n", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);	
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				comp_count++;
			}

			if (tok->type == 7) {
				// le
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRnz ", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #1\n", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				comp_count++;
			}

			if (tok->type == 8) {
				// eq
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRz ", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #1\n", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				comp_count++;
			}
			
			if (tok->type == 9) {
				// ge
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRzp ", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #1\n", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				comp_count++;
			}

			if (tok->type == 10) {
				// gt
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRp ", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_COMP_GOOD_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tCONST R0, #1\n", output);
				fputs(fileName, output);
				fputs("_COMP_END_", output);
				fprintf(output, "%d", comp_count);
				fputs("\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				comp_count++;
			}

			if (tok->type == 11) {
				// and
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tAND R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 12) {
				// or
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tOR R0, R0, R1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 13) {
				// not
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tNOT R0, R0\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 14) {
				// drop
				fputs("\tADD R6, R6, #1\n", output);
			}

			if (tok->type == 15) {
				// dup
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tADD R6, R6, #-1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);

			}

			if (tok->type == 16) {
				// swap
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tSTR R1, R6, #0\n", output);
				fputs("\tSTR R0, R6, #1\n", output);

			}

			if (tok->type == 17) {
				// max
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tLDR R1, R6, #1\n", output);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tCMP R0, R1\n", output);
				fputs("\tBRn ", output);
				fputs(fileName, output);
				fputs("_MAX_LT_0\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_MAX_END_0\n", output);
				fputs(fileName, output);
				fputs("_MAX_LT_0\n", output);
				fputs("\tSTR R1, R6, #0\n", output);
				fputs(fileName, output);
				fputs("_MAX_END_0\n", output);
			}

			if (tok->type == 18) {
				// low8
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tAND R1, R0, #15\n", output);
				fputs("\tSRL R0, R0, #4\n", output);
				fputs("\tAND R0, R0, #15\n", output);
				fputs("\tSLL R0, R0, #4\n", output);
				fputs("\tOR R0, R0, R1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 19) {
				fputs("\tLDR R0, R5, #", output);
				fprintf(output, "%d", tok->arg_no);
				fputs("\n\tADD R6, R6, #-1\n", output);
				fputs("\tSTR R0, R6, #0\n", output);
			}

			if (tok->type == 20) {
				// if
				push(if_count);
				fputs("\tADD R6, R6, #1\n", output);
				fputs("\tLDR R0, R6, #-1\n", output);
				fputs(fileName, output);
				fputs("_IF_", output);
				fprintf(output, "%d", if_count);
				fputs("\n", output);
				fputs("\tBRz ", output);
				fputs(fileName, output);
				fputs("_ELSE_", output);
				fprintf(output, "%d", if_count);
				fputs("\n", output);
				if_count++;
			}
			
			if (tok->type == 21) {
				// else
				int temp = peek();
				fputs("\tJMP ", output);
				fputs(fileName, output);
				fputs("_ENDIF_", output);
				fprintf(output, "%d", temp);
				fputs("\n", output);
				fputs(fileName, output);
				fputs("_ELSE_", output);
				fprintf(output, "%d", temp);
				fputs("\n", output);
			}

			if (tok->type == 22) {
				fputs(fileName, output);
				fputs("_ENDIF_", output);
				int temp = pop();
				fprintf(output, "%d", temp);
				fputs("\n", output);
			}

			if (tok->type == 23) {
				// defun
				fputs(".FALIGN\n", output);
				fputs(tok->str, output);
				fputs("\n\t;; prologue\n", output);
				fputs("\tSTR R7, R6, #-2\n", output);
				fputs("\tSTR R5, R6, #-3\n", output);
				fputs("\tADD R6, R6, #-3\n", output);
				fputs("\tADD R5, R6, #0\n", output);
				fputs("\t;; function body\n", output);
			}

			if (tok->type == 24) {
				// return
				fputs("\t;; function epilogue\n", output);
				fputs("\tLDR R0, R6, #0\n", output);
				fputs("\tSTR R0, R5, #2\n", output);
				fputs("\tADD R6, R5, #0\n", output);
				fputs("\tADD R6, R6, #2\n", output);
				fputs("\tLDR R7, R5, #1\n", output);
				fputs("\tLDR R5, R5, #0\n", output);
				fputs("\tRET\n", output);
			}

			if (tok->type == 25) {
				// outside
				fputs("\tLEA R7 ", output);
				fputs(tok->str, output);
				fputs("\n\tJSRR R7\n", output);
				
			}
		} 	
	}
	return 1;
	free(tok);
}
