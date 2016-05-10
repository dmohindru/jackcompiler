/* compilationengine.h
 * This file implements CompilationEngine Module of Jack Compiler
 * for Hack computer
 * Author: Dhruv Mohindru
 * Date: 23/03/2016
 */
#include "compilationengine.h"
void constructorCompilationEngine(char *fileName)
{
	//TODO: As check for filename extenstion .asm
	vmFile = fopen ( fileName, "w" );
	//fprintf(vmFile, "XML Content for file %s\n", fileName);
	//incrementer = 0;
	//temp stuff
	//memset(currentFunction,0,50);
	//strcpy(currentFunction,"dummy");
	//temp stuff end
	//if ( asmFile == NULL )
	//{
    //  perror(fileName);
    //}
}
void compileClass()
{
	memset(indentString, 0, 100);
	if(!hasMoreTokens()) //if block for class keyword
	{
		printf("Could not find class keyword at start of file\n");
		return; //or may be exit  
	}
	else
	{
		advance();//get a keyword class
		if(keyWord() != CLASS)
		{
			printf("Could not find class keyword at start of file\n");
			return; //or may be exit
		}
		//write here to vm file <class> tag here
		fprintf(vmFile, "<class>\n");
		strcat(indentString, "  ");//increase indentation
		fprintf(vmFile, "%s<keyword> class </keyword>\n", indentString);
	}
	
	if(!hasMoreTokens()) //if block for class name identifier
	{
		printf("Could not file class name identifier\n");
		return;
	}
	else
	{
		advance();
		if(tokenType() != IDENTIFIER)
		{
			printf("Could not file class name identifier\n");
			return;
		}
		//write here to vm file name identifier tag <identifier> className </identifer>
		fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
	}
	
	if(!hasMoreTokens()) //if block for '{' symbol
	{
		printf("Could not find symbol '{' after class decleration\n");
		return;
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL || symbol() != '{')
		{
			printf("Could not find symbol '{' after class decleration\n");
			return;
		}
		//write here to vm file for symbol { tag <symbol> { </symbol>
		fprintf(vmFile, "%s<symbol> { </symbol>\n", indentString);
	}
	compileClassVarDec(); //compile class variable decleration
	compileSubroutine(); //compile class subroutines
	
	if(!hasMoreTokens()) //if block for '}' symbol
	{
		printf("Could not find symbol '}' for class decleration\n");
		return;
	}
	else
	{
		advance();
		if(tokenType() != SYMBOL || symbol() != '}')
		{
			printf("Could not find symbol '}' for class decleration\n");
			return;
		}
		//write here to vm file for symbol } tag <symbol> } </symbol>
		fprintf(vmFile, "%s<symbol> } </symbol>\n", indentString);
	}
	//finally write a ending class tag </class>
	fprintf(vmFile, "</class>");
}
void compileClassVarDec()
{
	char *varDecToken;
	//check for static or field keyword code block
	if(!hasMoreTokens()) 
	{
		return;
	}
	else
	{
		advance();
		if(tokenType() != KEYWORD || keyWord() != STATIC || keyWord() != FIELD)
		{
			printf("Expected a key word 'static' or 'field' at line no %d\n", currentToken->line);
			return;
		}
		fprintf(vmFile, "%s<classVarDec>\n", indentString);
		strcat(indentString, "  ");
		if(keyWord() == STATIC)
		{
			fprintf(vmFile, "%s<keyword> static </keyword>\n", indentString); 
		}
		else
		{
			fprintf(vmFile, "%s<keyword> field </keyword>\n", indentString); 
		}
	}
	//check for type token code block
	if(!hasMoreTokens()) 
	{
		printf("token type not found at line %d\n", currentToken->line);
		return;
	}
	else
	{
		advance();
		if(tokenType() != KEYWORD || tokenType() != IDENTIFIER)
		{
			keyWord() != INT || keyWord() != CHAR || keyWord() != BOOLEAN
			printf("Expected a type of int, char, boolean or indetifier at line %d", currentToken->line);
			return;
		}
	}
}
void compileSubroutine()
{
}
void compileParameterList()
{
}
void compileVarDec()
{
}
void compileStatements()
{
}
void compileDo()
{
}
void compileLet()
{
}
void compileWhile()
{
}
void compileReturn()
{
}
void compileIf()
{
}
void compileExpression()
{
}
void compileTerm()
{
}
void compileExpressionList()
{
}
