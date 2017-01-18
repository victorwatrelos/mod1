# include "Matrix.hpp"

std::ostream&			operator<<(std::ostream& flux, Matrix const& m)
{
	flux << std::fixed << std::setprecision(5);
	flux << "Matrix:" << std::endl;
	flux << "[ " << std::setw(10) << m[0]  << " " << std::setw(10) << m[4]  << " " 
				 << std::setw(10) << m[8]  << " " << std::setw(10) << m[12] << " ]\n"
		 << "[ " << std::setw(10) << m[1]  << " " << std::setw(10) << m[5]  << " " 
		 		 << std::setw(10) << m[9]  << " " << std::setw(10) << m[13] << " ]\n"
		 << "[ " << std::setw(10) << m[2]  << " " << std::setw(10) << m[6]  << " "
				 << std::setw(10) << m[10] << " " << std::setw(10) << m[14] << " ]\n"
		 << "[ " << std::setw(10) << m[3]  << " " << std::setw(10) << m[7]  << " "
		 		 << std::setw(10) << m[11] << " " << std::setw(10) << m[15] << " ]\n";
	flux << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);
	return flux;
}
/*
Matrix					Matrix::operator*(const Matrix& n) const
{
	return Matrix(  _matrix[0]*n[0]  + _matrix[4]*n[1]  + _matrix[8]*n[2]   + _matrix[12]*n[3],
					_matrix[1]*n[0]  + _matrix[5]*n[1]  + _matrix[9]*n[2]   + _matrix[13]*n[3], 
					_matrix[2]*n[0]  + _matrix[6]*n[1]  + _matrix[10]*n[2]  + _matrix[14]*n[3], 
					_matrix[3]*n[0]  + _matrix[7]*n[1]  + _matrix[11]*n[2]  + _matrix[15]*n[3],
					_matrix[0]*n[4]  + _matrix[4]*n[5]  + _matrix[8]*n[6]   + _matrix[12]*n[7],
					_matrix[1]*n[4]  + _matrix[5]*n[5]  + _matrix[9]*n[6]   + _matrix[13]*n[7], 
					_matrix[2]*n[4]  + _matrix[6]*n[5]  + _matrix[10]*n[6]  + _matrix[14]*n[7], 
					_matrix[3]*n[4]  + _matrix[7]*n[5]  + _matrix[11]*n[6]  + _matrix[15]*n[7],
					_matrix[0]*n[8]  + _matrix[4]*n[9]  + _matrix[8]*n[10]  + _matrix[12]*n[11], 
					_matrix[1]*n[8]  + _matrix[5]*n[9]  + _matrix[9]*n[10]  + _matrix[13]*n[11],
					_matrix[2]*n[8]  + _matrix[6]*n[9]  + _matrix[10]*n[10] + _matrix[14]*n[11], 
					_matrix[3]*n[8]  + _matrix[7]*n[9]  + _matrix[11]*n[10] + _matrix[15]*n[11],
					_matrix[0]*n[12] + _matrix[4]*n[13] + _matrix[8]*n[14]  + _matrix[12]*n[15],
					_matrix[1]*n[12] + _matrix[5]*n[13] + _matrix[9]*n[14]  + _matrix[13]*n[15],
					_matrix[2]*n[12] + _matrix[6]*n[13] + _matrix[10]*n[14] + _matrix[14]*n[15],
					_matrix[3]*n[12] + _matrix[7]*n[13] + _matrix[11]*n[14] + _matrix[15]*n[15] );
}
*/
Matrix					Matrix::operator*(const Matrix& rhs) const
{
	return Matrix(  rhs[0]*_matrix[0]  + rhs[4]*_matrix[1]  + rhs[8]*_matrix[2]   + rhs[12]*_matrix[3],
					rhs[1]*_matrix[0]  + rhs[5]*_matrix[1]  + rhs[9]*_matrix[2]   + rhs[13]*_matrix[3], 
					rhs[2]*_matrix[0]  + rhs[6]*_matrix[1]  + rhs[10]*_matrix[2]  + rhs[14]*_matrix[3], 
					rhs[3]*_matrix[0]  + rhs[7]*_matrix[1]  + rhs[11]*_matrix[2]  + rhs[15]*_matrix[3],
					rhs[0]*_matrix[4]  + rhs[4]*_matrix[5]  + rhs[8]*_matrix[6]   + rhs[12]*_matrix[7],
					rhs[1]*_matrix[4]  + rhs[5]*_matrix[5]  + rhs[9]*_matrix[6]   + rhs[13]*_matrix[7], 
					rhs[2]*_matrix[4]  + rhs[6]*_matrix[5]  + rhs[10]*_matrix[6]  + rhs[14]*_matrix[7], 
					rhs[3]*_matrix[4]  + rhs[7]*_matrix[5]  + rhs[11]*_matrix[6]  + rhs[15]*_matrix[7],
					rhs[0]*_matrix[8]  + rhs[4]*_matrix[9]  + rhs[8]*_matrix[10]  + rhs[12]*_matrix[11], 
					rhs[1]*_matrix[8]  + rhs[5]*_matrix[9]  + rhs[9]*_matrix[10]  + rhs[13]*_matrix[11],
					rhs[2]*_matrix[8]  + rhs[6]*_matrix[9]  + rhs[10]*_matrix[10] + rhs[14]*_matrix[11], 
					rhs[3]*_matrix[8]  + rhs[7]*_matrix[9]  + rhs[11]*_matrix[10] + rhs[15]*_matrix[11],
					rhs[0]*_matrix[12] + rhs[4]*_matrix[13] + rhs[8]*_matrix[14]  + rhs[12]*_matrix[15],
					rhs[1]*_matrix[12] + rhs[5]*_matrix[13] + rhs[9]*_matrix[14]  + rhs[13]*_matrix[15],
					rhs[2]*_matrix[12] + rhs[6]*_matrix[13] + rhs[10]*_matrix[14] + rhs[14]*_matrix[15],
					rhs[3]*_matrix[12] + rhs[7]*_matrix[13] + rhs[11]*_matrix[14] + rhs[15]*_matrix[15] );
}

Matrix&    				Matrix::operator*=(const Matrix& rhs)
{
	*this = *this * rhs;
	return *this;
}

float					Matrix::operator[](int index) const
{
	    return _matrix[index];
}

float&					Matrix::operator[](int index)
{
	    return _matrix[index];
}

Matrix::Matrix()
{
	identity();
}

Matrix::Matrix( float m00, float m01, float m02, float m03,
				float m04, float m05, float m06, float m07,
				float m08, float m09, float m10, float m11, 
				float m12, float m13, float m14, float m15 )
{
	_matrix[0]  = m00;
	_matrix[1]  = m01;
	_matrix[2]  = m02;
	_matrix[3]  = m03;
	_matrix[4]  = m04;
	_matrix[5]  = m05;
	_matrix[6]  = m06;
	_matrix[7]  = m07;
	_matrix[8]  = m08;
	_matrix[9]  = m09;
	_matrix[10] = m10;
	_matrix[11] = m11;
	_matrix[12] = m12;
	_matrix[13] = m13;
	_matrix[14] = m14;
	_matrix[15] = m15;
}

Matrix&					Matrix::identity()
{
	_matrix[0]  = _matrix[5]  = _matrix[10] = _matrix[15] = 1.0f;
	_matrix[1]  = _matrix[2]  = _matrix[3]  = _matrix[4]  =
	_matrix[6]  = _matrix[7]  = _matrix[8]  = _matrix[9]  =
	_matrix[11] = _matrix[12] = _matrix[13] = _matrix[14] = 0.0f;
	return *this;
}

float					*Matrix::getMatrix()
{
	return _matrix;
}
