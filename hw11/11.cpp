// 11.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <conio.h>
#include <stdio.h>

int detSign(int ch) {
    if (ch == '-')
    {
        return -1;
    }
    else if (ch == '+')
    {
        return 1;
    }
    return 0;
}

float getInput()
{
    
    int sign = 1;
    int ch = '0';
    int zero = '0';
    float inputNumber = 0;
    int place = 1;

    ch = _getche();

    // determine sign of input number 
    sign = detSign(ch);
    if (sign != 0) {
        ch = zero;
    }
    else {
        sign = 1;
    }

    // get integer part of input number 
    while (ch >= '0' && ch <= '9')
    {
        inputNumber = (inputNumber * 10) + (ch - zero);
        ch = _getche();
    }

    if (ch == '.') {
        // get fraction
        ch = zero;
        while (ch >= '0' && ch <= '9')
        {
            inputNumber += ((ch - zero) / (float)place);
            place *= 10;
            ch = _getche();
        }
        return (float)sign * inputNumber;
    }
    else {
        return (float)sign * inputNumber;
    }
}

void calcMulByASM(float vi1[], float vi2[], float vo2[])
{
    __declspec(align(16)) float vi1Copy[100];
    __declspec(align(16)) float vi2Copy[100];
    __declspec(align(16)) float vo2Copy[400];

    for (int i = 0; i < 100; i++)
    {
        vi1Copy[i] = vi1[i];
        vi2Copy[i] = vi2[i];
    }

    _asm
    {
        mov ecx, 0
        MUL_LOOP:
        movaps xmm2, oword ptr vi1Copy[ecx]
            movaps xmm0, oword ptr vi2Copy[ecx]
            mulps xmm2, xmm0
            movaps oword ptr vo2Copy[ecx], xmm2
            add ecx, 16
            cmp ecx, 400
            jnz MUL_LOOP
    }

   
    for (int i = 0; i < 100; i++)
    {
        vo2[i] = vo2Copy[i];
    }
}

int main()
{
    float numberA, numberB;
    // get first input(a)
    numberA = getInput();
    printf("\nnumber A:  %f\n", numberA);
    // get second input(b)
    numberB = getInput();
    printf("\nnumber B:  %f\n", numberB);
    // create a step vector :  (vectorOfSteps[i+1] - vectorOfSteps[i] = 0.1)
    float vectorOfSteps[100];
    int i = 0;
    for (float j = 0; i < 100; j = j + 0.1, i++)
    {
        vectorOfSteps[i] = j;
    }

    float vi1[100], vi2[100], vo1[100], vo2[100];

    // part cpp
    // vi1 = X + a
    // vi2 = X + b
    // vo1 = vi1 * vi2
    for (int i = 0; i < 100; i++)
    {
        vi1[i] = numberA + vectorOfSteps[i];
        vi2[i] = numberB + vectorOfSteps[i];
        vo1[i] = vi1[i] * vi2[i];
    }

    // calculate mul with assembly code 
    calcMulByASM(vi1, vi2, vo2);

    printf("\n");
    // print results 
    for (i = 0; i < 100; i++)
    {
        printf("row %i : \n", i + 1);
        printf("vi1 : %f  |  vi2 : %f  |  vo1 : %f  | vo2 : %f\n", vi1[i], vi2[i], vo1[i], vo2[i]);
        printf("\n");
    }
    return 0;
}

