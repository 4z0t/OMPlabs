
#pragma once
#include <omp.h>
#include <iostream>
#include <stdint.h>
#include <assert.h>
#include <time.h>
using std::cout;
using std::endl;

const int LIMIT_ARR = 400;


void fill_array(int size, int* a)
{
	for (int i = 0; i < size; i++)
		a[i] = (int)rand();
}


int do_arrays(int size, int* a, int* b, int* c)
{
	int result = 1;
	int temp;
	for (int i = 0; i < size; i++)
	{
		if (a[i] & 1)
		{
			temp = (b[i] + a[i]);
		}
		else
		{
			if (c[i] != 0)
				temp = (b[i] / c[i]);
			else
				temp = 0;
		}
		if (temp != 0)
#pragma omp atomic
			result *= temp;
	}
	return result;
}
int do_arrays_parallel(int size, int* a, int* b, int* c)
{
	int result = 1;
	int temp;
#pragma omp parallel shared(a,b,c) private(temp) reduction(*:result)  if(size > LIMIT_ARR)
	{
#pragma omp for
		for (int i = 0; i < size; i++)
		{
			if (a[i] & 1)
			{
				temp = (b[i] + a[i]);
			}
			else
			{
				if (c[i] != 0)
					temp = (b[i] / c[i]);
				else
					temp = 0;
			}
			if (temp != 0)
				result = result * temp;
		}
	}
	return result;
}
int do_arrays_parallel_atomic(int size, int* a, int* b, int* c)
{
	int result = 1;
	int temp;
#pragma omp parallel shared(a,b,c,result) private(temp) if(size > LIMIT_ARR)
	{
#pragma omp for
		for (int i = 0; i < size; i++)
		{
			if (a[i] & 1)
			{
				temp = (b[i] + a[i]);
			}
			else
			{
				if (c[i] != 0)
					temp = (b[i] / c[i]);
				else
					temp = 0;
			}
			if (temp != 0)
				result = result * temp;
		}
	}
	return result;
}
int do_arrays_parallel_critical(int size, int* a, int* b, int* c)
{
	int result = 1;
	int temp;
#pragma omp parallel shared(a,b,c,result) private(temp)  if(size > LIMIT_ARR)
	{
#pragma omp for
		for (int i = 0; i < size; i++)
		{
			if (a[i] & 1)
			{
				temp = (b[i] + a[i]);
			}
			else
			{
				if (c[i] != 0)
					temp = (b[i] / c[i]);
				else
					temp = 0;
			}
			if (temp != 0)
#pragma omp critical
				result = result * temp;
		}
	}
	return result;
}
int do_arrays_parallel_sections_2(const int& size, int* a, int* b, int* c)
{
	int result2 = 1;
	int result1 = 1;
	int temp;
#pragma omp sections  private(temp) 
	{

#pragma omp section  
		{
			for (int i = 0; i < size / 2; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result2 = result2 * temp;
			}
		}
#pragma omp section  
		{
			for (int i = size / 2; i < size; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result1 = result1 * temp;
			}
		}
	}
	return result1 * result2;
}
int do_arrays_parallel_sections_4(const int& size, int* a, int* b, int* c)
{
	int result1 = 1;
	int result2 = 1;
	int result3 = 1;
	int result4 = 1;
	int temp;
#pragma omp sections  private(temp) 
	{

#pragma omp section  
		{
			for (int i = 0; i < size / 4; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result1 = result1 * temp;
			}
		}
#pragma omp section  
		{
			for (int i = size / 4; i < size / 2; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result2 = result2 * temp;
			}
		}
#pragma omp section  
		{
			for (int i = size / 2; i < size / 2 + size / 4; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result3 = result3 * temp;
			}
		}
#pragma omp section  
		{
			for (int i = size / 2 + size / 4; i < size; i++)
			{
				if (a[i] & 1)
				{
					temp = (b[i] + a[i]);
				}
				else
				{
					if (c[i] != 0)
						temp = (b[i] / c[i]);
					else
						temp = 0;
				}
				if (temp != 0)

					result4 = result4 * temp;
			}
		}
	}
	return result1 * result2 * result3 * result4;
}


double array_test(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays(n, a, b, c);
	double end = omp_get_wtime();

	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}
double array_test_parallel(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays_parallel(n, a, b, c);
	double end = omp_get_wtime();

	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}
double array_test_parallel_atomic(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays_parallel_atomic(n, a, b, c);
	double end = omp_get_wtime();


	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}
double array_test_parallel_critical(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays_parallel_critical(n, a, b, c);
	double end = omp_get_wtime();

	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}
double array_test_parallel_sections_2(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays_parallel_sections_2(n, a, b, c);
	double end = omp_get_wtime();

	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}
double array_test_parallel_sections_4(int n)
{

	int* a = new int[n] {};
	int* b = new int[n] {};
	int* c = new int[n] {};
	fill_array(n, a);
	fill_array(n, b);
	fill_array(n, c);

	double start = omp_get_wtime();
	auto res = do_arrays_parallel_sections_4(n, a, b, c);
	double end = omp_get_wtime();

	delete[]a;
	delete[]b;
	delete[]c;
	return (end - start);
}


uint32_t width_space = 15;
#define calc_time(n, t, r, func) srand(t);\
	r = func(n);\
	cout << r;\
	cout.fill(' ');\
	cout.width(width_space);
void array_tests(int n)
{
	time_t t = time(0);
	cout.fill(' ');
	cout << std::right;
	cout.width(width_space);
	cout << n << "\t\t";
	cout.fill(' ');
	cout << std::left;
	cout.width(width_space);
	double r;
	calc_time(n, t, r, array_test);
	calc_time(n, t, r, array_test_parallel);
	calc_time(n, t, r, array_test_parallel_atomic);
	calc_time(n, t, r, array_test_parallel_critical);
	calc_time(n, t, r, array_test_parallel_sections_2);
	calc_time(n, t, r, array_test_parallel_sections_4);
	cout << endl;
}


void array_tasks()
{
	int inc = 100000;
	for (int i = 0; i < 1000; i++)
	{
		array_tests(inc + i * inc);
	}
}
