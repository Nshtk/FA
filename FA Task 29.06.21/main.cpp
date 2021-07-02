#include <iostream>
#include <cstdint>

using namespace std;

bool oddCheck(uint16_t elem)
{
    uint8_t count = 0;

    for(int i=15; i>-1; i--)
        if((elem >> i) & 1)
            count++;

    return count%2;
}
uint8_t getDegree(uint16_t elem)
{
    for(int i=15; i>-1; i--)
        if((elem >> i) & 1)
            return i;
}
uint16_t mod(uint16_t res, uint8_t divider)                 
{
    uint8_t deg_mod = getDegree(divider);

    for(int i=15; i>0; i--)
    {
        if(deg_mod<i)
            res ^= (divider << (i - deg_mod)) ^ (((1 << i) | 0));
        else if(deg_mod==i)
            res^=divider^((1 << i) | 0);
        else
            res^=((1 << i) | 0);
    }
    return res;
}

class GaluaField
{
public:
    uint8_t elements[256];                                  // В массиве хранятся все элементы поля

    GaluaField()
    {
        for(int i=0; i<256; i++)
            elements[i]=i;
    }
    uint8_t add(uint8_t elem_a, uint8_t elem_b)
    {
        return elem_a^elem_b;
    }
    uint8_t multiply(uint8_t elem_a, uint8_t elem_b, const uint16_t modulo)
    {
        uint8_t deg_initial_res, deg_mod, res=0;
        uint16_t initial_res = 0;

        for(int i=7; i>0; i--)
            if((elem_b >> i) & 1)
                initial_res^=elem_a << i;
        deg_initial_res=getDegree(initial_res); deg_mod=getDegree(modulo);

        if(deg_initial_res<deg_mod)
            return initial_res;
        else                                                // Выше вынес алгоритм взятия модуля в отдельную функцию mod()
        {
            for(int i=15; i>0; i--)
            {
                if((initial_res >> i) & 1)
                {
                    if(deg_mod<i)
                        res ^= (modulo << (i - deg_mod)) ^ (((1 << i) | 0));
                    else if(deg_mod==i)
                        res^=modulo^((1 << i) | 0);
                    else
                        res^=((1 << i) | 0);
                }
            }
            return res;
        }
    }
    uint8_t inverse(uint8_t elem, const uint16_t modulo)
    {
        uint16_t res=1;
        for(int i=7; i>-1; i--)
        {
            if((elem >> i) & 1)
                res=multiply(res, elem, modulo);
            else
                res<<=1;
        }
        return res;
    }
    void findIrredusables()                                  // Функция выводит 60 полиномов вместо 30
    {
        uint8_t divider, power;

        for(uint16_t poly=0b111111111; poly>254; poly--)
        {
            if(!(poly & 1))                                  // Полиномы без x^0 сразу пропускаются
                continue;
            power=getDegree(poly)/2;
            for(divider|=1 << power; divider<255; divider++)
            {
                if(!mod(poly, divider))
                    break;
                if(getDegree(divider)==power)                // Если не удалось поделить полином на все полиномы от степени 8/2 до степени 7, то полином проверяется на нечетность
                {
                    if(oddCheck(poly))
                        print(poly);
                    break;
                }
            }
        }
    }
    void print(const uint16_t elem)
    {
        printf("%d: ", elem);
        for(int i=15; i>-1; i--)
            if((elem >> i) & 1)
                printf("x^%d ", i);
        cout << '\n';
    }
};

int main()
{
    GaluaField fld;
    const uint16_t prime_poly=0b100011011;

    //fld.print(fld.add(fld.elements[67], fld.elements[0]));
    fld.print(fld.multiply(fld.elements[7], fld.elements[150], prime_poly));
    //fld.print(fld.inverse(fld.elements[7], prime_poly));
    //fld.findIrredusables();

    return 0;
}
