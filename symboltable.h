/* symboltable.h
 * This file implements Symbol Table Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 14/06/2016
 */
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
typedef enum kind
{
	STATIC, FIELD, ARG, VAR
}kind;

void constructorSymbolTable();
void startSubroutine();
void define(char *name, char *type, kind kindType);
int varCount(kind kindType);
kind kindOf(char *name);
char* typeOf(char *name);
int indexOf(char *name);

#endif
