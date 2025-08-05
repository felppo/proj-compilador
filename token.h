#include <iostream>
#include <ctype.h>
#include <string>

using namespace std;

//Tipos de token aceitos pelo compilador
enum Names{
    UNDEF, //0
    ID, //1
    CONST_INT, //2
    CONST_CHAR, //3
    CONST_STRING, //4
    //BINOP
    ADD, //5
    MINUS, //6
    MULT, //7
    DIV, //8
    //RELOP
    ATRIB, //9
    EQ, //10
    NE, //11
    LT, //12
    LE, //13
    GT, //14
    GE, //15
    //BOOLOP
    AND, //16
    OR, //17
    NOT, //18
    //SEP
    L_PAR, //19
    R_PAR, //20
    L_BRCKT, //21
    R_BRCKT, //22
    L_BRACE, //23
    R_BRACE, //24
    COMMA, //25
    SEMICOLON, //26
    END_OF_FILE //27
};

//Formatos de token
class Token{
    public: 
        int name;
        int attribution;
        string lexeme;
    
        //<OP>
        //<SEP>
        Token(int name){
            this->name = name;
            attribution = UNDEF;
        }

        //<ID, name>
        //<CONT_CHAR, value>
        //<CONT_STRING, value>
        Token(int name, string lex){
            this->name = name;
            attribution = UNDEF;
            lexeme = lex;
        }
        
        //<CONST_INT, value>
        Token(int name, int attr){
            this->name = name;
            attribution = attr;
        }
};