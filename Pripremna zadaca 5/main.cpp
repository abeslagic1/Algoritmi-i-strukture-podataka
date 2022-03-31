#include <iostream>


int fibPom(int n, int i, int prosli, int pretprosli)
{
    int trenutni = prosli + pretprosli;
    if(n == i)
    {
        return trenutni;
    }
    return fibPom(n, i+1, trenutni, prosli);
}

int fib2_0(int n, int nula = 0, int jedan = 1)
{
    if(n <= 1)
    {
        return 1;
    }
    return fibPom(n, 2, jedan, nula);
}

int nzd(int x, int y)
{
    if(y == 0)
    {
        return x;
    }
    return nzd(y, x%y);
}
int main()
{
    std::cout << nzd(5,12) << std::endl;
   std:: cout << fib2_0(20) << std::endl;
    return 0;
}
