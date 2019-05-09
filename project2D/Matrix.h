#pragma once

// typedef a function pointer we'll be using in the map function
typedef float(*ModifyFunction)(float n);

class Matrix
{
public:
	/***
	 * @brief Makes a new matrix of size rows*cols
	 * @param rows Number of rows
	 * @param cols Number of columns
	 */
	Matrix(int rows, int cols);
	Matrix();
	~Matrix();

	// copy constructors
	Matrix(Matrix& mat);
	Matrix& operator=(Matrix const& mat);

	// move constructors
	Matrix(Matrix&& mat) noexcept;
	Matrix& operator=(Matrix&& mat) noexcept;

	/***
	 * @brief Allows indexing the matrix's values directly
	 * @param index Index of the desired row
	 * @return Array containing the row of the matrix
	 */
	float*  operator[](int index);

	/***
	 * @return The number of rows in the matrix
	 */
	int getRows() { return m_rowCount; }
	/***
	 * @return The number of columns in the matrix
	 */
	int getColumns() { return m_colCount; }
	/***
	 * @brief Gets the size of the matrix
	 * @param rows Pointer to the location to put the row value
	 * @param cols Pointer to the location to put the column value
	 */
	void getSize(int* rows, int* cols)const
	{
		*rows = m_rowCount; *cols = m_colCount;
	}

	/***
	 * @brief Returns a new matrix which is this matrix transposed
	 *          Meaning the rows are now the columns and the columns
	 *          are now the rows
	 * @return This matrix but transposed
	 */
	Matrix transposed();

	/***
	 * @brief Gets the dot product(? - or just called multiplication) of this
	 *          matrix and another matrix
	 * @param mat Other matrix to get the product of
	 * @return A new matrix containing the product of the multiplication
	 */
	Matrix product(Matrix& mat);

	/***
	 * @brief Applies a function to each value in the matrix
	 * @param func Pointer to the function to apply
	 */
	void map(ModifyFunction func);

	/***
	 * @brief Gives each element in the matrix a random value between -1 and 1
	 */
	void randomize();
	void mutate(float rate);

	// scalar operations
	/***
	 * @brief Adds a scalar value to each element of a matrix
	 * @param num Scalar value to add
	 * @return Resulting matrix after addition
	 */
	Matrix  operator+ (float num);
	/***
	 * @brief Adds a scalar value to each value of this matrix
	 * @param num Scalar value to add to this matrix
	 * @return Reference to this which has had the scalar value added to it
	 */
	Matrix& operator+=(float num);
	/***
	 * @brief Multiplies a scalar value by each element of a matrix
	 * @param mul Scalar value to multiply this matrix by
	 * @return Resulting matrix after multiplication
	 */
	Matrix  operator* (float mul);
	/***
	 * @brief Multiplies this matrix by a scalar value
	 * @param mul Scalar value to multiply this matrix by
	 * @return Reference to this after scalar multiplication
	 */
	Matrix& operator*=(float mul);

	// element-wise operations
	/***
	 * @brief Adds this matrix to another matrix and returns the result
	 * @param mat Other matrix to add this matrix to
	 * @return A matrix containing the result of the addition
	 */
	Matrix  operator+ (Matrix& mat);
	/***
	 * @brief Adds a matrix to this matrix and returns a reference to this
	 * @param mat Matrix to add to this matrix
	 * @return Reference to this which has been added to the other matrix
	 */
	Matrix& operator+=(Matrix& mat);
	/***
	 * @brief Subtracts a matrix from this matrix and returns it as a new one
	 * @param mat Other matrix to subtract from this matrix
	 * @return A matrix containing the result of the subtraction
	 */
	Matrix  operator- (Matrix& mat);
	/***
	 * @brief Subtracts a matrix from this matrix
	 * @param mat Other matric to subtract from this matrix
	 * @return Reference to this which has had the matrix subtracted from it
	 */
	Matrix& operator-=(Matrix& mat);
	/***
	 * @brief Multiplies this matrix by another matrix element-wise
	 *          Element-wise meaning multiplying each element by its
	 *          corresponding element
	 *          Other known as the Hadamard product
	 * @param mat Other matrix to multiply
	 * @return A matrix containing the result of the elemnt-wise multiplication
	 */
	Matrix  operator* (Matrix& mat);
	/***
	 * @brief Multiplies a matrix by this matrix element-wise
	 *          Otherwise known as the Hadamard product
	 * @param mat Other matrix to multiply by
	 * @return Reference to this which has been multiplied element-wise
	 */
	Matrix& operator*=(Matrix& mat);

	/***
	 * @brief Checks if this matrix is equal to another one.
	 * @param mat Matrix to test against
	 * @return Whether or not the matrices are equal
	 */
	bool operator==(Matrix& mat);
	/***
	 * @brief Checks if this matrix is about equal to another one.
	 *          Useful when accounting for float inaccuracies
	 * @param mat Matrix to test against
	 * @param err The amount of error allowed
	 * @return Whether or not the matrices are about equal
	 */
	bool equal(Matrix& mat, float err);

	/***
	 * @brief Gets the array in which values are stored
	 *          Shouldn't be used, use subscript operator instead
	 * @return 2D float array contining all values
	 */
	inline float** _getArray() const { return m_values; }

private:
	// values to keep track of the size
	int m_rowCount;
	int m_colCount;

	// the elements of the matrix
	float** m_values;
};