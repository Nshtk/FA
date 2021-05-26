#ifndef FA_KURS_COMPONENTS_H
#define FA_KURS_COMPONENTS_H

#include <algorithm>
#include <vector>
#include <cstdint>
#include <random>
#include <chrono>
#include "list_decorator.h"
#include "functions.h"

#define NUMBER_OF_KEGS 90

using namespace std;

template<class T>
class GeneratorTicket;
template <class T, template<class> class L>
class GeneratorLot;

class KegBag
{
private:
    vector<unsigned int> bag;
public:
    KegBag(unsigned &seed)
    {
        seed+=rand();                           // rand() прибавлен из-за того, что seed практически не меняется за время, достаточное для генерации нескольких билетов.
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
        out << "Remaining kegs in bag:";
        for(auto it : keg_bag.bag)
            out << ' ' << it;
        return out;
    }
};

class Field
{
protected:
    unsigned long long row_count;
    unsigned long long col_count;
    int8_t **field;
public:
    Field(unsigned long long rows, unsigned long long cols) : row_count(rows), col_count(cols)
    {
        unsigned long long j;
        int8_t field_values_in_row[2][5];

        field=new int8_t *[row_count];
        for(unsigned long long i=0; i<row_count; i++)
        {
            field[i]=new int8_t[col_count];
            for(j=0; j<col_count; j++)
                field[i][j] = 0;

            getValues(field_values_in_row);
            for (j = 0; j < 5; j++)
                field[i][field_values_in_row[0][j]] = field_values_in_row[1][j];
        }
    }
    ~Field()
    {
        for(int i=0; i<row_count; i++)
            delete[]field[i];
        delete[]field;
    }
    friend const ostream& operator<< (ostream &out, const Field &fld);
    friend istream& operator>> (istream &in, Field &fld);
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
            out << (int)fld.field[i][j] << ' ';
        out << '\n';
    }
    return out;
}

istream& operator>> (istream &in, Field &fld)
{
    int tmp;
    for(int i=0; i<fld.row_count; i++)
    {
        for(int j=0; j<fld.col_count; j++)
        {
            in >> tmp;
            fld.field[i][j]=tmp;
        }
        in.get(); in.get();
    }
}

template <class T, template<class> class L> class Game;

template<class T>
class Ticket
{
protected:
    unsigned long long lot_number, ticket_number, status;
    vector<T> fields;
public:
    Ticket(unsigned long long lot_num, unsigned long long ticket_num, unsigned int field_count, unsigned int status) :
            lot_number(lot_num), ticket_number(ticket_num),
            fields(field_count), status(status) {}
    ~Ticket()
    {
        fields.clear();
    }
    int getStatus()
    {
        return status;
    }
    int processTicket(vector<unsigned int> &numbers, int vic_type)
    {
        int i, j;
        int counters[3] = {0};                                            // Проверяют строку, поле и билет на зачеркивание всех чисел в строке/поле/всех полях.
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
                        if(fld.field[i][j] < 0)
                            counters[2]++;
                        else if (num == fld.field[i][j])
                        {
                            fld.field[i][j]*=-1;
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
                    status = 3;
                    return 1;
                }
            }
        }
        if(counters[0]==2 && vic_type==3)
        {
            status = 4;
            return 1;
        }
        return 0;
    }
    friend ostream& operator<< (ostream &out, const Ticket<T> &tic)
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
                out << "1-st TOUR WINNER, SOLD";
                break;
            case 3:
                out << "2-nd TOUR WINNER, SOLD";
                break;
            case 4:
                out << "3-rd TOUR WINNER, SOLD";
                break;
        }
        out << '\n';
        return out;
    }
    friend istream& operator>> (istream &in, Ticket<T> &tic)
    {
        unsigned int size=tic.fields.size();
        char c;

        in.ignore(12);
        in >> tic.lot_number;
        in.ignore(8,' ');
        in >> tic.ticket_number;
        in.get(c);
        for(int i=0; i<size; i++)
        {
            in.ignore(20, '\n');                                          // Пропуск Field.
            in>>tic.fields[i];
        }
        in.get(c);
        switch(c)
        {
            case 'U':
                tic.status = 0;
                break;
            case 'S':
                tic.status = 1;
                break;
            case '1':
                tic.status = 2;
                break;
            case '2':
                tic.status = 3;
                break;
            case '3':
                tic.status = 4;
                break;
        }
        in.ignore(30, '\n');
    }
    template <class C, template<class> class L> friend Ticket<C> *searchTickets(Game<C, L> &game, Ticket<C> *tic);
    template <class, template<class> class> friend class Lot;
};

class TicketRusLot : public Ticket<FieldRusLot>
{
public:
    TicketRusLot(unsigned long long lot_num, unsigned long long ticket_num, unsigned int status) : Ticket(lot_num, ticket_num, 2, status) {}
};

template <class T, template<class> class L>
class Lot
{
private:
    bool was_processed;
    unsigned long long lot_number;
    KegBag keg_bag;
    ListBasicInterface<L, Ticket<T>*> lot_tickets;
public:
    Lot(unsigned long long num, unsigned long long num_of_tickets, unsigned int sale_chance, unsigned &seed, GeneratorTicket<T> &gen) : was_processed(false), lot_number(num),
                                                                                                                            keg_bag(seed)
    {
        for(unsigned long long i=0; i<num_of_tickets; i++)
            lot_tickets.push_front(gen.getTicket(lot_number, i,(rand()%100)<sale_chance));
    }
    ~Lot()
    {
        lot_tickets.clear();
    }
    template <class, template<class> class> friend class Game;
    friend const ostream& operator<< (ostream &out, Lot<T, L> &lot)
    {
        for(auto it = lot.lot_tickets.begin(); it!=lot.lot_tickets.end(); it++)
            out << **it << '\n';
        return out;
    }
};


template <class T, template<class> class L>
class Game
{
private:
    unsigned seed;
    unsigned long long id;
    unsigned long long count_of_lots;
    unsigned long long count_of_tickets;
    ListBasicInterface<L, Lot<T, L>*> lot_tickets;
    ListBasicInterface<L, Ticket<T>*> first_tour_winner_tickets;
    ListBasicInterface<L, Ticket<T>*> second_tour_winner_tickets;
    ListBasicInterface<L, Ticket<T>*> third_tour_winner_tickets;

    Game(GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic) : seed(chrono::system_clock::now().time_since_epoch().count())
    {
        generationProceed(generationMenu(), gen_lot, gen_tic);      // Вызов меню для задания параметров генерации в качестве параметра для основной функции генерации
    }

    float generationMenu()
    {
        float sale_chance;
        cout << "Available generation parameters:\n"
                "Game id;\n"
                "Number of lots;\n"
                "Number of tickets in lot. Set this parameter to 0 if number of tickets in each lot should vary;\n"
                "Ticket sale chance (float). Set this parameter to number less than 0 if sale chance for tickets in each lot should vary.\t\t   "
                                            "Set this parameter to number greater than 1 to get random chance of sale for tickets in each lot.\n\n"
                "Enter parameters:\n";
        cin >> id >> count_of_lots >> count_of_tickets >> sale_chance;
        return sale_chance;
    }
    void generationProceed(float sale_chance, GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic)
    {
        if(!count_of_tickets && (sale_chance<0))                                // Не хочется еще больше плодить сущностей и делать отдельную функцию.
        {
            unsigned long long num;
            for(int i=0; i<count_of_lots; i++)
            {
                cout << "Enter number of tickets and/or sale chance:\n";
                cin >> num >> sale_chance;
                lot_tickets.push_front(gen_lot.getLot(i, num, floatChanceToInt(sale_chance), seed, gen_tic));
            }
        }
        else if(!count_of_tickets)
        {
            unsigned long long num;
            for(int i=0; i<count_of_lots; i++)
            {
                cout << "Enter number of tickets and/or sale chance:\n";
                cin >> num;
                lot_tickets.push_front(gen_lot.getLot(i, count_of_tickets, sale_chance, seed, gen_tic));
            }
        }
        else if(sale_chance<0)
        {
            for(int i=0; i<count_of_lots; i++)
            {
                cout << "Enter number of tickets and/or sale chance:\n";
                cin >> sale_chance;
                lot_tickets.push_front(gen_lot.getLot(i, count_of_tickets, floatChanceToInt(sale_chance), seed, gen_tic));
            }
        }
        else
        {
            sale_chance = floatChanceToInt(sale_chance);
            for (int i = 0; i < count_of_lots; i++)
                lot_tickets.push_front(gen_lot.getLot(i, count_of_tickets, sale_chance, seed, gen_tic));
        }
    }
    void printWinners(ListBasicInterface<L, Ticket<T>*> &winner_tickets) const
    {
        if(winner_tickets.empty())
        {
            cout << "No winners!";
            return;
        }
        for(auto it=winner_tickets.begin(); it!=winner_tickets.end(); it++)
            cout << **it <<'\n';
    }
    int processTour(unsigned int n, auto &lot, ListBasicInterface<L, Ticket<T>*> &tour_winner_tickets, int victory_type, vector<unsigned int> &kegs)
    {
        for(int i=0; i<n; i++)                                       // Определение победителей происходит как по условию - после каждого хода.
        {
            (*lot)->keg_bag.getKegs(kegs, 1);
            for(auto tic : (*lot)->lot_tickets)
                if(tic->getStatus() == 1)
                    if(tic->processTicket(kegs, victory_type))
                        tour_winner_tickets.push_front(tic);
        }
    }
public:
    ~Game()
    {
        lot_tickets.clear();
        first_tour_winner_tickets.clear();
        second_tour_winner_tickets.clear();
        third_tour_winner_tickets.clear();
    };
    int processLot(long int lot_to_process, auto lot)
    {
        if(lot==lot_tickets.end())                                          // Проверка на нахождение номера тиража в списке тиражей.
        {
            auto lt=lot_tickets.begin();
            for (; lt!=lot_tickets.end(); lt++)
            {
                if (lot_to_process == (*lt)->lot_number)
                {
                    lot_to_process = -1;
                    break;
                }
            }
            if(lot_to_process != -1)
                return 1;
            lot=lt;
        }
        if((*lot)->was_processed)
            return 2;

        vector<unsigned int> kegs;
        (*lot)->keg_bag.getKegs(kegs, 5);
        cout << "\nProcessing lot with id " << (*lot)->lot_number << "\nProcessing first tour...\n";
        for(auto tic : (*lot)->lot_tickets)
            if(tic->getStatus()==1)
                tic->processTicket(kegs, 1);
        processTour(10, lot, first_tour_winner_tickets, 1, kegs);               // Первый параметр - количество вынимаемых бочонков в туре.
        cout << "Processing second tour...\n";
        processTour(30, lot, second_tour_winner_tickets, 2, kegs);
        cout << "Processing third tour...\n";
        processTour(15, lot, third_tour_winner_tickets, 3, kegs);

        (*lot)->was_processed=true;
        cout << (*lot)->keg_bag;

        return 0;
    }
    int processGame(int process_mode, unsigned long long number)
    {
        switch(process_mode)
        {
            case 0:
                for(auto lot=lot_tickets.begin(); lot!=lot_tickets.end(); lot++)
                    if((err_code=processLot((*lot)->lot_number, lot)))
                        errCheck_Components((*lot)->lot_number);
                break;
            case 1:
                if((err_code=processLot(number, lot_tickets.end())))
                    errCheck_Components(number);
                break;
            default:
                cout << "\nNo such mode, try again.";
                return -1;
        }
        return err_code;
    }
    void printAllWinners()
    {
        cout << "\n\nFirst tour winners:\n";
        printWinners(first_tour_winner_tickets);
        cout << "\nSecond tour winners:\n";
        printWinners(second_tour_winner_tickets);
        cout << "\nThird tour winners:\n";
        printWinners(third_tour_winner_tickets);
    }

    template <class, template<class> class> friend class GeneratorGameRusLot;
    friend const ostream& operator<< (ostream &out, Game<T, L> &game)
    {
        out << "Game ID: " << game.id << "\n\n";
        for(auto it=game.lot_tickets.begin(); it!=game.lot_tickets.end(); it++)
            out << **it;
        return out;
    }
};

template <class C, template<class> class L>
Ticket<C> *searchTickets(Game<C, L> &game, Ticket<C> *tic)           // Функция может быть также использована при работе с компонентами, так как возвращает указатель типа Ticket.
{
    ifstream fp("output_file.txt");
    if(!fp.is_open())
    {
        err_code=1;
        errCheck_Main();
    }
    fp.ignore(10,'\n');                                                    // Пропуск Game id.
    fp.get();

    long long criteria[3], initial_value=0, count_of_matching;
    cout << "\nAvailable searching parameters:\n"
            "Lot number;\n"
            "Ticket number;\n"
            "Ticket status.\n"
            "If you enter a negative number for criteria, this criteria will not be taken into consideration when searching. "
            "In that case first matching ticket will be returned.\n";
    cin >> criteria[0] >> criteria[1] >> criteria[2];
    for(long long i : criteria)
        if(i<0)
            initial_value++;
    do
    {
        count_of_matching=initial_value;
        fp >> *tic;
        if(tic->lot_number==criteria[0] && criteria[0]>-1)
            count_of_matching++;
        if(tic->ticket_number==criteria[1] && criteria[1]>-1)
            count_of_matching++;
        if(tic->status==criteria[2] && criteria[2]>-1)
            count_of_matching++;
        if(count_of_matching==3)
        {
            cout << '\n' << *tic;
            fp.close();
            return tic;
        }
    }
    while(fp.get()!=EOF);

    fp.close();
    cout << "Ticket not found.\n";
    return nullptr;
}

#endif
