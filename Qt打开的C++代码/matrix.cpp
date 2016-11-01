#include "matrix.h"
#include <iostream>
using namespace std;

Matrix::Matrix(int _row):row(_row), column(_row)
{
	if(row > 0)
	{
        num = new double*[row];
		for(int i = 0; i < row; i++)
            num[i] = new double[column];
		for(int i = 0; i < row; i++)
			for(int j = 0; j < column; j++)
				num[i][j] = 0;
	} else
	{
		cout << "Error" << endl;
		row = column = 0;
	}
}

Matrix::Matrix(int _row, int _column):row(_row), column(_column)
{
	if(row > 0 && column > 0)
	{
        num = new double*[row];
		for(int i = 0; i < row; i++)
            num[i] = new double[column];
		for(int i = 0; i < row; i++)
			for(int j = 0; j < column; j++)
				num[i][j] = 0;
	} else
	{
		cout << "Error" << endl;
		row = column = 0;
    }
}

Matrix::Matrix(int _row, int _column, double temp[]):row(_row), column(_column)
{
    if(row > 0 && column > 0)
    {
        num = new double*[row];
        for(int i = 0; i < row; i++)
            num[i] = new double[column];
        for(int i = 0; i < row; i++)
            for(int j = 0; j < column; j++)
                num[i][j] = temp[i*column + j];
    } else
    {
        cout << "Error" << endl;
        row = column = 0;
    }
}

Matrix::Matrix(const Matrix& _copy):row(_copy.row), column(_copy.column)
{
    num = new double*[row];
	for(int i = 0; i < row; i++)
        num[i] = new double[column];
	for(int i = 0; i < row; i++)
		for(int j = 0; j < column; j++)
			num[i][j] = _copy.num[i][j];
}

void Matrix::display() const
{
    cout << "----------" << endl;
	cout << row << " " << column << endl;
	for(int i = 0; i < row; i++)
	{
		for(int j = 0; j < column; j++)
			cout << num[i][j] << " ";
		cout << endl;
	}
    cout << "----------" << endl;
}

void Matrix::display_product() const
{
	display();
}

Matrix Matrix::multiply(const Matrix& multiplier) const
{
	if(multiplier.row != column)
	{
		cout << "Error" << endl;
		return *this;
	}
	Matrix tmp(row, multiplier.column);
	for(int i = 0; i < tmp.row; i++)
		for(int j = 0; j < tmp.column; j++)
			for(int k = 0; k < column; k++)
				tmp.num[i][j] += num[i][k] * multiplier.num[k][j];
	return tmp;
}

Matrix::~Matrix()
{
    for(int i = 0; i < row; i++)
        delete[] num[i];
    delete[] num;
}

double* Matrix::dump()
{
    double *content = new double[row*column];
    for(int i = 0; i < row; i++)
        for(int j = 0; j < column; j++)
            content[i*column + j] = num[i][j];
    return content;
}
