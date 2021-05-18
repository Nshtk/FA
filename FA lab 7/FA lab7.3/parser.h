#ifndef FA_LAB7_3_PARSER_H
#define FA_LAB7_3_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <forward_list>

using namespace std;

struct Datetime
{
    unsigned int day = 0;
    unsigned int month = 0;
    unsigned int year = 0;
    unsigned int hour = 0;
    unsigned int minute = 0;
    unsigned int second = 0;
};

struct Message
{
    Datetime time;
    string username;
    string text;

    void print()
    {
        cout << time.day << '.' << time.month << '.' << time.year << ' ' << time.hour << ':' << time.minute << ':'
        << time.second << ' ' << username << ": " << text << '\n';
    }

    void clear()
    {
        time.day = 0;
        time.month = 0;
        time.year = 0;
        time.hour = 0;
        time.month = 0;
        time.second = 0;
        username.clear();
        text.clear();
    }
};

void getField(string &buf, unsigned int *field, int *i)
{
    string tmp;
    while(isdigit(buf[*i]))
    {
        tmp+=buf[*i];
        (*i)++;
    }
    *field=atoi(tmp.c_str());
}

void getTime(string &buf, int *i, unsigned int *fields[6])
{
    int j;
    for(j=0; j<3; j++, (*i)++)
        getField(buf, fields[j], i);
    for( ; j<6; j++, (*i)++)
        getField(buf, fields[j], i);
}
int readString(Message *msg, string &buf)
{
    string tmp;
    unsigned int *fields[6] = {&msg->time.day, &msg->time.month, &msg->time.year, &msg->time.hour, &msg->time.minute, &msg->time.second};
    int i=0;

    getTime(buf, &i, fields);
    if(i!=20)
        return 0;
    while(buf[i]!=':')
    {
        tmp += buf[i];
        i++;
    }
    msg->username=tmp;
    tmp.clear();
    i+=2;

    while(buf[i]!='\0')
    {
        tmp += buf[i];
        i++;
    }
    msg->text=tmp;

    return 1;
}

#endif
