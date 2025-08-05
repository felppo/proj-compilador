#include "scanner.h"    

//Recebe nome do arquivo
//Preenche input com o conteúdo
Scanner::Scanner(string input){
    pos = 0;
    line = 1;

    ifstream inputFile(input, ios::in);
    string line;

    if(inputFile.is_open()){
        while(getline(inputFile, line)){
            this->input.append(line + '\n');
        }
        inputFile.close();
    }else{
        cout << "Was unexpectedly unable to open file\n";
        exit(EXIT_FAILURE); 
    }
}

int Scanner::getLine(){
    return line;
}

//Método que retorna o próximo token da entrada
Token* Scanner::nextToken(){
    Token* tok;
    string lex;
    int state = 0;

    while(true){
        switch(state){
            case 0:
                if(isspace(input[pos])){ //Ignora espaços e quebras de linha
                    if(input[pos] == '\n'){
                        line++;
                    }
                }else if(isalpha(input[pos])){ //Para reconhecer token <ID>
                    state = 1;
                }else if(isdigit(input[pos])){ //Para reconhecer token <CONST_INT>
                    state = 3;
                }else if(int(input[pos]) == 39){ //Para reconhecer token <CONST_CHAR>
                    state = 5;
                }else if(int(input[pos]) == 34){ //Para reconhecer token <CONST_STRING>
                    state = 8;
                }else if(input[pos] == '+'){
                    pos++;
                    tok = new Token(5);
                    return tok;
                }else if(input[pos] == '-'){
                    pos++;
                    tok = new Token(6);
                    return tok;
                }else if(input[pos] == '*'){
                    pos++;
                    tok = new Token(7);
                    return tok;
                }else if(input[pos] == '/'){ //Verifica se é comentário ou divisão
                    state = 9;
                }else if(input[pos] == '='){
                    state = 12;
                }else if(input[pos] == '&'){
                    pos++;
                    if(input[pos] == '&'){
                        pos++;
                        tok = new Token(16);
                        return tok;
                    }else{
                        lexicalError("TOKEN_ERROR: Did you mean '&&'?");
                    }
                }else if(input[pos] == '|'){
                    pos++;
                    if(input[pos] == '|'){
                        pos++;
                        tok = new Token(17);
                        return tok;
                    }else{
                        lexicalError("TOKEN_ERROR: Did you mean '||'?");
                    }
                }else if(input[pos] == '!'){
                    state = 13;
                }else if(input[pos] == '<'){
                    state = 14;
                }else if(input[pos] == '>'){
                    state = 15;
                }else if(input[pos] == '('){
                    pos++;
                    tok = new Token(19);
                    return tok;
                }else if(input[pos] == ')'){
                    pos++;
                    tok = new Token(20);
                    return tok;
                }else if(input[pos] == '['){
                    pos++;
                    tok = new Token(21);
                    return tok;
                }else if(input[pos] == ']'){
                    pos++;
                    tok = new Token(22);
                    return tok;
                }else if(input[pos] == '{'){
                    pos++;
                    tok = new Token(23);
                    return tok;
                }else if(input[pos] == '}'){
                    pos++;
                    tok = new Token(24);
                    return tok;
                }else if(input[pos] == ','){
                    pos++;
                    tok = new Token(25);
                    return tok;
                }else if(input[pos] == ';'){
                    pos++;
                    tok = new Token(26);
                    return tok;
                }else if(input[pos] == '\0'){ //Para reconhecer token <END_OF_FILE>
                    tok = new Token(27);
                    line--;
                    return tok;
                }else{ //Caracteres não aceitos pela linguagem
                    string error1 = "TOKEN_ERROR: Unsupported character > ";
                    string error2 = " <\n";
                    lexicalError(error1 + input[pos] + error2);
                }

                pos++;
                break;

            case 1: //Input é uma letra
                if(!isalnum(input[pos]) && input[pos] != '_'){
                    state = 2;
                }

                lex += input[pos-1];
                pos++;
                break;

            case 2:
                tok = new Token(1, lex);
                pos--;

                return tok;

            case 3: //Input é um número
                if(!isdigit(input[pos])){
                    if(isalpha(input[pos])){
                        lexicalError("TOKEN_ERROR: Incorrect integer constant format");
                    }else{
                        state = 4;
                    }
                }

                lex += input[pos-1];
                pos++;
                break;

            case 4:
                tok = new Token(2, lex);
                pos--;

                return tok;

            case 5: //Input é '
                if(int(input[pos]) == 92){ //Para reconhecer \0 ou \n
                    state = 6;
                }else if(int(input[pos]) == 39){
                    lexicalError("TOKEN_ERROR: Incorrect character constant format");
                }else if(isprint(input[pos])){
                    lex += input[pos];
                    state = 7;
                }else{
                    lexicalError("TOKEN_ERROR: Unrecognized character. Unable to generate a character constant token");
                }
                
                pos++;
                break;

            case 6: //Input é barra invertida
                if(input[pos] == 'n'){
                    pos++;
                    if(int(input[pos]) == 39){
                        pos++;
                        tok = new Token(3, "'\n'");
                        return tok;
                    }
                }else if(input[pos] == '0'){
                    pos++;
                    if(int(input[pos]) == 39){
                        pos++;
                        tok = new Token(3, "'\0'");
                        return tok;
                    }
                }

                lexicalError("TOKEN_ERROR: Incorrect character constant format");
                break;

            case 7: //Input é um caractere printável
                if(int(input[pos]) == 39){
                    pos++;
                    tok = new Token(3, lex);
                    return tok;
                }else{
                    lexicalError("TOKEN_ERROR: Incorrect character constant format");
                }

            case 8: //Input é "
                if(int(input[pos]) == 34){
                    pos++;
                    tok = new Token(4, lex);
                    return tok;
                }else if(input[pos] == '\0'){
                    lexicalError("ERROR: Unterminated string.");
                }else if(isprint(input[pos])){
                    pos++;
                    lex += input[pos-1];
                }else{
                    lexicalError("TOKEN_ERROR: Unrecognized character. Unable to generate a string constant token");
                }

                break;

            case 9: //Input é /
                if(input[pos] == '/'){
                    state = 10;
                }else if(input[pos] == '*'){
                    state = 11;
                }else{
                    tok = new Token(8);
                    return tok;
                }

                pos++;
                break;
 
            case 10: //Ignora comentários de uma só linha
                if(input[pos] == '\n'){
                    state = 0;
                }
                pos++;
                break;

            case 11: /*Ignora comentários por várias linhas*/
                if(input[pos] == '\0'){
                    lexicalError("ERROR: Unterminated comment");
                }else if(input[pos] == '*'){
                    pos++;
                    if(input[pos] == '/'){
                        line++;
                        state = 0;
                    }
                }else if(input[pos] == '\n'){
                    line++;
                }

                pos++;
                break;

            case 12: //Input é =
                if(input[pos] == '='){
                    pos++;
                    tok = new Token(10);
                    return tok;
                }else{
                    tok = new Token(9);
                    return tok;
                }

            case 13: //Input é !
                if(input[pos] == '='){
                    pos++;
                    tok = new Token(11);
                    return tok;
                }else{
                    tok = new Token(18);
                    return tok;
                }

            case 14: //Input é <
                if(input[pos] == '='){
                    pos++;
                    tok = new Token(13);
                    return tok;
                }else{
                    tok = new Token(12);
                    return tok;
                }

            case 15: //Input é >
                if(input[pos] == '='){
                    pos++;
                    tok = new Token(15);
                    return tok;
                }else{
                    tok = new Token(14);
                    return tok;
                }

            default:
                lexicalError("TOKEN_ERROR");
        }
    }
}

void Scanner::lexicalError(string msg){
    cout << "Linha " << line << " -> " << msg << endl;
    
    //exit(EXIT_FAILURE);
    pos++;
}
