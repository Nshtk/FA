#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"

using namespace std;

class TeX_convertible
{
public:

};

class Matrix : public TeX_convertible
{
private:
    double **ptr;
    int size;

public:
    Matrix(int number) : size(number)
    {
        ptr=new double*[size];
        for(int i=0; i<size; i++)
            ptr[i]=new double[size];
    }
    Matrix(const Matrix &matrix) : Matrix(matrix.size)
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

    void printDetMatrix(ostream &out)
    {
        out << "\\begin{bmatrix}\n";
        int n=size-1;
        for(int i=0; i<size; i++)
        {
            for (int j = 0; j < size; j++)
                (j==n)? out << ptr[i][j] << "\\\\" : out << ptr[i][j] << " & ";
            out << '\n';
        }
        out << "\\end{bmatrix}\n";
    }

    int isEmpty()
    {
        if(this==nullptr)
            return 1;
        else
            return 0;
    }

    double* operator[](int index);
    Matrix operator+(Matrix matrix);
    Matrix operator-(Matrix matrix);
    Matrix operator*(Matrix matrix);
    string operator==(Matrix matrix);
    string operator!=(Matrix matrix);
    Matrix operator=(Matrix matrix);
    friend ostream& operator<< (ostream &out, const Matrix &matrix);
    friend ostream& operator<< (ostream &out, const Matrix *matrix);
    friend istream& operator>> (istream &in, const Matrix &matrix);
    friend double calcDet(Matrix matrix);
    friend Matrix calcTransp(Matrix matrix);
    friend double calcTrace(Matrix matrix);
    friend Matrix calcInverse(Matrix matrix);
    friend Matrix calcAdj(Matrix matrix);
    friend Matrix calcCofactor(Matrix A, int p, int q);
    friend Matrix *fillMatrix(ifstream &fp);
    friend Matrix calcExpo(Matrix matrix, int N);
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
Matrix Matrix::operator+(Matrix matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            ptr[i][j]+=matrix.ptr[i][j];
    return *this;
}
Matrix Matrix::operator-(Matrix matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            ptr[i][j]-=matrix.ptr[i][j];
    return *this;
}
Matrix Matrix::operator*(Matrix matrix)
{
    Matrix temp_matrix(*this);
    for (int i=0; i<size; i++)
    {
        for (int j=0; j<size; j++)
        {
            temp_matrix.ptr[i][j] = 0;
            for (int k=0; k<size; k++)
                temp_matrix.ptr[i][j] += ptr[i][k] * matrix.ptr[k][j];
        }
    }
    *this=temp_matrix;
    return *this;
}
string Matrix::operator==(Matrix matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(ptr[i][j] != matrix.ptr[i][j])
                return "false";
    return "true";
}
string Matrix::operator!=(Matrix matrix)
{
    for(int i=0; i<size; i++)
        for(int j=0; j<size; j++)
            if(ptr[i][j] == matrix.ptr[i][j])
                return "false";
    return "true";
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
istream& operator>>(istream &in, const Matrix &matrix)
{
    for(int i=0; i<matrix.size; i++)
        for(int j=0; j<matrix.size; j++)
            in >> matrix.ptr[i][j];

    return in;
}

Matrix *fillMatrix(ifstream &fp)
{
    int i, j, number, end_of_matrix=0;
    char c;
    vector<double> first_row;

    for(i=0; ; i++)
    {
        c=fp.peek();
        if(c<48 || c>57)
            throw "Values inside matrix aren't numbers.";

        fp >> number;

        first_row.push_back(number);

        if((c=fp.get())=='\n')
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

    int size_minus_one=matrix->size;
    if(end_of_matrix==0)
        for (i=1; i < matrix->size; i++)
            for (j=0; j < matrix->size; j++)
            {
                c=fp.peek();
                if(c<48 || c>57)
                    throw "Values inside matrix aren't numbers.";
                fp >> matrix->ptr[i][j];

                if(j!=matrix->size)
                    c=fp.get();
            }
    return matrix;
}

Matrix calcTransp(Matrix matrix)
{
    Matrix transpose(matrix.size);
    for (int i = 0; i < matrix.size; ++i)
        for (int j = 0; j < matrix.size; ++j) {
            transpose.ptr[j][i] = matrix.ptr[i][j];
        }
    return transpose;
}

double calcDet(Matrix matrix)
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

Matrix calcCofactor(Matrix A, int p, int q)
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

Matrix calcAdj(Matrix matrix)
{
    Matrix adj(matrix.size);
    if (matrix.size == 1)
    {
        adj.ptr[0][0] = 1;
        return adj;
    }
    int size=matrix.size;
    Matrix temp(size);
    int sign = 1;

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

Matrix calcInverse(Matrix matrix)
{
    double det=calcDet(matrix);
    if (det==0)
        throw "Singular matrix, unable to find inverse matrix.";

    int size=matrix.size;

    Matrix adj(size), inverse(size);
    adj=calcAdj(matrix);

    for (int i=0; i<matrix.size; i++)
        for (int j=0; j<size; j++)
            inverse.ptr[i][j] = adj.ptr[i][j]/det;

    return inverse;
}

double calcTrace(Matrix matrix)
{
    double trace=0;
    for (int i = 0; i < matrix.size; ++i)
        trace+=matrix.ptr[i][i];

    return trace;
}

Matrix calcExpo(Matrix matrix, int N)
{
    if(N < 1)
        throw "Can't calculate exponent while N is set to 0.";
    if(N == 1)
        return matrix;
    matrix * calcExpo(matrix, N-1);

    return matrix;
}

void calculateExpression(Matrix &A, Matrix &B, string operation, ofstream &fp)
{
    string res_of_cmp;
    try
    {
        if (!B.isEmpty())
        {
            switch (operation[0])
            {
                case '+':
                    fp << A << "+\n" << B << "=\n" << A + B;
                    break;
                case '-':
                    fp << A << "-\n" << B << "=\n" << A - B;
                    break;
                case '*':
                    fp << A << "*\n" << B << "=\n" << A * B;
                    break;
                case '=':
                    res_of_cmp = A==B;
                    fp << A << "==\n" << B << "=\n" << res_of_cmp;
                    break;
                case '!':
                    res_of_cmp = A!=B;
                    fp << A << "!=\n" << B << "=\n" << res_of_cmp;
                    break;

            }
        }
        else
        {
            switch (operation[0])
            {
                case 'd':
                    A.printDetMatrix(fp);
                    fp << "=\n" << calcDet(A);
                    break;
                case 'i':
                    fp << A << "^{-1}" << "=\n" << calcInverse(A);
                    break;
                case 't':
                    fp << A << "^{T}" << "=\n" << calcTransp(A);
                    break;
                case 's':
                    fp << "tr\n" << A << "=\n" << calcTrace(A);
                    break;
                case 'e':
                    fp << A << "^{E}" << "=\n" << calcExpo(A, 2);
                    break;
            }
        }
    }
    catch(const char *exc)
    {
        cerr << "Erorr occured while processing operation \"" << operation << "\": " << exc;
    }
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

            if (operation[0] == 'd' || operation[0] == 'r' || operation[0] == 'i' || operation[0] == 't' ||operation[0] == 's' || operation[0] == 'e')
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
        cerr << "Erorr occured while reading file: " << exc;
    }
    ofp << "\\]";

    ifp.close();
    ofp.close();

    return 0;
}

#pragma clang diagnostic pop