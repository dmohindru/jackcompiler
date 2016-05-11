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
	
	/*if(!hasMoreTokens()) //if block for '}' symbol
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
	}*/
	//token as already been read by previous function just check for 
	//syntatic correctness
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("Could not find symbol '}' for class decleration\n");
		return;
	}
	//write here to vm file for symbol } tag <symbol> } </symbol>
	fprintf(vmFile, "%s<symbol> } </symbol>\n", indentString);
	//finally write a ending class tag </class>
	fprintf(vmFile, "</class>");
}
void compileClassVarDec()
{
	//check for static or field keyword code block
	if(!hasMoreTokens()) //if no more tokens return to compileClass method
	{			
		return;
	}
	else
	{
		advance(); //read next token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case STATIC:
					fprintf(vmFile, "%s<classVarDec>\n", indentString);
					strcat(indentString, "  ");
					fprintf(vmFile, "%s<keyword> static </keyword>\n", indentString);
					break;
				case FIELD:
					fprintf(vmFile, "%s<classVarDec>\n", indentString);
					strcat(indentString, "  ");
					fprintf(vmFile, "%s<keyword> field </keyword>\n", indentString); 
					break;
				default:   //static or field keyword not found return to compileClass Method
					return;
			}
		}
		else //if the token type in not keyword this may be possible some other statement
		{	  //return to compileClass method	
			return;
		}
	}
	//we have come so far so it must be a classVarDecleration
	//check for syntatic correctness and exit program if some errors are found
	//check for 'type' token code block
	if(!hasMoreTokens()) 
	{
		printf("token 'type' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token
		if(tokenType() == KEYWORD)
		{
			switch(keyWord())
			{
				case INT:
					fprintf(vmFile, "%s<keyword> int </keyword>\n", indentString);
					break;
				case CHAR:
					fprintf(vmFile, "%s<keyword> char </keyword>\n", indentString);
					break;
				case BOOLEAN:
					fprintf(vmFile, "%s<keyword> boolean </keyword>\n", indentString);
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Variable declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(vmFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		}
		else  //not found legal keyword or identifier in 'type' decleration
		{
			printf("Variable declareation unknown type at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//variable name if block
	if(!hasMoreTokens()) 
	{
		printf("token variable name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for variable name
		if(tokenType() == IDENTIFIER)
		{
			fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		}
		else //not a valid variable name token
		{
			printf("token variable not found at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//below block of code for occurance of code like in next line
	//(',' varName)* ';' block
	if(!hasMoreTokens())
	{
		printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance();
		if(tokenType() == SYMBOL)
		{
			while(1)
			{
				if(symbol() == ';')
				{
					fprintf(vmFile, "%s<symbol> ; </symbol>\n", indentString);
					//strncpy(indentString, indentString, strlen(indentString)-2);
					indentString[strlen(indentString)-2] = '\0';
					fprintf(vmFile, "%s</classVarDec>\n", indentString);
					break;
				}
				else if(symbol() == ',')
				{
					fprintf(vmFile, "%s<symbol> , </symbol>\n", indentString);
					if(!hasMoreTokens()) //check for variable name token
					{
						printf("token variable name not found at line %d\n", currentToken->line);
						freeToken();
						fclose(vmFile);
						exit(1);
					}
					advance(); //get the next variable name token 
					if(tokenType() == IDENTIFIER)
					{
						fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
					}
					else //not a valid variable name
					{
						printf("token variable not found at line %d\n", currentToken->line);
						freeToken();
						fclose(vmFile);
						exit(1);
					}	
				}
				if(!hasMoreTokens()) // check for more token of type ',' or ';'
				{
					printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
					freeToken();
					fclose(vmFile);
					exit(1);
				}
				else
				{
					advance();
				}
			}
		}
		else
		{
			printf("terminating symbol ';' or ',' not found at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//at the end recussively call compileClassVarDec
	compileClassVarDec();
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
