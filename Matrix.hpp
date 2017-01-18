#ifndef MATRIX_HPP
# define MATRIX_HPP

# include <iostream>
# include <iomanip>

/*
 *
 * 		m0		m4		m8		m12
 * 		m1		m5		m9		m13
 * 		m2		m6		m10		m14
 * 		m3		m7		m11		m15
 *
 */

class			Matrix
{
	public:
						Matrix();
						Matrix( float m00, float m01, float m02, float m03,
								float m04, float m05, float m06, float m07,
								float m08, float m09, float m10, float m11, 
								float m12, float m13, float m14, float m15 );

		Matrix&			identity( void );
		Matrix	 	    operator*( const Matrix& rhs ) const;
		Matrix&		    operator*=(const Matrix& rhs);
		float   	    operator[]( int index ) const;
    	float&  	   	operator[]( int index );

		float			*getMatrix( void );

	private:
		float			_matrix[16];
};

std::ostream&			operator<<( std::ostream& flux, Matrix const& a );


#endif	//	MATRIX_HPP
