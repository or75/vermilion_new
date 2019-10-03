#pragma once

template <int XORSTART , int BUFLEN , int XREFKILLER>
class XorStr
{
private:
	XorStr();
public:
	char s[BUFLEN];
	XorStr( const char* xs );
};

template <int XORSTART , int BUFLEN , int XREFKILLER>
XorStr<XORSTART , BUFLEN , XREFKILLER>::XorStr( const char* xs )
{
	int xvalue = XORSTART;
	for ( int i = 0; i < ( BUFLEN - 1 ); i++ )
	{
		s[i] = xs[i - XREFKILLER] ^ xvalue;
		xvalue += 1;
		xvalue %= 256;
	}
	s[BUFLEN - 1] = 0;
}