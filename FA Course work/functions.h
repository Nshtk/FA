#ifndef FA_KURS_FUNCTIONS_H
#define FA_KURS_FUNCTIONS_H

int foundDuplicate(int8_t *arr, int i, int number)
{
    i++;
    for(int j=0; j<i; j++)
        if(number==arr[j])
            return 1;
    arr[i-1]=number;
    return 0;
}

void getValues(int8_t arr[2][5])
{
    int i, min;
    for(i=0; i<5; i++)
        while(foundDuplicate(arr[0], i, rand() % 9)) {}
    for(i=0; i<5; i++)
    {
        if((min=(arr[0][i]+1)*10-10)==0)
            min=1;
        while(foundDuplicate(arr[1], i, rand() % 10+min)) {}
    }
}

#endif
