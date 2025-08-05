#include "symboltable.h"

//Construtor que instancia uma nova tabela sem especificar uma tabela pai.
SymbolTable::SymbolTable(){
    parent = 0;
}

//Construtor que instancia uma nova tabela especificando uma tabela pai.
SymbolTable::SymbolTable(SymbolTable* p){
    parent = p;
}

//Método para adicionar palavras à tabela de símbolos
//Primeiro verifica se já está contida na tabela
//Caso contrário, insere a palavra na hierarquia
bool SymbolTable::add(STEntry* t){
    if (!(symbols.find(t->token->lexeme) == symbols.end())){
        //cout << " >>>>>> " << t->token->lexeme << " ja existe" << endl;
        return false;
    }
    //cout << " >>>>>> palavra enfiada" << endl;
    symbols.insert({t->token->lexeme, t});
    return true;
}

//Tenta remover um símbolo da tabela.
bool SymbolTable::remove(string name){
    return symbols.erase(name) != 0;
}

//Limpa a tabela.
void SymbolTable::clear(){
    symbols.clear();
}

//Verifica se a tabela está vazia.
bool SymbolTable::isEmpty(){
    return symbols.empty();
}

//Método para encontrar uma palavra especificada
//Primeiro verifica se ela está contida na tabela, se não, então retorna 0
//Caso encontre o símbolo, vasculha a hierarquia em busca do endereço e retorna o ponteiro
STEntry *SymbolTable::get(string name){
    SymbolTable *table = this;

    auto s = table->symbols.find(name);

    while (s == table->symbols.end()){
        table = table->parent;
        if (table == 0)
            return 0;

        s = table->symbols.find(name);
    } 
    
    return s->second;
}

SymbolTable *SymbolTable::getParent(){
    return parent;
}
