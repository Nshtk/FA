#include <iostream>
#include <list>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iterator>
#include <ctime>
#include <forward_list>
#include <cassert>
#include <random>
#include <chrono>
//#include ".h"
//#include ".h"

#define NUMBER_OF_KEGS 90

using namespace std;

int foundDuplicate(int *arr, int i, int number)
{
    i++;
    for(int j=0; j<i; j++)
        if(number==arr[j])
            return 1;
    arr[i-1]=number;
    return 0;
}

void getValues(int arr[2][5])
{
    int i, min;
    for(i=0; i<5; i++)
        while(foundDuplicate(arr[0], i, rand() % 9)) {}
    for(i=0; i<5; i++)
    {
        if((min=(arr[0][i]+1)*10-10)==0)
            min=1;
        while(foundDuplicate(arr[1], i, rand() % 10+min)) {}
    }
}

class KegBag
{
private:
    vector<unsigned int> bag;
public:
    KegBag()
    {
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        for(int i=1; i<NUMBER_OF_KEGS; i++)
            bag.push_back(i);
        shuffle(bag.begin(), bag.end(), default_random_engine(seed));
    }
    void getKegs(vector<unsigned int> &vec, unsigned int amount)
    {
        vec.clear();
        for(int i=0; i<amount; i++)
        {
            vec.push_back(bag.back());
            bag.pop_back();
        }
    }

    friend const ostream& operator<<(ostream &out, const KegBag &keg_bag)
    {
        out << "Reamaining kegs in bag:";
        for(auto it : keg_bag.bag)
            out << ' ' << it;
        return out;
    }
};

class Field
{
protected:
    unsigned int row_count;
    unsigned int col_count;
    unsigned int ***field;                  // Игровое поле [0], и поле [1], где отмечаются зачеркнутые числа.
public:
    Field(unsigned int rows, unsigned int cols) : row_count(rows), col_count(cols)
    {
        unsigned int j;
        int field_values_in_row[2][5];
        field=new unsigned int**[2];
        field[0]=new unsigned int*[row_count];
        field[1]=new unsigned int*[row_count];
        for(unsigned int i=0; i<row_count; i++)
        {
            field[0][i]=new unsigned int[col_count];
            field[1][i]=new unsigned int[col_count];
            for(j=0; j<col_count; j++)
            {
                field[0][i][j] = 0;
                field[1][i][j] = 0;
            }

            getValues(field_values_in_row);
            for (j = 0; j < 5; j++)
                field[0][i][field_values_in_row[0][j]] = field_values_in_row[1][j];
        }
    }
    ~Field()
    {
        cout << "field called" << endl;
        for(unsigned int i=0; i<row_count; i++)
        {
            delete[]field[0][i];
            delete[]field[1][i];
        }
        delete[]field[0];
        delete[]field[1];
    }
    friend const ostream& operator<< (ostream &out, const Field &fld);
    template<class> friend class Ticket;
};

class FieldRusLot : public Field
{
public:
    FieldRusLot() : Field(3, 9) {}
};

const ostream& operator<< (ostream &out, const Field &fld)
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
    unsigned int lot_number, ticket_number, status;
    vector<T> fields;
public:
    Ticket(unsigned int lot_num, unsigned int ticket_num, unsigned int field_count, unsigned int status) :
            lot_number(lot_num), ticket_number(ticket_num),
            fields(field_count), status(status) {}
    ~Ticket()
    {
        cout << "ticket called" << endl;
    }
    int getStatus()
    {
        return status;
    }
    int processTicket(vector<unsigned int> &numbers, int vic_type)
    {
        int i, j;
        int counters[3] = {0};                                            // Проверяют строку, поле и билет на зачеркивание всех чисел в строке/поле/всех полях
        for(auto &fld : fields)
        {
            counters[1]=0;
            for(i=0; i<fld.row_count; i++)
            {
                counters[2]=0;
                for(j=0; j<fld.col_count; j++)
                {
                    for(auto &num : numbers)
                    {
                        if(fld.field[1][i][j] == 1)
                            counters[2]++;
                        if (num == fld.field[0][i][j])
                        {
                            fld.field[1][i][j] = 1;
                            counters[2]++;
                            break;
                        }
                    }
                }
                if(counters[2]==5)
                {
                    if(vic_type==1)
                    {
                        status = 2;
                        return 1;
                    }
                    counters[1]++;
                }
            }
            if(counters[1]==3)
            {
                counters[0]++;
                if(vic_type==2)
                {
                    status = 2;
                    return 1;
                }
            }
        }
        if(counters[0]==2 && vic_type==3)
        {
            status = 2;
            return 1;
        }
        return 0;
    }
    friend ostream& operator<< (ostream &out, Ticket<T> &tic)
    {
        out << "Lot series: " << tic.lot_number << '\n';
        out << "Ticket: " << tic.ticket_number << "\n";

        unsigned int field_count=tic.fields.size();
        for(int i=0; i<field_count; i++)
            out << "Field " << i << '\n' << tic.fields[i];

        switch(tic.status)
        {
            case 0:
                out << "UNSOLD";
                break;
            case 1:
                out << "SOLD";
                break;
            case 2:
                out << "SOLD, WINNER";
                break;
        }

        out << '\n';
        return out;
    }
    template<class> friend class Lot;
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
    template <class> friend class Lot;
};

class GeneratorTicketRusLot : public GeneratorTicket<FieldRusLot>
{
public:
    Ticket<FieldRusLot> *getTicket(unsigned int lot_num, unsigned int ticket_num, unsigned int status) const override
    {
        return new TicketRusLot(lot_num, ticket_num, status);
    }
};

template<class> class Game;

template<class T>
class Lot
{
private:
    unsigned int lot_number;
    forward_list<Ticket<T>*> lot_tickets;
public:
    Lot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen) : lot_number(num)
    {
        if(rand() % 2 || sale_chance==100)                                                           // Полная/неполная реализация тиража
            for(unsigned int i=0; i<num_of_tickets; i++)
                lot_tickets.push_front(gen.getTicket(lot_number, i, 1));
        else
            for(unsigned int i = 0; i < num_of_tickets; i++)
                lot_tickets.push_front(gen.getTicket(lot_number, i,(rand()%100)<sale_chance));
    }
    ~Lot()
    {
        cout << "lot called" << endl;
        lot_tickets.clear();
    }
    template <class> friend class Game;
    friend ostream& operator<< (ostream &out, Lot<T> &lot)
    {
        for(auto it = lot.lot_tickets.begin(); it!=lot.lot_tickets.end(); it++)
            out << **it << '\n';
        return out;
    }
};
template<class T>
class GeneratorLot
{
public:
    virtual Lot<T> *getLot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen) const=0;
};

template<class T>
class GeneratorLotRusLot : public GeneratorLot<FieldRusLot>
{
public:
    Lot<FieldRusLot> *getLot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen) const override
    {
        return new Lot<FieldRusLot>(num, num_of_tickets, sale_chance, gen);
    }
};

template <class T>
class Game
{
private:
    unsigned int game_id;
    unsigned int game_size;
    class KegBag *keg_bag;
    forward_list<Lot<T>*> lot_tickets;
    forward_list<Ticket<T>*> first_tour_winner_tickets;
    forward_list<Ticket<T>*> second_tour_winner_tickets;
    forward_list<Ticket<T>*> third_tour_winner_tickets;

    Game(unsigned int id, unsigned int num_of_lot, unsigned int num_of_tic, float sale_chance, KegBag* kB, GeneratorLot<T> &gen_lot, GeneratorTicket<T> &gen_tic)
    : game_id(id), game_size(num_of_lot), keg_bag(kB)
    {
        sale_chance*=100;
        if(sale_chance<1 || sale_chance>100)
            sale_chance=rand() %100 +1;

        for(int i=0; i<game_size; i++)
            lot_tickets.push_front(gen_lot.getLot(i, num_of_tic, sale_chance, gen_tic));
    }
    void printWinners(forward_list<Ticket<T>*> &winner_tickets)
    {
        for(auto it=winner_tickets.begin(); it!=winner_tickets.end(); it++)
            cout << **it <<'\n';
    }
    int processTour(unsigned int n, auto &lot, forward_list<Ticket<T>*> &tour_winner_tickets, int victory_type, vector<unsigned int> &kegs)
    {
        for(int i=0; i<n; i++)
        {
            keg_bag->getKegs(kegs, 1);
            for (auto &tic : (*lot)->lot_tickets)
                if (tic->getStatus() == 1)
                    if (tic->processTicket(kegs, victory_type))
                        tour_winner_tickets.push_front(tic);
        }
    }
public:
    int processGame(int lot_number)
    {
        auto lot = lot_tickets.begin();
        for( ; lot!=lot_tickets.end(); lot++)
        {
            if (lot_number == (*lot)->lot_number)
            {
                lot_number = -1;
                break;
            }
        }
        if(lot_number!=-1)
            throw;

        vector<unsigned int> kegs;

        cout << "Processing first tour...\n";
        keg_bag->getKegs(kegs, 5);
        for(auto &tic : (*lot)->lot_tickets)
            tic->processTicket(kegs, 1);
        processTour(10, lot, first_tour_winner_tickets, 1, kegs);
        cout << "Processing second tour...\n";
        processTour(30, lot, second_tour_winner_tickets, 2, kegs);
        cout << "Processing third tour...\n\n";
        processTour(15, lot, third_tour_winner_tickets, 3, kegs);

        cout << *keg_bag;
        cout << "\n\nFirst tour winners:\n";
        printWinners(first_tour_winner_tickets);
        cout << "\nSecond tour winners:\n";
        printWinners(second_tour_winner_tickets);
        cout << "\nThird tour winners:\n";
        printWinners(third_tour_winner_tickets);
        return 0;
    }
    template<class> friend class GeneratorGameRusLot;
    friend ostream& operator<< (ostream &out, Game<T> &game)
    {
        out << "Game ID: " << game.game_id << "\n\n";
        for(auto it=game.lot_tickets.begin(); it!=game.lot_tickets.end(); it++)
            out << **it;
        return out;
    }
};

template<class T>
class GeneratorGame
{
public:
    virtual Game<T> *getGame(unsigned int id, unsigned int num_of_lot, unsigned int num_of_tic, float sale_chance, KegBag* kegBag,
                             GeneratorLot<T> &gen_lot, GeneratorTicket<T> &gen_tic) const=0;
};

template<class T>
class GeneratorGameRusLot : public GeneratorGame<FieldRusLot>
{
public:
    Game<FieldRusLot> *getGame(unsigned int id, unsigned int num_of_lot, unsigned int num_of_tic, float sale_chance, KegBag* kegBag,
                               GeneratorLot<T> &gen_lot, GeneratorTicket<T> &gen_tic) const override
    {
        return new Game<FieldRusLot>(id, num_of_lot, num_of_tic, sale_chance, kegBag, gen_lot, gen_tic);
    };
};

template<class T>
class SuperGenerator
{
private:
public:
    GeneratorGame<T> *gen_game;
    GeneratorLot<T> *gen_lot;
    GeneratorTicket<T> *gen_tic;
    SuperGenerator(GeneratorGame<T> *gen_one, GeneratorLot<T> *gen_two,GeneratorTicket<T> *gen_three) : gen_game(gen_one), gen_lot(gen_two),
                                                                                                        gen_tic(gen_three){}
    ~SuperGenerator()
    {
        delete gen_game;
        delete gen_lot;
        delete gen_tic;
    }
};

class SuperGeneratorRusLot : public SuperGenerator<FieldRusLot>
{
private:

public:
    SuperGeneratorRusLot() : SuperGenerator(new GeneratorGameRusLot<FieldRusLot>(), new GeneratorLotRusLot<FieldRusLot>(),
                                            new GeneratorTicketRusLot())
    {}
};

int main()
{
    ofstream ofp;
    ofp.open("output_file.txt");
    assert(ofp.is_open() && "Failed to open output file");

    srand(static_cast <unsigned>(time(nullptr)));

    KegBag *keg_bag = new KegBag;
    SuperGenerator<FieldRusLot> *super_gen = new SuperGeneratorRusLot();
    Game<FieldRusLot> *game=super_gen->gen_game->getGame(0, 1, 20000, 0.8, keg_bag, *super_gen->gen_lot,
                                                         *super_gen->gen_tic);
    game->processGame(0);
    ofp << *game;

    delete keg_bag; delete super_gen; delete game;
    ofp.close();
    return 0;
}
