#ifndef MATRIX_H
#define MATRIX_H
#include <string>

class Matrix //矩阵类
{
private:
    double **num; // the array to save the matrix
	int row, column;
public:
	Matrix():row(0),column(0){} // default
	Matrix(int); // constructor for square
	Matrix(int, int); // constructor for rectangle
    Matrix(int, int, double[]);
	Matrix(const std::string&); // constructor for file loading
	Matrix(const Matrix&); // copy constructor
	void display() const; // display the matrix
	void display_product() const; 
	Matrix multiply(const Matrix&) const; // multiply the matrices
	void save_product(const std::string&) const; // save the matrix in a text file
	void load(const std::string&); // load the matrix from a text file
	~Matrix(); // destructor

    double* dump();
};

#endif //MATRIX_H
