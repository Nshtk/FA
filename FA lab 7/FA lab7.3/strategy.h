#ifndef FA_LAB7_3_STRATEGY_H
#define FA_LAB7_3_STRATEGY_H

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

class StrategyDatetime : public Strategy<Message*>
{
    int compareValues(Message* const &a, Message* const &b) const override
    {
        int a_date = (*a).time.year * 1000 + (*a).time.month * 100 + (*a).time.day;
        int a_time = (*a).time.hour * 3600 + (*a).time.month * 60 + (*a).time.second;
        int b_date = (*b).time.year * 1000 + (*b).time.month * 100 + (*b).time.day;
        int b_time = (*b).time.hour * 3600 + (*b).time.month * 60 + (*b).time.second;

        if (a_date + a_time > b_date + b_time)
            return (1);
        if (a_date + a_time < b_date + b_time)
            return (-1);
        return (0);
    }
};

class StrategyUsername : public Strategy<Message *>
{
    int compareValues(Message* const &a, Message* const &b) const override
    {
        if ((*a).username > (*b).username)
            return (1);
        else if ((*a).username < (*b).username)
            return (-1);
        return (0);
    }
};

class StrategyMessage : public Strategy<Message *>
{
    int compareValues(Message* const &a, Message* const &b) const override
    {
        string a_text = (*a).text;
        string b_text = (*b).text;

        transform(a_text.begin(), a_text.end(), a_text.begin(), ::toupper);
        transform(b_text.begin(), b_text.end(), b_text.begin(), ::toupper);

        if (a_text > b_text)
            return 1;
        else if (a_text < b_text)
            return -1;
        return 0;
    }
};
class StrategyDatetimeUsername : public Strategy<Message *>
{
    int compareValues(Message* const &a, Message* const &b) const override
    {
        unsigned int a_date = (*a).time.year * 1000 + (*a).time.month * 100 + (*a).time.day;
        unsigned int a_time = (*a).time.hour * 3600 + (*a).time.month * 60 + (*a).time.second;
        unsigned int b_date = (*b).time.year * 1000 + (*b).time.month * 100 + (*b).time.day;
        unsigned int b_time = (*b).time.hour * 3600 + (*b).time.month * 60 + (*b).time.second;

        if (a_date + a_time > b_date + b_time)
            return 1;
        if (a_date + a_time < b_date + b_time)
            return -1;
        else
        {
            string a_username = (*a).username;
            string b_username = (*b).username;

            transform(a_username.begin(), a_username.end(), a_username.begin(), ::toupper);
            transform(b_username.begin(), b_username.end(), b_username.begin(), ::toupper);

            if (a_username > b_username)
                return 1;
            else if (a_username < b_username)
                return -1;
        }
        return 0;
    }
};

#endif
