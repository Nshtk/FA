#ifndef FA_LAB7_2_STRATEGY_H
#define FA_LAB7_2_STRATEGY_H

template<class T>
class Strategy
{
public:
    virtual int compareValues(const T &a, const T &b) const
    {}
};

template<class T>
class StrategyOne : public Strategy<T>
{
public:
    int compareValues(const T &a, const T &b) const override
    {
        if(a==b)
            return 0;
        else if(a>b)
            return 1;
        else
            return -1;
    }
};

#endif
