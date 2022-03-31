#include <iostream>
#include <string>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual int brojElemenata() const = 0;
    virtual const TipVrijednosti& operator[] (const TipKljuca &k) const = 0;
    virtual TipVrijednosti& operator[] (const TipKljuca &k) = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    Mapa(const Mapa &m) = delete;
    Mapa& operator = (const Mapa &m) = delete;
};

template <typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>
{
    int tren_Br_Elem;
    int kap;
    TipVrijednosti *nizTV;
    TipKljuca *nizTK;
    TipVrijednosti tempTipVrijednosti;

    public:
    NizMapa(int tempKap = 10): kap(tempKap), tren_Br_Elem(0), tempTipVrijednosti(TipVrijednosti())
    {
        nizTV = new TipVrijednosti[tempKap];
        nizTK = new TipKljuca[tempKap];
    }

    ~NizMapa()
    {
        delete [] nizTV;
        delete [] nizTK;
    }

    int brojElemenata() const override
    {
        return tren_Br_Elem;
    }

    const TipVrijednosti& operator [] (const TipKljuca &k) const override;
    TipVrijednosti& operator [](const TipKljuca &k) override;

    void obrisi() override;
    /*{
        tren_Br_Elem = 0;
    }*/

    void obrisi(const TipKljuca &kljuc) override;
    NizMapa(const NizMapa &m);
    NizMapa& operator = (const NizMapa &m);
};

template <typename TipKljuca, typename TipVrijednosti>
NizMapa< TipKljuca, TipVrijednosti> & NizMapa<TipKljuca, TipVrijednosti> :: operator = (const NizMapa &m)
{
    if(this == &m)
    {
        return *this;
    }
    delete [] nizTK;
    delete [] nizTV;

    kap = m.kap;
    tren_Br_Elem = m.tren_Br_Elem;

    nizTK = new TipKljuca[kap];
    nizTV = new TipVrijednosti[kap];

    for(int i = 0; i<tren_Br_Elem; i++)
    {
        nizTK[i] = m.nizTK[i];
        nizTV[i] = m.nizTV[i];
    }
    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti& NizMapa<TipKljuca, TipVrijednosti> :: operator [] (const TipKljuca &k) const
{
    for(int i = 0; i<tren_Br_Elem; i++)
    {
        if(nizTK[i] == k)
            return nizTV[i];
    }
    return tempTipVrijednosti;
}

template <typename TipKljuca, typename TipVrijednosti>
NizMapa <TipKljuca, TipVrijednosti> :: NizMapa(const NizMapa &m)
{
    kap = m.kap;
    tren_Br_Elem = m.tren_Br_Elem;

    nizTK = new TipKljuca[kap];
    nizTV = new TipVrijednosti[kap];

    for(int i = 0; i < tren_Br_Elem; i++)
    {
        nizTK[i] = m.nizTK[i];
        nizTV[i] = m.nizTV[i];
    }
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& NizMapa<TipKljuca, TipVrijednosti> :: operator [](const TipKljuca& kljuc)
{
    for(int i=0; i<tren_Br_Elem; i++)
    {
        if(nizTK[i] == kljuc)
            return nizTV[i];
    }

    if(tren_Br_Elem == kap)
    {
        if(kap == 0)
            kap = 100;

        else
        kap *= 2;

        TipVrijednosti *tmpVrijednost = new TipVrijednosti[kap];
        TipKljuca *tmpKljuc = new TipKljuca[kap];

        for(int i=0; i<tren_Br_Elem; i++)
        {
            tmpVrijednost[i] = nizTV[i];
            tmpKljuc[i] = nizTK[i];
        }

        delete [] nizTV;
        delete [] nizTK;
        nizTV = tmpVrijednost;
        nizTK = tmpKljuc;
    }
    nizTV[tren_Br_Elem] = TipVrijednosti();
    nizTK[tren_Br_Elem++] = kljuc;

    return nizTV[tren_Br_Elem-1];
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti> :: obrisi()
{
    if(!nizTK)
        delete [] nizTK;

    if(!nizTV)
        delete [] nizTV;

    kap = tren_Br_Elem = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti> :: obrisi (const TipKljuca& kljuc)
{
    bool ima = false;

    for(int i = 0; i < tren_Br_Elem; i++)
    {
        if(nizTK[i] == kljuc)
        {
            for(int j = i; j < tren_Br_Elem - 1; j++)
            {
                nizTV[j] = nizTV[j+1];
                nizTK[j] = nizTK[j+1];
                ima = true;
            }
        }
    }
    if(!ima) throw "Nema Kljuca";
    tren_Br_Elem--;
}

int main() {
    //std::cout << "Pripremna Zadaca Za Vjezbu 7, Zadatak 1";

    NizMapa <std::string, std::vector<std::string>> m;
    std::vector<std::string> gradovi;

    gradovi.push_back("Sarajevo");
    gradovi.push_back("Mostar");
    gradovi.push_back("Banja Luka");
    gradovi.push_back("Tuzla");

    m["BiH"] = gradovi;
    cout << m["BiH"].size() << " ";
    m["BiH"]. push_back("Bihac");
    cout << m["BiH"][0] << " " << m["BiH"][3] << " " << m["BiH"].size();

    return 0;
}
