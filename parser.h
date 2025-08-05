#include "symboltable.cpp"
#include "scanner.cpp"

class Parser{
	private:
		Scanner *scanner;
		Token *lToken; //Token presente

		void advance();
		void match(int);
		
	public:
		Parser(string);
        SymbolTable *getTable();

		void run();
        void program();
        void function();
        void varDeclaration();
        bool type();
        void paramTypes();
        void statement();
        void assign();
        void expression();
        bool binOP();
        bool relOP();
        bool logOP();

        void operation();
        bool checkForMore();
        void bracedStatement();
        void syntaxError(string);
};