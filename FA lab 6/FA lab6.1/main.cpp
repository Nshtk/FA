#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include <iomanip>
#include <sstream>

#define EPS 0.000001

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace std;

class TeX_convertible
{
public:
    virtual string convert() const {}
};

class Matrix : public TeX_convertible
{
private:
    double **ptr;
    int size;

public:
    Matrix(int number) : size(number)									//Констурктор обычный int
    {
        ptr=new double*[size];
        for(int i=0; i<size; i++)
            ptr[i]=new double[size];
    }
    Matrix(const Matrix &matrix) : Matrix(matrix.size)					 //Констурктор копий
    {
        for(int i=0; i<size; i++)
            for(int j=0; j<size; j++)
                ptr[i][j]=matrix.ptr[i][j];
    }
    ~Matrix()
    {
        for(int i=0; i<size; i++)
            delete[] ptr[i];
        delete[] ptr;
    }

    string convert() const override
    {
        stringstream out;

        out << "\\begin{pmatrix}\n";
        int n=size-1;
        for(int i=0; i<size; i++)
        {
            for (int j = 0; j < size; j++)
                (j==n)? out << fixed << setprecision(2) << ptr[i][j] << "\\\\" : out << fixed << setprecision(2) << ptr[i][j] << " & ";
            out << '\n';
        }
        out << "\\end{pmatrix}\n";

        string out_s=out.str();
        return out_s;
    }

    double* operator[](int index);
    Matrix operator+(Matrix &matrix);
    Matrix operator+=(Matrix &matrix);
    Matrix operator-(Matrix &matrix);
    Matrix operator-=(Matrix &matrix);
    Matrix operator*(Matrix &matrix);
    Matrix operator*=(Matrix matrix);
    Matrix operator*(double number);
    bool operator==(Matrix const &matrix);
    bool operator!=(Matrix const &matrix);
    Matrix operator=(Matrix matrix);
    friend ostream& operator<< (ostream &out, const Matrix &matrix);
    friend ostream& operator<< (ostream &out, const Matrix *matrix);
    friend istream& operator>> (istream &in, const Matrix &matrix);
    friend double calcDet(Matrix &matrix);
    friend double calcTrace(Matrix &matrix);
    friend Matrix calcTransp(Matrix &matrix);
    friend Matrix calcInverse(Matrix &matrix);
    friend Matrix calcAdj(Matrix &matrix);
    friend Matrix calcCofactor(Matrix &A, int p, int q);
    friend Matrix fillMatrix(ifstream &fp);
    friend Matrix calcExpo(Matrix &matrix, int N);
    friend void calculateExpression(vector<Matrix> &operands, string operation, ofstream &fp);
};

double* Matrix::operator[](int index)
{
    if (index >= size)
    {
        cout << "\nMatrix indexing error.\n";
        exit(0);
    }
    return ptr[index];
}
Matrix Matrix::operator+(Matrix &matrix)
{
    Matrix result(*this);

    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            result.ptr[i][j]+=matrix.ptr[i][j];

    return result;
}
Matrix Matrix::operator+=(Matrix &matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            ptr[i][j]+=matrix.ptr[i][j];

    return *this;
}
Matrix Matrix::operator-(Matrix &matrix)
{
    Matrix result(*this);
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            result.ptr[i][j]-=matrix.ptr[i][j];

    return result;
}
Matrix Matrix::operator-=(Matrix &matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            ptr[i][j]-=matrix.ptr[i][j];

    return *this;
}
Matrix Matrix::operator*(Matrix &matrix)
{
    Matrix result(*this);
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
        {
            result.ptr[i][j] = 0;
            for (int k=0; k<size; k++)
                result.ptr[i][j] += ptr[i][k] * matrix.ptr[k][j];
        }
    }

    return result;
}
Matrix Matrix::operator*=(Matrix matrix)
{
    Matrix result(*this);
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
        {
            result.ptr[i][j] = 0;
            for (int k=0; k<size; k++)
                result.ptr[i][j] += ptr[i][k] * matrix.ptr[k][j];
        }
    }

    *this=result;
    return *this;
}
Matrix Matrix::operator*(double number)
{
    Matrix result(*this);
    for (int i=0; i<size; i++)
        for (int j=0; j<size; j++)
            result.ptr[i][j]*=number;

    return result;
}
bool Matrix::operator==(Matrix const &matrix)
{
    double temp;
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
        {
            temp=fabs(ptr[i][j] - matrix.ptr[i][j]);
            if(temp > EPS)
                return false;
        }
    return true;
}
bool Matrix::operator!=(Matrix const &matrix)
{
    double temp;
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            for(int j=0; j<size; j++)
            {
                temp=fabs(ptr[i][j] - matrix.ptr[i][j]);
                if(temp > EPS)
                    return true;
            }
    return false;
}
Matrix Matrix::operator=(Matrix matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            ptr[i][j]=matrix.ptr[i][j];
    return *this;
}
ostream& operator<< (ostream &out, const Matrix &matrix)
{
    out << "\\begin{pmatrix}\n";
    int n=matrix.size-1;
    for(int i=0; i<matrix.size; i++)
    {
        for (int j = 0; j < matrix.size; j++)
            (j==n)? out << matrix.ptr[i][j] << "\\\\" : out << matrix.ptr[i][j] << " & ";
        out << '\n';
    }
    out << "\\end{pmatrix}\n";
    return out;
}
ostream& operator<<(ostream &out, const Matrix *matrix)
{
    out << "\\begin{pmatrix}\n";
    int n=matrix->size-1;
    for(int i=0; i<matrix->size; i++)
    {
        for (int j = 0; j < matrix->size; j++)
            (j==n)? out << matrix->ptr[i][j] << "\\\\" : out << matrix->ptr[i][j] << " & ";
        out << '\n';
    }
    out << "\\end{pmatrix}\n";
    return out;
}
istream& operator>>(istream &in, const Matrix &matrix)			   //Перегруженный оператор для заполнения матрицы только из входного потока пользователя
{
    for(int i=0; i<matrix.size; i++)
        for(int j=0; j<matrix.size; j++)
            in >> matrix.ptr[i][j];

    return in;
}

Matrix fillMatrix(ifstream &fp)									//Функция для заполнения матрицы из файла
{
    vector<double> first_row;
    double number;
    int i, j, end_of_matrix=0;
    char c;

    for(i=0; ; i++)
    {
        c=fp.peek();
        if(c<'-' || c>'9')
            throw "Values inside matrix aren't numbers.";

        fp >> number;

        first_row.push_back(number);

        if(fp.get()=='\n')
            break;
        if (fp.eof())
        {
            end_of_matrix=1;
            break;
        }
    }
    i++;
    Matrix matrix(i);
    for(i=0, j=0; j<matrix.size; j++)
        matrix.ptr[i][j]=first_row[j];

    if(end_of_matrix==0)
        for (i=1; i < matrix.size; i++)
            for (j=0; j < matrix.size; j++)
            {
                c=fp.peek();
                if(c<'-' || c>'9')
                    throw "Values inside matrix aren't numbers.";
                fp >> matrix.ptr[i][j];

                if(j!=matrix.size)
                    fp.get();
            }
    return matrix;
}

Matrix calcTransp(Matrix &matrix)
{
    Matrix transpose(matrix.size);
    for (int i = 0; i < matrix.size; ++i)
        for (int j = 0; j < matrix.size; ++j) {
            transpose.ptr[j][i] = matrix.ptr[i][j];
        }
    return transpose;
}

double calcDet(Matrix &matrix)
{
    double det=0;
    int size=matrix.size;
    Matrix submatrix(size);

    if (size==2)
        return ((matrix.ptr[0][0] * matrix.ptr[1][1]) - (matrix.ptr[1][0] * matrix.ptr[0][1]));
    else {
        for (int k=0; k<size; k++) {
            int subi=0;
            for (int i=1; i<size; i++) {
                int subj=0;
                for (int j=0; j<size; j++) {
                    if (j==k)
                        continue;
                    submatrix.ptr[subi][subj] = matrix.ptr[i][j];
                    subj++;
                }
                subi++;
            }
            submatrix.size-=1;
            det=det + (pow(-1, k) * matrix.ptr[0][k] * calcDet(submatrix));
            submatrix.size+=1;
        }
    }
    return det;
}

Matrix calcCofactor(Matrix &A, int p, int q)
{
    int size=A.size;
    Matrix temp(A.size);
    int i = 0, j = 0;

    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (row != p && col != q)
            {
                temp.ptr[i][j++] = A.ptr[row][col];

                if (j == size - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
    return temp;
}

Matrix calcAdj(Matrix &matrix)
{
    Matrix adj(matrix.size);
    if (matrix.size == 1)
    {
        adj.ptr[0][0] = 1;
        return adj;
    }
    int size=matrix.size, sign;
    Matrix temp(size);

    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
        {
            temp=calcCofactor(matrix, i, j);
            sign = ((i+j)%2==0)? 1: -1;
            temp.size-=1;
            adj.ptr[j][i] = (sign)*(calcDet(temp));
            temp.size+=1;
        }
    }
    return adj;
}

Matrix calcInverse(Matrix &matrix)
{
    double det=calcDet(matrix);
    if (fabs(det) < EPS)
        throw "Singular matrix, unable to find inverse matrix.";

    int size=matrix.size;

    Matrix adj(size), inverse(size);
    adj=calcAdj(matrix);

    for (int i=0; i<matrix.size; i++)
        for (int j=0; j<size; j++)
            inverse.ptr[i][j] = adj.ptr[i][j]/det;

    return inverse;
}

double calcTrace(Matrix &matrix)
{
    double trace=0;
    for (int i = 0; i < matrix.size; ++i)
        trace+=matrix.ptr[i][i];

    return trace;
}

Matrix calcExpo(Matrix &matrix, int N)
{
    if(N < 1)
        throw "Can't calculate exponent while N is set to 0.";
    if(N == 1)
        return matrix;
    matrix *= calcExpo(matrix, N-1);

    return matrix;
}

void calculateExpression(vector<Matrix> &operands, string operation, ofstream &fp)		//Функция для обработки выражения с матрицами и записи результата в выходной файл
{
    try
    {
        if (operands.size()>1)
        {
            switch (operation[0])
            {
                case '+':
                    fp << operands[0].convert() << "+\n" << operands[1].convert() << "=\n" << (operands[0] + operands[1]).convert();
                    break;
                case '-':
                    fp << operands[0].convert() << "-\n" << operands[1].convert() << "=\n" << (operands[0] - operands[1]).convert();
                    break;
                case '*':                                                                                       //Здесь также умножение числа и на число
                    if(operands[0].size==1)
                        fp << operands[0].ptr[0][0] << "\n*\n" << operands[1].convert() << "=\n" << (operands[1]*operands[0].ptr[0][0]).convert();
                    else if(operands[1].size==1)
                        fp << operands[0].convert() << "*\n" << operands[1].ptr[0][0] << "\n=\n" << (operands[0]*operands[1].ptr[0][0]).convert();
                    else
                        fp << operands[0].convert() << "*\n" << operands[1].convert() << "=\n" << (operands[0] * operands[1]).convert();
                    break;
                case '=':
                    fp << operands[0].convert() << "==\n" << operands[1].convert() << "=\n" << boolalpha << (operands[0]==operands[1]);
                    break;
                case '!':
                    fp << operands[0].convert() << "!=\n" << operands[1].convert() << "=\n" << boolalpha << (operands[0]!=operands[1]);
                    break;
            }
            operands.pop_back();
            operands.pop_back();
        }
        else
        {
            switch (operation[0])
            {
                case 'd':
                    fp << operands[0].convert() << "=\n" << calcDet(operands[0]) << '\n';
                    break;
                case 's':
                    fp << "tr\n" << operands[0] << "=\n" << calcTrace(operands[0]);
                    break;
                case 't':
                    fp << operands[0].convert() << "^{T}\n" << "=\n" << calcTransp(operands[0]).convert();
                    break;
                case 'i':
                    fp << operands[0].convert() << "^{-1}\n" << "=\n" << calcInverse(operands[0]).convert();
                    break;
                case 'e':
                    fp << operands[0].convert() << "^{E}\n" << "=\n" << calcExpo(operands[0], operation[1]-'0').convert();
                    break;
            }
            operands.pop_back();
        }
    }
    catch(const char *exc)
    {
        cerr << "Error occured while processing operation \"" << operation << "\": " << exc;
    }
    fp << '\n';
}

int main()
{
    vector<Matrix> operands;
    string operation;

    ifstream ifp;
    ofstream ofp;
    ifp.open("input_file.txt");
    if(!ifp.is_open())
        cout<<"Failed to open input file";
    ofp.open("output_file.tex");
    if(!ofp.is_open())
        cout<<"Failed to open output file";

    ofp << "\\[\n";
    try
    {
        while (!ifp.eof())
        {
            operands.push_back(fillMatrix(ifp));

            if (ifp.eof())
                break;

            ifp >> operation;
            ifp.get();
            if(ifp.peek()!='\n')
                operands.push_back(fillMatrix(ifp));

            calculateExpression(operands, operation, ofp);

            ifp.get();
        }
    }
    catch(const char *exc)
    {
        cerr << "Error occured while reading file: " << exc;
    }
    ofp << "\\]";

    ifp.close();
    ofp.close();

    return 0;
}