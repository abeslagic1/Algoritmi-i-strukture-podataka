#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

template <typename Tip>
void bubble_sort(Tip *niz, int vel)
{
    for(int i = (vel-1); i >= 1; i--)
    {
        for(int j = 1; j <= i; j++)
        {
            if(niz[j-1] > niz[j])
            {
                Tip temp (niz[j-1]);
                niz[j-1] = niz[j];
                niz[j] = temp;
            }
        }
    }
}

template <typename Tip>
int Particija(Tip *niz, int prvi, int zadnji)
{
    Tip pivot = niz[prvi];
    int p = prvi+1;
    while(p <= zadnji && niz[p] < pivot) p++;
    for(int i = (p+1); i <= zadnji; i++)
    {
        if(niz[i] < pivot)
        {
            Tip temp(niz[p]);
            niz[p] = niz[i];
            niz[i] = temp;
            p++;
        }
    }
    Tip temp = niz[prvi];
    niz[prvi] = niz[p-1];
    niz[p-1] = temp;
    return p-1;
}

template <typename Tip>
void QuickSort(Tip *niz, int prvi, int zadnji)
{
    if(prvi >= zadnji)
    {
        return;
    }
    int j = Particija(niz, prvi, zadnji);
    QuickSort(niz, prvi, j-1);
    QuickSort(niz, j+1, zadnji);
}

template <typename Tip>
void quick_sort(Tip *niz, int vel)
{
    QuickSort(niz, 0, vel-1);
}


template <typename Tip>
void selection_sort(Tip *niz, int vel)
{
    for(int i = 0; i <= vel-2; i++)
    {
        Tip min_el = niz[i];
        int p_min_el = i;
        for(int j = i+1; j <= vel-1; j++)
        {
            if(niz[j] < min_el)
            {
                min_el = niz[j];
                p_min_el = j;
            }
        }
        niz[p_min_el] = niz[i];
        niz[i] = min_el;
    }
}

template <typename Tip>
void merge_pom(Tip *niz, int l, int p, int q, int u)
{
    int i = 0;
    int j = q-l;
    int k = l;
    Tip *niz2;
    niz2 = new Tip[u+1];

    for(int m = 0; m <= u-l; m++)
    {
        niz2[m] = niz[l+m];
    }
    while(i <= p-l && j <= u-l)
    {
        if(niz2[i] < niz2[j])
        {
            niz[k] = niz2[i];
            i = i + 1;
        }
        else
        {
            niz[k] = niz2[j];
            j = j + 1;
        }
        k = k + 1;
    }

    while(i <= p-l)
    {
        niz[k] = niz2[i];
        k = k + 1;
        i = i + 1;
    }
    while(j <= u-l)
    {
        niz[k] = niz2[j];
        k = k + 1;
        j = j + 1;
    }

    delete[] niz2;
}

template <typename Tip>
void merge_sort_pom(Tip *niz, int l, int u)
{
    if(u > l)
    {
        int p = (l + u - 1)/2;
        int q = p + 1;
        merge_sort_pom(niz, l, p);
        merge_sort_pom(niz, q, u);
        merge_pom(niz, l, p, q, u);
    }
}

template <typename Tip>
void merge_sort(Tip *niz, int vel)
{
    int l = 0;
    int u = vel - 1;
    merge_sort_pom(niz, l, u);
}

void ucitaj(string filename, int* &niz, int &vel)
{
    int broj;
    int brojac = 0;
    niz = new int[1000];
    char temp_znak;
    ifstream ulaz(filename);

    if(ulaz)
    {
        while(ulaz.eof() && brojac < 1000)
        {
            ulaz >> broj >> temp_znak;
            niz[brojac++] = broj;
        }
    }
    vel = brojac;
}

void generisi(string filename, int vel)
{
    ofstream izlaz(filename);

    if(izlaz)
    {
        for(int i = 0; i < vel; i++)
            izlaz << rand();
    }
    izlaz.close();
}
template <typename Tip>
bool DaLiProgramRadi(Tip *niz, int vel)
{
    for(int i = 0; i < vel -1; i++)
    {
        if(niz[i] > niz[i+1]) return false;
    }
    return true;
}

int main() {
    //std::cout << "Pripremna Zadaca za Vjezbu 6, Zadatak 1";
    int *niz(nullptr), n, vel(100);
    generisi("autotest.txt", vel);
    ucitaj("autotest.txt", niz, vel);

    cout << "Odaberite jedan od algoritama: " << endl;
    cout << "1. bubble sort" << endl;
    cout << "2. selection sort" << endl;
    cout << "3. quick sort" << endl;
    cout << "4. merge sort" << endl;

    cin >> n;

    if(n == 1) bubble_sort(niz, 100);
    else if(n == 2) selection_sort(niz, 100);
    else if(n == 3) quick_sort(niz, 100);
    else if(n == 4) merge_sort(niz, 100);
    else merge_sort(niz, 100);

    if(DaLiProgramRadi(niz, vel))
    {
        cout << "Program radi.";
    }
    else
    {
        cout << "Program ne radi.";
    }

    generisi("autotest2.txt", 100);

    return 0;
}
