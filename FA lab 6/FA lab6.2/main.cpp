#include <iostream>
#include <cstring>
#include <list>
#include <iterator>
#include <algorithm>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#define ALPHABET_SIZE 27
#define NOT_VALID_RESULT INT_MIN

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace std;

int strToInt(char *str, char *buf, int *i)                  //Функция для обработки чисел во входной строке
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
    virtual string convert() const {}
};

class Monome : public TeX_convertible
{
private:
    char **vars_and_their_pows;
    int coef;

public:
    Monome()
    {
        vars_and_their_pows=(char**)malloc(ALPHABET_SIZE*sizeof(char*));
        for(int i=0; i<ALPHABET_SIZE; i++)
            vars_and_their_pows[i]=(char*)calloc(2, sizeof(char));
        coef=1;
    }
    Monome(char *str) : Monome()
    {
        int length=strlen(str), i=0, j, k, add_minus;
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

    Monome operator+(Monome &monome);
    Monome operator+=(Monome &monome);
    Monome operator-(Monome &monome);
    Monome operator-=(Monome &monome);
    Monome operator*(Monome &monome);
    Monome operator*=(Monome &monome);
    Monome operator/(char var);
    Monome operator/=(char var);
    bool operator==(const Monome& monome);
    bool operator!=(const Monome& monome);
    Monome operator=(const Monome& monome);
    friend ostream& operator<< (ostream &out, const Monome &monome);
    friend ostream& operator<< (ostream &out, const Monome *monome);
    friend istream& operator>> (istream &in, Monome &monome);
    friend class Polynome;
    friend int cleanAndEmptyCheck(Polynome *polynome);
    friend bool isHomogeneous (const Polynome &polynome);
    friend bool isHarmonic (Polynome polynome);
};

Monome Monome::operator+(Monome &monome)
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

    Monome result(*this);
    if(not_equal)
        result.coef=NOT_VALID_RESULT;
    else
        result.coef+=monome.coef;
    return result;
}

Monome Monome::operator+=(Monome &monome)
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

Monome Monome::operator-(Monome &monome)
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

    Monome result(*this);
    if(not_equal)
        result.coef=NOT_VALID_RESULT;
    else
        result.coef-=monome.coef;
    return result;
}

Monome Monome::operator-=(Monome &monome)
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

Monome Monome::operator*(Monome &monome)
{
    Monome result(*this);

    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if((vars_and_their_pows[i][0]==monome.vars_and_their_pows[i][0]) && vars_and_their_pows[i][0]!=0)
        {
            result.vars_and_their_pows[i][1]=vars_and_their_pows[i][1]+monome.vars_and_their_pows[i][1];
        }
        else if(monome.vars_and_their_pows[i][0]!=0)
        {
            result.vars_and_their_pows[i][0] = monome.vars_and_their_pows[i][0];
            result.vars_and_their_pows[i][1] = monome.vars_and_their_pows[i][1];
        }
    }

    result.coef=coef*monome.coef;
    return result;
}

Monome Monome::operator*=(Monome &monome)
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

Monome Monome::operator/(char var)
{
    Monome result(*this);
    int k=var-97;

    if((--result.vars_and_their_pows[k][1])!=0)
        result.coef*=(result.vars_and_their_pows[k][1]+1);
    else
        result.vars_and_their_pows[k][0]=0;

    return result;
}

Monome Monome::operator/=(char var)
{
    int k=var-97;

    if((--vars_and_their_pows[k][1])!=0)
        coef*=vars_and_their_pows[k][1]+1;
    else
        vars_and_their_pows[k][0]=0;

    return *this;
}

bool Monome::operator==(const Monome& monome)
{
    if(coef!=monome.coef)
        return true;
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
            return false;
    }

    return true;
}

bool Monome::operator!=(const Monome& monome)
{
    if(coef!=monome.coef)
        return true;
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        if(vars_and_their_pows[i][0]!=monome.vars_and_their_pows[i][0] || vars_and_their_pows[i][1]!=monome.vars_and_their_pows[i][1])
            return true;
    }
    return false;
}

Monome Monome::operator=(const Monome& monome)
{
    coef=monome.coef;
    for(int i=0; i<ALPHABET_SIZE; i++)
    {
        vars_and_their_pows[i][0]=monome.vars_and_their_pows[i][0];
        vars_and_their_pows[i][1]=monome.vars_and_their_pows[i][1];
    }
    return *this;
}

ostream& operator<<(ostream &out, const Monome &monome)
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
ostream& operator<<(ostream &out, const Monome *monome)
{
    if(monome->coef!=1)
    {
        if(monome->coef==-1)
            out << '-';
        else
            out << monome->coef;
    }
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if(monome->vars_and_their_pows[i][0]>0)
            out << monome->vars_and_their_pows[i][0];
        if(monome->vars_and_their_pows[i][1]!=1)
            out << '^' << monome->vars_and_their_pows[i][1];
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
}

class Polynome : public TeX_convertible
{
private:
    list<Monome> container;

    int readStr(char **str, char *buf)
    {
        for(; (**str)>39; (*str)++, buf++)
        {
            *buf=**str;
        }
        *buf='\0';

        if(**str=='\0')
            return 0;

        (*str)+=3;
        return 1;
    }

public:
    Polynome() = default;                           //Конструктор для пустого полинома
    Polynome(char* str)                             //Конструктор обычный char*
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
    Polynome(const Polynome &polynome)               //Конструктор копий
    {
        container=polynome.container;
    }
    ~Polynome()
    {
        container.clear();
    }

    int getCoef(const Monome &monome) const          //Метод для получения коэффициента монома
    {
        return monome.coef;
    }
    int isNULL()                                     //Метод для проверки на nullptr, используемый при обработке вектора полиномов (выражения с полиномами)
    {
        if(this==nullptr)
            return 1;
        else
            return 0;
    }

    string convert() const override
    {
        stringstream out;

        out << '(';
        for(const Monome &monome : container)
        {
            out << monome.convert() << ' ';
        }
        out << ')';

        string out_s=out.str();
        return out_s;
    }

    Polynome operator+(Polynome &polynome);
    Polynome operator+=(Polynome &polynome);
    Polynome operator-(Polynome &polynome);
    Polynome operator-=(Polynome &polynome);
    Polynome operator*(Polynome &polynome);
    Polynome operator*=(Polynome &polynome);
    Polynome operator/(char var);
    Polynome operator/=(char var);
    bool operator==(const Polynome &polynome);
    bool operator!=(const Polynome &polynome);
    Polynome operator=(const Polynome &polynome);
    friend ostream& operator<< (ostream &out, const Polynome &polynome);
    friend ostream& operator<< (ostream &out, const Polynome *polynome);
    friend istream& operator>> (istream &in, Polynome &polynome);
    friend istream& operator>> (istream &in, Polynome &polynome);
    friend int cleanAndEmptyCheck(Polynome *polynome);
    friend bool isHomogeneous (const Polynome &polynome);
    friend bool isHarmonic (Polynome polynome);
};

int cleanAndEmptyCheck(Polynome *polynome)                //Очистка контейнера с мономами от мономов с коэффициентом 0 и проверка полинома на пустоту
{
    Polynome temp;
    int not_empty;

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

Polynome Polynome::operator+(Polynome &polynome)
{
    Polynome result=*this;
    Monome temp;
    int monome_not_utilised;

    for(Monome &monome_b : polynome.container)
    {
        monome_not_utilised=1;
        for(Monome &monome_a : result.container)
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
            result.container.emplace_back(monome_b);
    }

    cleanAndEmptyCheck(&result);
    return result;
}

Polynome Polynome::operator+=(Polynome &polynome)
{
    Monome temp;
    int monome_not_utilised;

    for(Monome &monome_b : polynome.container)
    {
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
    Polynome result=*this;
    Monome temp;
    int monome_not_utilised;

    for(Monome &monome_b : polynome.container)
    {
        monome_b.coef*=-1;
        monome_not_utilised=1;
        for(Monome &monome_a : result.container)
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
            result.container.emplace_back(monome_b);
        }
    }

    cleanAndEmptyCheck(&result);
    return result;
}

Polynome Polynome::operator-=(Polynome &polynome)
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

Polynome Polynome::operator*(Polynome &polynome)
{
    Polynome result;

    for(Monome &monome_b : polynome.container)
    {
        for(Monome &monome_a : container)
        {
            result.container.emplace_back(monome_b*monome_a);
        }
    }

    return result;
}

Polynome Polynome::operator*=(Polynome &polynome)
{
    Polynome result;

    for(Monome &monome_b : polynome.container)
    {
        for(Monome &monome_a : result.container)
        {
            result.container.emplace_back(monome_b*monome_a);
        }
    }

    return *this;
}

Polynome Polynome::operator/(char var)
{
    Polynome result(*this);

    for(Monome &monome : result.container)
    {
        monome/=var;
    }

    cleanAndEmptyCheck(&result);
    return result;
}

Polynome Polynome::operator/=(char var)
{
    for(Monome &monome : container)
    {
        monome/=var;
    }

    cleanAndEmptyCheck(this);
    return *this;
}

bool Polynome::operator==(const Polynome& polynome)
{
    int equal;

    for(Monome monome_b : polynome.container)
    {
        equal=0;
        for(Monome &monome_a : container)
        {
            if(monome_b==monome_a)
            {
                equal = 1;
                break;
            }
        }
        if(!equal)
            return false;
    }

    return true;
}

bool Polynome::operator!=(const Polynome& polynome)
{
    int equal;

    for(Monome monome_b : polynome.container)
    {
        equal=0;
        for(Monome &monome_a : container)
        {
            if(monome_b==monome_a)
            {
                equal = 1;
                break;
            }
        }
        if(!equal)
            return true;
    }

    return false;
}

Polynome Polynome::operator=(const Polynome& polynome)
{
    container.clear();
    for(const Monome &monome : polynome.container)
    {
        container.push_back(monome);
    }

    return *this;
}

ostream& operator<< (ostream &out, const Polynome &polynome)
{
    out << '(';
    for(const Monome &monome : polynome.container)
    {
        if(polynome.getCoef(monome)>0)
            out << " +" << monome;
        else
            out << ' ' << monome;
    }
    out << ')';

    return out;
}
ostream& operator<< (ostream &out, const Polynome *polynome)
{
    out << '(';
    for(const Monome &monome : polynome->container)
    {
        if(polynome->getCoef(monome)>0)
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

bool isHarmonic(Polynome polynome)
{
    vector<Polynome> derivatives;
    Polynome temp_pol;
    Monome temp_mon, first;
    int i, j, length, not_found;
    string characters_in_monome;

    first=polynome.container.front();

    for(i = 0; i < ALPHABET_SIZE; i++)
    {
        if(first.vars_and_their_pows[i][0]!=0)
            characters_in_monome.push_back(first.vars_and_their_pows[i][0]);
    }

    length=characters_in_monome.length();
    for (Monome &monome : polynome.container)
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

Polynome *fillPolynome(ifstream &fp)
{
    Polynome *polynome = new Polynome;
    fp >> *polynome;

    return polynome;
}

void calculateExpression(Polynome &A, Polynome &B, string operation, ofstream &fp)     //Функция для обработки выражения с полиномами и записи результата в выходной файл
{
    try
    {
        if (!B.isNULL())
        {
            switch (operation[0])
            {
                case '+':
                    fp << A.convert() << " + " << B.convert() << " = " << (A + B).convert();
                    break;
                case '-':
                    fp << A.convert() << " - " << B.convert() << " = " << (A - B).convert();
                    break;
                case '*':
                    fp << A.convert() << " * " << B.convert() << " = " << (A * B).convert();
                    break;
                case '=':
                    fp << A.convert() << " == " << B.convert() << " = " << boolalpha << (A==B);
                    break;
                case '!':
                    fp << A.convert() << " != " << B.convert() << " = " << boolalpha << (A!=B);
                    break;
            }
        }
        else
        {
            switch (operation[0])
            {
                case 'h':
                    fp << A.convert() << " is Homogeneous" << " = " << boolalpha << isHomogeneous(A);
                    break;
                case 'g':
                    fp << A.convert() << " is Harmonic" << " = " << boolalpha << isHarmonic(A);
                    break;
                case '/':
                    fp << A.convert() << " / " << operation[1] << " = " << (A / operation[1]).convert();
                    break;
            }
        }
        fp << " \\\\";
    }
    catch(const char *exc)
    {
        cerr << "Erorr occured while processing operation \"" << operation << "\": " << exc;
    }
    fp << endl;
}

int main()
{
    ifstream ifp;
    ofstream ofp;
    ifp.open("input_file.txt");
    if(!ifp.is_open())
        cout<<"Failed to open input file.";
    ofp.open("output_file.tex");
    if(!ofp.is_open())
        cout<<"Failed to open output file.";

    vector<Polynome*> p_array;
    string operation;

    try
    {
        while (true)
        {
            p_array.push_back(fillPolynome(ifp));

            ifp >> operation;

            if(ifp.get()=='\n')
                p_array.push_back(nullptr);
            else
            {
                ifp.get();
                p_array.push_back(fillPolynome(ifp));
            }

            calculateExpression(*p_array[0], *p_array[1], operation, ofp);

            p_array.pop_back();
            p_array.pop_back();

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
