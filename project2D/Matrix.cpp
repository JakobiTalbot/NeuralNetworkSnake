#include "Matrix.h"
#include <math.h>
#include <random>
#include <time.h>

Matrix::Matrix(int rows, int cols)
	: m_rowCount(rows), m_colCount(cols)
{
	m_values = new float*[rows];
	srand((unsigned int)time(NULL));

	// initialize all elements to 0
	for (int i = 0; i < rows; ++i)
	{
		m_values[i] = new float[cols];
		for (int j = 0; j < cols; ++j)
			m_values[i][j] = 0.0f;
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < m_rowCount; ++i)
		delete[] m_values[i];
	if (m_rowCount > 0)
		delete[] m_values;
}

// Copy constructor
Matrix::Matrix(Matrix& mat)
{
	mat.getSize(&m_rowCount, &m_colCount);

	m_values = new float*[m_rowCount];
	for (int i = 0; i < m_rowCount; ++i)
	{
		m_values[i] = new float[m_colCount];
		for (int j = 0; j < m_colCount; ++j)
			m_values[i][j] = mat[i][j];
	}
}

// Copy assignment operator
Matrix& Matrix::operator=(Matrix const& mat)
{
	for (int i = 0; i < m_rowCount; ++i)
		delete[] m_values[i];
	if (m_rowCount > 0)
		delete[] m_values;

	mat.getSize(&m_rowCount, &m_colCount);

	float** matVals = mat._getArray();

	m_values = new float*[m_rowCount];
	for (int i = 0; i < m_rowCount; ++i)
	{
		m_values[i] = new float[m_colCount];
		for (int j = 0; j < m_colCount; ++j)
			m_values[i][j] = matVals[i][j];
	}

	return *this;
}

// Move constructor
Matrix::Matrix(Matrix&& mat) noexcept
{
	mat.getSize(&m_rowCount, &m_colCount);

	m_values = new float*[m_rowCount];
	for (int i = 0; i < m_rowCount; ++i)
	{
		m_values[i] = new float[m_colCount];
		for (int j = 0; j < m_colCount; ++j)
		{
			m_values[i][j] = mat[i][j];
			mat[i][j] = 0.0f;
		}
	}

}

// Move assignment operator
Matrix& Matrix::operator=(Matrix&& mat) noexcept
{
	for (int i = 0; i < m_rowCount; ++i)
		delete[] m_values[i];
	if (m_rowCount > 0)
		delete[] m_values;

	mat.getSize(&m_rowCount, &m_colCount);

	m_values = new float*[m_rowCount];
	for (int i = 0; i < m_rowCount; ++i)
	{
		m_values[i] = new float[m_colCount];
		for (int j = 0; j < m_colCount; ++j)
		{
			m_values[i][j] = mat[i][j];
			mat[i][j] = 0.0f;
		}
	}

	return *this;
}

float* Matrix::operator[](int index)
{
	return m_values[index];
}

Matrix Matrix::transposed()
{
	// transposing is just switching the columns and rows
	Matrix m(m_colCount, m_rowCount);
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[x][y] = m_values[y][x];
	return m;
}

Matrix Matrix::product(Matrix& mat)
{
	// matrices must match columns/rows
	if (mat.getRows() != m_colCount)
		return *this;

	Matrix m(m_rowCount, mat.getColumns());
	for (int i = 0; i < m.getRows(); ++i)
	{
		for (int j = 0; j < m.getColumns(); ++j)
		{
			float sum = 0.0f;
			for (int k = 0; k < m_colCount; ++k)
				sum += (m_values[i][k] * mat[k][j]);
			m[i][j] = sum;
		}
	}
	return m;
}

void Matrix::map(ModifyFunction func)
{
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] = func(m_values[y][x]);
}

Matrix Matrix::operator*(float mul)
{
	Matrix m(m_rowCount, m_colCount);

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[y][x] = m_values[y][x] * mul;

	return m;
}

Matrix& Matrix::operator*=(float mul)
{
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] *= mul;
	return *this;
}

Matrix Matrix::operator+(float num)
{
	Matrix m(m_rowCount, m_colCount);

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[y][x] = m_values[y][x] + num;

	return m;
}

Matrix& Matrix::operator+=(float num)
{
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] += num;
	return *this;
}

Matrix Matrix::operator+(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	Matrix m(m_rowCount, m_colCount);
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[y][x] = m_values[y][x] + mat[y][x];

	return m;
}

Matrix& Matrix::operator+=(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] += mat[y][x];

	return *this;
}

Matrix Matrix::operator-(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	Matrix m(m_rowCount, m_colCount);
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[y][x] = m_values[y][x] - mat[y][x];

	return m;
}

Matrix& Matrix::operator-=(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] -= mat[y][x];

	return *this;
}

Matrix Matrix::operator*(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	Matrix m(m_rowCount, m_colCount);
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m[y][x] = m_values[y][x] * mat[y][x];

	return m;
}

Matrix& Matrix::operator*=(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return *this;
	}

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] *= mat[y][x];

	return *this;
}

bool Matrix::operator==(Matrix& mat)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return false;
	}

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			if (m_values[y][x] != mat[y][x])
				return false;

	return true;
}

bool Matrix::equal(Matrix& mat, float err)
{
	if (mat.getRows() != m_rowCount
		|| mat.getColumns() != m_colCount)
	{
		// columns and rows don't match
		return false;
	}

	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			if (fabs(m_values[y][x] - mat[y][x]) > err)
				return false;

	return true;
}

void Matrix::randomize()
{
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			m_values[y][x] = ((rand() % 2000) - 1000.f) / 1000.f;
}

// default constructor which should never be used
Matrix::Matrix()
{
	m_rowCount = -1;
	m_colCount = -1;
}

void Matrix::mutate(float rate)
{
	for (int y = 0; y < m_rowCount; ++y)
		for (int x = 0; x < m_colCount; ++x)
			if ((rand() % 1000) / 1000.f <= rate)
				m_values[y][x] = ((rand() % 2000) - 1000.f) / 1000.f;
}