#pragma once

#define M_PI 3.14159265358979323846

#define POW(x) ((x)*(x))

float mysqrt( float x );
double myatan( double x , int n );
double myatan2( double y , double x );
double mypow( double a , double b );
double myfact( double x );
double mysin( double x );
double mycos( double x );
float myfabs( float x );

#define VectorDistance(a,b) mysqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorSubtract(a,b,c) {(c)[0]=(a)[0]-(b)[0];(c)[1]=(a)[1]-(b)[1];(c)[2]=(a)[2]-(b)[2];}
#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}