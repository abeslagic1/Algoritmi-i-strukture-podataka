#include <iostream>
#include <stdexcept>

using namespace std;

template <typename Tip>
class Lista{

    public:
    Lista() {}
    virtual ~Lista() {}
    virtual int brojElemenata() const = 0;
    virtual Tip trenutni() const = 0;
    virtual Tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const Tip& el) = 0;
    virtual void dodajIza(const Tip& el) = 0;
    virtual Tip& operator[] (int n) = 0;
    virtual Tip operator[] (int n) const = 0;

};

template<typename Tip>
class NizLista : public Lista<Tip>
{
    protected:
    int velicina, kapacitet, sadasnji;
    Tip* niz;
    public:
    NizLista();
    NizLista(const NizLista<Tip>& lista)
    {
        velicina = lista.velicina;
        kapacitet = lista.kapacitet;
        sadasnji = lista.sadasnji;
        niz = new Tip[lista.velicina];
        for(int i=0; i<velicina; i++)
        {
            niz[i] = lista[i];
        }
    }
    ~NizLista()
    {
        delete[] niz;
        sadasnji = 0;
        velicina = 0;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip& trenutni()
    {
        return niz[sadasnji];
    }
    Tip trenutni() const
    {
        return niz[sadasnji];
    }
    bool prethodni()
    {
        if(sadasnji != 0)
        {
            sadasnji--;
            return true;
        }
        return false;
    }
    bool sljedeci()
    {
        if(sadasnji != velicina - 1)
        {
            sadasnji++;
            return true;
        }
        return false;
    }
    void pocetak()
    {
        sadasnji = 0;
    }
    void kraj()
    {
        sadasnji = velicina - 1;
    }
    void obrisi();
    void dodajIspred(const Tip& el);
    void dodajIza(const Tip& el);
    Tip& operator[] (int n)
    {
        if(n<0) throw "IZUZETAK";
        return niz[n];
    }
    Tip operator[] (int n) const
    {
        if(n<0) throw "IZUZETAK";
        return niz[n];
    }
    NizLista &operator=(const NizLista& n);

};

template<typename Tip>
NizLista<Tip>::NizLista()
{
    kapacitet = 100;
    velicina = 0;
    sadasnji = 0;
    niz = new Tip[kapacitet];
}

template<typename Tip>
void NizLista<Tip>::obrisi()
{
    if(velicina == 0) throw "IZUZETAK";
    for(int i = sadasnji; i < velicina; i++)
    {
        niz[i] = niz[i+1];
    }
    velicina--;
}

template<typename Tip>
NizLista<Tip>& NizLista<Tip>::operator= (const NizLista<Tip>& li)
{
    if(kapacitet < li.kapacitet)
    {
        delete[] niz;
        niz = new Tip[li.kapacitet];
        for(int i=0; i < li.kapacitet; i++)
        {
            niz[i] = li[i];
        }
    }
    else
    {
        for(int i = 0; i < li.kapacitet; i++)
        {
            niz[i] = li[i];
        }
    }
    sadasnji = li.sadasnji;
    velicina = li.velicina;
    kapacitet = li.kapacitet;
    return* this;
}

template<typename Tip>
void NizLista<Tip>::dodajIspred(const Tip& el)
{
    if(velicina == kapacitet)
    {
        kapacitet *= 2;
        Tip* niz2 = new Tip[kapacitet];
        for(int i = 0; i < velicina; i++)
        {
            niz2[i] = niz[i];
        }
        delete[] niz;
        niz = niz2;
    }
    if(velicina == 0)
    {
        niz[0] = el;
        velicina++;
    }
    else
    {
        velicina++;
        for(int i = velicina - 1; i > sadasnji; i--)
        {
            niz[i] = niz[i-1];
        }
        niz[sadasnji++] = el;

    }
}

template<typename Tip>
void NizLista<Tip>::dodajIza(const Tip& el)
{
    if(velicina == kapacitet)
    {
        kapacitet *= 2;
        Tip* niz2 = new Tip[kapacitet];
        for(int i = 0; i < velicina; i++)
        {
            niz2[i] = niz[i];
        }
        delete [] niz;
        niz = niz2;
    }
    if(velicina == 0)
    {
        niz[0] = el;
        velicina++;
    }
    else if(sadasnji + 1  == velicina)
    {
        velicina++;
        niz[velicina - 1] = el;
    }
    else
    {
        velicina++;
        for(int i = velicina - 1; i > sadasnji; i--)
        {
            niz[i] = niz[i-1];
        }
        niz[++sadasnji] = el;
        sadasnji--;
    }
}

// class Jednostruka Lista

template<typename Tip>
class JednostrukaLista : public Lista<Tip>
{
    private:
    struct Cvor
    {
        Tip element;
        Cvor* naredni;
    };
    int velicina;
    Cvor* prvi;
    Cvor* posljednji;
    Cvor* sadasnji;

    public:
    JednostrukaLista() : velicina(0), prvi(0), posljednji(0), sadasnji(0){}
    JednostrukaLista(const JednostrukaLista& l);
    ~JednostrukaLista()
    {
        Cvor* tmp = prvi;
        Cvor* tmp2;
        while(tmp != posljednji)
        {
            tmp2 = tmp;
            tmp = tmp->naredni;
            delete tmp2;
        }
        delete posljednji;
    }
    int brojElemenata() const
    {
        return velicina;
    }
    Tip& trenutni()
    {
        return sadasnji ->element;
    }
    Tip trenutni() const
    {
       return sadasnji -> element;
    }
    void pocetak()
    {
        if(velicina == 0) throw "IZUZETAK";
        sadasnji = prvi;
    }
    void kraj()
    {
        if(velicina == 0) throw "IZUZETAK";
        sadasnji = posljednji;
    }
    bool prethodni();
    bool sljedeci();
    void obrisi();
    void dodajIspred(const Tip& el);
    void dodajIza(const Tip& el);

    Tip& operator [] (int n)
    {
        Cvor* tmp = prvi;
        int brojac = 0;
        while(n != brojac)
        {
            tmp = tmp->naredni;
            brojac++;
        }
        return tmp->element;
    }
    Tip operator [](int n) const
    {
        Cvor* tmp = prvi;
        int brojac = 0;
        while(n != brojac)
        {
            tmp = tmp->naredni;
            brojac++;
        }
        return tmp->element;
    }
    JednostrukaLista& operator =(const JednostrukaLista& l);
};


template<typename Tip>
bool JednostrukaLista<Tip>::prethodni()
{
    if(velicina == 0) throw "IZUZETAK";
    if(sadasnji == prvi) return false;
    Cvor* tmp = prvi;
    while(tmp->naredni != sadasnji)
    {
        tmp = tmp->naredni;
    }
    sadasnji = tmp;
    return true;
}

template<typename Tip>
bool JednostrukaLista<Tip>::sljedeci()
{
    if(velicina == 0) throw "IZUZETAK";
    if(sadasnji == posljednji) return false;
    sadasnji = sadasnji->naredni;
    return true;
}


template<typename Tip>
JednostrukaLista<Tip>::JednostrukaLista (const JednostrukaLista& l)
{
    velicina = l.velicina;
    Cvor* tmp = new Cvor;
    tmp->element = l.prvi->element;
    Cvor* pom = l.prvi;
    prvi = tmp;
    posljednji = tmp;
    if(l.sadasnji == l.prvi)
    {
        sadasnji = tmp;
    }
    while(pom != l.posljednji)
    {
        tmp->naredni = new Cvor;
        tmp = tmp->naredni;
        if(pom->naredni == l.posljednji)
        {
            posljednji = tmp;
        }
        pom = pom->naredni;
        tmp->element = pom->element;
        if(pom == l.sadasnji)
        {
            sadasnji = tmp;
        }
    }
}

template<typename Tip>
void JednostrukaLista<Tip>::dodajIza(const Tip& el)
{
    Cvor* tmp = new Cvor;
    tmp->element = el;
    if(velicina == 0)
    {
        velicina++;
        prvi = posljednji = sadasnji = tmp;
    }
    else if(sadasnji == posljednji)
    {
        velicina++;
        sadasnji->naredni = tmp;
        posljednji = tmp;
    }
    else
    {
        velicina++;
        tmp->naredni = sadasnji->naredni;
        sadasnji->naredni = tmp;
    }
}


template<typename Tip>
void JednostrukaLista<Tip>::dodajIspred(const Tip& el)
{
    Cvor* tmp = new Cvor;
    tmp->element = el;
    if(velicina == 0)
    {
        velicina++;
        prvi = posljednji = sadasnji = tmp;
    }
    else if(sadasnji == prvi)
    {
        velicina++;
        tmp->naredni = prvi;
        prvi = tmp;
        posljednji = prvi->naredni;
    }
    else
    {
        velicina++;
        Cvor* pom = prvi;
        while(pom->naredni != sadasnji)
        {
            pom = pom->naredni;
        }
        pom->naredni = tmp;
        tmp->naredni = sadasnji;
    }
}

template<typename Tip>
void JednostrukaLista<Tip>::obrisi()
{
    if(velicina == 1)
    {
        delete sadasnji;
        velicina--;
        prvi = sadasnji = posljednji = nullptr;
    }
    else if(sadasnji == prvi)
    {
        prvi = prvi->naredni;
        delete sadasnji;
        sadasnji = prvi;
        velicina--;
    }
    else if(sadasnji == posljednji)
    {
        Cvor* tmp = prvi;
        while(tmp->naredni != posljednji)
        {
            tmp = tmp->naredni;
        }
        delete posljednji;
        posljednji = sadasnji = tmp;
        posljednji->naredni = tmp;
        velicina--;
    }
    else
    {
        Cvor* tmp = prvi;
        while(tmp->naredni != sadasnji)
        {
            tmp = tmp->naredni;
        }
        tmp->naredni = sadasnji-> naredni;
        delete sadasnji;
        sadasnji = tmp->naredni;
        velicina--;
    }
}


template<typename Tip>
JednostrukaLista<Tip>& JednostrukaLista<Tip>::operator = (const JednostrukaLista& l)
{
    while(velicina != 0) obrisi();
    Cvor* tmp = new Cvor;
    tmp->element = l.prvi->element;
    Cvor* pom = l.prvi;
    prvi = tmp;
    posljednji = tmp;
    if(l.sadasnji == l.prvi)
    {
        sadasnji == posljednji;
    }
    while(pom != l.posljednji)
    {
        tmp->naredni = new Cvor;
        tmp = tmp->naredni;
        if(pom->naredni == l.posljednji)
        {
            posljednji = tmp;
        }
        pom = pom->naredni;
        tmp->element = pom->element;
        if(pom == l.sadasnji)
        {
            sadasnji = tmp;
        }
    }
    velicina = l.velicina;
    return *this;
}

int main()
{
    //std::cout << "Pripremna Zadaća 2 2019/2020, Zadatak 1";
    JednostrukaLista<int> jlista;
    NizLista<int> nizlista;
    jlista.dodajIspred(1);
    nizlista.dodajIspred(1);
    cout << jlista.trenutni() << endl;
    cout << nizlista.trenutni() << endl;
    return 0;
}
