#include "Block.h"

Block::Block()
{
    this->name="";
    this->sym="";
    this->hp=0;
    this->maxhp=0;
    this->type_num=0;
}
Block::Block(string name,string sym,int hp,int maxhp,Blocktype type,int type_num)
{
    this->name=name;
    this->sym=sym;
    this->hp=hp;
    this->maxhp=maxhp;
    this->type=type;
    this->type_num=type_num;
}
Block::~Block()
{
    //dtor
}
