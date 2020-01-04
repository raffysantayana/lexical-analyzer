/* Filename: LexicalAnalyzer.cpp

CSC 340-01- Programming Languages
Raffy Santayana
	Worked together with Matthew Solorzano

A lexical analyzer system for simple arithmetic expressions.

Operator/keyword: +, -, *, /, %, (, ), ;, TRUE, FALSE, ||, &&, !, if, else, {, }, =
id: (a+b+...+z+A+B+...Z)(a+b+...+z+A+B+...Z+0+1+2+...+9)*
int_constant: (0+1+2+...+9)(0+1+2+...+9)*

*/

#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

/* Global declarations */
/* Variables */

int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream in_fp("syntax.txt");

/* Function declarations */
void getChar();
void addChar();
void getNonBlank();
void ifstmt();
void boolexpr();
void boolterm();
void boolfactor();
void assign();
void expr();
void term();
void factor();
int lex();      // to get the nextToken


/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define MOD_OP 25	/*Added this identifier for the modulo operator (%)*/
#define EQUAL_SIGN 26 /*Added this identifier for the equals sign (=)*/
#define LEFT_PAREN 27	/*Added this identifier for the left parenthese ( */
#define RIGHT_PAREN 28	/*Added this identifier for the right parenthese ) */
#define SEMICOLON 29	/*Added this identifier for the semicolon ;*/
#define LEFT_CURLY 30	/*Added this identifier for the left curly brace { */
#define RIGHT_CURLY 31	/*Added this identifier for the right curly brace } */
#define NOT 32	/*Added this identifier for the NOT operator (!)*/
#define AND 33	/*Added this identifier for the AND operator (&&)*/
#define OR 34	/*Added this identifier for the OR operator (||)*/
#define IF 35	/*Added this identifier for the if statmenet*/
#define ELSE 36	/*Added this identifier for the else statement*/
#define TRUE 37	/*Added this identifier for TRUE*/
#define FALSE 38	/*Added this identifier for FALSE*/
/******************************************************/
/* main driver */
void main(){
	/* Open the input data file and process its contents */

	if (in_fp.fail())
	{
		cout << "File could not be opened\n";
		cin.get();
		exit(1);
	}
	else {
		getChar();
		lex();  // Getting the nextToken
		ifstmt();
	}
	in_fp.close();

	system("PAUSE");
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses
and return the token */
int lookup(char ch) {
	switch (ch){
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
	case '%':	/*identifying the modulo operator*/
		addChar();
		nextToken = MOD_OP;
		break;
	case '=':	/*identifying the equals sign*/
		addChar();
		nextToken = EQUAL_SIGN;
		break;
	case ';':	/*identifying the semicolon*/
		addChar();
		nextToken = SEMICOLON;
		break;
	case '{':	/*identifying the left curly brace*/
		addChar();
		nextToken = LEFT_CURLY;
		break;
	case '}':	/*identifying the right curly brace*/
		addChar();
		nextToken = RIGHT_CURLY;
		break;
	case '!':	/*identifying the NOT operator*/
		addChar();
		nextToken = NOT;
		break;
	case '&':	/*identifying the AND operator*/
		addChar();
		getChar();
		addChar();
		getChar;
		if (!strcmp(lexeme, "&&"))
		{
			nextToken = AND;
			break;
		}
		else{
			cout << "Not a valid token";
			break;
		}
	case '|':	/*identifying the OR operator*/
		addChar();
		getChar();
		addChar;
		getChar;
		if (strcmp(lexeme, "||"))
		{
			nextToken = OR;
			break;
		}
		else {
			cout << "Not a valid token";
			break;
		}
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}
/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(){
	if (lexLen <= 98)
	{
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0;
	}
	else
		cout << " Error - lexeme is too long \n";
}
/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
	in_fp.get(nextChar);
	if (in_fp.eof())   // if no more character in the file
		nextChar = EOF;

	if (nextChar != EOF) {
		if (isalpha(nextChar))
		{
			charClass = LETTER;
		}
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else charClass = UNKNOWN;
	}
	else
		charClass = EOF;
}
/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}
/***************************************************** /
/* lex - a simple lexical analyzer for arithmetic
expressions */
int lex() {
	lexLen = 0;
	getNonBlank();
	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT)
		{
			addChar();
			getChar();
		}
		if (!strcmp(lexeme, "if"))
		{
			nextToken = IF;
			break;
		}
		else if (!strcmp(lexeme, "else")) { 
			nextToken = ELSE;
			break;
		}
		else if (!strcmp(lexeme, "TRUE")) {
			nextToken = TRUE;
			break;
		}
		else if (!strcmp(lexeme, "FALSE")) {
			nextToken = FALSE;
			break;
		}
		else {
			nextToken = IDENT;
			break;
		}
		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;
		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */
	cout << "Next token is: " << nextToken
		<< "       Next lexeme is " << lexeme << "\n";
	return nextToken;
} /* End of function lex */

//Outputs string "Error"
void error() {
	cout << "Error\n";
}

  //The rules of the EBNF
void ifstmt() {
	if (nextToken == IF) {
		lex();
		if (nextToken == LEFT_PAREN) {
			lex();
			while (nextToken == TRUE || nextToken == FALSE || nextToken == NOT || nextToken == LEFT_PAREN || nextToken == AND || nextToken == OR)
				boolexpr();
			if (nextToken == RIGHT_PAREN) {
				lex();
				if (nextToken == LEFT_CURLY) {
					lex();
					assign();
					if (nextToken == RIGHT_CURLY) {
						lex();
						if (nextToken == ELSE) {
							lex();
							if (nextToken == LEFT_CURLY) {
								lex();
								assign();
								if (nextToken == RIGHT_CURLY) 
									lex();
								else
									error();
							}
							else
								error();
						}
					}
					else
						error();
				}
				else
					error();
			}
			else
				error();
		}
		else
			error();
	}
	else
		error();
}

void boolexpr(){
	if (nextToken == TRUE || nextToken == FALSE || nextToken == NOT || nextToken == LEFT_PAREN || nextToken == AND)
		boolterm();
	else if (nextToken == OR) {
		lex();
		boolterm();
	}
	else
		error();
}

void boolterm(){
	if (nextToken == TRUE || nextToken == FALSE || nextToken == NOT || nextToken == LEFT_PAREN)
		boolfactor();
	else if (nextToken == AND) {
			lex();
			boolfactor();
		}
	else
		error();
}

void boolfactor(){
	if (nextToken == TRUE || nextToken == FALSE)
		lex();
	else if (nextToken == NOT) {
		lex();
		boolfactor();
	}
	else if (nextToken == LEFT_PAREN) {
		lex();
		boolexpr();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			error();
	}
	else
		error();
}

void assign(){
	if (nextToken == IDENT) {
		lex();
		if (nextToken == EQUAL_SIGN) {
			lex();
			expr();
			if (nextToken == SEMICOLON) {
				lex();
				if (nextToken == IDENT)
					assign();
			}
			else
				error();
		}
		else
			error();
	}
}

void expr() {
	term();

	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
}

void term() {
	factor();
	while (nextToken == MULT_OP || nextToken == DIV_OP || nextToken == MOD_OP) {
		lex();
		factor();
	}
}

void factor() {
	if (nextToken == IDENT || nextToken == INT_LIT)
		lex();

	else if (nextToken == LEFT_PAREN) {
		lex();
		expr();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			error();
	}
	else
		error();
}