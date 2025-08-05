#include "token.h"

class STEntry{
    public:
        Token *token;
        bool reserved;

        STEntry(Token*);   
        STEntry(Token*, bool);
};
