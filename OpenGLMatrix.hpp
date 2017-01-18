#ifndef OPENGLMATRIX_HPP
# define OPENGLMATRIX_HPP

# include "Matrix.hpp"
# include <cmath>

class					OpenGLMatrix
{
	public:
						OpenGLMatrix();
		Matrix			getMatrix( void ) const;
		void			setMatrix(Matrix matrix);
		void			scale( float scale );
		void			rotateX( float alpha );
		void			rotateY( float alpha );
		void			rotateZ( float alpha );
		void			translate( float x, float y, float z );
		void			project( float fovy, float aspect, float near, float far );
		void			computeProjectionMatrix( float fovy, float aspect, float near, float far );
		float			*getMatrixArray( void );
		void			resetMatrix( void );
		OpenGLMatrix&	multMatrix(const OpenGLMatrix& rhs);

	private:
		Matrix			createScaleMatrix( float scale ) const;
		Matrix			createRotXMatrix( float alpha ) const;
		Matrix			createRotYMatrix( float alpha ) const;
		Matrix			createRotZMatrix( float alpha ) const;
		Matrix			createTranslationMatrix( float x, float y, float z ) const;
		Matrix			createProjectionMatrix( float fovy, float aspect, float near, float far ) const;

	private:
		Matrix			_matrix;
};

std::ostream&			operator<<( std::ostream& flux,OpenGLMatrix const& a );

#endif
