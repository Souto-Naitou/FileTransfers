#include "Phill.h"

unsigned int Phill::GetRandColor()
{
    unsigned int result = 0;

    int R = rand() % 0xff;
    int G = rand() % 0xff;
    int B = rand() % 0xff;
    result= R;
    result = (result << 8) + G;
    result = (result << 8) + B;
    result = (result << 8) + 0xff;

	return result;
}

void Phill::shuffle(int* arr, int len)
{
    for (int i = len - 1; i > 0; i--)
    {
        int j = rand() % i;
        int temp = arr[j];
        arr[j] = arr[i];
        arr[i] = temp;
    }
}