#include <iostream>

using namespace std;

template <typename Tip>
class Red
{
    struct Cvor
    {
        Tip element;
        Cvor *sljedeci;
        Cvor(const Tip &e, Cvor *s): element(e), sljedeci(s){}
    };

    int duzina;
    Cvor *pocetak, *kraj;

    public:
    Red(): duzina(0), pocetak(nullptr), kraj(nullptr){}
    Red(const Red &r);
    Red & operator= (const Red &r);
    ~Red() {brisi();}
    void brisi();
    void stavi(const Tip& el);
    Tip skini();
    Tip& celo() const;
    int brojElemenata() const
    {
        return duzina;
    }

};

template <typename Tip>
Tip& Red<Tip>::celo() const
{
    if(!pocetak) throw "Red je prazan";
    return pocetak -> element;
}

template <typename Tip>
Tip Red<Tip>::skini()
{
    if(!pocetak) throw "Red je prazan";
    Cvor *temp(pocetak);

    Tip x(pocetak -> element);
    pocetak = pocetak -> sljedeci;
    delete temp;
    duzina--;
    return x;
}

template <typename Tip>
void Red<Tip>::stavi(const Tip& el)
{
    if(!pocetak)
    {
        pocetak = kraj = new Cvor(el, nullptr);
        duzina++;
        return;
    }

    kraj -> sljedeci = new Cvor(el, nullptr);
    kraj = kraj -> sljedeci;
    duzina++;
}

template <typename Tip>
Red<Tip>& Red<Tip>:: operator= (const Red &r)
{
    if(this == &r) return *this;

    Cvor *temp(r.pocetak);
    Cvor *temp2(nullptr), *tempStari(nullptr);
    pocetak = nullptr;
    kraj = nullptr;
    duzina = 0;

    while(temp)
    {
        temp2 = new Cvor(temp -> element, nullptr);

        if(!pocetak)
        {
            pocetak = kraj = tempStari = temp2;
            temp = temp -> sljedeci;
            duzina++;
            continue;
        }
        tempStari -> sljedeci = temp2;
        tempStari = kraj = temp2;
        duzina++;
        temp = temp -> sljedeci;
    }
    return *this;
}

template <typename Tip>
Red<Tip>::Red(const Red &r)
{
    Cvor *temp(r.pocetak);
    Cvor *temp2(nullptr), *tempStari(nullptr);
    pocetak = nullptr;
    kraj = nullptr;
    duzina = 0;

    while(temp)
    {
        temp2 = new Cvor(temp -> element, nullptr);

        if(!pocetak)
        {
            pocetak = kraj = tempStari = temp2;
            temp = temp -> sljedeci;
            duzina++;
            continue;
        }

        tempStari -> sljedeci = temp2;
        tempStari = kraj = temp2;
        duzina++;
        temp = temp -> sljedeci;
    }
}

template <typename Tip>
void Red<Tip>::brisi()
{
    Cvor *temp(pocetak);

    while(pocetak)
    {
        if(pocetak == kraj)
        {
            delete pocetak;
            pocetak = nullptr;
            kraj = nullptr;
            duzina = 0;
            return;
        }
        pocetak = pocetak -> sljedeci;
        delete temp;
        temp = pocetak;
        duzina--;
    }
}

int main()
{
    int br_elemenata;
    Red<int> r;
    for(int i = 0; i < 5; i++) r.stavi(i);

    Red<int> r2 (r);
    Red<int> r3;

    r3 = r;

    br_elemenata = r3.brojElemenata();

    cout << br_elemenata << endl;

    r.skini();
    br_elemenata = r.brojElemenata();

    cout << br_elemenata << endl;

    br_elemenata = r2.brojElemenata();
    cout << "Red 2 ima: " << br_elemenata << " elemenata." << endl;

    r2.brisi();

    br_elemenata = r2.brojElemenata();

    cout << "Red 2 ima: " << br_elemenata << " nakon brisanja." << endl;



    return 0;
}
