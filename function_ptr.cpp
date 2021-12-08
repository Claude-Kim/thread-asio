#include <iostream>

using func_ptr = int(*)(int,int);

template<typename T>
T some_func(T a, T b)
{
    return a+b;
}

void process_data(func_ptr func, int a, int b)
{
    std::cout << func(a, b) << std::endl;
}

template<typename T, typename T2>
void process_dataT(T func, T2 a, T2 b)
{
    std::cout << func(a, b) << std::endl;
}

int main()
{
    process_data(some_func, 2, 3);
    process_dataT((func_ptr)some_func, 2, 3); //expilict type casting
}