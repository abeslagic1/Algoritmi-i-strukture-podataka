#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

//  Z1.

void radixSort(vector<int> &a)
{
    vector<vector<int>> tabela(10);
    int tempBroj(10);
    int max(a[0]);
    int index = 0;

    while(max/(tempBroj/10) != 0)
    {
        for(int i = 0; i < a.size(); i++)
        {
            if(a[i] > max)
                max = a[i];

            switch((a[i] / (tempBroj/10)) % 10)
            {
                case 0: tabela[0].push_back(a[i]);
                break;
                case 1: tabela[1].push_back(a[i]);
                break;
                case 2: tabela[2].push_back(a[i]);
                break;
                case 3: tabela[3].push_back(a[i]);
                break;
                case 4: tabela[4].push_back(a[i]);
                break;
                case 5: tabela[5].push_back(a[i]);
                break;
                case 6: tabela[6].push_back(a[i]);
                break;
                case 7: tabela[7].push_back(a[i]);
                break;
                case 8: tabela[8].push_back(a[i]);
                break;
                case 9: tabela[9].push_back(a[i]);
                break;

            }
        }

        tempBroj = tempBroj * 10;

        for(int i = 0; i < tabela.size(); i++)
        {
            for(int j = 0; j < tabela[i].size(); j++)
            {
                a[index++] = tabela[i][j];
            }
        }

        index = 0;

        for(int i = 0; i < 10; i++)
        {
            tabela[i].resize(0);
        }
    }
}


// Z2.

bool DaLiJeList(vector<int> &a, int i, int vel)
{
    return (i >= vel/2 && i < vel);
}

void PopD(vector<int> &a, int i, int vel)
{
    int veci(2*i+1);
    int dd(2*i+2);
    int temp;

    while(!DaLiJeList(a, i, vel))
    {
        if(dd < vel && a[dd] > a[veci])
            veci = dd;

        if(i >= vel)
            return;

        if(a[i] > a[veci])
            return;


        temp = a[i];
        a[i] = a[veci];
        a[veci] = temp;

        i = veci;
        veci = 2*i+1;
        dd = 2*i+2;
    }
}

void PopG(vector<int> &a, int i)
{
    int temp;

    while(i != 0 && a[i] > a[(i-1)/2])
    {
        temp = a[i];
        a[i] = a[(i-1)/2];
        a[(i-1)/2] = temp;
        i = (i-1)/2;
    }
}

void stvoriGomilu(vector<int> &a)
{
    for(int i = (a.size()-1); i >= 0; i--)
    {
        PopD(a, i, a.size());
    }
}


void umetniUGomilu(vector<int> &a, int umetni, int &velicina)
{
    if(velicina >= a.size())
        a.resize(a.size() + 1);

    a[velicina++] = umetni;
    PopG(a, velicina - 1);
}

int izbaciPrvi(vector<int> &a, int &velicina)
{
    if(velicina == 0)
        throw;

    int temp = a[0];

    a[0] = a[--velicina];
    a[velicina] = temp;

    if(velicina >= 0)
        PopD(a, 0, velicina);

    return a[velicina];
}

void gomilaSort(vector<int> &a)
{
    int temp;
    int vel = a.size();

    stvoriGomilu(a);

    for(int i = a.size() - 1; i >= 1; i--)
    {
        temp = a[0];
        a[0] = a[i];
        a[i] = temp;
        vel--;

        PopD(a, 0, vel);
    }
}


int main()
{
    vector<int> a;
    int n;

    cout << "Unesi elemente vektora: " << endl;

    while((cin >> n))
    {
        a.push_back(n);
    }

    cout << "Uneseni elementi vektora su: " << endl;
    for(int i = 0; i < a.size(); i++)
    {
        cout << a[i] << endl;
    }

    radixSort(a);

    cout << "Sortirani elementi pomocu radixSort-a su: " << endl;
    for(int i = 0; i < a.size(); i++)
    {
        cout << a[i] << endl;
    }

    int velicina = a.size();

    stvoriGomilu(a);
    umetniUGomilu(a, 100, velicina);
    cout << "prvi element je: " << endl <<
    izbaciPrvi(a, velicina) << endl;
    cout << endl;

    cout << "Elementi gomile su: " << endl;
    for(int i = 0; i < a.size(); i++)
    {
        cout << a[i] << endl;
    }


    //std::cout << "Zadaća 3, Zadatak 1";
    return 0;
}
