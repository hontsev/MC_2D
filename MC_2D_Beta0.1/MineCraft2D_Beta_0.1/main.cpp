#include <iostream>
#include <cstdio>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <stdlib.h>
#include <ctime>

#include "include/Block.h"

#define M_MAXNUM 1
#define ARTICLE_MYMAXNUM 20
#define ARTICLE_MYSYNMAXNUM 5
#define ARTICLE_SYNMAXNUM 10
#define ARTICLE_BOXMAXNUM 20
#define INV_MAXNUM 10
#define Map_MAXLEN 100
#define Map_MAXWID 1000
#define ENTITY_MAXNUM 20
#define SCREENWID 64
#define SCREENLEN 23
#define ARTICLELIST_MAXNUM 50
#define BLOCKLIST_MAXNUM 50
#define ARTICLE_MAXSTOVENUM 50

using namespace std;

int Randint(int a,int b) ;

const int article_mywid=ARTICLE_MYMAXNUM/4;
const int article_mylen=4;
const int article_mysynwid=2;
const int article_mysynlen=2;
const int article_synwid=3;
const int article_synlen=3;
const int article_maxgroupnum=64;


enum Articletype{
                 emptyarticle,
                 awood,
                 awoodenplank,
                 awoodenstick,
                 awoodenpick,
                 awoodensword,
                 awoodenspade,
                 awoodenaxe,
                 aearth,
                 aleaves,
                 astone,
                 astonestick,
                 astonepick,
                 astonesword,
                 astonespade,
                 astoneaxe,
                 astove,
                 aironstone,
                 airon,
                 aironstick,
                 aironpick,
                 aironsword,
                 aironspade,
                 aironaxe,
                 ascissors,
                 acoal,
                 aworkbench,
                 aapple
                 };

enum Gamemode{
              common,
              use,
              gameover,
              mainlist
              };

enum Choosemovearea{
                    myarticlearea,
                    mysynthesisarea,
                    synthesisarea,
                    stovearea
                    };




struct Article                                      //物品
{
    string name;
    string sym;
    Articletype type;
    int type_num;
    int num;
};

struct Stove
{
    bool havestove;
    bool firing;
    double firetime_max;
    double firetime;
    double article_firetime;
    struct Article firingarticle;
    struct Article oldarticle;
    struct Article firearticle;
    struct Article newarticle;
};

struct Stove Stovelist[ARTICLE_MAXSTOVENUM];

Block Blocklist[BLOCKLIST_MAXNUM]            //方块列表  type_num:表示透光程度，目前暂时用来标识空气、水等。
{
 //名字,符号,最大生命,当前生命,类型,类型附加值
    *new Block("空气","  ",1,1,emptyblock,9),
    {"土","□",10,10,earth,0},
    {"岩石","■",33,33,stone,0},
    {"煤矿","▲",34,34,coal,0},
    {"铁矿","★",37,37,ironstone,0},
    {"木头","川",20,20,wood,0},
    {"木板","三",20,20,woodenplank,0},
    {"水","~~",1,1,water,1},
    {"树叶","叕",2,2,leaves,1},
    {"工作台","田",20,20,workbench,1},
    {"基岩","爨",-10,-10,bedrock,0},
    {"熔炉","鼎",33,33,stove,0}
};

struct Article Articlelist[ARTICLELIST_MAXNUM]      //物品列表  type_num:最后一位表示类型，0:空 1:可摆放物 2:工具 3:消耗品 4:其他
{
  //名字,符号,类型,类型附加值,数目
    {"空","__",emptyarticle,0,1},
    {"泥土","土",aearth,1,0},
    {"碎石","石",astone,1,0},
    {"煤","煤",acoal,104,0},
    {"铁矿","铁矿",aironstone,1,0},
    {"铁锭","铁",airon,4,0},
    {"原木","木",awood,1,0},
    {"木板","板",awoodenplank,1,0},
    {"木镐","木镐",awoodenpick,602,0},
    {"石镐","石镐",astonepick,1302,0},
    {"铁镐","铁镐",aironpick,4002,0},
    {"木斧","木斧",awoodenaxe,602,0},
    {"石斧","石斧",astoneaxe,1302,0},
    {"铁斧","铁斧",aironaxe,4002,0},
    {"木剑","木剑",awoodensword,602,0},
    {"石剑","石剑",astonesword,1302,0},
    {"铁剑","铁剑",aironsword,4002,0},
    {"木铲","木铲",awoodenspade,602,0},
    {"石铲","石铲",astonespade,1302,0},
    {"铁铲","铁铲",aironspade,4002,0},
    {"苹果","苹果",aapple,33,0},
    {"木棍","木棍",awoodenstick,4,0},
    {"工作台","台",aworkbench,1,0},
    {"熔炉","炉",astove,1,0},
    {"树叶","叶",aleaves,1,0},
    {"剪子","剪",ascissors,502,0}
};

struct Article Emptyarticle={"空","__",emptyarticle,0,1};
///////////////////////////////////////

string MAJOR_SYM = "囧";
int MAJOR_LEN = 1;
int MAJOR_WID = 5;
int nowx;
int nowy;
int mainx;
int mainy;
int GameRun;
int speedx;
int speedy;
int jump;
int face;
int Inventory[INV_MAXNUM+1];
int mainlistchoose;
int Nowtime = 0;
int Nowstove ;
double hp;
double hunger;
bool testmode;
bool opensyn;
bool openstove;
bool openmyinv;
bool openbox;
struct Article Myarticles[ARTICLE_MYMAXNUM+1];
struct Article Synarticles[ARTICLE_SYNMAXNUM+1];
struct Article Mysynarticles[ARTICLE_MYSYNMAXNUM+1];
//struct Article Stovearticles[ARTICLE_MAXSTOVENUM][ARTICLE_STOMAXNUM+1];
struct Article * Nowchoosearticle = (struct Article*)malloc(sizeof(struct Article));
struct Article * Targetarticle = (struct Article*)malloc(sizeof(struct Article));
int Nowchoose;
int Nowuse;
int nowcatch;
Gamemode gamemode;
Choosemovearea choosearea;
char Outimg[Map_MAXLEN+1][Map_MAXWID+1];
struct Block Blockinfo[Map_MAXLEN+1][Map_MAXWID/2+1];

struct Block * Nowhitblock = (struct Block*)malloc(sizeof(struct Block));

///////////////////////////////////////
class Message                                       //输出消息的类
{
public:
    Message(int x, int y);
    void Addmessage(string mess);
    void Writeall();
    void Clean();
private:
    //int message_maxnum;
    int x0;
    int y0;
    string messagelist[M_MAXNUM];
    int nowmessagenum;
    void Gotoxy(int x, int y);
};

class Interface                                     //输出图形的类
{
public:
    Interface();
    void Writeoutimg();
    void Articlechoosemove(int dirc);
    void Writeinventory();
    void Writeoutmainlist();
    void Writeoutblank();
private:
    void Gotoxy(int x, int y);
    void Writeblank(int x, int y);
    void Writeunit(int x, int y, string sym);
    void Writecircle(int x, int y);
    void Writebars(int x, int y);
    void Writewords(int x, int y, string words);
};

class Drawmap
{
public:
    Drawmap();
    void Redrawwholemap();
    void Drawtree(int x, int y, int treekind);
    void Drawmount(int x, int y, int height);
    void Drawhole(int x, int y, int holesize);
    void Drawminerals(int uph, int downh, int pronum, int type);
    void Drawpassage(int x,int y,int length,int dir);
private:
    void Drawintomap(int x, int y, int num);
};
///////////////////////////////////////

Message::Message(int x, int y)
{
    x0 = x;
    y0 = y;
    nowmessagenum = 0;
    for(int n=0;n<M_MAXNUM;n++)
        {
            messagelist[n]="          ";
        }
}

void Message::Addmessage(string mess)               //添加信息
{
    if(nowmessagenum >= M_MAXNUM)
    {
        for(int n=0;n<M_MAXNUM-1;n++)
        {
            messagelist[n]=messagelist[n+1];
        }
        messagelist[M_MAXNUM-1]=mess;
    }
    else
    {
        messagelist[nowmessagenum] = mess;
        nowmessagenum++;
    }
}

void Message::Writeall()                            //输出信息
{
    for(int n=0;n<nowmessagenum;n++)
        {
            Gotoxy(x0,y0+n);
            cout<<messagelist[n];
            if(messagelist[n].length()<15)for(int s=messagelist[n].length();s<=15;s++)printf(" ");
        }
}

void Message::Gotoxy(int x, int y)                  //设置光标到坐标
{
    short x0=x;
    short y0=y;
	COORD p = {x0,y0};
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, p);
}

void Message::Clean()                               //清空
{
    for(int n=0;n<M_MAXNUM;n++)
        {
            messagelist[n]="             ";
        }
}

///////////////////////////////////////

Interface::Interface()
{
    for(int n=0;n<Map_MAXLEN/2;n++)
    {
        for(int m=0;m<Map_MAXWID/2;m++)
        {
            Blockinfo[n][m].name=" ";
            Blockinfo[n][m].hp=0;
            Blockinfo[n][m].type=emptyblock;
            Blockinfo[n][m].type_num=0;
        }
    }
}

void Interface::Writeoutimg()                       //输出图像
{
    nowx=mainx-SCREENWID/2-1;
    nowy=mainy-SCREENLEN/2-1;
    for(int n=1;n<=Map_MAXLEN;n++)
    {
        for(int m=1;m<=Map_MAXWID/2;m++)
        {
            if(Blockinfo[n][m].hp!=0){Outimg[n][m*2-1]=Blockinfo[n][m].sym[0];Outimg[n][m*2]=Blockinfo[n][m].sym[1];}
            else {Outimg[n][m*2-1]=' ';Outimg[n][m*2]=' ';}
        }
    }
    /////////
    for(int n=0;n<(int)MAJOR_SYM.length();n++){Outimg[mainy][mainx+n-2]=MAJOR_SYM[n];}
    /////////
    Writeinventory();

    for(int n=1;n<=SCREENLEN;n++)
    {
        Gotoxy(1,n);
        for(int m=1;m<=SCREENWID;m++)
        {
            if(n+nowy>Map_MAXLEN||n+nowy<=0||m+nowx<=0||m+nowx>Map_MAXWID&&Outimg[n+nowy][m+nowx]!=' ')printf(" ");
            else
                printf("%c",Outimg[n+nowy][m+nowx]);
        }
    }
}

void Interface::Articlechoosemove(int dirc)         //移动选框
{
    switch(dirc)
    {
    case 1:
        if(choosearea == myarticlearea)
        {
            if(Nowchoose - article_mywid <= 0)
            {
                if(opensyn){choosearea = synthesisarea;Nowchoose = ARTICLE_SYNMAXNUM;}
                else if(openstove){choosearea = stovearea;Nowchoose = 3;}
                else{choosearea = mysynthesisarea;Nowchoose = ARTICLE_MYSYNMAXNUM;}
            }
            else{Nowchoose -= article_mywid;}
        }
        else if(choosearea == mysynthesisarea)
        {
            if(Nowchoose - article_mysynwid >= 1){Nowchoose -= article_mysynwid;}
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose - article_synwid >= 1){Nowchoose -= article_synwid;}
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose - 1 >= 1){Nowchoose -= 1;}
        }
    break;
    case 2:
        if(choosearea == myarticlearea)
        {
            if(Nowchoose + 1 <= ARTICLE_MYMAXNUM){Nowchoose += 1;}
        }
        else if(choosearea == mysynthesisarea)
        {
            if(Nowchoose + 1 <= ARTICLE_MYSYNMAXNUM){Nowchoose += 1;}
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose + 1 <= ARTICLE_SYNMAXNUM){Nowchoose += 1;}
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose + 1 <= 3){Nowchoose += 1;}
        }
    break;
    case 3:
        if(choosearea == myarticlearea)
        {
            if(Nowchoose + article_mywid <= ARTICLE_MYMAXNUM){Nowchoose += article_mywid;}
        }
        else if(choosearea == mysynthesisarea)
        {
            if(Nowchoose + article_mysynwid <= ARTICLE_MYSYNMAXNUM){Nowchoose += article_mysynwid;}
            else {choosearea = myarticlearea;Nowchoose = 1;}
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose + article_synwid <= ARTICLE_SYNMAXNUM){Nowchoose += article_synwid;}
            else {choosearea = myarticlearea;Nowchoose = 1;}
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose + 1 <= 3){Nowchoose += 1;}
            else {choosearea = myarticlearea;Nowchoose = 1;}
        }
    break;
    case 4:
        if(choosearea == myarticlearea)
        {
            if(Nowchoose - 1 > 0 ){Nowchoose -= 1;}
        }
        else if(choosearea == mysynthesisarea)
        {
            if(Nowchoose - 1 > 0 ){Nowchoose -= 1;}
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose - 1 > 0 ){Nowchoose -= 1;}
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose - 1 > 0 ){Nowchoose -= 1;}
        }
    break;
    }
}

void Interface::Writeunit(int x, int y, string sym) //画物品栏的单元
{
    int length = sym.length();
    for(int n=0;n<length;n++)
    Outimg[y+1][x+2+n]=sym[n];
}

void Interface::Writeblank(int x, int y)            //画各种栏的背景
{
    for(int n=y;n<=y+2;n++)
        for(int m=x;m<=x+5;m++)Outimg[n][m]='.';
}

void Interface::Writecircle(int x, int y)           //画选框
{
    string circleimg = "XX";
    for(int n=x;n<=x+4;n+=2)
    {
        Outimg[y][n]=circleimg[0];
        Outimg[y][n+1]=circleimg[1];
        Outimg[y+2][n]=circleimg[0];
        Outimg[y+2][n+1]=circleimg[1];
    }
        Outimg[y+1][x]=circleimg[0];
        Outimg[y+1][x+1]=circleimg[1];
        Outimg[y+1][x+4]=circleimg[0];
        Outimg[y+1][x+5]=circleimg[1];
}

void Interface::Writeinventory()                    //添加各种栏和窗口
{
    //////画快捷栏
    for(int n=1;n<=10;n++)
    { Writeblank(nowx+3+(n-1)*6,nowy+SCREENLEN-3);
      Outimg[nowy+SCREENLEN-3][nowx+3+(n-1)*6]=(char)(n%10+48); }
    Writecircle(nowx+3+(Nowuse-1)*6,nowy+SCREENLEN-3);
    for(int n=1;n<=10;n++)
    {
        if(Inventory[n])
        {
            Writeunit(nowx+3+(n-1)*6,nowy+SCREENLEN-3,Myarticles[Inventory[n]].sym);
            Outimg[nowy+SCREENLEN-1][nowx+3+(n-1)*6+4]=(char)(Myarticles[Inventory[n]].num/10+48);
            Outimg[nowy+SCREENLEN-1][nowx+3+(n-1)*6+5]=(char)(Myarticles[Inventory[n]].num%10+48);
        }
        else Writeunit(nowx+3+(n-1)*6,nowy+SCREENLEN-3,"__");
    }
    if(!testmode)               ///////画状态栏
    {
        Writebars(nowx+3,nowy+SCREENLEN-4);
    }
    if(gamemode == use)
    {
        //////////////////////画主物品栏
        int writex0=(SCREENWID-article_mywid*6)/2+nowx+16;
        int writey0=SCREENLEN-article_mylen*3+nowy-6;
        Writewords(writex0,writey0-1,"物品栏");
        for(int n=1;n<=article_mylen;n++)
            for(int m=1;m<=article_mywid;m++)
            {
                Writeblank(writex0+(m-1)*6,writey0+(n-1)*3);
            }
        if(choosearea==myarticlearea){Writecircle(writex0+(Nowchoose-1)%article_mywid*6,writey0+(Nowchoose-1)/article_mywid*3);}
        for(int n=1;n<=article_mylen;n++)
            for(int m=1;m<=article_mywid;m++)
            {
                if(Myarticles[(n-1)*article_mywid+m].type!=emptyarticle)
                {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,Myarticles[(n-1)*article_mywid+m].sym);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+4]=(char)(Myarticles[(n-1)*article_mywid+m].num/10+48);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+5]=(char)(Myarticles[(n-1)*article_mywid+m].num%10+48);}
                else {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,"__");}
            }
        //////////////////////

        if(opensyn)
        {
        //////////////////////画合成台
        writex0=(SCREENWID-article_mywid*6)/2+nowx-14;
        writey0=SCREENLEN-article_mylen*3+nowy-6;
        Writewords(writex0,writey0-1,"合成台");
        for(int n=1;n<=article_synlen;n++)
            for(int m=1;m<=article_synwid;m++)
            {
                Writeblank(writex0+(m-1)*6,writey0+(n-1)*3);
            }
        Writeblank(writex0+6,writey0+10);
        if(choosearea==synthesisarea)
        {
            if(Nowchoose == 10)
            {Writecircle(writex0+6,writey0+10);}
            else
            {Writecircle(writex0+(Nowchoose-1)%article_synwid*6,writey0+(Nowchoose-1)/article_synwid*3);}
        }
        for(int n=1;n<=article_synlen;n++)
            for(int m=1;m<=article_synwid;m++)
            {
                if(Synarticles[(n-1)*article_synwid+m].type!=emptyarticle)
                {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,Synarticles[(n-1)*article_synwid+m].sym);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+4]=(char)(Synarticles[(n-1)*article_synwid+m].num/10+48);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+5]=(char)(Synarticles[(n-1)*article_synwid+m].num%10+48);}
                else {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,"__");}
            }
        if(Synarticles[10].type!=emptyarticle)
        {
            Writeunit(writex0+6,writey0+10,Synarticles[10].sym);
            Outimg[writex0+6+2][writey0+10+4]=(char)(Synarticles[10].num/10+48);
            Outimg[writex0+6+2][writey0+10+5]=(char)(Synarticles[10].num%10+48);
        }
        else
            Writeunit(writex0+6,writey0+10,"__");
        //////////////////////
        }
        else if(openstove)
        {
        //////////////////////画熔炉
        writex0=(SCREENWID-article_mywid*6)/2+nowx-8;
        writey0=SCREENLEN-article_mylen*3+nowy-4;
        Writewords(writex0,writey0-1,"熔炉");
        Writeblank(writex0,writey0);
        Writeblank(writex0,writey0+4);
        Writeblank(writex0+8,writey0+3);
        if(Stovelist[Nowstove].firetime/Stovelist[Nowstove].firetime_max>=0.77)
        Writewords(writex0,writey0+3,"■■■");
        else if(Stovelist[Nowstove].firetime/Stovelist[Nowstove].firetime_max>=0.33)
        Writewords(writex0,writey0+3,"■■__");
        else if(Stovelist[Nowstove].firetime/Stovelist[Nowstove].firetime_max>=0.1)
        Writewords(writex0,writey0+3,"■____");
        else Writewords(writex0,writey0+3,"______");
        if(Stovelist[Nowstove].firing)Writewords(writex0+10,writey0+2,"▼");
        else Writewords(writex0+10,writey0+2,"▽");
        if(choosearea==stovearea)
        {
            if(Nowchoose == 1)
            Writecircle(writex0,writey0);
            else if(Nowchoose == 2)
                Writecircle(writex0,writey0+4);
            else if(Nowchoose == 3)
                Writecircle(writex0+8,writey0+3);
        }
        if(Stovelist[Nowstove].oldarticle.type!=emptyarticle)
            {Writeunit(writex0,writey0,Stovelist[Nowstove].oldarticle.sym);
             Outimg[writey0+2][writex0+4]=(char)(Stovelist[Nowstove].oldarticle.num/10+48);
             Outimg[writey0+2][writex0+5]=(char)(Stovelist[Nowstove].oldarticle.num%10+48);}
        else {Writeunit(writex0,writey0,"__");}
        if(Stovelist[Nowstove].firearticle.type!=emptyarticle)
            {Writeunit(writex0,writey0+4,Stovelist[Nowstove].firearticle.sym);
             Outimg[writey0+6][writex0+4]=(char)(Stovelist[Nowstove].firearticle.num/10+48);
             Outimg[writey0+6][writex0+5]=(char)(Stovelist[Nowstove].firearticle.num%10+48);}
        else {Writeunit(writex0,writey0+4,"__");}
        if(Stovelist[Nowstove].newarticle.type!=emptyarticle)
            {Writeunit(writex0+8,writey0+3,Stovelist[Nowstove].newarticle.sym);
             Outimg[writey0+5][writex0+12]=(char)(Stovelist[Nowstove].newarticle.num/10+48);
             Outimg[writey0+5][writex0+13]=(char)(Stovelist[Nowstove].newarticle.num%10+48);}
        else {Writeunit(writex0+8,writey0+3,"__");}

        //////////////////////
        }
        else
        {
        //////////////////////画合成栏
        writex0=(SCREENWID-article_mywid*6)/2+nowx-8;
        writey0=SCREENLEN-article_mylen*3+nowy-6;
        Writewords(writex0,writey0-1,"合成栏");
        for(int n=1;n<=article_mysynlen;n++)
            for(int m=1;m<=article_mysynwid;m++)
            {
                Writeblank(writex0+(m-1)*6,writey0+(n-1)*3);
            }
        Writeblank(writex0+4,writey0+7);
        if(choosearea==mysynthesisarea)
        {
            if(Nowchoose == 5)
            {Writecircle(writex0+4,writey0+7);}
            else
            {Writecircle(writex0+(Nowchoose-1)%article_mysynwid*6,writey0+(Nowchoose-1)/article_mysynwid*3);}
        }
        for(int n=1;n<=article_mysynlen;n++)
            for(int m=1;m<=article_mysynwid;m++)
            {
                if(Mysynarticles[(n-1)*article_mysynwid+m].type!=emptyarticle)
                {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,Mysynarticles[(n-1)*article_mysynwid+m].sym);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+4]=(char)(Mysynarticles[(n-1)*article_mysynwid+m].num/10+48);
                 Outimg[writey0+(n-1)*3+2][writex0+(m-1)*6+5]=(char)(Mysynarticles[(n-1)*article_mysynwid+m].num%10+48);}
                else {Writeunit(writex0+(m-1)*6,writey0+(n-1)*3,"__");}
            }
        if(Mysynarticles[5].type!=emptyarticle)
        {
            Writeunit(writex0+4,writey0+7,Mysynarticles[5].sym);
            Outimg[writex0+4+2][writey0+7+4]=(char)(Mysynarticles[5].num/10+48);
            Outimg[writex0+4+2][writey0+7+5]=(char)(Mysynarticles[5].num%10+48);
        }
        else
            Writeunit(writex0+4,writey0+7,"__");
        //////////////////////
        }
    }
}

void Interface::Writebars(int x, int y)             //画生命条和饥饿度条
{
    for(int n=0;n<60;n++)Outimg[y][x+n]=' ';
    Outimg[y][x]='H';
    Outimg[y][x+1]='P';
    for(int n=1;n<=10;n++){Outimg[y][x+n*2]=-95;Outimg[y][x+n*2+1]=-16;}
    for(int n=1;n<=(int)(hp);n++){Outimg[y][x+n*2]=-95;Outimg[y][x+n*2+1]=-15;}
    Outimg[y][x+36]='F';
    Outimg[y][x+37]='o';
    Outimg[y][x+38]='o';
    Outimg[y][x+39]='d';
    for(int n=1;n<=10;n++){Outimg[y][x+38+n*2]=-95;Outimg[y][x+38+n*2+1]=-16;}
    for(int n=1;n<=(int)(hunger+1);n++){Outimg[y][x+38+n*2]=-95;Outimg[y][x+38+n*2+1]=-15;}
}

void Interface::Writeoutmainlist()                  //画主菜单
{   //wid:64,len:23
    Gotoxy(6,2);
    printf("##   ## ## ##  ## ##### ##### ##### ##### ##### ####");
    Gotoxy(6,3);
    printf("### ### ## ### ## ##    ##    ## ## ## ## ##     ##");
    Gotoxy(6,4);
    printf("####### ## ## ### ##### ##    ####  ##### #####  ##");
    Gotoxy(6,5);
    printf("## # ## ## ##  ## ##    ##    ## ## ## ## ##     ##");
    Gotoxy(6,6);
    printf("##   ## ## ##  ## ##### ##### ## ## ## ## ##     ##");
    Gotoxy(10,7);
    printf("MineCraft 2D Beta0.1");
    Gotoxy(10,8);
    printf("By ZhangYao. 2013.12~2014.1 =w=");
    Gotoxy(28,10);
    printf("开始游戏");
    Gotoxy(28,12);
    printf("退出游戏");
    Gotoxy(3,16);
    printf("*操作：asdw 控制角色方向 e键 打开/关闭物品栏 空格键 跳跃 ");
    Gotoxy(3,17);
    printf("       数字键 切换手上物品 j 鼠标左键 k 鼠标右键 ");
    Gotoxy(3,18);
    printf("       q在装备栏界面丢弃物品,在主界面退出游戏 m开/关创造模式 ");
    Gotoxy(3,19);
    printf("*游戏玩法参考游戏《MineCraft》(《我的世界》)。此为简陋demo");
    for(int n=1;n<=2;n++)
    {
        if(n==mainlistchoose)
        {
            Gotoxy(26,10+(n-1)*2);
            printf("☆");
        }
        else
        {
            Gotoxy(26,10+(n-1)*2);
            printf("  ");
        }
    }
}

void Interface::Writeoutblank()                     //清屏
{
    for(int n=0;n<SCREENLEN+2;n++)
        for(int m=0;m<SCREENWID+16;m++)
    {
        Gotoxy(m,n);
        printf(" ");
    }
}

void Interface::Writewords(int x, int y, string words)  //输出一行文字
{
    for(int n=0;n<(int)words.length();n++)
        Outimg[y][x+n]=words[n];
    if(words.length()%2==1)
        Outimg[y][x+words.length()]=' ';
}

void Interface::Gotoxy(int x, int y)                //设置光标到坐标
{
    short x0=x;
    short y0=y;
    COORD p = {x0,y0};
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(h, p);
}

///////////////////////////////////////

Drawmap::Drawmap()
{
    ;
}

void Drawmap::Redrawwholemap()
{
    int sealinenum = Map_MAXLEN - SCREENLEN/2 - 30;
    int treenum = 20;
    int mountnum =8;
    int holenum = 15;
    int passagenum = 11;

    for(int n=1;n<=mountnum;n++)
    {
        int y0 = sealinenum -1;
        int x0 = Randint(1,Map_MAXWID/2);
        int h = Randint(4,15);
        Drawmount(x0,y0,h);
    }

    for(int n=1;n<=treenum;n++)
    {
        int x0 = Randint(1,Map_MAXWID/2);
        int y0 = 0;
        for(int m=SCREENLEN/2;m<=Map_MAXLEN-SCREENLEN/2;m++)
        {
            y0 = m ;
            if(Blockinfo[y0+1][x0+1].type == earth&&
               Blockinfo[y0][x0+1].type == emptyblock&&
               Blockinfo[y0-1][x0+1].type == emptyblock&&
               Blockinfo[y0-2][x0+1].type == emptyblock&&
               Blockinfo[y0-3][x0+1].type == emptyblock)
                break;
        }
        if(y0!=Map_MAXLEN-SCREENLEN/2)
        {
            int tsize = Randint(1,100);
            if(tsize%2==0)Drawtree(x0,y0,1);
            else Drawtree(x0,y0,2);
        }
    }
    Drawminerals(sealinenum-10,Map_MAXLEN-SCREENLEN/2-6,50,4);
    Drawminerals(sealinenum-15,Map_MAXLEN-SCREENLEN/2-6,40,3);
    for(int n=1;n<=holenum;n++ )
    {
        int x0 = Randint(1,Map_MAXWID/2);
        int y0 ;
        int tsize = Randint(3,7);
        while(1)
        {
            y0 = Randint(sealinenum-5,Map_MAXLEN-SCREENLEN/2-tsize);
            if(Blockinfo[y0][x0].type == stone )break;
        }
        Drawhole(x0,y0,tsize);
    }
    for(int n=1;n<=passagenum;n++)
    {
        int x0 = Randint(1,Map_MAXWID/2);
        int y0 ;
        while(1)
        {
            y0 = Randint(sealinenum-5,Map_MAXLEN-SCREENLEN/2);
            if(Blockinfo[y0][x0].type == stone )break;
        }
        int length = Randint(5,20);
        int dir = Randint(1,100);
        if(dir<33)Drawpassage(x0,y0,length,1);
        else if(dir>66)Drawpassage(x0,y0,length,2);
        else Drawpassage(x0,y0,length,3);
    }
}

void Drawmap::Drawintomap(int x, int y, int num)
{
    if(x>1&&y>1&&x<Map_MAXWID/2-1&&y<Map_MAXLEN-1)
        Blockinfo[y][x] =  Blocklist[num];
}

void Drawmap::Drawtree(int x, int y, int treekind)
{
    switch(treekind)
    {
    case 1:
        Drawintomap(x+1,y-3,8);
        Drawintomap(x,y-2,8);
        Drawintomap(x+1,y-2,8);
        Drawintomap(x+2,y-2,8);
        Drawintomap(x,y-1,8);
        Drawintomap(x+2,y-1,8);
        Drawintomap(x+1,y-1,5);
        Drawintomap(x+1,y,5);
        break;
    case 2:
        Drawintomap(x+1,y-4,8);
        Drawintomap(x,y-3,8);
        Drawintomap(x+2,y-3,8);
        Drawintomap(x,y-1,8);
        Drawintomap(x+2,y-1,8);
        Drawintomap(x+1,y-3,5);
        Drawintomap(x+1,y-2,5);
        Drawintomap(x+1,y-1,5);
        Drawintomap(x+1,y,5);
        break;
    default:
        break;
    }
}

void Drawmap::Drawmount(int x, int y, int height)
{
    double wid = height*4;
    double h;
    int inth;
    for(int n=0;n<=wid;n++)
    {
        h = height-(n-wid/2)*(n-wid/2)/(4*height);
        inth = (int)h;
        for(int m=0;m<inth;m++)
        {
            if(inth>2&&m<=inth-3)Drawintomap(x+n,y-m,2);
            else Drawintomap(x+n,y-m,1);
        }
    }
}

void Drawmap::Drawhole(int x, int y, int holesize)
{
    for(int n=y-holesize/2;n<y+holesize/2;n++)
        for(int m=x-holesize/2;m<x+holesize/2;m++)
        {
           if((y-n)*(y-n)+(x-m)*(x-m) <= (holesize/2)*(holesize/2))
            Drawintomap(m,n,0);
        }
}

void Drawmap::Drawminerals(int uph, int downh, int pronum, int typenum)
{
    int num = Map_MAXWID*pronum/800;
    for(int n=0;n<num;n++)
    {
        int x0 = Randint(1,Map_MAXWID/2);
        int y0 = Randint(uph,downh+3);
        if(Blockinfo[y0][x0].type==stone)Drawintomap(x0,y0,typenum);
        if(Blockinfo[y0+1][x0].type==stone)Drawintomap(x0,y0+1,typenum);
        int ss = Randint(1,100);
        if(ss<50){if(Blockinfo[y0-1][x0-1].type==stone)Drawintomap(x0+1,y0-1,typenum);
                  if(Blockinfo[y0][x0-1].type==stone)Drawintomap(x0+1,y0,typenum);}
        else {if(Blockinfo[y0+1][x0+1].type==stone)Drawintomap(x0+1,y0+1,typenum);
              if(Blockinfo[y0+2][x0+1].type==stone)Drawintomap(x0+1,y0+2,typenum);}
    }
}

void Drawmap::Drawpassage(int x,int y,int length,int dir)
{
    switch(dir)
    {
    case 1:
        for(int n=0;n<length;n++)Drawintomap(x+n,y-n,0);
        for(int n=0;n<length;n++)Drawintomap(x+n,y-n+1,0);
    case 2:
        for(int n=0;n<length;n++)Drawintomap(x+n,y+n,0);
        for(int n=0;n<length;n++)Drawintomap(x+n,y+n+1,0);
    case 3:
        for(int n=0;n<length;n++)Drawintomap(x+n,y,0);
        for(int n=0;n<length;n++)Drawintomap(x+n,y+1,0);
    default:
        break;
    }
}

///////////////////////////////////////

void Gamebegin();
void Gameend();
void Gameexit();
void PutInterfaceinfo();
void Loop();
void Move();
void Hit(int faceto);
void Articleuse(int faceto);
void Breakblock(struct Block *theblock, bool itisme);
void Addtoinv(int num);
void Leftbutton();
void Rightbutton();
void Createarticles(Choosemovearea area);
void Deletearticles(Choosemovearea area);
void Deletearticle(struct Article* article,int num);
void Createarticle(struct Article* createarticle,struct Article* article,int num);
struct Article* Findcreatearticle(Articletype type);
void Addarticles(struct Article* a,struct Article* b);
void Exchangearticles(struct Article* a,struct Article* b);
void Fire();

string int2str(int a);

DWORD WINAPI  Getthekey(LPVOID lpParamter);
HANDLE hThread1 = CreateThread(NULL,0,Getthekey,NULL,0,NULL);

Message messa(SCREENWID+1,SCREENLEN-4);             //系统消息
Message messb(SCREENWID+1,SCREENLEN-3);             //坐标
Message messc(SCREENWID+1,SCREENLEN-2);             //目标物品/方块 状态
Message messd(SCREENWID+1,SCREENLEN-1);             //武器及耐久
Interface theInterface;
Drawmap drawmap;

int main()                                          //入口
{
    memset(Outimg,0,sizeof(Outimg));
    mainlistchoose = 1;
    GameRun = 1;
    Loop();
}


void Gamebegin()                                    //游戏初始化
{
    messa.Clean();
    messb.Clean();
    messc.Clean();
    messd.Clean();
    memset(Outimg,0,sizeof(Outimg));
    theInterface.Writeoutblank();
    GameRun = 1;
    Nowstove = 0;
    gamemode = common;
    speedx=0;
    speedy=0;
    jump=0;
    face=0;
    hp=10.0;
    hunger=9.9;
    memset(Inventory,0,sizeof(Inventory));
    for(int n=1;n<=ARTICLE_MYMAXNUM;n++)Deletearticle(&Myarticles[n],Myarticles[n].num);
    for(int n=1;n<=ARTICLE_MYSYNMAXNUM;n++)Deletearticle(&Mysynarticles[n],Mysynarticles[n].num);
    for(int n=1;n<=ARTICLE_SYNMAXNUM;n++)Deletearticle(&Synarticles[n],Synarticles[n].num);
    for(int n=1;n<=ARTICLE_MAXSTOVENUM;n++)Stovelist[n].havestove=0;
    PutInterfaceinfo();
    Sleep(50);
    mainx=Map_MAXWID/2+1;
    mainy=SCREENLEN/2+30;
    for(int n=SCREENLEN/2+1;n<=Map_MAXLEN-SCREENLEN/2-4;n++)
    {
        if(Blockinfo[n+1][mainx/2].type!=emptyblock)
            {mainy = n;break;}
    }

    nowx=mainx-SCREENWID/2;
    nowy=mainy-SCREENLEN/2;
    choosearea = myarticlearea;
    Nowchoose = 1;
    Nowuse = 1;
    Nowchoosearticle = &Emptyarticle;
    Targetarticle = &Emptyarticle;
    nowcatch = 0;
    choosearea = myarticlearea;
    /*
    Createarticle(Findcreatearticle(awoodenplank),&Myarticles[1],20);
    Createarticle(Findcreatearticle(awoodenstick),&Myarticles[2],20);
    Createarticle(Findcreatearticle(aworkbench),&Myarticles[3],3);
    Createarticle(Findcreatearticle(astove),&Myarticles[4],3);
    Createarticle(Findcreatearticle(awood),&Myarticles[5],3);
    Createarticle(Findcreatearticle(aironstone),&Myarticles[6],10);
    */
    messa.Addmessage("游戏开始");
}

void Loop()                                         //主控制函数。不断循环，每轮循环时重新计算各种参数和重绘可视界面。
{
    while(GameRun)
    {
        Sleep(80);
        if(gamemode == gameover)
        {
            theInterface.Writeoutblank();continue;
        }
        else if(gamemode == mainlist)
        {
            theInterface.Writeoutmainlist();continue;
        }
        else
        {
        Fire();
        Move();
        string tempstr="坐标("+int2str(mainx)+","+int2str(Map_MAXLEN-mainy)+")";
        messb.Addmessage(tempstr);
        if(gamemode == common)
        {
            if(Nowhitblock->type!=emptyblock)
            {
                tempstr=Nowhitblock->name+",hp:"+int2str(Nowhitblock->hp);
                messc.Addmessage(tempstr);
            }
            else messc.Addmessage(" ");
        }
        else if(gamemode == use)
        {
            if(Nowchoosearticle->type!=emptyarticle)
            {
                tempstr=Nowchoosearticle->name+",num:"+int2str(Nowchoosearticle->num);
                messc.Addmessage(tempstr);
            }
            else messc.Addmessage(" ");
        }
        if(Myarticles[Inventory[Nowuse]].type_num%10==2)
        {
            tempstr=Myarticles[Inventory[Nowuse]].name+" 耐久:"+int2str(Myarticles[Inventory[Nowuse]].type_num/10%1000);
            messd.Addmessage(tempstr);
        }
        else {messd.Addmessage("未持工具");}
        //messa.Addmessage(int2str(Nowstove));
        messa.Writeall();
        messb.Writeall();
        messc.Writeall();
        messd.Writeall();
        theInterface.Writeoutimg();
        if(!testmode)
        {
            if(hp<10&&hunger>8)hp+=0.05;
            if(hunger<0.0001)hp-=0.05;
            if(hp<0.0001)Gameend();
        }
        }
    }
}

void Gameend()                                      //游戏结束，回到开始界面
{
    messa.Addmessage("GameOver!");
    gamemode = gameover;
    messa.Clean();
    messb.Clean();
    messc.Clean();
    Sleep(300);
    theInterface.Writeoutblank();
    gamemode = mainlist;
}

void Gameexit()                                     //游戏退出
{
    exit(0);
}

void PutInterfaceinfo()                             //绘制地图
{
    for(int n=1;n<=Map_MAXLEN;n++)
        for(int m=1;m<=Map_MAXWID/2;m++)
            Blockinfo[n][m]=Blocklist[0];
    for(int n=Map_MAXLEN - SCREENLEN/2 - 32;n<=Map_MAXLEN - SCREENLEN/2 - 30 ;n++)
        for(int m=1;m<=Map_MAXWID/2;m++)
            Blockinfo[n][m]=Blocklist[1];
    for(int n=Map_MAXLEN - SCREENLEN/2 - 30;n<=Map_MAXLEN - SCREENLEN/2 - 3 ;n++)
        for(int m=1;m<=Map_MAXWID/2;m++)
            Blockinfo[n][m]=Blocklist[2];
    drawmap.Redrawwholemap();
    for(int n=Map_MAXLEN-SCREENLEN/2-2;n<=Map_MAXLEN-SCREENLEN/2;n++)
        for(int m=1;m<=Map_MAXWID/2;m++)
            Blockinfo[n][m]=Blocklist[10];
}

void Move()                                         //主角的移动
{
    if(mainx<SCREENWID/2){mainx+=2;messa.Addmessage("到达地图边界");}
    else if(mainx>Map_MAXWID-SCREENWID/2){mainx-=2;messa.Addmessage("到达地图边界");}
    if(mainy<SCREENLEN/2){mainy+=1;messa.Addmessage("到达地图边界");}
    else if(mainy>Map_MAXLEN-SCREENLEN/2){mainy-=1;messa.Addmessage("到达地图边界");}
    if(speedx>0)
    {
        hunger-=0.01;
        speedx=0;
        if(Blockinfo[mainy][mainx/2+1].type==emptyblock)
        {mainx+=2;}
    }
    else if(speedx<0)
    {
        hunger-=0.01;
        speedx=0;
        if(Blockinfo[mainy][mainx/2-1].type==emptyblock)
        {mainx-=2;}
    }
    if(testmode)
    {
        if(Blockinfo[mainy+1][mainx/2].type==emptyblock&&speedy<0)
        {
            mainy+=1;
            speedy=0;
            jump=0;
        }
        else if(Blockinfo[mainy-1][mainx/2].type==emptyblock&&speedy>0)
        {
            mainy-=1;
            speedy=0;
            jump=0;
        }
        else {speedy=0;jump=0;}
    }
    else if(speedy>0)
    {
        hunger-=0.005;
        if(Blockinfo[mainy-1][mainx/2].type==emptyblock)
        {
            speedy-=1;
            mainy-=1;
        }
        else
        {
            speedy=-1;
        }
        Nowhitblock=&Blocklist[0];
    }
    else
    {
        if(Blockinfo[mainy+1][mainx/2].type==emptyblock&&!testmode)
        {
            mainy+=1;
            speedy-=1;
            Nowhitblock=&Blocklist[0];
        }
        else
        {
            if(-speedy>3)
            hp-=(double)(((-speedy)*0.2)*((-speedy)*0.12));
            jump=0;
            speedy=0;
        }
    }
}

void Fire()
{
    Articletype thetype;
    for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
    {
        if(Stovelist[n].havestove==true)
        {
            switch(Stovelist[n].oldarticle.type)
            {
            case aironstone:thetype = airon;break;
            case awood:thetype = acoal;break;
            default:thetype = emptyarticle;break;
            }
               //string tempstr;
               // tempstr = Stovelist[n].oldarticle.sym+","+Stovelist[n].firingarticle.sym+int2str(Randint(1,100));
               // messa.Addmessage(tempstr);
            if(Stovelist[n].firetime>=0)
            {
                Stovelist[n].firing=true;
                Stovelist[n].firetime-=1;
                Stovelist[n].article_firetime+=1;
            }
                else {Stovelist[n].firing=false;}
                if(Stovelist[n].oldarticle.type==emptyarticle)
                    {Stovelist[n].firing=false;
                     Deletearticle(&Stovelist[n].firingarticle,1);
                     Stovelist[n].article_firetime=0;}
                if(Stovelist[n].oldarticle.type!=emptyarticle&&             //准备下一个物品
                   Stovelist[n].firingarticle.type==emptyarticle&&
                   thetype!=emptyarticle&&
                   (Stovelist[n].newarticle.type==thetype||Stovelist[n].newarticle.type == emptyarticle))
                {
                    Stovelist[n].firing=true;
                    Createarticle(Findcreatearticle(thetype),&Stovelist[n].firingarticle,1);
                    //Deletearticle(&Stovelist[n].oldarticle,1);
                    Stovelist[n].article_firetime=0;
                }
                if(Stovelist[n].article_firetime>125&&Stovelist[n].firingarticle.type!=emptyarticle)     //烧好现有
                {
                    Stovelist[n].article_firetime=0;
                    Stovelist[n].firing=false;
                    Createarticle(&Stovelist[n].firingarticle,&Stovelist[n].newarticle,1);
                    Deletearticle(&Stovelist[n].oldarticle,1);
                }
                if(Stovelist[n].firetime<=0 &&
                   Stovelist[n].firearticle.type!=emptyarticle&&
                   Stovelist[n].firingarticle.type!=emptyarticle)        //无柴
                {
                    switch(Stovelist[n].firearticle.type)
                    {
                    case acoal:Stovelist[n].firetime=12.5*80;Stovelist[n].firetime_max=12.5*80;Deletearticle(&Stovelist[n].firearticle,1); break;
                    case awood:Stovelist[n].firetime=12.5*15;Stovelist[n].firetime_max=12.5*15;Deletearticle(&Stovelist[n].firearticle,1);break;
                    case awoodenplank:Stovelist[n].firetime=12.5*15;Stovelist[n].firetime_max=12.5*15;Deletearticle(&Stovelist[n].firearticle,1);break;
                    case awoodenstick:Stovelist[n].firetime=12.5*5;Stovelist[n].firetime_max=12.5*5;Deletearticle(&Stovelist[n].firearticle,1);break;
                    default:break;
                    }
                }
                /*
                if(Stovelist[n].firetime<=0&&
                   Stovelist[n].firearticle.type==emptyarticle&&
                   Stovelist[n].firingarticle.type==emptyarticle)
                {
                    Stovelist[n].firing=false;
                    Stovelist[n].article_firetime=0;
                }
                */
          }
    }
}

void Articleuse(int faceto)                         //地图上使用右键（K）的事件
{
    bool canuse = false;
    switch(face)                                    //判断眼前有否能操作的方块，有则直接进入方块界面 而不做其余动作
    {
    case 1:if(Blockinfo[mainy-1][mainx/2].type == workbench||
              (Blockinfo[mainy-1][mainx/2].type == emptyblock&&
               Blockinfo[mainy-2][mainx/2].type == workbench))
              {
                  opensyn = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            else if(Blockinfo[mainy-1][mainx/2].type == stove)
              {
                  if(Blockinfo[mainy-1][mainx/2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy-1][mainx/2].type_num /10%100;

                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy-1][mainx/2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            else if(Blockinfo[mainy-1][mainx/2].type == emptyblock&&Blockinfo[mainy-2][mainx/2].type == stove)
            {
                if(Blockinfo[mainy-2][mainx/2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy-2][mainx/2].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy-2][mainx/2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
            }
            break;
    case 2:if((Blockinfo[mainy][mainx/2+1].type == workbench)||(Blockinfo[mainy][mainx/2+1].type == emptyblock&&
               Blockinfo[mainy][mainx/2+2].type == workbench))
              {
                  opensyn = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            else if(Blockinfo[mainy][mainx/2+1].type == stove)
            {
                if(Blockinfo[mainy][mainx/2+1].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy][mainx/2+1].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy][mainx/2+1].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
            }
            else if(Blockinfo[mainy][mainx/2+1].type == emptyblock&&Blockinfo[mainy][mainx/2+2].type == stove)
              {
                  if(Blockinfo[mainy][mainx/2+2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy][mainx/2+2].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy][mainx/2+2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            break;
    case 3:if(Blockinfo[mainy+1][mainx/2].type == workbench||
              (Blockinfo[mainy+1][mainx/2].type == emptyblock&&
               Blockinfo[mainy+2][mainx/2].type == workbench))
              {
                  opensyn = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            else if(Blockinfo[mainy+1][mainx/2].type == stove)
            {
                if(Blockinfo[mainy+1][mainx/2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy+1][mainx/2].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy+1][mainx/2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
            }
            else if(Blockinfo[mainy+1][mainx/2].type == emptyblock&&Blockinfo[mainy+2][mainx/2].type == stove)
              {
                  if(Blockinfo[mainy+2][mainx/2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy+2][mainx/2].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy+2][mainx/2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            break;
    case 4:if(Blockinfo[mainy][mainx/2-1].type == workbench||
              (Blockinfo[mainy][mainx/2-1].type == emptyblock&&
               Blockinfo[mainy][mainx/2-2].type == workbench))
              {
                  opensyn = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
           else if(Blockinfo[mainy][mainx/2-1].type == stove)
            {
                if(Blockinfo[mainy][mainx/2-1].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy][mainx/2-1].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy][mainx/2-1].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
            }
            else if(Blockinfo[mainy][mainx/2-1].type == emptyblock&&Blockinfo[mainy][mainx/2-2].type == stove)
              {
                  if(Blockinfo[mainy][mainx/2-2].type_num /10%100 != 0)
                  {
                      Nowstove = Blockinfo[mainy][mainx/2-2].type_num /10%100;
                  }
                  else
                  {
                      for(int n=1;n<ARTICLE_MAXSTOVENUM;n++)
                      {
                          if(Stovelist[n].havestove==false)
                          {
                              Stovelist[n].havestove=true;
                              Nowstove = n;
                              Blockinfo[mainy][mainx/2-2].type_num += 10*n;
                              break;
                          }
                      }
                  }
                  openstove = true;
                  openmyinv = true;
                  choosearea = myarticlearea;
                  Nowchoose = 1;
                  Nowhitblock->hp=Nowhitblock->maxhp;
                  Nowhitblock=&Blocklist[0];
                  gamemode=use;
                  goto tend;
              }
            break;
    default:break;
    }
    if(Myarticles[Inventory[Nowuse]].type_num%10 != 1) //手中所持是否可以放置
    {
        if(Myarticles[Inventory[Nowuse]].type_num%10 == 3) //手中所持是否可以使用
        {
            canuse = true;
            switch(Myarticles[Inventory[Nowuse]].type)
            {
            case aapple:if(hunger<6.9)hunger+=4;else hunger=10.99;break;
            default:break;
            }
        }
    }
    else                                            //放置方块的情况
    {
    Blocktype tblock ;
    string tname;
    string tsym;
    int ttypenum;
    int thp;
    int tmaxhp;
    switch(Myarticles[Inventory[Nowuse]].type)
    {
    case emptyarticle:goto tend;break;
    case aearth:tblock = earth;break;
    case astone:tblock = stone;break;
    case awood:tblock = wood;break;
    case awoodenplank:tblock = woodenplank;break;
    case aworkbench:tblock = workbench;break;
    case aironstone:tblock = ironstone;break;
    case astove:tblock = stove;break;
    case aleaves:tblock = leaves;break;
    default:break;
    }
    for(int n=1;n<BLOCKLIST_MAXNUM;n++)
    {
        if(Blocklist[n].type==tblock)
        {
            tname = Blocklist[n].name;
            tsym = Blocklist[n].sym;
            thp = Blocklist[n].hp;
            tmaxhp = Blocklist[n].maxhp;
            ttypenum = Blocklist[n].type_num;
        }
    }
    switch(faceto)                          //根据朝向判断面前能否放置方块
    {
    case 1:
        if(Blockinfo[mainy-1][mainx/2].type!=emptyblock&&
           Blockinfo[mainy-1][mainx/2].type!=water)
            break;
        else if(Blockinfo[mainy-2][mainx/2].type!=emptyblock&&
                Blockinfo[mainy-2][mainx/2].type!=water)
        {
            canuse = true;
            Blockinfo[mainy-1][mainx/2].name = tname;
            Blockinfo[mainy-1][mainx/2].sym = tsym;
            Blockinfo[mainy-1][mainx/2].hp = thp;
            Blockinfo[mainy-1][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy-1][mainx/2].type = tblock;
            Blockinfo[mainy-1][mainx/2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy-3][mainx/2].type!=emptyblock&&
                 Blockinfo[mainy-3][mainx/2].type!=water)||
                (Blockinfo[mainy-2][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy-2][mainx/2-1].type!=water)||
                (Blockinfo[mainy-2][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy-2][mainx/2+1].type!=water)  )
        {
            canuse = true;
            Blockinfo[mainy-2][mainx/2].name = tname;
            Blockinfo[mainy-2][mainx/2].sym = tsym;
            Blockinfo[mainy-2][mainx/2].hp = thp;
            Blockinfo[mainy-2][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy-2][mainx/2].type = tblock;
            Blockinfo[mainy-2][mainx/2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy-1][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2+1].type!=water)||
                (Blockinfo[mainy-1][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2-1].type!=water))
        {
            canuse = true;
            Blockinfo[mainy-1][mainx/2].name = tname;
            Blockinfo[mainy-1][mainx/2].sym = tsym;
            Blockinfo[mainy-1][mainx/2].hp = thp;
            Blockinfo[mainy-1][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy-1][mainx/2].type = tblock;
            Blockinfo[mainy-1][mainx/2].type_num = ttypenum;
            break;
        }
        break;
    case 2:
        if(Blockinfo[mainy][mainx/2+1].type!=emptyblock&&
           Blockinfo[mainy][mainx/2+1].type!=water)
            break;
        else if(Blockinfo[mainy][mainx/2+2].type!=emptyblock&&
                Blockinfo[mainy][mainx/2+2].type!=water)
        {
            canuse = true;
            Blockinfo[mainy][mainx/2+1].name = tname;
            Blockinfo[mainy][mainx/2+1].sym = tsym;
            Blockinfo[mainy][mainx/2+1].hp = thp;
            Blockinfo[mainy][mainx/2+1].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2+1].type = tblock;
            Blockinfo[mainy][mainx/2+1].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy][mainx/2+3].type!=emptyblock&&
                 Blockinfo[mainy][mainx/2+3].type!=water)||
                (Blockinfo[mainy-1][mainx/2+2].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2+2].type!=water)||
                (Blockinfo[mainy+1][mainx/2+2].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2+2].type!=water)  )
        {
            canuse = true;
            Blockinfo[mainy][mainx/2+2].name = tname;
            Blockinfo[mainy][mainx/2+2].sym = tsym;
            Blockinfo[mainy][mainx/2+2].hp = thp;
            Blockinfo[mainy][mainx/2+2].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2+2].type = tblock;
            Blockinfo[mainy][mainx/2+2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy-1][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2+1].type!=water)||
                (Blockinfo[mainy+1][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2+1].type!=water))
        {
            canuse = true;
            Blockinfo[mainy][mainx/2+1].name = tname;
            Blockinfo[mainy][mainx/2+1].sym = tsym;
            Blockinfo[mainy][mainx/2+1].hp = thp;
            Blockinfo[mainy][mainx/2+1].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2+1].type = tblock;
            Blockinfo[mainy][mainx/2+1].type_num = ttypenum;
            break;
        }
        break;
    case 3:
        if(Blockinfo[mainy+1][mainx/2].type!=emptyblock&&
           Blockinfo[mainy+1][mainx/2].type!=water)
            break;
        else if(Blockinfo[mainy+2][mainx/2].type!=emptyblock&&
                Blockinfo[mainy+2][mainx/2].type!=water)
        {
            canuse = true;
            Blockinfo[mainy+1][mainx/2].name = tname;
            Blockinfo[mainy+1][mainx/2].sym = tsym;
            Blockinfo[mainy+1][mainx/2].hp = thp;
            Blockinfo[mainy+1][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy+1][mainx/2].type = tblock;
            Blockinfo[mainy+1][mainx/2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy+3][mainx/2].type!=emptyblock&&
                 Blockinfo[mainy+3][mainx/2].type!=water)||
                (Blockinfo[mainy+2][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy+2][mainx/2-1].type!=water)||
                (Blockinfo[mainy+2][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy+2][mainx/2+1].type!=water)  )
        {
            canuse = true;
            Blockinfo[mainy+2][mainx/2].name = tname;
            Blockinfo[mainy+2][mainx/2].sym = tsym;
            Blockinfo[mainy+2][mainx/2].hp = thp;
            Blockinfo[mainy+2][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy+2][mainx/2].type = tblock;
            Blockinfo[mainy+2][mainx/2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy+1][mainx/2+1].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2+1].type!=water)||
                (Blockinfo[mainy+1][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2-1].type!=water))
        {
            canuse = true;
            Blockinfo[mainy+1][mainx/2].name = tname;
            Blockinfo[mainy+1][mainx/2].sym = tsym;
            Blockinfo[mainy+1][mainx/2].hp = thp;
            Blockinfo[mainy+1][mainx/2].maxhp = tmaxhp;
            Blockinfo[mainy+1][mainx/2].type = tblock;
            Blockinfo[mainy+1][mainx/2].type_num = ttypenum;
            break;
        }
        break;
    case 4:
        if(Blockinfo[mainy][mainx/2-1].type!=emptyblock&&
           Blockinfo[mainy][mainx/2-1].type!=water)
            break;
        else if(Blockinfo[mainy][mainx/2-2].type!=emptyblock&&
                Blockinfo[mainy][mainx/2-2].type!=water)
        {
            canuse = true;
            Blockinfo[mainy][mainx/2-1].name = tname;
            Blockinfo[mainy][mainx/2-1].sym = tsym;
            Blockinfo[mainy][mainx/2-1].hp = thp;
            Blockinfo[mainy][mainx/2-1].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2-1].type = tblock;
            Blockinfo[mainy][mainx/2-1].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy][mainx/2-3].type!=emptyblock&&
                 Blockinfo[mainy][mainx/2-3].type!=water)||
                (Blockinfo[mainy-1][mainx/2-2].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2-2].type!=water)||
                (Blockinfo[mainy+1][mainx/2-2].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2-2].type!=water)  )
        {
            canuse = true;
            Blockinfo[mainy][mainx/2-2].name = tname;
            Blockinfo[mainy][mainx/2-2].sym = tsym;
            Blockinfo[mainy][mainx/2-2].hp = thp;
            Blockinfo[mainy][mainx/2-2].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2-2].type = tblock;
            Blockinfo[mainy][mainx/2-2].type_num = ttypenum;
            break;
        }
        else if((Blockinfo[mainy-1][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy-1][mainx/2-1].type!=water)||
                (Blockinfo[mainy+1][mainx/2-1].type!=emptyblock&&
                 Blockinfo[mainy+1][mainx/2-1].type!=water))
        {
            canuse = true;
            Blockinfo[mainy][mainx/2-1].name = tname;
            Blockinfo[mainy][mainx/2-1].sym = tsym;
            Blockinfo[mainy][mainx/2-1].hp = thp;
            Blockinfo[mainy][mainx/2-1].maxhp = tmaxhp;
            Blockinfo[mainy][mainx/2-1].type = tblock;
            Blockinfo[mainy][mainx/2-1].type_num = ttypenum;
            break;
        }
        break;
    default:
        break;
    }
    }
    if(canuse&&!testmode)                           //成功放置后，从手中移除物品
    {
        Myarticles[Inventory[Nowuse]].num-=1;
        if(Myarticles[Inventory[Nowuse]].num<=0)
        {
            Myarticles[Inventory[Nowuse]].name=Articlelist[0].name;
            Myarticles[Inventory[Nowuse]].sym=Articlelist[0].sym;
            Myarticles[Inventory[Nowuse]].num=0;
            Myarticles[Inventory[Nowuse]].type=Articlelist[0].type;
            Myarticles[Inventory[Nowuse]].type_num=Articlelist[0].type_num;
            Inventory[Nowuse]=0;
        }
    }
    tend:;
}

void Hit(int faceto)                                //地图上使用左键（J）的事件
{
    switch(faceto)
    {
    case 1:
        if(Blockinfo[mainy-1][mainx/2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy-1][mainx/2];}
        else if(Blockinfo[mainy-2][mainx/2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy-2][mainx/2];}
        break;
    case 2:
        if(Blockinfo[mainy][mainx/2+1].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy][mainx/2+1];}
        else if(Blockinfo[mainy][mainx/2+2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy][mainx/2+2];}
        break;
    case 3:
        if(Blockinfo[mainy+1][mainx/2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy+1][mainx/2];}
        else if(Blockinfo[mainy+2][mainx/2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy+2][mainx/2];}
        break;
    case 4:
        if(Blockinfo[mainy][mainx/2-1].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy][mainx/2-1];}
        else if(Blockinfo[mainy][mainx/2-2].type!=emptyblock)
        {Nowhitblock=&Blockinfo[mainy][mainx/2-2];}
        break;
    default:
        break;
    }
    if(testmode)Breakblock(Nowhitblock,true);
    else
    {
        switch(Nowhitblock->type)
        {
        case stone:
        case ironstone:
        case coal:
        case stove:
            if(Myarticles[Inventory[Nowuse]].type==awoodenpick)Nowhitblock->hp-=2;
            else if(Myarticles[Inventory[Nowuse]].type==astonepick)Nowhitblock->hp-=3;
            else if(Myarticles[Inventory[Nowuse]].type==aironpick)Nowhitblock->hp-=4;
            else Nowhitblock->hp-=1;
            break;
        case earth:
            if(Myarticles[Inventory[Nowuse]].type==awoodenspade)Nowhitblock->hp-=2;
            else if(Myarticles[Inventory[Nowuse]].type==astonespade)Nowhitblock->hp-=3;
            else if(Myarticles[Inventory[Nowuse]].type==aironspade)Nowhitblock->hp-=4;
            else Nowhitblock->hp-=1;
            break;
        case wood:
        case woodenplank:
        case workbench:
            if(Myarticles[Inventory[Nowuse]].type==awoodenspade)Nowhitblock->hp-=2;
            else if(Myarticles[Inventory[Nowuse]].type==astonespade)Nowhitblock->hp-=3;
            else if(Myarticles[Inventory[Nowuse]].type==aironspade)Nowhitblock->hp-=4;
            else Nowhitblock->hp-=1;
            break;
        case leaves:
            if(Myarticles[Inventory[Nowuse]].type==ascissors)Nowhitblock->hp-=2;
            else Nowhitblock->hp-=1;
            break;
        default:Nowhitblock->hp-=1;break;
        }
        if(Nowhitblock->hp <= 0&&Nowhitblock->hp > -9&&Nowhitblock->type!=emptyblock)
        {Breakblock(Nowhitblock,true);}
    }
}

/*方位：
        1
      4   2
        3
*/

void Breakblock(struct Block *theblock, bool itisme) //打破方块的事件
{
    if(itisme)
    {
        Blocktype tblock = theblock->type;
        Articletype tarticle;
        switch(tblock)
        {
        case earth:tarticle = aearth;break;
        case stone:if(Myarticles[Inventory[Nowuse]].type==awoodenpick||
                     Myarticles[Inventory[Nowuse]].type==astonepick||
                     Myarticles[Inventory[Nowuse]].type==aironpick)
                     tarticle = astone;
                  else tarticle = emptyarticle;
                  break;
        case stove:tarticle = astove;break;
        case wood:tarticle = awood;break;
        case woodenplank:tarticle = awoodenplank;break;
        case workbench:tarticle = aworkbench;break;
        case leaves:if(Myarticles[Inventory[Nowuse]].type==ascissors)tarticle = aleaves;
                    else if(Randint(1,100)<15)tarticle = aapple;
                    else tarticle = emptyarticle;
                    break;
        case coal:if(Myarticles[Inventory[Nowuse]].type==awoodenpick||
                     Myarticles[Inventory[Nowuse]].type==astonepick||
                     Myarticles[Inventory[Nowuse]].type==aironpick)
                     tarticle = acoal;
                  else tarticle = emptyarticle;
                  break;
        case ironstone:if(Myarticles[Inventory[Nowuse]].type==astonepick||
                          Myarticles[Inventory[Nowuse]].type==aironpick)
                          tarticle = aironstone;
                       else tarticle = emptyarticle;
                       break;
        default:break;
        }
        for(int n=1;n<=ARTICLE_MYMAXNUM;n++)
        {
            if(tarticle!=emptyarticle)
            {
                if(Myarticles[n].type==tarticle)
                {
                    if(Myarticles[n].num>=article_maxgroupnum)continue;
                    else
                    {
                        Myarticles[n].num++;
                        string tempstr;
                        tempstr = "获得:"+Myarticles[n].name;
                        messa.Addmessage(tempstr);
                        break;
                    }
                }
                else if(Myarticles[n].type==emptyarticle)
                {
                    for(int m=1;m<ARTICLELIST_MAXNUM;m++)
                    {
                        if(Articlelist[m].type==tarticle)
                        {
                            Myarticles[n].name = Articlelist[m].name;
                            Myarticles[n].num = 1;
                            Myarticles[n].sym = Articlelist[m].sym;
                            Myarticles[n].type = Articlelist[m].type;
                            Myarticles[n].type_num = Articlelist[m].type_num;
                            string tempstr;
                            tempstr = "获得:"+Myarticles[n].name;
                            messa.Addmessage(tempstr);
                            break;
                        }
                    }
                    break;
            }
            }
        }
        if(Myarticles[Inventory[Nowuse]].type_num%10==2)        //是工具，则会消耗一个耐久。
        {
            Myarticles[Inventory[Nowuse]].type_num-=10;
            if(Myarticles[Inventory[Nowuse]].type_num/10==0)
               Deletearticle(&Myarticles[Inventory[Nowuse]],1);
        }
        if(tblock==stove)
        {
            int num = theblock->type_num/10&100;
            Stovelist[num].havestove=false;
            Deletearticle(&Stovelist[num].firearticle,Stovelist[num].firearticle.num);
            Deletearticle(&Stovelist[num].oldarticle,Stovelist[num].oldarticle.num);
            Deletearticle(&Stovelist[num].newarticle,Stovelist[num].oldarticle.num);
            Deletearticle(&Stovelist[num].firingarticle,Stovelist[num].firingarticle.num);
        }
    }
    theblock->hp=Blocklist[0].hp;
    theblock->name=Blocklist[0].name;
    theblock->sym=Blocklist[0].sym;
    theblock->type=Blocklist[0].type;
    theblock->type_num=Blocklist[0].type_num;
}

void Addtoinv(int num)                              //快捷栏的物品切换
{
    if(Myarticles[Nowchoose].type!=emptyarticle)
    {
        for(int n=1;n<10;n++)
        {
            if(Inventory[n]==Nowchoose)
            {
                Inventory[n]=0;
            }
        }
        Inventory[num]=Nowchoose;
    }
}

void Deletearticle(struct Article *article,int tnum)
{
    if(article->num > tnum)
        article->num -= tnum;
    else
    {
        article->num = 0;
        article->name = "Empty";
        article->sym = "__";
        article->type = emptyarticle;
        article->type_num = 0;
    }
}

void Createarticles(Choosemovearea area)
{
    switch(area)
    {
    case mysynthesisarea:                   //在个人合成栏里的合成项目
        //合成木板
        if((Mysynarticles[1].type == awood       &&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == emptyarticle)
            ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == awood             &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == emptyarticle)
           ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == awood       &&     Mysynarticles[4].type == emptyarticle)
           ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == awood))
        {Createarticle(Findcreatearticle(awoodenplank),&Mysynarticles[5],4);break;}
        //合成合成台
        else if((Mysynarticles[1].type == awoodenplank&&    Mysynarticles[2].type == awoodenplank       &&
                 Mysynarticles[3].type == awoodenplank&&    Mysynarticles[4].type == awoodenplank))
        {Createarticle(Findcreatearticle(aworkbench),&Mysynarticles[5],1);break;}
        //合成木棍
        else if((Mysynarticles[1].type == awoodenplank&&    Mysynarticles[2].type == emptyarticle       &&
                 Mysynarticles[3].type == awoodenplank&&    Mysynarticles[4].type == emptyarticle)
                ||
                (Mysynarticles[1].type == emptyarticle&&    Mysynarticles[2].type == awoodenplank       &&
                 Mysynarticles[3].type == emptyarticle&&    Mysynarticles[4].type == awoodenplank))
        {Createarticle(Findcreatearticle(awoodenstick),&Mysynarticles[5],4);break;}
        //合成剪子
        else if((Mysynarticles[1].type == airon       &&    Mysynarticles[2].type == emptyarticle   &&
                 Mysynarticles[3].type == emptyarticle&&    Mysynarticles[4].type == airon)
                ||
                (Mysynarticles[1].type == emptyarticle&&    Mysynarticles[2].type == airon          &&
                 Mysynarticles[3].type == airon       &&    Mysynarticles[4].type == emptyarticle))
        {Createarticle(Findcreatearticle(ascissors),&Mysynarticles[5],4);break;}

        else {Createarticle(&Articlelist[0],&Mysynarticles[5],1);}
        break;
    case synthesisarea:                     //在大合成台
        //合成木板
         if((Synarticles[1].type == awood &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == awood && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == awood&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == awood && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awood && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == awood&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == awood && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awood && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == awood))
        {Createarticle(Findcreatearticle(awoodenplank),&Synarticles[10],4);}
        //合成木镐
    else if((Synarticles[1].type == awoodenplank &&  Synarticles[2].type == awoodenplank && Synarticles[3].type == awoodenplank&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(awoodenpick),&Synarticles[10],1);}
        //合成石镐
    else if((Synarticles[1].type == astone &&  Synarticles[2].type == astone && Synarticles[3].type == astone&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(astonepick),&Synarticles[10],1);}
        //合成铁镐
    else if((Synarticles[1].type == airon &&  Synarticles[2].type == airon && Synarticles[3].type == airon&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(aironpick),&Synarticles[10],1);}
        //合成木铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == awoodenplank && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(awoodenspade),&Synarticles[10],1);}
        //合成石铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == astone && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(astonespade),&Synarticles[10],1);}
        //合成铁铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == airon && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Createarticle(Findcreatearticle(aironspade),&Synarticles[10],1);}
        //合成炉子
    else if((Synarticles[1].type == astone &&  Synarticles[2].type == astone && Synarticles[3].type == astone&&
             Synarticles[4].type == astone && Synarticles[5].type == emptyarticle && Synarticles[6].type == astone&&
             Synarticles[7].type == astone && Synarticles[8].type == astone && Synarticles[9].type == astone))
        {Createarticle(Findcreatearticle(astove),&Synarticles[10],1);}

        else {Createarticle(&Articlelist[0],&Synarticles[10],1);}
        break;
    default:break;
    }
}

void Deletearticles(Choosemovearea area)
{
    switch(area)
    {
    case mysynthesisarea:                   //在个人合成栏里的合成项目
        //合成木板
        if((Mysynarticles[1].type == awood       &&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == emptyarticle)
            ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == awood             &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == emptyarticle)
           ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == awood       &&     Mysynarticles[4].type == emptyarticle)
           ||
            (Mysynarticles[1].type == emptyarticle&&     Mysynarticles[2].type == emptyarticle       &&
            Mysynarticles[3].type == emptyarticle&&     Mysynarticles[4].type == awood))
        {if(Mysynarticles[1].type==awood){Deletearticle(&Mysynarticles[1],1);}
            else if(Mysynarticles[2].type==awood){Deletearticle(&Mysynarticles[2],1);}
            else if(Mysynarticles[3].type==awood){Deletearticle(&Mysynarticles[3],1);}
            else if(Mysynarticles[4].type==awood){Deletearticle(&Mysynarticles[4],1);}}
        //合成合成台
        else if((Mysynarticles[1].type == awoodenplank&&    Mysynarticles[2].type == awoodenplank       &&
                 Mysynarticles[3].type == awoodenplank&&    Mysynarticles[4].type == awoodenplank))
        {Deletearticle(&Mysynarticles[1],1);
         Deletearticle(&Mysynarticles[2],1);
         Deletearticle(&Mysynarticles[3],1);
         Deletearticle(&Mysynarticles[4],1);}
        //合成木棍
        else if((Mysynarticles[1].type == awoodenplank&&    Mysynarticles[2].type == emptyarticle       &&
                 Mysynarticles[3].type == awoodenplank&&    Mysynarticles[4].type == emptyarticle)
                ||
                (Mysynarticles[1].type == emptyarticle&&    Mysynarticles[2].type == awoodenplank       &&
                 Mysynarticles[3].type == emptyarticle&&    Mysynarticles[4].type == awoodenplank))
        {if(Mysynarticles[1].type == awoodenplank){Deletearticle(&Mysynarticles[1],1);Deletearticle(&Mysynarticles[3],1);}
        else if(Mysynarticles[2].type == awoodenplank){Deletearticle(&Mysynarticles[2],1);Deletearticle(&Mysynarticles[4],1);}}
        //合成剪子
        else if((Mysynarticles[1].type == airon       &&    Mysynarticles[2].type == emptyarticle   &&
                 Mysynarticles[3].type == emptyarticle&&    Mysynarticles[4].type == airon)
                ||
                (Mysynarticles[1].type == emptyarticle&&    Mysynarticles[2].type == airon          &&
                 Mysynarticles[3].type == airon       &&    Mysynarticles[4].type == emptyarticle))
        {if(Mysynarticles[1].type == airon){Deletearticle(&Mysynarticles[1],1);Deletearticle(&Mysynarticles[4],1);}
        else if(Mysynarticles[2].type == airon){Deletearticle(&Mysynarticles[2],1);Deletearticle(&Mysynarticles[3],1);}}

        else {Createarticle(&Articlelist[0],&Mysynarticles[5],1);}
        break;
    case synthesisarea:                     //在大合成台
        //合成木板
         if((Synarticles[1].type == awood &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == awood && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == awood&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == awood && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awood && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == awood&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == awood && Synarticles[8].type == emptyarticle && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awood && Synarticles[9].type == emptyarticle)
            ||
            (Synarticles[1].type == emptyarticle &&  Synarticles[2].type == emptyarticle && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == emptyarticle && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == emptyarticle && Synarticles[9].type == awood))
        {for(int n=1;n<=9;n++)if(Synarticles[n].type==awood){Deletearticle(&Synarticles[n],1);break;}}
        //合成木镐
    else if((Synarticles[1].type == awoodenplank &&  Synarticles[2].type == awoodenplank && Synarticles[3].type == awoodenplank&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[1],1);
         Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[3],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成石镐
    else if((Synarticles[1].type == astone &&  Synarticles[2].type == astone && Synarticles[3].type == astone&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[1],1);
         Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[3],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成铁镐
    else if((Synarticles[1].type == airon &&  Synarticles[2].type == airon && Synarticles[3].type == airon&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[1],1);
         Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[3],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成木铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == awoodenplank && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成石铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == astone && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成铁铲
    else if((Synarticles[1].type == emptyarticle &&  Synarticles[2].type == airon && Synarticles[3].type == emptyarticle&&
             Synarticles[4].type == emptyarticle && Synarticles[5].type == awoodenstick && Synarticles[6].type == emptyarticle&&
             Synarticles[7].type == emptyarticle && Synarticles[8].type == awoodenstick && Synarticles[9].type == emptyarticle))
        {Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[5],1);
         Deletearticle(&Synarticles[8],1);}
        //合成炉子
    else if((Synarticles[1].type == astone &&  Synarticles[2].type == astone && Synarticles[3].type == astone&&
             Synarticles[4].type == astone && Synarticles[5].type == emptyarticle && Synarticles[6].type == astone&&
             Synarticles[7].type == astone && Synarticles[8].type == astone && Synarticles[9].type == astone))
        {Deletearticle(&Synarticles[1],1);
         Deletearticle(&Synarticles[2],1);
         Deletearticle(&Synarticles[3],1);
         Deletearticle(&Synarticles[4],1);
         Deletearticle(&Synarticles[6],1);
         Deletearticle(&Synarticles[7],1);
         Deletearticle(&Synarticles[8],1);
         Deletearticle(&Synarticles[9],1);}

        else {Createarticle(&Articlelist[0],&Synarticles[10],1);}
        break;
    default:break;
    }
}

void Createarticle(struct Article* createarticle,struct Article* article,int num)
{
    if(createarticle)
    {
        if(article->type!=createarticle->type)
        {article->name = createarticle->name;
        article->num = num;
        article->sym = createarticle->sym;
        article->type = createarticle->type;
        article->type_num = createarticle->type_num;}
        else
        {
            if(article->num+num<64)article->num+=num;
            else article->num=64;
        }
    }
}

struct Article* Findcreatearticle(Articletype type)
{
    for(int n=1;n<=ARTICLELIST_MAXNUM;n++)
    {
        if(Articlelist[n].type == type)
            return &Articlelist[n];
    }
    return NULL;
}

void Addarticles(struct Article* a,struct Article* b)    //加在前一个上边
{
    if(a->num+b->num>64)
    {
        b->num -= a->num+b->num-64;
        a->num = 64;
    }
    else
    {
        a->num += b->num;
        Deletearticle(b,b->num);
    }
}

void Exchangearticles(struct Article* a,struct Article* b)
{
    string tname;
    string tsym;
    int tnum;
    int ttype_num;
    Articletype ttype;
    tname = a->name;
    tnum = a->num;
    tsym = a->sym;
    ttype = a->type;
    ttype_num = a->type_num;
    a->name = b->name;
    a->num = b->num;
    a->sym = b->sym;
    a->type = b->type;
    a->type_num = b->type_num;
    b->name = tname;
    b->num = tnum;
    b->sym = tsym;
    b->type = ttype;
    b->type_num = ttype_num;
}

void Leftbutton()                                   //在物品栏点左键（J）的事件
{
    switch(choosearea)
    {
    case myarticlearea:Targetarticle = &Myarticles[Nowchoose];break;
    case mysynthesisarea:Targetarticle = &Mysynarticles[Nowchoose];break;
    case synthesisarea:Targetarticle = &Synarticles[Nowchoose];break;
    case stovearea:if(Nowchoose == 1)Targetarticle = &Stovelist[Nowstove].oldarticle;
                   else if(Nowchoose == 2)Targetarticle = &Stovelist[Nowstove].firearticle;
                   else if(Nowchoose == 3)Targetarticle = &Stovelist[Nowstove].newarticle;
                   break;
    default:Targetarticle = &Emptyarticle;break;
    }
    if(choosearea == myarticlearea)
    {
    if(Nowchoosearticle->type!=emptyarticle)                //手中有物
    {
        if(Nowchoosearticle->type!=Targetarticle->type)   //此区可以操作
        {
            Exchangearticles(Nowchoosearticle,Targetarticle);
        }
        else Addarticles(Targetarticle,Nowchoosearticle);
    }
    else if(Targetarticle->type!=emptyarticle)              //手中无物，此栏有物
    {
        Exchangearticles(Nowchoosearticle,Targetarticle);
    }
    }
    else  //此栏为合成栏
    {
        if(choosearea == mysynthesisarea)
        {
            if(Nowchoose==5)  //选择的是提取物栏
            {
                if(Targetarticle->type!=emptyarticle){
            if(Nowchoosearticle->type == emptyarticle)
            {
                Exchangearticles(Nowchoosearticle,Targetarticle);
                Deletearticle(Targetarticle,Targetarticle->num);
                Deletearticles(mysynthesisarea);
                Createarticles(mysynthesisarea);
            }
            else if(Nowchoosearticle->type == Targetarticle->type)
            {
                Addarticles(Nowchoosearticle,Targetarticle);
                Deletearticle(Targetarticle,Targetarticle->num);
                Deletearticles(mysynthesisarea);
                Createarticles(mysynthesisarea);
            }
            }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type==emptyarticle)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(mysynthesisarea);
                    }
                    else if(Targetarticle->type == Nowchoosearticle->type)
                    {
                        Addarticles(Targetarticle,Nowchoosearticle);
                        Createarticles(mysynthesisarea);
                    }
                }
                else
                {
                    Exchangearticles(Nowchoosearticle,Targetarticle);
                }
            }
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose==10)  //选择的是提取物栏
            {
                if(Targetarticle->type!=emptyarticle){
            if(Nowchoosearticle->type == emptyarticle)
            {
                Exchangearticles(Nowchoosearticle,Targetarticle);
                Deletearticle(Targetarticle,Targetarticle->num);
                Deletearticles(synthesisarea);
                Createarticles(synthesisarea);
            }
            else if(Nowchoosearticle->type == Targetarticle->type)
            {
                Addarticles(Nowchoosearticle,Targetarticle);
                Deletearticle(Targetarticle,Targetarticle->num);
                Deletearticles(synthesisarea);
                Createarticles(synthesisarea);
            }
            }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type==emptyarticle)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(synthesisarea);
                    }
                    else if(Targetarticle->type == Nowchoosearticle->type)
                    {
                        Addarticles(Targetarticle,Nowchoosearticle);
                        Createarticles(synthesisarea);
                    }
                }
                else
                {
                    Exchangearticles(Nowchoosearticle,Targetarticle);
                }
            }
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose==3)  //选择的是提取物栏
            {
                if(Targetarticle->type!=emptyarticle)
                {
                    //string tempstr;
                    //tempstr = int2str(Nowchoose)+","+Targetarticle->name+",";
                    //messa.Addmessage(tempstr);
                    if(Nowchoosearticle->type == emptyarticle)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Deletearticle(Targetarticle,Targetarticle->num);
                    }
                    else if(Nowchoosearticle->type == Targetarticle->type)
                    {
                        Addarticles(Nowchoosearticle,Targetarticle);
                        Deletearticle(Targetarticle,Targetarticle->num);
                    }

                }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type==emptyarticle)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                    }
                    else if(Targetarticle->type == Nowchoosearticle->type)
                    {
                        Addarticles(Targetarticle,Nowchoosearticle);
                    }
                }
                else
                {
                    Exchangearticles(Nowchoosearticle,Targetarticle);
                }
            }
        }
    }
}

void Rightbutton()                                  //在物品栏点右键（K）的事件
{
    switch(choosearea)
    {
    case myarticlearea:Targetarticle = &Myarticles[Nowchoose];break;
    case mysynthesisarea:Targetarticle = &Mysynarticles[Nowchoose];break;
    case synthesisarea:Targetarticle = &Synarticles[Nowchoose];break;
    case stovearea:if(Nowchoose == 1)Targetarticle = &Stovelist[Nowstove].oldarticle;
                   else if(Nowchoose == 2)Targetarticle = &Stovelist[Nowstove].firearticle;
                   else if(Nowchoose == 3)Targetarticle = &Stovelist[Nowstove].newarticle;
                   break;
    default:Targetarticle = &Emptyarticle;break;
    }
    if(choosearea == myarticlearea)
    {
    if(Nowchoosearticle->type!=emptyarticle)
    {
        if(Targetarticle->type == emptyarticle)
        {
            Targetarticle->name = Nowchoosearticle->name;
            Targetarticle->sym = Nowchoosearticle->sym;
            Targetarticle->type = Nowchoosearticle->type;
            Targetarticle->type_num = Nowchoosearticle->type_num;
            Targetarticle->num = 1;
            Deletearticle(Nowchoosearticle,1);
        }
        else if(Nowchoosearticle->type!=Targetarticle->type)
        {
            Exchangearticles(Nowchoosearticle,Targetarticle);
        }
        else if(Targetarticle->num < 64)
        {
            Targetarticle->num += 1;
            Deletearticle(Nowchoosearticle,1);
        }
    }
    else if(Targetarticle->type!=emptyarticle)
    {
        if(Targetarticle->num == 1)
        {
            Exchangearticles(Nowchoosearticle,Targetarticle);
        }
        else
        {
            Nowchoosearticle->name = Targetarticle->name;
            Nowchoosearticle->sym = Targetarticle->sym;
            Nowchoosearticle->num = Targetarticle->num/2;
            Nowchoosearticle->type = Targetarticle->type;
            Nowchoosearticle->type_num = Targetarticle->type_num;
            Targetarticle->num -= Nowchoosearticle->num;
        }
    }
    }
    else  //此栏为合成栏
    {
        if(choosearea == mysynthesisarea)
        {
            if(Nowchoose==5)  //选择的是提取物栏
            {if(Targetarticle->type!=emptyarticle){
            for(int s=0;s<64;s++)
            {
                if(Nowchoosearticle->type == emptyarticle)
                {
                    Exchangearticles(Nowchoosearticle,Targetarticle);
                    Deletearticles(mysynthesisarea);
                    Createarticles(mysynthesisarea);
                }
                else if(Nowchoosearticle->type == Targetarticle->type)
                {
                    Addarticles(Nowchoosearticle,Targetarticle);
                    Deletearticles(mysynthesisarea);
                    Createarticles(mysynthesisarea);
                }
            }
            }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type == emptyarticle)
                    {
                        Targetarticle->name = Nowchoosearticle->name;
                        Targetarticle->sym = Nowchoosearticle->sym;
                        Targetarticle->type = Nowchoosearticle->type;
                        Targetarticle->type_num = Nowchoosearticle->type_num;
                        Targetarticle->num = 1;
                        Deletearticle(Nowchoosearticle,1);
                        Createarticles(mysynthesisarea);
                    }
                    else if(Nowchoosearticle->type!=Targetarticle->type)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(mysynthesisarea);
                    }
                    else if(Targetarticle->num < 64)
                    {
                        Targetarticle->num += 1;
                        Deletearticle(Nowchoosearticle,1);
                        Createarticles(mysynthesisarea);
                    }
                }
                else if(Targetarticle->type!=emptyarticle)
                {
                    if(Targetarticle->num == 1)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(mysynthesisarea);
                    }
                    else
                    {
                        Nowchoosearticle->name = Targetarticle->name;
                        Nowchoosearticle->sym = Targetarticle->sym;
                        Nowchoosearticle->num = Targetarticle->num/2;
                        Nowchoosearticle->type = Targetarticle->type;
                        Nowchoosearticle->type_num = Targetarticle->type_num;
                        Targetarticle->num -= Nowchoosearticle->num;
                        Createarticles(mysynthesisarea);
                    }
                }
            }
        }
        else if(choosearea == synthesisarea)
        {
            if(Nowchoose==10)  //选择的是提取物栏
            {if(Targetarticle->type!=emptyarticle){
            for(int s=0;s<64;s++)
            {
            if(Nowchoosearticle->type == emptyarticle)
            {
                Exchangearticles(Nowchoosearticle,Targetarticle);
                Deletearticles(synthesisarea);
                Createarticles(synthesisarea);
            }
            else if(Nowchoosearticle->type == Targetarticle->type)
            {
                Addarticles(Nowchoosearticle,Targetarticle);
                Deletearticles(synthesisarea);
                Createarticles(synthesisarea);
            }
            }
            }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type == emptyarticle)
                    {
                        Targetarticle->name = Nowchoosearticle->name;
                        Targetarticle->sym = Nowchoosearticle->sym;
                        Targetarticle->type = Nowchoosearticle->type;
                        Targetarticle->type_num = Nowchoosearticle->type_num;
                        Targetarticle->num = 1;
                        Deletearticle(Nowchoosearticle,1);
                        Createarticles(synthesisarea);
                    }
                    else if(Nowchoosearticle->type!=Targetarticle->type)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(synthesisarea);
                    }
                    else if(Targetarticle->num < 64)
                    {
                        Targetarticle->num += 1;
                        Deletearticle(Nowchoosearticle,1);
                        Createarticles(synthesisarea);
                    }
                }
                else if(Targetarticle->type!=emptyarticle)
                {
                    if(Targetarticle->num == 1)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                        Createarticles(synthesisarea);
                    }
                    else
                    {
                        Nowchoosearticle->name = Targetarticle->name;
                        Nowchoosearticle->sym = Targetarticle->sym;
                        Nowchoosearticle->num = Targetarticle->num/2;
                        Nowchoosearticle->type = Targetarticle->type;
                        Nowchoosearticle->type_num = Targetarticle->type_num;
                        Targetarticle->num -= Nowchoosearticle->num;
                        Createarticles(synthesisarea);
                    }
                }
            }
        }
        else if(choosearea == stovearea)
        {
            if(Nowchoose==3)  //选择的是提取物栏
            {if(Targetarticle->type!=emptyarticle){
            for(int s=0;s<64;s++)
            {
            if(Nowchoosearticle->type == emptyarticle)
            {
                Exchangearticles(Nowchoosearticle,Targetarticle);
            }
            else if(Nowchoosearticle->type == Targetarticle->type)
            {
                Addarticles(Nowchoosearticle,Targetarticle);
            }
            }
            }
            }
            else
            {
                if(Nowchoosearticle->type!=emptyarticle)
                {
                    if(Targetarticle->type == emptyarticle)
                    {
                        Targetarticle->name = Nowchoosearticle->name;
                        Targetarticle->sym = Nowchoosearticle->sym;
                        Targetarticle->type = Nowchoosearticle->type;
                        Targetarticle->type_num = Nowchoosearticle->type_num;
                        Targetarticle->num = 1;
                    }
                    else if(Nowchoosearticle->type!=Targetarticle->type)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                    }
                    else if(Targetarticle->num < 64)
                    {
                        Targetarticle->num += 1;
                        Deletearticle(Nowchoosearticle,1);
                    }
                }
                else if(Targetarticle->type!=emptyarticle)
                {
                    if(Targetarticle->num == 1)
                    {
                        Exchangearticles(Nowchoosearticle,Targetarticle);
                    }
                    else
                    {
                        Nowchoosearticle->name = Targetarticle->name;
                        Nowchoosearticle->sym = Targetarticle->sym;
                        Nowchoosearticle->num = Targetarticle->num/2;
                        Nowchoosearticle->type = Targetarticle->type;
                        Nowchoosearticle->type_num = Targetarticle->type_num;
                        Targetarticle->num -= Nowchoosearticle->num;
                    }
                }
            }
        }
    }
}

void Pressbutton(char key)                          //对键盘按键进行反应
{
    switch(gamemode)
    {
    case common:
        switch(key)
        {
        case 'a':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            if(face==4)
            speedx=-1;
            face=4;
            break;
        case 'd':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            if(face==2)
            speedx=1;
            face=2;
            break;
        case 'w':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            face=1;
            if(testmode)speedy=1;
            break;
        case 's':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            face=3;
            if(testmode)speedy=-1;
            break;
        case ' ':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            if(!jump){speedy=2;jump=1;}
            break;
        case 'e':
            choosearea = myarticlearea;
            Nowchoose = 1;
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            gamemode=use;
            openmyinv=1;
            break;
        case 'q':
            Nowhitblock->hp=Nowhitblock->maxhp;
            Nowhitblock=&Blocklist[0];
            //GameRun=0;
            Gameend();
            break;
        case 'm':
            if(!testmode){testmode = true;messa.Addmessage("创造模式开启。");}
            else {testmode = false;hp = 10;hunger = 10.9;messa.Addmessage("创造模式关闭。");}
            break;
        case 'j':Hit(face);break;
        case 'k':Articleuse(face);break;
        case '1':Nowuse=1;break;
        case '2':Nowuse=2;break;
        case '3':Nowuse=3;break;
        case '4':Nowuse=4;break;
        case '5':Nowuse=5;break;
        case '6':Nowuse=6;break;
        case '7':Nowuse=7;break;
        case '8':Nowuse=8;break;
        case '9':Nowuse=9;break;
        case '0':Nowuse=10;break;
        default:break;
        }
        break;
    case use:
        switch(key)
        {
        case 'a':theInterface.Articlechoosemove(4);break;
        case 'd':theInterface.Articlechoosemove(2);break;
        case 'w':theInterface.Articlechoosemove(1);break;
        case 's':theInterface.Articlechoosemove(3);break;
        case 'e':
                 gamemode=common;
                 openmyinv=0;
                 openbox=0;
                 openstove=0;
                 opensyn=0;
                 break;
        case 'q':Deletearticle(Nowchoosearticle,Nowchoosearticle->num);break;
        case 'j':Leftbutton();break;
        case 'k':Rightbutton();break;
        case '1':Addtoinv(1);break;
        case '2':Addtoinv(2);break;
        case '3':Addtoinv(3);break;
        case '4':Addtoinv(4);break;
        case '5':Addtoinv(5);break;
        case '6':Addtoinv(6);break;
        case '7':Addtoinv(7);break;
        case '8':Addtoinv(8);break;
        case '9':Addtoinv(9);break;
        case '0':Addtoinv(10);break;
        default:break;
        }
        break;
    case mainlist:
        switch(key)
        {
        case 'w':if(mainlistchoose>1)mainlistchoose--;break;
        case 's':if(mainlistchoose<2)mainlistchoose++;break;
        case ' ':
            switch(mainlistchoose)
            {
            case 1:theInterface.Writeoutblank();Gamebegin();break;
            case 2:theInterface.Writeoutblank();Gameexit();break;
            default:break;
            }
            break;
        default:break;
        }
            break;
    default:
        break;
    }
}

DWORD WINAPI  Getthekey(LPVOID lpParamter)          //用于响应键盘操作的子线程
{
    Sleep(15);
    while(1)
    {
        if(GameRun)
        {
            char a;
            a=getch();
            Pressbutton(a);
        }
        else if(!GameRun)Sleep(500);
    }
}

string int2str(int a)                               //把int转化成字符串用于输出。
{
    string b="";
    while(a/10!=0)
    {
            b=(char)(a%10+48)+b;
            a/=10;
    }
    b=(char)(a+48)+b;
    return b;
}

int Randint(int a,int b)                            //提供随机整数
{
    if(a>=b)return a;
    else
    {

        if((int)time(0)==Nowtime){return rand()%(b-a)+a;}
        else {srand((int)time(0));Nowtime = time(0);return rand()%(b-a)+a;}
    }
}

