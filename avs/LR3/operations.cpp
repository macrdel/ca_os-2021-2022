#include "operations.h"

bool NEG(bool l)
{
	return !l;
}

bool AND(bool l, bool r)
{
	return (l && r);
}

bool OR(bool l, bool r)
{
	return (l || r);
}

bool EQ(bool l, bool r)
{
	return (l == r);
}

bool XOR(bool l, bool r)
{
	return l != r;
}

bool IMPL(bool l, bool r)
{
	return OR(!l, r);
}

bool SHEFF(bool l, bool r)
{
	return OR(!l, !r);
}

bool PIRS(bool l, bool r)
{
	return AND(!l, !r);
}

