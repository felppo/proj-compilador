#include <fstream>

class Scanner{
    private: 
        string input; //Armazena o texto de entrada
        int pos; //Posição atual
        int line; //Linha no código original
    
    public:
        //Construtor
        Scanner(string);

        //Pega o número da linha
        int getLine();
    
        //Método que retorna o próximo token da entrada
        Token* nextToken();        
    
        //Método para manipular erros
        void lexicalError(string);
};
