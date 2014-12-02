#include "ial.h"

int kmp_search(string S, string W)
{
	int m = 0;
	int i = 0;
	int index = strGetLength(&W)-1;
	int T[index];
	for(int i = 0; i < index; i++)
	{
		T[i] = 0;
	}
	kmp_table(W, T);
	while((m+i) < strGetLength(&S))
	{
		if(W.str[i] == S.str[m + i])
		{
			if(i == strGetLength(&W) - 1)
			{
				return m;
			}
			i++;
		}
		else
		{
			if(T[i] > -1)
			{
				m = m + i - T[i];
				i = T[i];
			}
			else
			{
				i = 0;
				m++;
			}
		}
	}
	return -20;
}


void kmp_table(string W, int *T)
{
	int pos = 2;
	int cnd = 0;

	T[0] = -1;
	T[1] = 0;

	while(pos < strGetLength(&W))
	{
		if(W.str[pos-1] == W.str[cnd])
		{
			T[pos] = ++cnd;
			pos++;
		}
		else if(cnd > 0)
		{
			cnd = T[cnd];
		}
		else
		{
			T[pos++] = 0;
		}
	}
}

string merge_sort(string m)
{
	int length = strGetLength(&m);
	if(length <= 1)
	{
		return m;
	}
	string left, right;
	strInit(&left);
	strInit(&right);
	int middle = length/2;
	for(int i = 0; i < middle; i++)
	{
		strAddChar(&left, m.str[i]);
	}
	for(int i = middle; i < length; i++)
	{
		strAddChar(&right, m.str[i]);
	}
	left = merge_sort(left);
	right = merge_sort(right);

	return merge(left, right);
}

string merge(string left, string right)
{
	string result;
	strInit(&result);
	int x = 0, y = 0;
	int length_L = strGetLength(&left);
	int length_R = strGetLength(&right);
	while((length_L-x > 0) || (length_R-y > 0))
	{
		if((length_L-x > 0) && (length_R-y > 0))
		{
			if(left.str[x] <= right.str[y])
			{
				strAddChar(&result, left.str[x++]);
			}
			else
			{
				strAddChar(&result, right.str[y++]);
			}
		}
		else if(length_L-x > 0)
		{
			strAddChar(&result, left.str[x++]);
		}
		else if (length_R-y > 0)
		{
			strAddChar(&result, right.str[y++]);
		}
	}
	return result;
}