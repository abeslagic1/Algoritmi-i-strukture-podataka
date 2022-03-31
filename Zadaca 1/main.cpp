#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Tip>
class Iterator;

template <typename Tip>
class Lista
{
    public:
    Lista(){}
    virtual ~Lista(){}
    virtual int brojElemenata() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci()= 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& element) = 0;
    virtual void dodajIza(const Tip& element) = 0;
    virtual Tip& operator[] (int i) = 0;
    virtual Tip operator[] (int i) const = 0;
    virtual int LDuzina() const = 0;
    virtual int DDuzina() const = 0;

    Lista(const Lista& ) = delete;
    Lista & operator= (const Lista&) = delete;
};

template <typename Tip>
class DvostrukaLista : public Lista<Tip>
{
    struct Cvor
    {
        Tip element;
        Cvor *sljedeciCvor;
        Cvor *prethodniCvor;
        Cvor(const Tip &el, Cvor *prethodni, Cvor *sljedeci = nullptr)
        {
            element = el;
            prethodniCvor = prethodni;
            sljedeciCvor = sljedeci;
        }

        Cvor(Cvor *prethodni = nullptr, Cvor *sljedeci = nullptr)
        {
            prethodniCvor = prethodni;
            sljedeciCvor = sljedeci;
        }
    };

    Cvor *trenutniCvor, *pocetakCvor, *krajCvor;

    int lduzina, dduzina;

    public:
    friend class Iterator<Tip>;

    DvostrukaLista()
    {
        pocetakCvor = trenutniCvor = new Cvor();
        krajCvor = new Cvor();

        pocetakCvor -> sljedeciCvor = krajCvor;
        krajCvor -> prethodniCvor = pocetakCvor;

        lduzina = 0;
        dduzina = 0;
    }

    ~DvostrukaLista();
    DvostrukaLista(const DvostrukaLista &l);
    DvostrukaLista & operator = (const DvostrukaLista &l);

    Tip & trenutni() override
    {
        return trenutniCvor -> sljedeciCvor -> element;
    }
    Tip trenutni() const
    {
        return trenutniCvor -> sljedeciCvor -> element;
    }
    int brojElemenata() const override
    {
        return dduzina + lduzina;
    }

    bool prethodni() override
    {
        if(dduzina + lduzina == 0) return false;
        if(lduzina == 0) return false;

        trenutniCvor = trenutniCvor -> prethodniCvor;
        dduzina++;
        lduzina--;

        return true;
    }
    bool sljedeci() override
    {
        if(dduzina + lduzina == 0) return false;
        if(dduzina == 0) return false;
        if(dduzina == 1) return false;

        trenutniCvor = trenutniCvor -> sljedeciCvor;
        dduzina--;
        lduzina++;

        return true;
    }
    void pocetak() override
    {
        trenutniCvor = pocetakCvor;
        dduzina = dduzina + lduzina;
        lduzina = 0;
    }
    void kraj() override
    {
        trenutniCvor = krajCvor -> prethodniCvor -> prethodniCvor;
        lduzina = lduzina + dduzina;
        dduzina = 0;
    }
    void obrisi() override;
    void dodajIspred(const Tip& el) override;
    void dodajIza(const Tip& el) override;

    Tip& operator[] (int i) override;
    Tip operator[] (int i) const;

    int LDuzina() const override
    {
        return lduzina;
    }
    int DDuzina() const override
    {
        return dduzina;
    }

};

template <typename Tip>
DvostrukaLista<Tip>::DvostrukaLista(const DvostrukaLista &l)
{
    Cvor *temp(l.pocetakCvor -> sljedeciCvor), *temp2(nullptr);

    pocetakCvor = nullptr;
    trenutniCvor = nullptr;
    krajCvor = nullptr;

    lduzina = l.lduzina;
    dduzina = l.dduzina;

    pocetakCvor = trenutniCvor = new Cvor();
    krajCvor = new Cvor();

    pocetakCvor -> sljedeciCvor = krajCvor;
    krajCvor -> prethodniCvor = pocetakCvor;

    Cvor *prethodniCvorTemp(pocetakCvor), *sljedeciCvorTemp(krajCvor), *zapamtiTrenutni(nullptr);

    while(temp)
    {
        temp2 = new Cvor(temp -> element, nullptr, nullptr);
        if(l.trenutniCvor == temp)
        {
            zapamtiTrenutni = temp2;
        }

        temp2 -> prethodniCvor = prethodniCvorTemp;
        temp2 -> sljedeciCvor = sljedeciCvorTemp;

        prethodniCvorTemp -> sljedeciCvor = temp2;
        prethodniCvorTemp -> sljedeciCvor -> prethodniCvor = temp2;

        prethodniCvorTemp = temp2;

        temp = temp -> sljedeciCvor;
    }
    trenutniCvor = zapamtiTrenutni;
}

template <typename Tip>
DvostrukaLista<Tip>:: ~DvostrukaLista()
{
    while(pocetakCvor)
    {
        trenutniCvor = pocetakCvor;
        pocetakCvor = pocetakCvor -> sljedeciCvor;
        delete trenutniCvor;
    }
    pocetakCvor = nullptr;
    trenutniCvor = nullptr;
    krajCvor = nullptr;
}

template <typename Tip>
Tip& DvostrukaLista<Tip>::operator [] (int i)
{
    if(i > (dduzina + lduzina) || i < 0) throw "Pristup izvan liste... ";
    if(dduzina + lduzina == 0) throw "Prazna lista... ";

    Cvor *temp(pocetakCvor -> sljedeciCvor);

    for(int j = 0; j < i; j++)
    {
        temp = temp -> sljedeciCvor;
    }
    return temp -> element;
}

template <typename Tip>
Tip DvostrukaLista<Tip>::operator[] (int i) const
{
    if(i > (dduzina + lduzina) || i < 0) throw "Pristup izvan liste.. ";

    Cvor *temp(pocetakCvor -> sljedeciCvor);

    for(int j = 0; j < i; j++)
    {
        temp = temp -> sljedeciCvor;
    }
    return temp -> element;
}

template <typename Tip>
DvostrukaLista<Tip> & DvostrukaLista<Tip>::operator= (const DvostrukaLista &l)
{
    if(this == &l) return *this;

    while(pocetakCvor)
    {
        trenutniCvor = pocetakCvor;
        pocetakCvor = pocetakCvor -> sljedeciCvor;
        delete trenutniCvor;
    }
    pocetakCvor =nullptr;
    trenutniCvor = nullptr;
    krajCvor = nullptr;

    Cvor *temp(l.pocetakCvor -> sljedeciCvor);
    Cvor *temp2(nullptr);

    pocetakCvor = nullptr;
    trenutniCvor = nullptr;
    krajCvor = nullptr;

    lduzina = l.lduzina;
    dduzina = l.dduzina;

    pocetakCvor = trenutniCvor = new Cvor();
    krajCvor = new Cvor();

    pocetakCvor -> sljedeciCvor = krajCvor;
    krajCvor -> prethodniCvor = pocetakCvor;

    Cvor *prethodniCvorTemp(pocetakCvor), *sljedeciCvorTemp(krajCvor);
    Cvor *zapamtiTrenutni(nullptr);

    while(temp)
    {
        temp2 = new Cvor(temp -> element, nullptr, nullptr);
        if(l.trenutniCvor == temp)
        {
            zapamtiTrenutni = temp2;
        }
        temp2 -> prethodniCvor = prethodniCvorTemp;
        temp2 -> sljedeciCvor = sljedeciCvorTemp;

        prethodniCvorTemp -> sljedeciCvor = temp2;
        prethodniCvorTemp -> sljedeciCvor -> prethodniCvor = temp2;
        prethodniCvorTemp = temp2;

        temp = temp -> sljedeciCvor;
    }
    trenutniCvor = zapamtiTrenutni;
    return *this;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIspred(const Tip &el)
{
    Cvor *temp(new Cvor(el, nullptr));

    if(dduzina + lduzina == 0)
    {
        pocetakCvor -> sljedeciCvor = temp;
        temp -> prethodniCvor = pocetakCvor;

        temp -> sljedeciCvor = krajCvor;
        krajCvor -> prethodniCvor = temp;
        dduzina++;
        return;
    }

    if(lduzina == 0 && dduzina != 0)
    {
        temp -> sljedeciCvor = pocetakCvor -> sljedeciCvor;
        temp -> prethodniCvor = pocetakCvor;

        pocetakCvor -> sljedeciCvor -> prethodniCvor = temp;
        pocetakCvor -> sljedeciCvor = temp;

        trenutniCvor = trenutniCvor -> sljedeciCvor;
    }

    if(dduzina != 0 && lduzina != 0)
    {
        temp -> sljedeciCvor = trenutniCvor -> sljedeciCvor;
        temp -> sljedeciCvor -> prethodniCvor = temp;

        trenutniCvor -> sljedeciCvor = temp;
        temp -> prethodniCvor = trenutniCvor;
        trenutniCvor = trenutniCvor -> sljedeciCvor;
    }
    lduzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::dodajIza (const Tip &el)
{
    Cvor *temp(new Cvor(el, nullptr));
    Cvor *pomocniTrenutni(trenutniCvor -> sljedeciCvor);

    if(dduzina + lduzina == 0)
    {
        pocetakCvor -> sljedeciCvor = temp;
        temp -> prethodniCvor = pocetakCvor;

        temp -> sljedeciCvor = krajCvor;
        krajCvor -> prethodniCvor = temp;
        dduzina++;
        return;
    }

    if(lduzina == 0 && dduzina != 0)
    {
        temp -> prethodniCvor = pomocniTrenutni;
        temp -> sljedeciCvor = pomocniTrenutni -> sljedeciCvor;

        pomocniTrenutni -> sljedeciCvor -> prethodniCvor = temp;
        pomocniTrenutni -> sljedeciCvor = temp;
    }

    if(dduzina != 0 && lduzina != 0)
    {
        temp -> prethodniCvor =pomocniTrenutni;
        temp -> sljedeciCvor = pomocniTrenutni -> sljedeciCvor;

        pomocniTrenutni -> sljedeciCvor -> prethodniCvor = temp;
        pomocniTrenutni -> sljedeciCvor = temp;
    }
    dduzina++;
}

template <typename Tip>
void DvostrukaLista<Tip>::obrisi()
{
    if(dduzina == 0) throw "Nema se sta obrisati.";
    Cvor *temp(trenutniCvor -> sljedeciCvor);

    trenutniCvor -> sljedeciCvor = temp -> sljedeciCvor;
    temp -> sljedeciCvor -> prethodniCvor = trenutniCvor;

    if(dduzina-1 == 0 && lduzina != 0)
    {
        trenutniCvor = krajCvor -> prethodniCvor -> prethodniCvor;
        dduzina++;
        lduzina--;
    }
    dduzina--;
    delete temp;
}

template <typename Tip>
class Iterator
{
    const DvostrukaLista<Tip> *tempDvostrukaLista;
    typename DvostrukaLista<Tip>::Cvor *tempTrenutni;
    int tempDDuzina, tempLDuzina;

    public:
    Iterator(const DvostrukaLista<Tip> &dl):
    tempDvostrukaLista(&dl), tempTrenutni(dl -> trenutniCvor), tempDDuzina(dl -> dduzina), tempLDuzina(dl ->lduzina) {}

    Iterator(const Lista<Tip> &l)
    {
        tempDvostrukaLista = (DvostrukaLista<Tip> *) &l;
        tempTrenutni = tempDvostrukaLista -> trenutniCvor;
        tempDDuzina = tempDvostrukaLista -> dduzina;
        tempLDuzina = tempDvostrukaLista -> lduzina;
    }

    bool sljedeci()
    {
        if(tempDDuzina + tempLDuzina == 0) return false;
        if(tempDDuzina == 1) return false;
        if(tempDDuzina == 0) return false;

        tempTrenutni = tempTrenutni -> sljedeciCvor;
        tempDDuzina--;
        tempLDuzina++;

        return true;
    }

    bool prethodni()
    {
        if(tempLDuzina == 0) return false;
        if(tempDDuzina + tempLDuzina == 0) return false;

        tempTrenutni = tempTrenutni -> prethodniCvor;
        tempDDuzina++;
        tempLDuzina--;

        return true;
    }

    void pocetak()
    {
        tempTrenutni = tempDvostrukaLista -> pocetakCvor;
        tempDDuzina += tempLDuzina;
        tempLDuzina = 0;
    }

    void kraj()
    {
        tempTrenutni = tempDvostrukaLista -> krajCvor;
        tempLDuzina += tempDDuzina;
        tempDDuzina = 0;
    }

    Tip& trenutni()
    {
        return tempTrenutni -> sljedeciCvor -> element;
    }
};

template <typename Tip>
Tip dajMaksimum(const Lista<Tip> &n)
{
    Iterator<Tip> it(n);

    Tip tempMax(it.trenutni());
    it.pocetak();
    for(int i = 0; i < n.brojElemenata(); i++)
    {
        if(it.trenutni() > tempMax)
        {
            tempMax = it.trenutni();
        }
        it.sljedeci();
    }
    return tempMax;
}


int main()
{
    Lista<int> * l;
    l = new DvostrukaLista<int>;
    for (int i=0; i< 40000; i++)
    {
        l -> dodajIspred(5);
    }

    l -> pocetak();
    l -> dodajIspred(4);
    cout << l -> trenutni() << endl;
    l -> prethodni();
    cout << l -> trenutni() << endl;

    l -> dodajIza(7);
    cout << l -> trenutni() << endl;
    l -> sljedeci();
    cout << l -> trenutni() << endl;

    cout << l -> brojElemenata() << endl;

    l -> obrisi();
    cout << l -> brojElemenata() << endl;

    l-> kraj();
    cout << l -> trenutni() << endl;

    l -> prethodni();
    l -> dodajIza(9);
    cout << (*l)[0] << endl;


    return 0;
}
