#ifndef FA_LAB7_4_STRATEGY_H
#define FA_LAB7_4_STRATEGY_H

#include <algorithm>
#include "parser.h"

using namespace std;

template<typename T>
class Strategy
{
public:
    virtual int compareValues(const T &a, const T &b) const
    {}
};

class StrategyId : public Strategy<Contract*>
{
    int compareValues(Contract* const &a, Contract* const &b) const override
    {
        if ((*a).owner.last_name > (*b).owner.last_name)
            return (1);
        else if ((*a).owner.last_name < (*b).owner.last_name)
            return (-1);
        else
        {
            if ((*a).owner.second_name > (*b).owner.second_name)
                return (1);
            else if ((*a).owner.second_name < (*b).owner.second_name)
                return (-1);
            else
            {
                if ((*a).owner.second_name > (*b).owner.second_name)
                    return (1);
                else if ((*a).owner.second_name < (*b).owner.second_name)
                    return (-1);
                else
                {
                    if ((*a).id > (*b).id)
                        return (1);
                    else if ((*a).id < (*b).id)
                        return (-1);
                }
            }
        }
        return (0);
    }
};

class StrategyDate : public Strategy<Contract*>
{
    int compareValues(Contract* const &a, Contract* const &b) const override
    {
        if ((*a).duration > (*b).duration)
            return (1);
        else if ((*a).duration < (*b).duration)
            return (-1);
        return (0);
    }
};

class StrategyPrice : public Strategy<Contract*>
{
    int compareValues(Contract* const &a, Contract* const &b) const override
    {
        if ((*a).price > (*b).price)
            return (1);
        else if ((*a).price < (*b).price)
            return (-1);
        return (0);
    }
};

#endif
