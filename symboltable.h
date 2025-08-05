#include <map>
#include "stentry.cpp"

//Classe que representa uma tabela de símbolos.
//Utiliza <map> para armazenar os símbolos.
class SymbolTable{
    private:
        SymbolTable* parent; //Referência à tabela pai (escopo imediatamente anterior).
        map<string, STEntry*> symbols; //Armazena os símbolos do escopo corrente.

    public:
        SymbolTable();
        SymbolTable(SymbolTable*);

        bool add(STEntry*);
        bool remove(string);
        void clear();
        bool isEmpty();
        STEntry *get(string);
        SymbolTable *getParent();
};
