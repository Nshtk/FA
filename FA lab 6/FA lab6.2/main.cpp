#include <iostream>
#include <cstring>
#include <list>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <cassert>

#define ALPHABET_SIZE 27
#define NOT_VALID_RESULT INT_MIN

using namespace std;

int strToInt(char *str, char *buf, int *i)                  // Функция для обработки чисел во входной строке
{
    int add_minus=1;

    buf[0]='1';
    buf[1]=' ';
    buf[2]=' ';

    if(str[*i]=='-')
    {
        add_minus=-1;
        (*i)++;
    }
    else if(str[*i]=='+')
        (*i)++;

    for(int j=0; isdigit(str[*i]); (*i)++, j++)
        buf[j]=str[*i];

    return add_minus;
}

class Polynome;

class TeX_convertible
{
public:
    virtual string convert() const = 0;
};

class Monome : public TeX_convertible
{
private:
    char **vars_and_their_pows;
    int coef;

public:
    Monome() : coef(1)
    {
        vars_and_their_pows=(char**)malloc(ALPHABET_SIZE*sizeof(char*));
        for(int i=0; i<ALPHABET_SIZE; i++)
            vars_and_their_pows[i]=(char*)calloc(2, sizeof(char));
    }
    explicit Monome(char *str) : Monome()
    {
        unsigned int length=strlen(str);
        int i=0, j, k, add_minus;
        char buf[] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', '\0'};

        add_minus=strToInt(str, buf, &i);
        coef=atoi(buf)*add_minus;
        for(j=0; i<length && j!=ALPHABET_SIZE; j++)
        {
            k=str[i]-97;
            vars_and_their_pows[k][0]=str[i];
            if(str[++i]=='^')
            {
                add_minus=strToInt(str, buf, &(++i));
                vars_and_their_pows[k][1]=atoi(buf)*add_minus;
            }
            else
                vars_and_their_pows[k][1]=1;
        }
    }
    Monome(Monome const &monome) : Monome()
    {
        coef=monome.coef;
        for(int i=0; i<ALPHABET_SIZE; i++)
        {
            vars_and_their_pows[i][0]=monome.vars_and_their_pows[i][0];
            vars_and_their_pows[i][1]=monome.vars_and_their_pows[i][1];
        }
    }
    ~Monome()
    {
        for(int i=0; i<ALPHABET_SIZE; i++)
            free(vars_and_their_pows[i]);
        free(vars_and_their_pows);
    }

    string convert() const override
    {
        stringstream out;

        if(coef>0)
            out << '+' << coef;
        else
            out << coef;

        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if(vars_and_their_pows[i][0]>0)
                out << vars_and_their_pows[i][0];
            if(vars_and_their_pows[i][1]!=1 && vars_and_their_pows[i][1]!=0)
                out << '^' << +vars_and_their_pows[i][1];
        }

        string out_s=out.str();
        return out_s;
    }

    Monome operator+(const Monome &monome);
    Monome &operator+=(const Monome &monome);
    Monome operator-(const Monome &monome);
    Monome &operator-=(const Monome &monome);
    Monome operator*(const Monome &monome);
    Monome &operator*=(const Monome &monome);
    Monome operator/(char var);
    Monome &operator/=(char var);
    bool operator==(const Monome& monome);
    bool operator!=(const Monome& monome);
    Monome& operator=(const Monome& monome);
    friend ostream& operator<< (ostream &out, Monome &monome);
    friend istream& operator>> (istream &in, Monome &monome);
    friend class Polynome;
    friend unsigned int cleanAndEmptyCheck(Polynome *polynome);
    friend bool isHomogeneous (const Polynome &polynome);
    friend bool isHarmonic (Polynome &polynome);
};

Monome &Monome::operator+=(const Monome &monome)
{
    int not_equal=0;
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
        {
            not_equal=1;
            break;
        }
    }

    if(not_equal)
        coef=NOT_VALID_RESULT;
    else
        coef+=monome.coef;

    return *this;
}

Monome Monome::operator+(const Monome &monome)
{
    return Monome(*this)+=monome;
}

Monome &Monome::operator-=(const Monome &monome)
{
    int not_equal=0;
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
        {
            not_equal=1;
            break;
        }
    }

    if(not_equal)
        coef=NOT_VALID_RESULT;
    else
        coef+=monome.coef;
    return *this;
}

Monome Monome::operator-(const Monome &monome)
{
    return Monome(*this)-=monome;
}

Monome &Monome::operator*=(const Monome &monome)
{
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if((vars_and_their_pows[i][0]==monome.vars_and_their_pows[i][0]) && vars_and_their_pows[i][0]!=0)
        {
            vars_and_their_pows[i][1]=vars_and_their_pows[i][1]+monome.vars_and_their_pows[i][1];
        }
        else if(monome.vars_and_their_pows[i][0]!=0)
        {
            vars_and_their_pows[i][0] = monome.vars_and_their_pows[i][0];
            vars_and_their_pows[i][1] = monome.vars_and_their_pows[i][1];
        }
    }

    coef*=monome.coef;
    return *this;
}

Monome Monome::operator*(const Monome &monome)
{
    return Monome(*this)*=monome;
}

Monome &Monome::operator/=(char var)
{
    int k=var-97;

    if((--vars_and_their_pows[k][1])!=0)
        coef*=vars_and_their_pows[k][1]+1;
    else
        vars_and_their_pows[k][0]=0;

    return *this;
}

Monome Monome::operator/(char var)
{
    return Monome(*this)/=var;
}

bool Monome::operator==(const Monome& monome)
{
    if(coef!=monome.coef)
        return false;
    for(int i=0; i<ALPHABET_SIZE; i++)
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
            return false;

    return true;
}

bool Monome::operator!=(const Monome& monome)
{
    if(coef!=monome.coef)
        return true;
    for(int i=0; i<ALPHABET_SIZE; i++)
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
            return true;

    return false;
}

Monome& Monome::operator=(const Monome& monome)
{
    if(&monome!=this)
    {
        coef = monome.coef;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            vars_and_their_pows[i][0] = monome.vars_and_their_pows[i][0];
            vars_and_their_pows[i][1] = monome.vars_and_their_pows[i][1];
        }
    }
    return *this;
}

ostream& operator<<(ostream &out, Monome &monome)
{
    if(monome.coef!=1)
    {
        if(monome.coef==-1)
            out << '-';
        else
            out << monome.coef;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if(monome.vars_and_their_pows[i][0]>0)
            out << monome.vars_and_their_pows[i][0];
        if(monome.vars_and_their_pows[i][1]!=1 && monome.vars_and_their_pows[i][1]!=0)
            out << '^' << +monome.vars_and_their_pows[i][1];
    }

    return out;
}

istream& operator>> (istream &in, Monome &monome)
{
    char buf[128];
    char c, i;

    in.get(c);
    for(i=0; c!='\n' && c!=' '; i++)
    {
        buf[i]=c;
        in.get(c);
    }
    buf[i]='\0';
    monome=Monome(buf);
    in.unget();

    return in;
}

int readStr(char **str, char *buf);
class Polynome : public TeX_convertible
{
private:
    list<Monome> container;
public:
    Polynome() = default;                                    // Конструктор для пустого полинома
    explicit Polynome(char* str)                             // Конструктор обычный char*
    {
        int str_not_empty=1;
        char *buf=(char*)malloc(64);

        while(str_not_empty)
        {
            str_not_empty=readStr(&str, buf);
            container.emplace_back(Monome(buf));
        }
        free(buf);
    }
    Polynome(const Polynome &polynome)                       // Конструктор копий
    {
        container=polynome.container;
    }
    ~Polynome()
    {
        container.clear();
    }

    int getCoef(const Monome &monome) const                  // Метод для получения коэффициента монома
    {
        return monome.coef;
    }

    string convert() const override
    {
        stringstream out;

        out << '(';
        for(const Monome &monome : container)
            out << monome.convert() << ' ';
        out << ')';

        string out_s=out.str();
        return out_s;
    }

    Polynome operator+(const Polynome &polynome);
    Polynome &operator+=(const Polynome &polynome);
    Polynome operator-(Polynome &polynome);
    Polynome &operator-=(Polynome &polynome);
    Polynome operator*(const Polynome &polynome);
    Polynome &operator*=(const Polynome &polynome);
    Polynome operator/(char var);
    Polynome &operator/=(char var);
    bool operator==(const Polynome &polynome);
    bool operator!=(const Polynome &polynome);
    Polynome &operator=(const Polynome &polynome);
    friend ostream& operator<< (ostream &out, Polynome &polynome);
    friend istream& operator>> (istream &in, Polynome &polynome);
    friend int readStr(char **str, char *buf);
    friend unsigned int cleanAndEmptyCheck(Polynome *polynome);
    friend bool isHomogeneous (const Polynome &polynome);
    friend bool isHarmonic (Polynome &polynome);
};

int readStr(char **str, char *buf)
{
    for(; (**str)>39; (*str)++, buf++)
        *buf=**str;

    *buf='\0';

    if(**str=='\0')
        return 0;

    (*str)+=3;
    return 1;
}

unsigned int cleanAndEmptyCheck(Polynome *polynome)                //Очистка контейнера с мономами от мономов с коэффициентом 0 и проверка полинома на пустоту
{
    Polynome temp;
    unsigned int not_empty;

    for(const Monome &monome : polynome->container)
    {
        if(monome.coef!=0)
        {
            temp.container.push_back(monome);
        }
    }
    not_empty=temp.container.size();

    if(!not_empty)
        polynome->container.clear();
    else
        *polynome=temp;

    return not_empty;
}

Polynome &Polynome::operator+=(const Polynome &polynome)
{
    Monome temp;
    int monome_not_utilised;

    for(const Monome &monome_b : polynome.container)
    {
        monome_not_utilised=1;
        for(Monome &monome_a : this->container)
        {
            temp=monome_a+monome_b;
            if(temp.coef!=NOT_VALID_RESULT)
            {
                monome_a=temp;
                monome_not_utilised=0;
                break;
            }
        }
        if(monome_not_utilised)
            container.emplace_back(monome_b);
    }

    cleanAndEmptyCheck(this);
    return *this;
}

Polynome Polynome::operator+(const Polynome &polynome)
{
    return Polynome(*this)+=polynome;
}

Polynome &Polynome::operator-=(Polynome &polynome)
{
    Monome temp;
    int monome_not_utilised;

    for(Monome &monome_b : polynome.container)
    {
        monome_b.coef*=-1;
        monome_not_utilised=1;
        for(Monome &monome_a : this->container)
        {
            temp=monome_b+monome_a;
            if(temp.coef!=NOT_VALID_RESULT)
            {
                monome_a=temp;
                monome_not_utilised=0;
                break;
            }
        }
        if(monome_not_utilised)
        {
            container.emplace_back(monome_b);
        }
    }

    cleanAndEmptyCheck(this);
    return *this;
}

Polynome Polynome::operator-(Polynome &polynome)
{
    return Polynome(*this)-=polynome;
}

Polynome &Polynome::operator*=(const Polynome &polynome)
{
    Polynome result;

    for(const Monome &monome_b : polynome.container)
        for(Monome &monome_a : result.container)
            result.container.emplace_back(monome_a*monome_b);

    return *this;
}

Polynome Polynome::operator*(const Polynome &polynome)
{
    return Polynome(*this)*=polynome;
}

Polynome &Polynome::operator/=(char var)
{
    for(Monome &monome : container)
        monome/=var;

    cleanAndEmptyCheck(this);
    return *this;
}

Polynome Polynome::operator/(char var)
{
    return Polynome(*this)/=var;
}

bool Polynome::operator==(const Polynome& polynome)
{
    bool equal;

    for(Monome monome_b : polynome.container)
    {
        equal= false;
        for(Monome &monome_a : container)
        {
            if(monome_b==monome_a)
            {
                equal = true;
                break;
            }
        }
        if(!equal)
            return equal;
    }
    return equal;
}

bool Polynome::operator!=(const Polynome& polynome)
{
    return !(*this==polynome);
}

Polynome& Polynome::operator=(const Polynome& polynome)
{
    if(&polynome!=this)
    {
        container.clear();
        for (const Monome &monome : polynome.container)
            container.push_back(monome);
    }

    return *this;
}

ostream& operator<< (ostream &out, Polynome &polynome)
{
    out << '(';
    for(Monome &monome : polynome.container)
    {
        if(polynome.getCoef(monome)>0)
            out << " +" << monome;
        else
            out << ' ' << monome;
    }
    out << ')';

    return out;
}

istream& operator>> (istream &in, Polynome &polynome)
{
    Monome monome;
    char c;
    while(in.get(c))
    {
        if(c==' ')
        {
            if(in.get()==' ')
                break;
        }
        else if(c=='\n')
            break;
        in.unget();

        in >> monome;
        polynome.container.emplace_back(monome);
    }
    return in;
}

bool isHomogeneous(const Polynome &polynome)
{
    int sum, sum_prev=0, i;

    for(const Monome &monome : polynome.container)
    {
        sum=0;
        for(i=0; i<ALPHABET_SIZE; i++)
        {
            if(monome.vars_and_their_pows[i][1]!=0)
                sum+=monome.vars_and_their_pows[i][1];
        }
        if(sum_prev!=0)
        {
            if(sum!=sum_prev)
            {
                return false;
            }
        }
        else
            sum_prev=sum;
    }
    return true;
}

bool isHarmonic(Polynome &polynome)
{
    vector<Polynome> derivatives;
    Polynome temp_pol;
    Monome temp_mon, first;
    unsigned int i, j, length, not_found;
    string characters_in_monome;

    first=polynome.container.front();

    for(i = 0; i < ALPHABET_SIZE; i++)
    {
        if(first.vars_and_their_pows[i][0]!=0)
            characters_in_monome.push_back(first.vars_and_their_pows[i][0]);
    }

    length=characters_in_monome.length();
    for (const Monome &monome : polynome.container)
    {
        for(i = 0; i < ALPHABET_SIZE; i++)
        {
            if(monome.vars_and_their_pows[i][0]!=0)
            {
                not_found=1;
                for (j = 0; j < length; j++)
                {
                    if(monome.vars_and_their_pows[i][0] == characters_in_monome[j])
                    {
                        not_found=0;
                        break;
                    }
                }
                if(not_found)
                {
                    characters_in_monome.push_back(monome.vars_and_their_pows[i][0]);
                    length++;
                }
            }
        }
    }
    for(i=0; i<length; i++)
    {
        derivatives.push_back(temp_pol);
        for (Monome &monome : polynome.container)
        {
            temp_mon = monome / characters_in_monome[i];
            temp_mon /= characters_in_monome[i];
            derivatives[i].container.push_back(temp_mon);
        }
    }

    length=derivatives.size();
    for(i=0; i<length; i++)
        temp_pol+=derivatives[i];

    if(cleanAndEmptyCheck(&temp_pol)==0)
        return true;

    return false;
}

Polynome fillPolynome(ifstream &fp)
{
    Polynome polynome;
    fp >> polynome;

    return polynome;
}

void calculateExpression(vector<Polynome> &operands, string operation, ofstream &fp)     //Функция для обработки выражения с полиномами и записи результата в выходной файл
{
    if (operands.size()>1)
    {
        switch (operation[0])
        {
            case '+':
                fp << operands[0].convert() << " + " << operands[1].convert() << " = " << (operands[0] + operands[1]).convert();
                break;
            case '-':
                fp << operands[0].convert() << " - " << operands[1].convert() << " = " << (operands[0] - operands[1]).convert();
                break;
            case '*':
                fp << operands[0].convert() << " * " << operands[1].convert() << " = " << (operands[0] * operands[1]).convert();
                break;
            case '=':
                fp << operands[0].convert() << " == " << operands[1].convert() << " = " << boolalpha << (operands[0]==operands[1]);
                break;
            case '!':
                fp << operands[0].convert() << " != " << operands[1].convert() << " = " << boolalpha << (operands[0]!=operands[1]);
                break;
        }
        operands.pop_back();
    }
    else
    {
        switch (operation[0])
        {
            case 'h':
                fp << operands[0].convert() << " is Homogeneous" << " = " << boolalpha << isHomogeneous(operands[0]);
                break;
            case 'g':
                fp << operands[0].convert() << " is Harmonic" << " = " << boolalpha << isHarmonic(operands[0]);
                break;
            case '/':
                fp << operands[0].convert() << " / " << operation[1] << " = " << (operands[0] / operation[1]).convert();
                break;
        }
    }
    operands.pop_back();
    fp << " \\\\\n";
}

int main()
{
    ifstream ifp;
    ifp.open("input_file.txt");
    assert(ifp.is_open() && "Failed to open output file");
    ofstream ofp;
    ofp.open("output_file.tex");
    assert(ofp.is_open() && "Failed to open output file");

    vector<Polynome> operands;
    string operation;

    try
    {
        while (true)
        {
            operands.push_back(fillPolynome(ifp));

            ifp >> operation;

            if(ifp.get()!='\n')
            {
                ifp.get();
                operands.push_back(fillPolynome(ifp));
            }

            calculateExpression(operands, operation, ofp);

            if(ifp.peek()==EOF)
                break;
        }
    }
    catch(const char *exc)
    {
        cerr << "Erorr occured while reading file: " << exc;
    }

    ifp.close();
    ofp.close();
    return 0;
}
