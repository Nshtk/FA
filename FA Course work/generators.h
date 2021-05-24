#ifndef FA_KURS_GENERATORS_H
#define FA_KURS_GENERATORS_H

#include "error_check.h"
#include "components.h"

template<class T>
class GeneratorTicket
{
public:
    virtual Ticket<T> *getTicket(unsigned int lot_num, unsigned int ticket_num, unsigned int status) const = 0;
    template <class, template<class> class> friend class Lot;
};

class GeneratorTicketRusLot : public GeneratorTicket<FieldRusLot>
{
public:
    Ticket<FieldRusLot> *getTicket(unsigned int lot_num, unsigned int ticket_num, unsigned int status) const override
    {
        return new TicketRusLot(lot_num, ticket_num, status);
    }
};

template <class T, template<class> class L>
class GeneratorLot
{
public:
    virtual Lot<T, L> *getLot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen) const=0;
};

template <class T, template<class> class L>
class GeneratorLotRusLot : public GeneratorLot<FieldRusLot, L>
{
public:
    Lot<FieldRusLot, L> *getLot(unsigned int num, unsigned int num_of_tickets, unsigned int sale_chance, GeneratorTicket<T> &gen) const override
    {
        return new Lot<FieldRusLot, L>(num, num_of_tickets, sale_chance, gen);
    }
};

template <class T, template<class> class L>
class GeneratorGame
{
public:
    virtual Game<T, L> *getGame(unsigned int id, unsigned int num_of_lot, unsigned int num_of_tic, float sale_chance,
                                GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic) const=0;
};

template <class T, template<class> class L>
class GeneratorGameRusLot : public GeneratorGame<FieldRusLot, L>
{
public:
    Game<FieldRusLot, L> *getGame(unsigned int id, unsigned int num_of_lot, unsigned int num_of_tic, float sale_chance,
                                  GeneratorLot<T, L> &gen_lot, GeneratorTicket<T> &gen_tic) const override
    {
        return new Game<FieldRusLot, L>(id, num_of_lot, num_of_tic, sale_chance, gen_lot, gen_tic);
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