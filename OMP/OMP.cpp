#include <omp.h>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <time.h>
using std::cout;
using std::endl;

const int LIMIT = 400;

//0302

class Matrix
{
public:
	Matrix();
	Matrix(size_t n);
	Matrix(size_t n, size_t m);
	~Matrix();

	uint32_t get(size_t i, size_t j)
	{
		assert(i < rows&& j < cols);
		return _data[i][j];
	}


	void set(size_t i, size_t j, const uint32_t& v)
	{
		assert(i < rows&& j < cols);
		_data[i][j] = v;
	}

	size_t get_rows() { return rows; }
	size_t get_cols() { return cols; }

	const uint32_t* const* get_data() { return _data; };
private:
	uint32_t** _data = nullptr;
	size_t rows, cols;

	void _alloc(size_t n, size_t m)
	{
		_data = new uint32_t * [n] {};
		for (size_t i = 0; i < m; i++)
		{
			_data[i] = new uint32_t[m]{};
		}
		rows = n;
		cols = m;
	}

	void _free()
	{
		assert(_data != nullptr);
		for (size_t i = 0; i < rows; i++)
		{
			delete[] _data[i];
		}
		delete[] _data;
		rows = 0;
		cols = 0;
	}


};

Matrix::Matrix() :rows(0), cols(0)
{
}

Matrix::Matrix(size_t n)
{
	_alloc(n, n);
}

Matrix::Matrix(size_t n, size_t m)
{
	_alloc(n, m);
}

Matrix::~Matrix()
{
	_free();
}

uint32_t count_bits(uint32_t v)
{
	uint32_t res = 0;
	for (size_t i = 0; i < sizeof(uint32_t) * 8; i++)
	{
		if ((v & (1 << i)))res++;
	}
	return res;
}

uint32_t count_row_pairs_parallel(const uint32_t* row, size_t size)
{
	uint32_t res = 0;
#pragma omp parallel shared(row) reduction(+:res) if (size > LIMIT)
	{
#pragma omp for 
		for (int i = 0; i < size; i++)
		{

			for (int j = i + 1; j < size; j++)
			{
				res = res + count_bits(row[i] * row[j]);
			}
		}
	}
	return res;
}


uint32_t count_row_pairs(const uint32_t* row, size_t size)
{
	uint32_t res = 0;


	for (size_t i = 0; i < size; i++)
		for (int j = i + 1; j < size; j++)
		{
			res += count_bits(row[i] * row[j]);
		}

	return res;
}

uint32_t count_mat_parallel(Matrix* m)
{
	uint32_t res = 0;
	int mat_size = m->get_rows();
#pragma omp parallel shared(m) reduction(+:res) if(mat_size > LIMIT)
	{
#pragma omp for
		for (int i = 0; i < mat_size; i++)
		{
			res = res + count_row_pairs_parallel(m->get_data()[i], m->get_cols());
		}
	}
	return res;
}

uint32_t count_mat(Matrix* m)
{
	uint32_t res = 0;
	{
		res = 0;
		for (size_t i = 0; i < m->get_rows(); i++)
		{
			res += count_row_pairs(m->get_data()[i], m->get_cols());
		}
	}
	return res;
}

void fill_matrix(Matrix* m)
{
	{
		for (size_t i = 0; i < m->get_rows(); i++)

			for (size_t j = 0; j < m->get_cols(); j++)
			{
				m->set(i, j, (uint32_t)rand() + 1);
			}
	}

}



void run_test_parallel(uint32_t n1, uint32_t n2)
{
	Matrix m{ n1,n2 };

	fill_matrix(&m);
	double start = omp_get_wtime();
	auto res = count_mat_parallel(&m);
	double end = omp_get_wtime();

	cout << "parallel " << n1 << " " << n2 << " " << res << " for " << (end - start) << endl;

}

void run_test(uint32_t n1, uint32_t n2)
{
	Matrix m{ n1,n2 };

	fill_matrix(&m);
	double start = omp_get_wtime();
	auto res = count_mat(&m);
	double end = omp_get_wtime();

	cout << n1 << " " << n2 << " " << res << " for " << (end - start) << endl;

}

void run_both(uint32_t n1, uint32_t n2)
{
	time_t t = time(0);
	srand(t);
	run_test(n1, n2);
	srand(t);
	run_test_parallel(n1, n2);
}

void matrix_tasks()
{
	run_both(100, 100);
	run_both(200, 200);
	run_both(300, 300);
	run_both(400, 400);
	run_both(500, 500);
	run_both(600, 600);
	run_both(1000, 1000);
	run_both(2000, 2000);
	run_both(5000, 5000);
}


int do_arrays_parallel(int size, int* a, int* b, int* c)
{

}

int do_arrays(int size, int* a, int* b, int* c)
{

}

void fill_array(int size, int* a)
{
	for (int i = 0; i < size; i++)
		a[i] = (int)rand();
}

void array_test_parallel(int n)
{

}

void array_test(int n)
{

}

void array_tests(int n)
{
	time_t t = time(0);
	srand(t);
	array_test(n);
	srand(t);
	array_test_parallel(n);
}


void array_tasks()
{

}

int main()
{
	matrix_tasks();



}



