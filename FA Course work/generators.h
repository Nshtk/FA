#ifndef FA_KURS_GENERATORS_H
#define FA_KURS_GENERATORS_H

#include "error_check.h"
#include "components.h"

template<class T>
class GeneratorTicket
{
public:
    virtual Ticket<T> *getTicket(unsigned long long lot_num, unsigned long long ticket_num, unsigned int status) const = 0;
};

class GeneratorTicketRusLot : public GeneratorTicket<FieldRusLot>
{
public:
    Ticket<FieldRusLot> *getTicket(unsigned long long lot_num, unsigned long long ticket_num, unsigned int status) const override
    {
        return new TicketRusLot(lot_num, ticket_num, status);
    }
};

template <class T, template<class> class L>
class GeneratorLot
{
public:
    virtual Lot<T, L> *getLot(unsigned long long num, unsigned long long num_of_tickets, unsigned int sale_chance, unsigned &seed, GeneratorTicket<T> &gen) const = 0;
};

template <class T, template<class> class L>
class GeneratorLotRusLot : public GeneratorLot<FieldRusLot, L>
{
public:
    Lot<FieldRusLot, L> *getLot(unsigned long long num, unsigned long long num_of_tickets, unsigned int sale_chance, unsigned &seed, GeneratorTicket<T> &gen) const override
    {
        return new Lot<FieldRusLot, L>(num, num_of_tickets, sale_chance, seed, gen);
    }
};

template <class T, template<class> class L>
class GeneratorGame
{
public:
    virtual Game<T, L> *getGame(GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic) const = 0;
};

template <class T, template<class> class L>
class GeneratorGameRusLot : public GeneratorGame<FieldRusLot, L>
{
public:
    Game<FieldRusLot, L> *getGame(GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic) const override
    {
        return new Game<FieldRusLot, L>(gen_lot, gen_tic);
    };
};

template <class T, template<class> class L>
class SuperGenerator
{
public:
    GeneratorGame<T, L> *gen_game;
    GeneratorLot<T, L> *gen_lot;
    GeneratorTicket<T> *gen_tic;
    SuperGenerator(GeneratorGame<T, L> *gen_one, GeneratorLot<T, L> *gen_two, GeneratorTicket<T> *gen_three) : gen_game(gen_one), gen_lot(gen_two),
                                                                                                               gen_tic(gen_three){}
    ~SuperGenerator()
    {
        delete gen_game;
        delete gen_lot;
        delete gen_tic;
    }
};

template <template<class> class L>
class SuperGeneratorRusLot : public SuperGenerator<FieldRusLot, L>
{
public:
    SuperGeneratorRusLot() : SuperGenerator<FieldRusLot, L>::SuperGenerator(new GeneratorGameRusLot<FieldRusLot, L>(), new GeneratorLotRusLot<FieldRusLot, L>(),
                                                                            new GeneratorTicketRusLot()) {}
};

#endif