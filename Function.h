#pragma once

class Function
{
    typedef double(*function_t)(double);

public:
    Function(function_t function)
    {
        _function = function;
    }

    double compute(double x)
    {
        return _function(x);
    }

    double operator()(double x)
    {
        return _function(x);
    }

private:
    function_t _function;
};