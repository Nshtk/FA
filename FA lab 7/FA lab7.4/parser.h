#ifndef FA_LAB7_4_PARSER_H
#define FA_LAB7_4_PARSER_H

#include <string>

using namespace std;

struct Date
{
    unsigned int day = 0;
    unsigned int month = 0;
    unsigned int year = 0;

    void clear()
    {
        day = year = month = 0;
    }
};

struct Person
{
    string first_name;
    string second_name;
    string last_name;

    void clear()
    {
        first_name.clear();
        last_name.clear();
        second_name.clear();
    }
};

struct Contract
{
    Person owner;
    Date end_date;
    Date start_date;
    unsigned int id = 0;
    unsigned int price = 0;
    unsigned int duration = 0;
    bool valid_status = false;

    void clear()
    {
        id = 0;
        price = 0;
        duration = 0;
        valid_status = false;
        owner.clear();
        end_date.clear();
        start_date.clear();
    }

    void print()
    {
        cout << owner.last_name << ' ' << owner.first_name << ' ' << owner.second_name<< ", Id = " << id
        << ", price = " << price << ", validity: " << start_date.day << '.' << start_date.month << '.' << start_date.year << " - ";
        if(valid_status)
            cout << "current time";
        else
            cout << end_date.day<< '.' << end_date.month<< '.' << end_date.year<< " (" << duration << " days)";
    }
};

int readName(string *field, string &buf, unsigned int *i)
{
    for( ; buf[*i]!=' ' && buf[*i]!=0; (*i)++)
        (*field) += buf[*i];
}

int readPerson(Person *prs, string &buf)
{
    string *fields[3] = {&prs->last_name, &prs->first_name, &prs->second_name};
    for(unsigned int i=0, j=0; j<3; i++, j++)
    {
        readName(fields[j], buf, &i);
    }
}

void getField(string &buf,unsigned int *field, unsigned int *i)
{
    string tmp;
    while(isdigit(buf[*i]))
    {
        tmp+=buf[*i];
        (*i)++;
    }
    *field=atoi(tmp.c_str());
}

int getTime(string &buf, unsigned int *fields[6], unsigned int *i)
{
    int j;
    for(j=0; j<3; j++, (*i)++)
        getField(buf, fields[j], i);
    (*i)+=5;
    if(buf[*i]=='c')
    {
        *fields[3]=99;
        *fields[4]=99;
        *fields[5]=9999;
        (*i)+=3;
        return 1;
    }
    for( ; j<6; j++, (*i)++)
        getField(buf, fields[j], i);
    return 0;
}

int readContract(Contract *ctr, Person *prs, string &buf)
{
    string tmp;
    unsigned int *fields[6] = {&ctr->start_date.day, &ctr->start_date.month, &ctr->start_date.year, &ctr->end_date.day, &ctr->end_date.month, &ctr->end_date.year,};

    ctr->owner=*prs;

    unsigned int i=0;
    for( ; isdigit(buf[i]); i++)
        tmp+=buf[i];
    ctr->id=atoi(tmp.c_str());
    tmp.clear();
    i+=6;

    unsigned int check=i;
    if(getTime(buf, fields, &i))
    {
        ctr->valid_status = true;
        if(i-check!=19)
            return 0;
    }
    else
    {
        if (i-check!=27)
            return 0;
    }

    for( ; isdigit(buf[i]); i++)
        tmp+=buf[i];
    ctr->price=atoi(tmp.c_str());

    ctr->duration = (ctr->end_date.year * 365 + ctr->end_date.month * 12+ ctr->end_date.day)-(ctr->start_date.year * 365 + ctr->start_date.month * 12 + ctr->start_date.day);
    return 1;
}

#endif
