#include "parser.cpp"

//Versão em inglês porque sim
//Ganhe acesso à versão em pt-BR por apenas R$37,98 mensalmente

//---------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

    if(argc != 2){
        cout << "Execução na linha de comando: ./exec nome_input.cmm\n";
        return 1;
    }

    Parser *parser = new Parser(argv[1]);

    SymbolTable *table = parser->getTable();

    table->add(new STEntry(new Token(1, "if"), true));
    table->add(new STEntry(new Token(1, "else"), true));
    table->add(new STEntry(new Token(1, "for"), true));
    table->add(new STEntry(new Token(1, "while"), true));
    table->add(new STEntry(new Token(1, "return"), true));
    table->add(new STEntry(new Token(1, "int"), true));
    table->add(new STEntry(new Token(1, "char"), true));
    table->add(new STEntry(new Token(1, "void"), true));

    parser->run();
    
    return 0;
}