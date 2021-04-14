#pragma once
#include "Function.h"

class IntegralSolver
{
public:
    IntegralSolver(Function& func) : _func(func) { }

    double solve(double a, double b, int steps)
    {
        double result = 0;
        double step = (b - a) / steps;
#pragma omp parallel
        {
#pragma omp for reduction(+:result)
            for (int i = 0; i < steps; i++)
            {
                double x_left = a + i * step;
                double x_right = x_left + step;
                double func = _func(x_left);
                result += func * (x_right - x_left);
            }
        }
        return result;
    }

private:
    Function& _func;
};