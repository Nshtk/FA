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

    int isNULL()														 //Метод для проверки на nullptr, используемый при обработке вектора матриц (выражения с матрицами)
    {
        if(this==nullptr)
            return 1;
        else
            return 0;
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
    friend Matrix *fillMatrix(ifstream &fp);
    friend Matrix calcExpo(Matrix &matrix, int N);
    friend void calculateExpression(Matrix &A, Matrix &B, string operation, ofstream &fp);
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

Matrix *fillMatrix(ifstream &fp)									//Функция для заполнения матрицы из файла
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
    Matrix *matrix=new Matrix(i);
    for(i=0, j=0; j<matrix->size; j++)
        matrix->ptr[i][j]=first_row[j];

    if(end_of_matrix==0)
        for (i=1; i < matrix->size; i++)
            for (j=0; j < matrix->size; j++)
            {
                c=fp.peek();
                if(c<'-' || c>'9')
                    throw "Values inside matrix aren't numbers.";
                fp >> matrix->ptr[i][j];

                if(j!=matrix->size)
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

void calculateExpression(Matrix &A, Matrix &B, string operation, ofstream &fp)		//Функция для обработки выражения с матрицами и записи результата в выходной файл
{
    try
    {
        if (!B.isNULL())
        {
            switch (operation[0])
            {
                case '+':
                    fp << A.convert() << "+\n" << B.convert() << "=\n" << (A + B).convert();
                    break;
                case '-':
                    fp << A.convert() << "-\n" << B.convert() << "=\n" << (A - B).convert();
                    break;
                case '*':                                                                                       //Здесь также умножение числа и на число
                    if(A.size==1)
                        fp << A.ptr[0][0] << "\n*\n" << B.convert() << "=\n" << (B*A.ptr[0][0]).convert();
                    else if(B.size==1)
                        fp << A.convert() << "*\n" << B.ptr[0][0] << "\n=\n" << (A*B.ptr[0][0]).convert();
                    else
                        fp << A.convert() << "*\n" << B.convert() << "=\n" << (A * B).convert();
                    break;
                case '=':
                    fp << A.convert() << "==\n" << B.convert() << "=\n" << boolalpha << (A==B);
                    break;
                case '!':
                    fp << A.convert() << "!=\n" << B.convert() << "=\n" << boolalpha << (A!=B);
                    break;
            }
        }
        else
        {
            switch (operation[0])
            {
                case 'd':
                    fp << A.convert() << "=\n" << calcDet(A) << '\n';
                    break;
                case 's':
                    fp << "tr\n" << A << "=\n" << calcTrace(A);
                    break;
                case 't':
                    fp << A.convert() << "^{T}\n" << "=\n" << calcTransp(A).convert();
                    break;
                case 'i':
                    fp << A.convert() << "^{-1}\n" << "=\n" << calcInverse(A).convert();
                    break;
                case 'e':
                    fp << A.convert() << "^{E}\n" << "=\n" << calcExpo(A, operation[1]-'0').convert();
                    break;
            }
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
    vector<Matrix*> m_array;
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
            m_array.push_back(fillMatrix(ifp));

            if (ifp.eof())
                break;

            ifp >> operation;
            ifp.get();
            if(ifp.peek()=='\n')
                m_array.push_back(nullptr);
            else
                m_array.push_back(fillMatrix(ifp));

            calculateExpression(*m_array[0], *m_array[1], operation, ofp);
            m_array.pop_back();
            m_array.pop_back();

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