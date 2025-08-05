#include "parser.h"

SymbolTable *table = new SymbolTable();

string *vet;

void allocArray(){
    vet = new string[28];

    vet[0] = "UNDEF"; //0
    vet[1] = "ID"; //1
    vet[2] = "INT"; //2
    vet[3] = "CHAR"; //3
    vet[4] = "STRING"; //4
    vet[5] = "+"; //5
    vet[6] = "-"; //6
    vet[7] = "*"; //7
    vet[8] = "/"; //8
    vet[9] = "="; //9
    vet[10] = "=="; //10
    vet[11] = "!="; //11
    vet[12] = "<"; //12
    vet[13] = "<="; //13
    vet[14] = ">"; //14
    vet[15] = ">="; //15
    vet[16] = "&&"; //16
    vet[17] = "||"; //17
    vet[18] = "!"; //18
    vet[19] = "("; //19
    vet[20] = ")"; //20
    vet[21] = "["; //21
    vet[22] = "]"; //22
    vet[23] = "{"; //23
    vet[24] = "}"; //24
    vet[25] = ","; //25
    vet[26] = ";"; //26
    vet[27] = "END_OF_FILE"; //27
}

void freeArray(){
    delete[] vet;
}

SymbolTable *Parser::getTable(){
	return table;
}

Parser::Parser(string input){
	scanner = new Scanner(input);
}

void Parser::advance(){
	lToken = scanner->nextToken();
}

void Parser::match(int tok){
	if(lToken->name == tok){
		if(tok == 1)
            table->add(new STEntry(lToken));

		advance();
	}else{
		allocArray();
		string msg1 = "SYNTAX_ERROR: Compiler was expecting '";
		string msg2 = "'. Received '";
		string msg3 = "' instead";
		syntaxError(msg1 + vet[tok] + msg2 + vet[lToken->name] + msg3);
	}
}

void Parser::run(){
	advance();	

	program();
	
	cout << "\n\nYour code (as weird as it was) has been compiled successfully. You're welcome ;)\n";
}

void Parser::program(){
    while(lToken->name != 27)
        function();

    match(END_OF_FILE);
}

void Parser::function(){
    if(lToken->lexeme == "void"){
    	advance();
    }else{
    	if(!type()){
    		if(lToken->name == 1){
				string msg1 = "Unrecognized type '";
				string msg2 = "'";
				syntaxError(msg1 + lToken->lexeme + msg2);
			}
    	}
    }

    match(1); //<ID>
    match(19); //<L_PAR>
    paramTypes();
    match(20); //<R_PAR>
    match(23); //<L_BRACE>

    while(type()){
    	do{
			varDeclaration();
		}while(checkForMore());

		match(26); //<SEMICOLON>
    }

    while(lToken->name != 24){ //<R_BRACE>
    	statement();

    	if(lToken->name == 24) //<R_BRACE>
    		break;
    }

    match(24); //<R_BRACE>
}

bool Parser::type(){
	if(lToken->lexeme == "char" || lToken->lexeme == "int"){
		advance();
		return true;
	}
	
	return false;
}

void Parser::paramTypes(){
	if(lToken->lexeme == "void"){ //Verifica se não existem parâmetros
		advance();
	}else{
		do{
			if(!type())
				syntaxError("Expected type declaration. Did you mean 'char' or 'int'?");

			match(1); //<ID>

			//Verifica se o parâmetro é um vetor
			if(lToken->name == 21){ //<L_BRCKT>
				advance();
				match(22); //<R_BRCKT>
			}
		}while(checkForMore()); //Verifica se existe mais de um parâmetro
	}
}

bool Parser::checkForMore(){
	if(lToken->name == 1){ //<ID>
		syntaxError("SYNTAX_ERROR: Expected ',' between parameters");
	}else if(lToken->name != 25){ //<COMMA>
		return false;
	}
	
	advance();
	return true;
}

void Parser::varDeclaration(){
	match(1); //<ID>

	//Verifica se a variável é um vetor
	if(lToken->name == 21){ //<L_BRCKT>
		advance();
		match(2); //<CONST_INT>
		match(22); //<R_BRCKT>
	}
}

//Recebe na posição atual, sem avanço
void Parser::expression(){ 
	if(lToken->name == 6 || lToken->name == 18){ //<MINUS> e <NOT>
		advance();
		expression();
	}else if(lToken->name == 19){ //Operação entre parênteses
		advance();
		expression();
		match(20); //<R_PAR>
	}else if(lToken->name == 2 || lToken->name == 3 || lToken->name == 4){ //Constantes (<CONST_INT>, <CONST_CHAR>, <CONST_STRING>)
		advance();

		if(binOP() || relOP() || logOP()){
			operation();
		}
	}else if(lToken->name == 1){ //<ID>
		advance();
		if(lToken->name == 19){ //<L_PAR>
			advance();

			if(lToken->name != 20){ //<R_PAR>
				do{
					expression();
				}while(checkForMore());
			}

			match(20); //<R_PAR>
		}else if(lToken->name == 21){ //<L_BRCKT>
			advance();
			expression();
			match(22); //<R_BRCKT>
		}else if(binOP() || relOP() || logOP()){
			operation();
		}
	}else{
		syntaxError("Unrecognized expression format");
	}
}

void Parser::operation(){
	expression();
}

void Parser::assign(){
	if(lToken->name == 21){ //<L_BRCKT>
		advance();
		expression();
		match(22); //<R_BRACKT>
	}
	match(9); //<ATRIB>
	expression();
}

//Recebe na posição atual, retorna o token seguinte
void Parser::bracedStatement(){
	if(lToken->name == 23){ //<L_BRACE>
		advance();

		while(lToken->name != 24) //<R_BRACE>
			statement();

		advance();
	}else{
		statement();
	}
}

//Recebe na posição atual, retorna token sem avanço
void Parser::statement(){ 
	if(lToken->name == 1){ //Recebe ID. Reconhece IF, ELSE, WHILE, FOR e RETURN
		if(lToken->lexeme == "if"){ //IF
			advance();
			match(19); //<L_PAR>
			expression();
			match(20); //<R_PAR>

			bracedStatement();

			if(lToken->name == 1 && lToken->lexeme == "else"){ //ELSE
				advance();
				bracedStatement();
			}
		}

		else if(lToken->lexeme == "while"){ //WHILE
			advance();
			match(19); //<L_PAR>
			expression();
			match(20); //<R_PAR>
			bracedStatement();
		}

		else if(lToken->lexeme == "for"){ //FOR
			advance();
			match(19); //<L_PAR>

			if(lToken->name == 1){ //<ID>
				advance();
				assign();
			}

			match(26); //<SEMICOLON>

			if(lToken->name != 26){ //<SEMICOLON>
				expression();
			}

			advance();

			if(lToken->name == 1){ //<ID>
				advance();
				assign();
			}

			match(20); //<R_PAR>
			bracedStatement();
		}

		else if(lToken->lexeme == "return"){ //RETURN
			advance();
			expression();
			match(26); //<SEMICOLON>
		}

		else{ //<ID>
			advance();

			if(lToken->name == 19){ //<L_PAR>
				advance();

				if(lToken->name != 20){ //<R_PAR>
					do{
						expression();
					}while(checkForMore());
				}

				match(20); //<R_PAR>
				match(26); //<SEMICOLON>
			}else{
				assign();
				match(26); //<SEMICOLON>
			}
		}
	}else if(lToken->name == 26){ //SEMICOLON
		advance();
	}else{
		syntaxError("SYNTAX_ERROR: Unrecognized statement format");
	}
}

//Verificam a ocorrência de operadores
//Retornam true ou false com um avanço de token
bool Parser::binOP(){
	if(lToken->name == 5 || lToken->name == 6 || lToken->name == 7 || lToken->name == 8){
		advance();
		return true;
	}else{
		return false;
	}
}

bool Parser::relOP(){
	if(lToken->name == 10 || lToken->name == 11 || lToken->name == 12 || lToken->name == 13 || lToken->name == 14 || lToken->name == 15){
		advance();
		return true;
	}else{
		return false;
	}
}

bool Parser::logOP(){
	if(lToken->name == 16 || lToken->name == 17){
		advance();
		return true;
	}else{
		return false;
	}
}

void Parser::syntaxError(string msg){
	cout << endl << "Linha " << scanner->getLine() << " -> " << msg << endl;

	freeArray();

	//exit(EXIT_FAILURE);
	advance();
}
