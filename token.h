/*
 * token.h
 */

#pragma once
#define MAX_TOKEN_LENGTH 200
#include <stdio.h>

typedef struct {
  int type;             // token type
  int literal_value;    // this field used to store the value of literal tokens
  int arg_no;           // this field used to store the index of argument literals argN
  char str[MAX_TOKEN_LENGTH];
} token;



//Returns 0 on success
//Returns 1 on success and final token
//Returns 2 on broken token
int read_token (token *theToken, FILE *theFile);
