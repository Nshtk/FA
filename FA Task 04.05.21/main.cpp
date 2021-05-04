#include <iostream>
#include <cstring>
#include <list>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

class GameNotation;
class Iteration;

class Board
{
private:
    string field[8][8];
public:
    Board()
    {
        field[0][0]= "wR"; field[0][7]="wR";
        field[0][1]="wN"; field[0][6]= "wN";
        field[0][2]= "wB"; field[0][5]="wB";
        field[0][3]= "wQ"; field[0][4]= "wK";
        field[7][0]= "bR"; field[7][7]= "bR";
        field[7][1]= "bN"; field[7][6]= "bN";
        field[7][2]= "bB"; field[7][5]= "bB";
        field[7][3]= "bK"; field[7][4]= "bQ";
        int i;
        for(i=0; i<8; i++)
        {
            field[1][i]= "wP";
            field[6][i]= "bP";
        }
        for(i=2; i<6; i++)
            for(int j=0; j<8; j++)
                field[i][j]= "--";
    }
    void moveFigure(string figure, int row_orig, int col_orig, int row_dest, int col_dest)
    {
        field[row_dest][col_dest]=field[row_orig][col_orig];
        field[row_orig][col_orig]= "--";
    }
    string searchFigure(string figure, int row, int col)                 // Тут следовало сделать еще пару функций
    {                                                                    // Еще отсутсвует поиск коней (не хватило времени), поэтому ими невозможно ходить, зная только конечную точку прибытия
        int i;
        string org;
        for(i=0; i<8; i++)
            if(field[row][i]==figure)
            {moveFigure(figure, row, i, row, col); org.push_back(char(row+'a')); org.push_back(char(i+'0')); return org;}
        for(i=0; i<8; i++)
            if(field[i][col]==figure)
            { moveFigure(figure, i, col, row, col); org.push_back(char(i+'a')); org.push_back(char(col+'0')); return org;}
        int j;
        for(i=row; i<8; i++)
            for(j=col; j<8; j++)
                if(field[i][j]==figure)
                { moveFigure(figure, i, j, row, col); org.push_back(char(i+'a')); org.push_back(char(j+'0')); return org;}
        for(i=row; i>0; i--)
            for(j=col; j>0; j--)
                if(field[i][j]==figure)
                { moveFigure(figure, i, j, row, col); org.push_back(char(i+'a')); org.push_back(char(j+'0')); return org;}
        for(i=row; i<8; i++)
            for(j=col; j>0; j--)
                if(field[i][j]==figure)
                { moveFigure(figure, i, j, row, col); org.push_back(char(i+'a')); org.push_back(char(j+'0')); return org;}
        for(i=row; i>0; i--)
            for(j=col; j<8; j++)
                if(field[i][j]==figure)
                { moveFigure(figure, i, j, row, col); org.push_back(char(i+'a')); org.push_back(char(j+'0')); return org;}
    }
    void printBoard()
    {
        for(int i=0; i<8; i++)
        {
            for (int j = 0; j < 8; j++)
                cout << field[i][j];
            cout << '\n';
        }
    }

    friend istream& readFile(istream &in, GameNotation &game_not, Board &board);
    friend class Iteration;
    friend class PlayerMove;
};

class SevenTag
{
private:
    string event, site, date, round, white, black, result;
public:
    friend istream& operator>>(istream &in, SevenTag &seven_tag);
    friend ostream& operator<<(ostream &out, SevenTag &seven_tag);
};

istream& operator>>(istream &in, SevenTag &seven_tag)
{
    getline(in, seven_tag.event);
    getline(in, seven_tag.site);
    getline(in, seven_tag.date);
    getline(in, seven_tag.round);
    getline(in, seven_tag.white);
    getline(in, seven_tag.black);
    getline(in, seven_tag.result);
}

ostream& operator<<(ostream &out, SevenTag &seven_tag)
{
    out << seven_tag.event << '\n' << seven_tag.site << '\n' << seven_tag.date << '\n' << seven_tag.round << '\n'
    << seven_tag.white << '\n' << seven_tag.black << '\n' << seven_tag.result << '\n';

    return out;
}

class PlayerMove
{
private:
    string origin, destination;
public:
    void analyzeMove(char color, Board &board)                      //Тут, естественно, тоже следовало добавить функции
    {
        string figure;
        figure.push_back(color);
        int row, col;
        if(destination[0]>'`' && destination[0]<'i')
        {
            figure.push_back('P');
            row=destination[1]-'1';
            col=destination[0]-'a';
            origin=board.searchFigure(figure, row, col);
        }
        else if(destination[0]=='K')
        {
            figure.push_back(destination[0]);
            row=destination[2]-'1';
            col=destination[1]-'a';
            origin=board.searchFigure(figure, row, col);
        }
        else if(destination[0]=='Q')
        {
            figure.push_back(destination[0]);
            row=destination[2]-'1';
            col=destination[1]-'a';
            origin=board.searchFigure(figure, row, col);
        }
        else if(destination[0]=='R')
        {
            figure.push_back(destination[0]);
            row=destination[2]-'1';
            col=destination[1]-'a';
            origin=board.searchFigure(figure, row, col);
        }
        else if(destination[0]=='N')
        {
            figure.push_back(destination[0]);
            row=destination[2]-'1';
            col=destination[1]-'a';
            origin=board.searchFigure(figure, row, col);
        }
        else if(destination[0]=='B')
        {
            figure.push_back(destination[0]);
            row=destination[2]-'1';
            col=destination[1]-'a';
            origin=board.searchFigure(figure, row, col);
        }

    }

    friend void operator>>(istream &in, Iteration &iteration);
    friend ostream& operator<<(ostream &out, Iteration &iteration);
};

class Iteration
{
private:
    PlayerMove move_white, move_black;
public:

    friend istream& readFile(istream &in, GameNotation &game_not, Board &board);
    friend void operator>>(istream &in, Iteration &iteration);
    friend ostream& operator<<(ostream &out, Iteration &iteration);
};

void operator>>(istream &in, Iteration &iteration)
{
    while(in.get()!='.')
    {
        if (in.eof())
            return;
    }
    in.get();
    in >> iteration.move_white.destination >> iteration.move_black.destination;
}

ostream& operator<<(ostream &out, Iteration &iteration)
{
    out << iteration.move_white.destination << ' ' << iteration.move_black.destination << '\n';
    return out;
}

class GameNotation
{
private:
    SevenTag game_meta_inf;
    unsigned int amount_of_moves;
    vector<Iteration> iterations;
public:
    GameNotation()
    {

    }

    friend istream& readFile(istream &in, GameNotation &game_not, Board &board);
    friend stringstream *returnOut(GameNotation &game_not);
};

istream& readFile(istream &in, GameNotation &game_not, Board &board)                    // При чтении должны еще обрабатываться случаи ракировки/шаха/мата, поэтому файл не читается до конца
{
    in >> game_not.game_meta_inf;
    in.get();
    Iteration iteration;
    for(int i=0; !in.eof(); i++)
    {
        in >> iteration;
        iteration.move_white.analyzeMove('w', board);
        iteration.move_black.analyzeMove('b', board);
        game_not.iterations.push_back(iteration);
        board.printBoard();
        cout << '\n';
    }

}

stringstream *returnOut(GameNotation &game_not)                 // Функция возвратит поток (уже отформатированный (насколько хватило времени) по тз), который сконвертируется в строку и запишется в бинарник
{
    auto *out = new stringstream;
    *out << game_not.game_meta_inf << game_not.amount_of_moves << '\n';
    for(auto &move : game_not.iterations)
        *out << move;

    return out;
}

int main()
{
    ifstream ifp;
    ifp.open("input_file.pgn");
    ofstream ofp;
    ofp.open("output_file.bin", ios::out | ios::binary);
    if(!ofp.is_open() || !ifp.is_open())
    {
        cout << "Failed to open input file.";
        return 1;
    }

    Board board;
    GameNotation game;

    board.printBoard();
    cout << '\n';
    readFile(ifp, game, board);
    string str = returnOut(game)->str();
    ofp.write(str.c_str(), str.length());                                       // Запись в бинарник



    ifp.close();
    ofp.close();
    return 0;
}
