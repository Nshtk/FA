#include <iostream>
#include <cstdint>
#include <vector>

using namespace std;

bool isZero(uint16_t divider)
{
    if(divider==0)
    {
        cerr << "\nDivision by zero spotted, aborting calculation\n";
        return true;
    }
    return false;
}
bool isOdd(uint16_t elem)
{
    uint8_t count=0;

    for(int i=sizeof(elem)*8-1; i>-1; i--)
        if((elem >> i) & 1)
            count++;

    return count%2;
}
uint8_t getDegree(uint16_t elem)
{
    for(int i=sizeof(elem)*8-1; i>-1; i--)
        if((elem >> i) & 1)
            return i;
}
uint16_t mod(uint16_t res, uint8_t divider, uint8_t deg_mod)
{
    if(isZero(divider))
        return 0;
    for(int i=sizeof(res)*8-1; i>0; i--)
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

void printPolynomial(uint16_t elem)
{
    for(int i=sizeof(elem)*8-1; i>-1; i--)
        if((elem >> i) & 1)
            printf("x^%d ", i);
    cout << '\n';
}
void printPolynomialVector(const vector<uint8_t> &irreducibles)
{
    for(uint8_t it : irreducibles)
    {
        cout << "x^8 ";
        printPolynomial(it);
    }
}

class GaloisField
{
public:
    uint8_t add(uint8_t elem_a, uint8_t elem_b)
    {
        return elem_a^elem_b;
    }
    uint8_t multiply(uint8_t elem_a, uint8_t elem_b, uint16_t modulo)
    {
        if(isZero(modulo))
            return 0;
        uint8_t deg_initial_res, deg_mod, res=0;
        uint16_t initial_res = 0;

        for(int i=7; i>0; i--)
            if((elem_b >> i) & 1)
                initial_res^=elem_a << i;

        deg_initial_res=getDegree(initial_res); deg_mod=getDegree(modulo);
        if(deg_initial_res<deg_mod)
            return initial_res;
        else
        {
            for(int i=15; i>0; i--)
            {
                if((initial_res >> i) & 1)
                {
                    if(deg_mod<i)
                        res^=(modulo << (i - deg_mod)) ^ (((1 << i) | 0));
                    else if(deg_mod==i)
                        res^=modulo^((1 << i) | 0);
                    else
                        res^=((1 << i) | 0);
                }
            }
            return res;
        }
    }
    uint8_t inverse(uint8_t elem, uint16_t modulo)
    {
        uint8_t res=1;

        for(int i=7; i>-1; i--)
        {
            if((elem >> i) & 1)
                res=multiply(res, elem, modulo);
            res=multiply(res, res, modulo);
        }
        return res;
    }
    vector<uint8_t> findIrreducibles()
    {
        vector<uint8_t> irreducibles;
        uint8_t divider=0, power, deg_mod;

        for(uint16_t poly=0b111111111; poly>254; poly--)
        {
            if(!(poly & 1))
                continue;

            power=getDegree(poly)/2;
            for(divider|=1 << power; divider<255; divider++)
            {
                deg_mod=getDegree(divider);
                if(!mod(poly, divider, deg_mod))
                    break;
                if(deg_mod==power)
                {
                    if(isOdd(poly))
                        irreducibles.push_back(poly);
                    break;
                }
            }
        }
        return irreducibles;
    }
};

int main()
{
    GaloisField fld;
    const uint16_t prime_poly=0b100011011;

    printPolynomial(fld.add(67, 0));
    printPolynomial(fld.multiply(21, 12, prime_poly));
    printPolynomial(fld.inverse(140, prime_poly));

    vector<uint8_t> irreducibles=fld.findIrreducibles();
    printPolynomialVector(irreducibles);

    return 0;
}
