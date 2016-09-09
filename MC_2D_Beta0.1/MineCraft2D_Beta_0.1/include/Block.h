#ifndef BLOCK_H
#define BLOCK_H

#include <string>
using namespace std;

enum Blocktype{
               emptyblock,
               wood,
               leaves,
               earth,
               stone,
               coal,
               ironstone,
               water,
               woodenplank,
               workbench,
               bedrock,
               stove,
};


class Block
{
    public:
        Block();
        Block(string name,string sym,int hp,int maxhp,Blocktype type,int type_num);
        virtual ~Block();
        string name;
        string sym;
        int hp;
        int maxhp;
        Blocktype type;
        int type_num;
};

#endif // BLOCK_H
