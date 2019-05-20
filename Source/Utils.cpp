#include "Utils.hpp"
#include <stdlib.h>

void CmpAndAssignIfGreater(int &dst, int src)
{
	if (src > dst)
		dst = src;
}
void CmpAndAssignIfLess(int &dst, int src)
{
	if (src < dst)
		dst = src;
}

void AllocIntArrayAndFill(int *&arr, int sz, int val)
{
	arr = (int*)malloc(sz * sizeof(int));
	for (int i = 0; i < sz; i++)
		arr[i] = val;
}

void InitRectBoundsArray(int *&arr, int sz)
{
	arr = (int*)malloc(sz * sizeof(int));
	int boundsInitValues[] = { INT_MAX, INT_MAX, INT_MIN, INT_MIN };

	for (int i = 0; i < sz; i++)
		arr[i] = boundsInitValues[i%4];
}