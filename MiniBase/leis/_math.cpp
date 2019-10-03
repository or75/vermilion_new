#include "_math.h"

#pragma warning (disable:4002)

float mysqrt( float x )
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x;
	i = 0x5f375a86 - ( i >> 1 );
	x = *(float*)&i;
	x = x*( 1.5f - xhalf*x*x );
	x = x*( 1.5f - xhalf*x*x );
	x = x*( 1.5f - xhalf*x*x );
	x = 1 / x;
	return x;
}

double myatan( double x , int n )
{
	double a = 0.0;// 1st term
	double sum = 0.0;
	if ( x == 1.0 ) return M_PI / 4.0;
	if ( x == -1.0 ) return -M_PI / 4.0;
	if ( n > 0 )
	{
		if ( ( x < -1.0 ) || ( x > 1.0 ) )
		{
			if ( x > 1.0 )
				sum = M_PI / 2.0;
			else
				sum = -M_PI / 2.0;
			a = -1.0 / x;
			for ( int j = 1; j <= n; j++ )
			{
				sum += a;
				a *= -1.0*( 2.0*j - 1 ) / ( ( 2.0*j + 1 )*x*x );
			}
		}
		else
		{
			sum = 0.0;
			a = x;
			for ( int j = 1; j <= n; j++ )
			{
				sum += a;
				a *= -1.0*( 2.0*j - 1 )*x*x / ( 2.0*j + 1 );
			}
		}
	}
	return sum;
}

double myatan2( double y , double x )
{
	double u = myatan( y / x , 10 );
	if ( x < 0.0 )
	{
		if ( u > 0.0 )
			u -= M_PI;
		else
			u += M_PI;
	}
	return u;
}

double mypow( double a , double b )
{
	double c = 1;
	for ( int i = 0; i<b; i++ )
		c *= a;
	return c;
}

double myfact( double x )
{
	double ret = 1;
	for ( int i = 1; i <= x; i++ )
		ret *= i;
	return ret;
}

double mysin( double x )
{
	double y = x;
	double s = -1;
	for ( int i = 3; i <= 100; i += 2 )
	{
		y += s*( mypow( x , i ) / myfact( i ) );
		s *= -1;
	}
	return y;
}

double mycos( double x )
{
	double y = 1;
	double s = -1;
	for ( int i = 2; i <= 100; i += 2 )
	{
		y += s*( mypow( x , i ) / myfact( i ) );
		s *= -1;
	}
	return y;
}

float myfabs( float x )
{
	if ( x < 0.f )
		return -x;
	else
		return x;
}