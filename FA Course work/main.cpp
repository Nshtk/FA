#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <ctime>
#include <cmath>
#include <forward_list>
//#include ".h"
//#include ".h"

using namespace std;

/*int foundDuplicate(unsigned int *arr, unsigned int i, unsigned int number)
{
    for(unsigned int j; j<i; j++)
        if(number==arr[j])
            return 0;

    arr[i-1]=number;
    return 1;
}*/

class Field
{
protected:
    unsigned int row_count;
    unsigned int col_count;
    unsigned int **field[2];                  // Игровое поле [0], и поле [1], где отмечаются зачеркнутые числа.
public:
    Field(unsigned int rows, unsigned int cols, unsigned int rng) : row_count(rows), col_count(cols)
    {
        unsigned int j;

        field[0]=new unsigned int*[row_count];
        field[1]=new unsigned int*[row_count];
        for(unsigned int i=0; i<row_count; i++)
        {
            field[0][i]=new unsigned int[col_count];
            field[1][i]=new unsigned int[col_count];
            for(j=0; j<col_count; j++)
            {
                field[0][i][j] = (rand() % rng) * (rand() % 2);
                field[1][i][j] = 0;
            }
        }
    }
    ~Field()
    {
        for(unsigned int i=0; i<row_count; i++)
        {
            delete[]field[0][i];
            delete[]field[1][i];
        }
        delete[]field[0];
        delete[]field[1];
    }

    friend ostream& operator<< (ostream &out, Field &fld);
};

class FieldRusLot : public Field
{
public:
    FieldRusLot() : Field(3, 9, 100) {}
};

ostream& operator<< (ostream &out, Field &fld)
{
    for(int i=0; i<fld.row_count; i++)
    {
        for(int j=0; j<fld.col_count; j++)
        {
            if(fld.field[1][i][j]==1)
                out << '-' << fld.field[0][i][j] << '-';                    // Выделение зачернутых чисел
            else
                out << fld.field[0][i][j];
            out << ' ';
        }
        out << '\n';
    }
    return out;
}

template<class T>
class Ticket
{
protected:
    //string lottery_name;
    unsigned int lot_number, ticket_number, sold_status;
    vector<T> fields;
public:
    Ticket(unsigned int lot_num, unsigned int ticket_num, unsigned int field_count, unsigned int status) : lot_number(lot_num), ticket_number(ticket_num),
    fields(field_count), sold_status(status) {}

    friend ostream& operator<< (ostream &out, Ticket<T> &tic)
    {
        out << "Lot series: " << tic.lot_number << '\n';
        out << "Ticket: " << tic.ticket_number << "\n";

        unsigned int field_count=tic.fields.size();
        for(int i=0; i<field_count; i++)
            out << "Field " << i << '\n' << tic.fields[i];

        tic.sold_status? out << "SOLD" : out << "UNSOLD"; out << '\n';
        return out;
    }
};

class TicketRusLot : public Ticket<FieldRusLot>
{
public:
    TicketRusLot(unsigned int lot_num, unsigned int ticket_num, unsigned int status) : Ticket(lot_num, ticket_num, 2, status) {}
};

template<class T>
class GeneratorTicket
{
public:
    virtual Ticket<T> *getTicket(unsigned int lot_num, unsigned int ticket_num, unsigned int status) const = 0;
};

class GeneratorTicketRusLot : public GeneratorTicket<FieldRusLot>
{
public:
    Ticket<FieldRusLot> *getTicket(unsigned int lot_num, unsigned int ticket_num, unsigned int status) const override
    {
        return new TicketRusLot(lot_num, ticket_num, status);
    }
};

template<class T>
class Lot
{
private:
    unsigned int lot_number;
    forward_list<Ticket<T>> lot_tickets;
    Lot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen_tic) : lot_number(num)
    {
        //lot_tickets.push_front(TicketRusLot(1, 2, 3));
        if(rand() % 2 || sale_chance==100)                                                              // Полная/неполная реализация тиража
            for(unsigned int i=0; i<num_of_tickets; i++)
                lot_tickets.push_front(*gen_tic.getTicket(lot_number, i, 1));
        else
        {
            for (unsigned int i = 0; i < num_of_tickets; i++)
                lot_tickets.push_front(*gen_tic.getTicket(lot_number, i,(rand()%100)<sale_chance));
        }
    }
public:
    ~Lot()
    {
        lot_tickets.clear();
    }

    friend ostream& operator<< (ostream &out, Lot<T> &lot)
    {
        for(auto it = lot.lot_tickets.begin(); it!=lot.lot_tickets.end(); it++)
        {
            out << *it << '\n';
        }
        return out;
    }
    template <class TT> friend class Game;
};

template <class T>
class Game
{
private:
    unsigned int game_id;
    unsigned int game_size;
    GeneratorTicket<T> *gen_tic;
    forward_list<Lot<T>> lot_tickets;

    Game(unsigned int id, unsigned int size, float sale_chance, GeneratorTicket<T> *gen) : game_id(id), game_size(size), gen_tic(gen)
    {
        sale_chance*=100;
        if(sale_chance<1 || sale_chance>100)
            sale_chance=rand() %100 +1;

        for(int i=0; i<game_size; i++)
            lot_tickets.push_front(Lot<T>(i, 20, int(sale_chance), *gen_tic));
    }
public:
    friend class GeneratorGameRusLot;
    friend ostream& operator<< (ostream &out, Game<T> &game)
    {
        out << "Game ID: " << game.game_id << "\n\n";
        for(auto it=game.lot_tickets.begin(); it!=game.lot_tickets.end(); it++)
            out << *it;
    }
};

template<class T>
class GeneratorGame
{
public:
    virtual Game<T> *getGame(unsigned int id, unsigned int size, float sale_chance, GeneratorTicket<T> *gen) const=0;
};

class GeneratorGameRusLot : public GeneratorGame<FieldRusLot>
{
public:
    Game<FieldRusLot> *getGame(unsigned int id, unsigned int size, float sale_chance, GeneratorTicket<FieldRusLot> *gen) const override
    {
        return new Game<FieldRusLot>(id, size, sale_chance, gen);
    };
};

int main()
{
    ofstream ofp;
    ofp.open("output_file.txt");
    if(!ofp.is_open())
        cout<<"Failed to open output file.";

    srand(static_cast <unsigned>(time(nullptr)));

    GeneratorGame<FieldRusLot> *gen_game_rus = new GeneratorGameRusLot();
    GeneratorTicket<FieldRusLot> *gen_tic_rus= new GeneratorTicketRusLot();
    Game<FieldRusLot> *game = gen_game_rus->getGame(0, 2, 0.8, gen_tic_rus);

    ofp << *game;

    ofp.close();
    return 0;
}
