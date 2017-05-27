#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <conio.h>
#include <time.h>
#include <sstream>

const char UP = 72;
const char LEFT = 75;
const char RIGHT = 77;
const char DOWN = 80;
const char ESC = 27;
const char SPACE = 32;

using namespace std;


///////////////////////////////
//  CLASS POSITION
///////////////////////////////
class Position
{
    public:
        int x,y;
        Position();
        Position(int);
        Position(int,int);
};

Position::Position(){
    x=0;
    y=0;
}

Position::Position(int c):x(c),y(c){
}

Position::Position(int cx,int cy):x(cx),y(cy){
}

///////////////////////////////
//  CLASS SIZE
///////////////////////////////
class MySize
{
    public:
        int width,height;
        MySize();
        MySize(int);
        MySize(int,int);
};

MySize::MySize():width(0),height(0){
}

MySize::MySize(int s):width(s),height(s){
}

MySize::MySize(int sw,int sh):width(sw),height(sh){
}

///////////////////////////////
//  CLASS POSITION
///////////////////////////////
class Direction
{
    public:
        int dx,dy;
        Direction();
        Direction(int);
        Direction(int,int);
};

Direction::Direction(){
    dx=1;
    dx=1;
}

Direction::Direction(int d):dx(d),dy(d){
}

Direction::Direction(int cx,int cy):dx(cx),dy(cy){
}


///////////////////////////////
//  CLASS SCREEN
///////////////////////////////

class Screen
{
    protected:
        COORD coord={0,0};
        void GotoXY(int,int);
    public:
        void DrawPixel(int, int, char);
        void DrawPixel(Position, char);
        void DeletePixel(int, int);
        void DeletePixel(Position);
};

void Screen::GotoXY(int x,int y)
{
    coord.X=x;
    coord.Y=y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
}

void Screen::DrawPixel(int x, int y, char pixel)
{
    GotoXY(x,y);
    cout<<pixel;
}

void Screen::DrawPixel(Position p, char pixel)
{
    DrawPixel(p.x,p.y,pixel);
}


void Screen::DeletePixel(int x, int y)
{
    GotoXY(x,y);
    cout<<" ";
}

void Screen::DeletePixel(Position p)
{
    DeletePixel(p.x,p.y);
}

///////////////////////////////
//  CLASS MYWINDOW
///////////////////////////////

class MyWindow: public Screen
{
    protected:
        Position pos;
        MySize mwSize;
        string title;
        void DrawHorizontalLine(int,int,int, char);
        void DrawVerticalLine(int,int,int, char);
    public:
        MyWindow();
        MyWindow(string);
        MyWindow(MySize);
        MyWindow(MySize, string, Position p=Position(0,0));
        void ShowMessage(Position,string);
        void Draw();
        void SetTitle(string t){title=t;}

        inline Position GetPosition(){return pos;}
        inline MySize GetMySize(){return mwSize;}
        inline string GetTitle(){return title;}

        ~MyWindow();
};

MyWindow::MyWindow()
{
    pos = Position(0,0);
    mwSize=MySize(80,20);
    title=string("MyWindow");
}

MyWindow::MyWindow(string t)
{
    pos = Position(0,0);
    mwSize=MySize(80,20);
    title=t;
}

MyWindow::MyWindow(MySize s)
{
    pos = Position(0,0);
    mwSize = s;
    title=string("MyWindow");
}


MyWindow::MyWindow(MySize s, string t, Position p): mwSize(s), title(t), pos(p)
{

}

void MyWindow::ShowMessage(Position p,string message)
{
    GotoXY(p.x,p.y);
    cout<<message;
}

void MyWindow::DrawHorizontalLine(int xi,int xf,int y, char c)
{
    int i=xi;
    while(i<xf)
        DrawPixel(i++,y,c);
}

void MyWindow::DrawVerticalLine(int yi,int yf,int x, char c)
{
    int i=yi;
    while(i<yf)
        DrawPixel(x,i++,c);
}

void MyWindow::Draw()
{
    // the Title
    int tamTitle=title.length();
    int l=(mwSize.width-tamTitle)/2;
    char c='*';

    DrawHorizontalLine(pos.x, pos.x + l-1, pos.y, c);
    ShowMessage(Position(pos.x + l,pos.y), title);
    DrawHorizontalLine(pos.x + l + tamTitle+1, pos.x + mwSize.width, pos.y, c);
    DrawHorizontalLine(pos.x, pos.x + mwSize.width, pos.y + mwSize.height - 1, c);
    DrawVerticalLine(pos.y ,pos.y + mwSize.height - 1, pos.x, c);
    DrawVerticalLine(pos.y, pos.y + mwSize.height-1, pos.x + mwSize.width - 1, c);
}

MyWindow::~MyWindow()
{
}

///////////////////////////////
//  CLASS CHARACTER
///////////////////////////////
class Character:public Screen
{
    protected:
        Position pos;
        char shape;
    public:
        Character();
        Character(char);
        Character(Position,char);
        virtual void Draw();
        void SetShape(char);
        void Move(Position);
        void MoveAndDraw(Position);
        Position GetPos();
};

Character::Character(): pos(Position(0,0)),shape('è')
{

}

Character::Character(char c): pos(Position(0,0)),shape(c)
{

}

Character::Character(Position p,char c): pos(p),shape(c)
{

}

void Character::Draw()
{
    DrawPixel(pos,shape);
}

void Character::SetShape(char s)
{
    shape= s;
}

void Character::Move(Position p)
{
    pos=p;
}

void Character::MoveAndDraw(Position p)
{
    DeletePixel(pos);
    Move(p);
    Draw();
}

Position Character::GetPos()
{
    return pos;
}

///////////////////////////////
//  CLASS BUG
///////////////////////////////
class Bug:public Character
{
    protected:
        Direction dir;
    public:
        Bug();
        Bug(char);
        Bug(Position,char, Direction d=Direction(1,1));

        inline void SetDirection(Direction d){dir=d;}
        inline Direction GetDirection(){return dir;}
        void Draw();
};

Bug::Bug():Character(),dir(Direction(1,1))
{

}

Bug::Bug(char c):Character(c),dir(Direction(1,1))
{

}

Bug::Bug(Position p,char c, Direction d): Character(p,c),dir(d)
{

}

void Bug::Draw()
{
    DeletePixel(pos);
    Move(Position(pos.x+dir.dx,pos.y+dir.dy));
    Character::Draw();
}

///////////////////////////////
//  CLASS OBSTACLE
///////////////////////////////

class Obstacle:public Character
{
    protected:
        int state;

    public:
        Obstacle();
        Obstacle(char);
        Obstacle(Position,char);
        void Draw();
        inline void SetState(int s){state=s;}
        inline int GetState(){return state;}
        bool Impacted(Bug b);
};

Obstacle::Obstacle():Character(),state(1)
{

}

Obstacle::Obstacle(char c):Character(c),state(1)
{

}

Obstacle::Obstacle(Position p,char c): Character(p,c), state(1)
{

}

void Obstacle::Draw()
{
    shape = (char)(rand()*10 + 64);
    Character::Draw();
}

bool Obstacle::Impacted(Bug b)
{
    if(pos.x==b.GetPos().x && pos.y==b.GetPos().y ){
        state=0;
        return true;
    }
    return false;
}


///////////////////////////////
//  CLASS ONEGAME
///////////////////////////////

const int NOBSTACLES=5;

class OneGame
{
    protected:

        MyWindow *windowGame;
        Obstacle *myObstacles[NOBSTACLES];
        Bug *myBug;
        int score;
    public:

        OneGame();
        void Start();
        void Refresh();
        bool Crashed(Bug);
        ~OneGame();
};

OneGame::OneGame()
{
    windowGame = new MyWindow(MySize(40,15),string("Bichoooooooo!!!"),Position(5,5));
    Position t=windowGame->GetPosition();
    t.x+=5;
    t.y+=4;

    myBug = new Bug(t,'#',Direction(1,0));

    Position t2;
    MySize st=windowGame->GetMySize();

    t=windowGame->GetPosition();
    t.x++;
    t.y++;

    srand (time(NULL));

    for(int i=0;i<NOBSTACLES;i++)
    {
        t2.x=t.x+rand()%(st.width-2);
        t2.y=t.y+rand()%(st.height-2);
        myObstacles[i]=new Obstacle(t2,'$');
    }
}

void OneGame::Refresh()
{
    score=0;

    for(int i=0;i<NOBSTACLES;i++)
    {
        myObstacles[i]->Impacted(*myBug);

        if(myObstacles[i]->GetState()==1)
            myObstacles[i]->Draw();
        else
            score++;
    }

    myBug->Draw();
    stringstream s;
    s<<score;
    windowGame->ShowMessage(Position(0,1),string("Score: ")+s.str());

}

bool OneGame::Crashed(Bug b)
{
    Position pb=b.GetPos();
    Position pw=windowGame->GetPosition();
    MySize sw=windowGame->GetMySize();
    if(pb.x > pw.x && pb.x < pw.x +sw.width-1 && pb.y > pw.y && pb.y < pw.y +sw.height-1)
        return false;
    return true;
}

void OneGame::Start()
{
    char c;
    bool endGame=false;
    windowGame->Draw();

    do
    {
        if(kbhit())
        {
            c = getch();

            Direction d=myBug->GetDirection();

            switch (c)
            {
                case ESC:               // press ESC to exit
                    endGame = true;
                    break;
                case SPACE:               // press SPACE to clear screen
                    windowGame->ShowMessage(Position(0,21),string("escape key pressed"));
                    break;
                case UP:
                    d.dy=-1;d.dx=0;
                    break;
                case DOWN:
                    d.dy=1;d.dx=0;
                    break;
                case LEFT:
                    d.dx=-1;d.dy=0;
                    break;
                case RIGHT:
                    d.dx=1;d.dy=0;
                    break;
            }
            myBug->SetDirection(d);
        }

        if(Crashed(*myBug))
        {
            endGame=true;
            windowGame->ShowMessage(Position(0,21),string("Bichooooo crashed!...."));
            break;
        }
        else
        {
            Refresh();
        }

        if(score==NOBSTACLES)
        {
            endGame=true;
            windowGame->ShowMessage(Position(0,21),string("You Win!...."));
        }

        Sleep(200);

    }while(!endGame);

    windowGame->ShowMessage(Position(0,22),"The End...");
}

OneGame::~OneGame()
{
    delete windowGame;
    for(int i=0;i<5;i++)
        delete myObstacles[i];
}

/////////////////////////////
// THE MAIN FUNCTION
////////////////////////////

int main()
{
    OneGame MyGame;
    MyGame.Start();
    return 0;
}
