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
					strcat(indentString, "  "); //increase the indent
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
					indentString[strlen(indentString)-2] = '\0'; //decrease the indent
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
	//check for constructor or function or method keyword code block
	//since token has been ready by previous function like compileVarDec or 
	//compileSubroutine which is called recussively
	if(tokenType() == KEYWORD)
	{
		switch(keyWord())
		{
			case CONSTRUCTOR:
				fprintf(vmFile, "%s<subroutineDec>\n", indentString);
				strcat(indentString, "  ");//increase the indent
				fprintf(vmFile, "%s<keyword> constructor </keyword>\n", indentString);
				break;
			case FUNCTION:
				fprintf(vmFile, "%s<subroutineDec>\n", indentString);
				strcat(indentString, "  ");//increase the indent
				fprintf(vmFile, "%s<keyword> function </keyword>\n", indentString); 
				break;
			case METHOD:
				fprintf(vmFile, "%s<subroutineDec>\n", indentString);
				strcat(indentString, "  ");//increase the indent
				fprintf(vmFile, "%s<keyword> method </keyword>\n", indentString);
				break;
			default:   //static or field keyword not found return to compileClass Method
				printf("Not found any thing\n");
				return;
		}
	}
	else //if the token type in not keyword this may be possible some other statement
	{	  //return to compileClass method	
		return;
	}
	//we have come so far so it must be a subroutine Decleration
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
				case VOID:
					fprintf(vmFile, "%s<keyword> void </keyword>\n", indentString);
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Return 'type' unknown at line %d\n", currentToken->line);
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
			printf("Return 'type' unknown at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//subroutine name check block
	if(!hasMoreTokens()) 
	{
		printf("token subroutine name not found at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for subroutine name
		if(tokenType() == IDENTIFIER)
		{
			fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		}
		else //not a valid subroutine name token
		{
			printf("invalid subroutine name at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//symbol '(' check block
	if(!hasMoreTokens()) 
	{
		printf("expected symbol '(' at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '('
		if(tokenType() == SYMBOL && symbol() == '(')
		{
			fprintf(vmFile, "%s<symbol> ( </symbol>\n", indentString);
		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '(' at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//compile paramater list
	compileParameterList();
	//since next token has already been read by compileParameterList() so just
	//check for symbol ')'
	if(tokenType() != SYMBOL || symbol() != ')')
	{
		printf("expected symbol ')' at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	fprintf(vmFile, "%s<symbol> ) </symbol>\n", indentString);
	//compile subroutine body
	//check for symbol '{' for start of subroutine body
	if(!hasMoreTokens()) 
	{
		printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get next token for symbol '{'
		if(tokenType() == SYMBOL && symbol() == '{')
		{
			fprintf(vmFile, "%s<symbol> { </symbol>\n", indentString);
		}
		else //not a valid subroutine name token
		{
			printf("expected symbol '{' at line for subroutine body %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
	}
	//compile var decleration
	compileVarDec();
	//compile statements
	compileStatements();
	//since next token has already been read by compileVarDec() or compileStatements so just
	//check for symbol '}'
	if(tokenType() != SYMBOL || symbol() != '}')
	{
		printf("expected symbol '}' at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	fprintf(vmFile, "%s<symbol> } </symbol>\n", indentString);
	indentString[strlen(indentString)-2] = '\0'; //decrease the indent
	fprintf(vmFile, "%s</subroutineDec>\n", indentString);
	//recussively call compileSubroutine
	if(!hasMoreTokens())
	{
		printf("expected symbol '}' for class decleration\n");
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //advance for next compileSubroutine() function
	}
	compileSubroutine();	
	
}
void compileParameterList()
{
	//make sure to reduce the indent before returning
	if(!hasMoreTokens()) 
	{
		printf("expected return 'type' or symbol ')' for paramenter list at line %d\n", currentToken->line);
		freeToken();
		fclose(vmFile);
		exit(1);
	}
	else
	{
		advance(); //get the next 'type' token or ')' symbol token
		if(tokenType() == KEYWORD) //type of int, char or boolean
		{
			switch(keyWord())
			{
				case INT:
					fprintf(vmFile, "%s<parameterList>\n", indentString);
					strcat(indentString, "  ");//increase the indent
					fprintf(vmFile, "%s<keyword> int </keyword>\n", indentString);
					break;
				case CHAR:
					fprintf(vmFile, "%s<parameterList>\n", indentString);
					strcat(indentString, "  ");//increase the indent
					fprintf(vmFile, "%s<keyword> char </keyword>\n", indentString);
					break;
				case BOOLEAN:
					fprintf(vmFile, "%s<parameterList>\n", indentString);
					strcat(indentString, "  ");//increase the indent
					fprintf(vmFile, "%s<keyword> boolean </keyword>\n", indentString);
					break;
				default: //not a valid keyword found in 'type' decleration
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(vmFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER) //type of some class name 'identifer'
		{
			fprintf(vmFile, "%s<parameterList>\n", indentString);
			strcat(indentString, "  ");//increase the indent
			fprintf(vmFile, "%s<identifier> %s </identifier>\n", indentString, identifier());
		}
		else if(tokenType() == SYMBOL && symbol() == ')') //we just found an empty parameter list
		{
			//indentString[strlen(indentString)-2] = '\0'; //decrease the indent
			fprintf(vmFile, "%s<parameterList>\n", indentString);
			fprintf(vmFile, "%s</parameterList>\n", indentString);
			return;
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
	//loop through multiple parameter list
	while(1)
	{
		if(!hasMoreTokens()) // check for more token of type ','
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
		else
		{
			advance();
		}
		//check for symbol ','
		if(tokenType() == SYMBOL)
		{
			switch(symbol())
			{
				case ')':
					indentString[strlen(indentString)-2] = '\0'; //decrease the indent
					fprintf(vmFile, "%s</parameterList>\n", indentString);
					return;
					//break;
				case ',':
					fprintf(vmFile, "%s<symbol> , </symbol>\n", indentString);
					break;
				default:
					printf("Parameter decleration error at line %d\n", currentToken->line);
					freeToken();
					fclose(vmFile);
					exit(1);
			}
		}
		if(!hasMoreTokens()) // check for more token of type keyword int, char, boolean
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
		else
		{
			advance();
		}
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
					printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
					freeToken();
					fclose(vmFile);
					exit(1);
			}
		}
		else if(tokenType() == IDENTIFIER)
		{
			fprintf(vmFile, "%s<identifier> %s </identifer>\n", indentString, identifier());
		}
		else
		{
			printf("Parameter list declareation unknown 'type' at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
		if(!hasMoreTokens()) // check for more token for identifier
		{
			printf("Parameter decleration error at line %d\n", currentToken->line);
			freeToken();
			fclose(vmFile);
			exit(1);
		}
		else
		{
			advance();
		}
		
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
	
}
void compileVarDec()
{
	//check for var keyword code block
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
				case VAR:
					fprintf(vmFile, "%s<varDec>\n", indentString);
					strcat(indentString, "  "); //increase the indent
					fprintf(vmFile, "%s<keyword> var </keyword>\n", indentString);
					break;
				default:   //var keyword not found return to compileSubroutine Method
					return;
			}
		}
		else //if the token type in not keyword this may be possible some other statement
		{	  //return to compileSubroutine method	
			return;
		}
	}
	//we have come so far so it must be a VarDecleration
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
					indentString[strlen(indentString)-2] = '\0'; //decrease the indent
					fprintf(vmFile, "%s</varDec>\n", indentString);
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
	compileVarDec();
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
