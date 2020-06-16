#include <cmath>
#include <iostream>
#include <vector>

const double EPS = 1e-6;
const int PRECISION = 20;

typedef std::vector<double> Column;
typedef std::vector<double> Row;
typedef std::vector<Row> Matrix;

using std::cout;
using std::endl;

struct Equation {
    Equation(const Matrix &a, const Column &b):
        a(a),
        b(b)
    {}

    Matrix a;
    Column b;
};

struct Position {
    Position(int column, int row):
        column(column),
        row(row)
    {}

    int column;
    int row;
};

Equation ReadEquation() {
    int size;
    std::cin >> size;
    Matrix a(size, std::vector <double> (size, 0.0));
    Column b(size, 0.0);
    for (int row = 0; row < size; ++row) {
        for (int column = 0; column < size; ++column)
            std::cin >> a[row][column];
        std::cin >> b[row];
    }
    return Equation(a, b);
}

Position SelectPivotElement(
  const Matrix &a, 
  std::vector <bool> &used_rows, 
  std::vector <bool> &used_columns) {
    // This algorithm selects the first free element.
    // You'll need to improve it to pass the problem.
    Position pivot_element(0, 0);
    //find a suitable left most pivot point
    while (used_rows[pivot_element.row]){
        ++pivot_element.row;
    }
    while (used_columns[pivot_element.column]){
        ++pivot_element.column;
    }
    while( pivot_element.row < a.size() ){
	int r = pivot_element.row;
	int c = pivot_element.column;
	if (a[r][c] != 0)
	{	    
	    break;
	}
	++pivot_element.row;
    }
    
    return pivot_element;
}

void SwapLines(Matrix &a, Column &b, std::vector <bool> &used_rows, Position &pivot_element) {
    std::swap(a[pivot_element.column], a[pivot_element.row]);
    std::swap(b[pivot_element.column], b[pivot_element.row]);
    std::swap(used_rows[pivot_element.column], used_rows[pivot_element.row]);
    pivot_element.row = pivot_element.column; //now row index is same as column index
}

void ProcessPivotElement(Matrix &a, Column &b, const Position &pivot_element) {
    // Write your code here
    //gaussian elimnation:
    int r = pivot_element.row;
    double pivot = a[r][r];
    //normalize current pivot to 1 along with its row
    for( size_t c = r; c < a.size(); ++c )
    {
	a[r][c] /= pivot;
    }
    b[r] /= pivot;

    //substitute into other rows
    for( size_t i = 0; i < a.size(); ++i ) //for each row
    {
	if( i == r ) continue;
	double v = a[i][r]; //current value in selected row, this is also the multiplication factor since pivot element is 1
	for( size_t c = r; c < a.size(); ++c ) //for each column starting from r to end
	{
	    a[i][c] -= v * a[r][c];
	}
	b[i] -= v * b[r];
    }
}

void MarkPivotElementUsed(const Position &pivot_element, std::vector <bool> &used_rows, std::vector <bool> &used_columns) {
    used_rows[pivot_element.row] = true;
    used_columns[pivot_element.column] = true;
}

Column SolveEquation(Equation equation) {
    Matrix &a = equation.a;
    Column &b = equation.b;
    int size = a.size();

    std::vector <bool> used_columns(size, false);
    std::vector <bool> used_rows(size, false);
    
    for (int step = 0; step < size; ++step) {
        Position pivot_element = SelectPivotElement(a, used_rows, used_columns);
        SwapLines(a, b, used_rows, pivot_element);
        ProcessPivotElement(a, b, pivot_element);
        MarkPivotElementUsed(pivot_element, used_rows, used_columns);

	// cout << "-----------" << endl;
	// for( auto & i : a ){
	//     for( auto & j : i ){
	// 	cout << j << " ";
	//     }
	//     cout << endl;
	// }
    }

    return b;
}

void PrintColumn(const Column &column) {
    int size = column.size();
    std::cout.precision(PRECISION);
    for (int row = 0; row < size; ++row)
        std::cout << column[row] << std::endl;
}

int main() {
    Equation equation = ReadEquation();
    Column solution = SolveEquation(equation);
    PrintColumn(solution);
    return 0;
}
