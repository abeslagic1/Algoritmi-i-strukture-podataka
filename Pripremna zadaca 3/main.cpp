#include <iostream>
#include <stdexcept>

using namespace std;

template<typename Tip>
class Stek
{
    protected:
    int kapacitet;
    int vrhSteka;
    Tip* niz;

    public:
    Stek();
    Stek(const Stek<Tip>& novistek);
    ~Stek()
    {
        delete[] niz;
        kapacitet = 0;
        vrhSteka = -1;
    }
    void brisi()
    {
        vrhSteka = vrhSteka - 1;
    }
    void stavi(const Tip& element);
    Tip skini();
    Tip& vrh() const
    {
        if(vrhSteka == -1) throw "Stek je prazan!";
        return niz[vrhSteka];
    }
    int brojElemenata() const
    {
        return vrhSteka + 1;
    }
    Stek& operator =(const Stek& novistek);
};

/*
template<typename Tip>
void Stek<Tip>::brisi()
{
    vrhSteka =- 1;
}*/

template<typename Tip>
Stek<Tip>::Stek(const Stek<Tip>& novistek)
{
    kapacitet = novistek.kapacitet;
    vrhSteka = novistek.vrhSteka;
    niz = new Tip[kapacitet];
    for(int i = 0; i < vrhSteka+1; i++)
    {
        niz[i] = novistek.niz[i];
    }
}

template<typename Tip>
Stek<Tip>::Stek()
{
    kapacitet = 10000;
    vrhSteka =-1;
    niz = new Tip[kapacitet];
}

template<typename Tip>
void Stek<Tip>::stavi(const Tip& element)
{
    if(vrhSteka == -1)
    {
        niz[0] = element;
        vrhSteka++;
    }
    else
    {
        niz[++vrhSteka] = element;
    }
}

template<typename Tip>
Tip Stek<Tip>::skini()
{
    if(vrhSteka == -1) throw "Stek je prazan!";
    int a = vrhSteka + 1;
    vrhSteka--;
    return a;
}

template<typename Tip>
Stek<Tip>& Stek<Tip>::operator= (const Stek<Tip>& novistek)
{
    if(kapacitet < novistek.kapacitet)
    {
        delete[] niz;
        niz = new Tip[novistek.kapacitet];
        for(int i = 0; i < novistek.vrhSteka + 1; i++)
        {
            niz[i] = novistek.niz[i];
        }
    }
    else
    {
        for(int i = 0; i < novistek.vrhSteka + 1; i++)
        {
            niz[i] = novistek.niz[i];
        }
    }
    kapacitet = novistek.kapacitet;
    vrhSteka = novistek.vrhSteka;
    return *this;
}

int main()
{
    Stek<int> si;
    si.stavi(1);
    si.stavi(3);
    si.stavi(5);
    si.stavi(7);

    Stek<double> sd;
    sd.stavi(2.2);
    sd.stavi(4.4);
    sd.stavi(6.6);
    sd.stavi(8.8);

    cout << "Trenutni element steka je: " << si.vrh() << endl;
    cout << "Broj elemenata na steku je: " << si.brojElemenata() << endl;

    si.skini();
    cout << "Trenutni element steka je: " << si.vrh() << endl;
    cout << "Broj elemenata na steku je: " << si.brojElemenata() << endl;

    si.skini();
    cout << "Trenutni element steka je: " << si.vrh() << endl;
    cout << "Broj elemenata na steku je: " << si.brojElemenata() << endl;

    cout << "Broj double elemenata na steku sd je: " << sd.brojElemenata() << endl;
    sd.brisi();
    cout << "Broj double elemenata na steku sd je: " << sd.brojElemenata() << endl;
    return 0;
}
