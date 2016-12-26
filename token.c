/*
 * token.c
 */

#define MAX_TOKEN_LENGTH 200
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
  int type;             // token type
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];
  int op_type; 
} token;
 
// Returns 0 on success
// Returns 1 on success and final token
// Returns 2 on broken token
int read_token (token *theToken, FILE *theFile) {
	char buffer[200];
	int calcNum = 0; // literal_value

	// for checking if token is a number
	int isDigit;
	int isDigitNeg;

	int c = -1;
	c = fgetc(theFile);
	
	// iterate through whitespaces
	if (c == 32 || c == 10 || c == 9) {
		while (c == 32 || c == 10 || c == 9) {
			c = fgetc(theFile); 
		}
	}

	// iterate through comments
	if (c == 59) {
		while(c != 10) {
			c = fgetc(theFile); // ignore comments
		}
		c = -1;
		theToken->type = -1;
		return 0;
	} 
	
	int i; 
	int j;
	
	// read in token
	i = 0;
	while (c != 32 && c != 10 && c != 9 && !feof(theFile)) {
		buffer[i] = (char) c;
		c = fgetc(theFile);
		i++;
	}

	// broken token
	if(i == 0){
		theToken->type = -1;
		return 2;
	}

	buffer[i] = '\0'; // null terminate buffer

	// check if token is number
	isDigit = isdigit(buffer[0]);
	if (buffer[0] == '-') {
		isDigitNeg = isdigit(buffer[1]);
	}

	if (isDigit != 0 || isDigitNeg != 0) {
		theToken->type = 0; // number literal
		
		// positive number
		if (buffer[0] != '-') {
			int pow_ten = 1;

			// decimal multiplier
			for (int m = 0; m < i - 1; m++) {
				pow_ten *= 10;
			}

			if (i == 1) {
				theToken->literal_value = buffer[0] - 48;
			} else {
				for (int k = 0; k < i; k++) {
					// calculate decimal value
					calcNum = calcNum + ((buffer[k] - 48) * pow_ten);
					pow_ten = pow_ten / 10;
				}

				theToken->literal_value = calcNum;
			}

		} else {
			// negative number
			int pow_ten = 1;

			for (int m = 1; m < i - 1; m++) {
				pow_ten *= 10;
			}

			if (i == 2) {
				theToken->literal_value = buffer[1] - 48;
			} else {
				for (int k = 1; k < i; k++) {
					calcNum = calcNum + ((buffer[k] - 48) * pow_ten);
					pow_ten = pow_ten / 10;
				}

				theToken->literal_value = 0 - calcNum;
			}
		}

	} else if (buffer[0] == '+') {
			theToken->type = 1; 

	} else if (buffer[0] == '-') {
			theToken->type = 2; 

	} else if (buffer[0] == '*') {
			theToken->type = 3; 

	} else if (buffer[0] == '/') {
			theToken->type = 4; 

	} else if (buffer[0] == '%') {
			theToken->type = 5; 

	} else if (buffer[0] == 'l' && buffer[1] == 't') {
			theToken->type = 6; 
			
	} else if (buffer[0] == 'l' && buffer[1] == 'e') {
			theToken->type = 7; 

	} else if (buffer[0] == 'e' && buffer[1] == 'q') {
			theToken->type = 8; 

	} else if (buffer[0] == 'g' && buffer[1] == 'e') {
			theToken->type = 9; 

	} else if (buffer[0] == 'g' && buffer[1] == 't') {
			theToken->type = 10; 

	} else if (buffer[0] == 'a' && buffer[1] == 'n' && buffer[2] == 'd') {
			theToken->type = 11; 

	} else if (buffer[0] == 'o' && buffer[1] == 'r') {
			theToken->type = 12; 

	} else if (buffer[0] == 'n' && buffer[1] == 'o' && buffer[2] == 't') {
			theToken->type = 13;
 
	} else if (buffer[0] == 'd' && buffer[1] == 'r' && buffer[2] == 'o' && buffer[3] == 'p') {
			theToken->type = 14;

	} else if (buffer[0] == 'd' && buffer[1] == 'u' && buffer[2] == 'p') {
			theToken->type = 15;

	} else if (buffer[0] == 's' && buffer[1] == 'w' && buffer[2] == 'a' && buffer[3] == 'p') {
			theToken->type = 16;

	} else if (buffer[0] == 'm' && buffer[1] == 'a' && buffer[2] == 'x') {
			theToken->type = 17;

	} else if (buffer[0] == 'l' && buffer[1] == 'o' && buffer[2] == 'w' && buffer[3] == '8') {
			theToken->type = 18;

	} else if (buffer[0] == 'a' && buffer[1] == 'r' && buffer[2] == 'g') {
		theToken->type = 19;
		
		// get number following arg
		calcNum = 0;
			
		int pow_ten = 1; 
		for (int m = 0; m < i - 4; m++) {
				pow_ten *= 10;
		}
		
		if (i == 4) {
			theToken->arg_no = buffer[3] - 48 + 2;
		} else {
			for (int k = 0; k < i - 3; k++) {
				calcNum = calcNum + ((buffer[k + 3] - 48) * pow_ten);
				pow_ten = pow_ten / 10;
			}

			theToken->arg_no = calcNum + 2;
		} 

	} else if (buffer[0] == 'i' && buffer[1] == 'f') {
		theToken->type = 20;

	} else if (buffer[0] == 'e' && buffer[1] == 'l' && buffer[2] == 's' && buffer[3] == 'e') {
		theToken->type = 21;

	} else if (buffer[0] == 'e' && buffer[1] == 'n' && buffer[2] == 'd' && buffer[3] == 'i' &&
		buffer[4] == 'f') {
			theToken->type = 22;

	} else if (buffer[0] == 'd' && buffer[1] == 'e' && buffer[2] == 'f' && buffer[3] == 'u' && 
		buffer[4] == 'n') {
		memset(theToken->str, 0, strlen(theToken->str)); 
		theToken->type = 23;
		c = fgetc(theFile);

		// get function name
		if (c == 32 || c == 10 || c == 9) {
			while (c == 32 || c == 10 || c == 9) {
				c = fgetc(theFile); 
			}
		} 

		i = 0;
		while (c != 32 && c != 10 && c != 9 && !feof(theFile)) {
			buffer[i] = (char) c;
			c = fgetc(theFile);
			i++;
		}
		
		// store function name
		j = 0;
		while (j < i) {
			theToken->str[j] = buffer[j];
			j++;
		}

	} else if (buffer[0] == 'r' && buffer[1] == 'e' && buffer[2] == 't' && buffer[3] == 'u' && 
		buffer[4] == 'r' && buffer[5] == 'n') {
		theToken->type = 24;

	} else {
		// other defined functions
		memset(theToken->str, 0, strlen(theToken->str)); 
		theToken->type = 25;

		for (int k = 0; k < i; k++) {
			theToken->str[k] = (char) buffer[k];
		}
	}

	if (feof(theFile)) {
		return 1;
	} else {
		return 0;
	}
}
