#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    Mapa() {}
    virtual ~Mapa(){}
    virtual int brojElemenata() const = 0;
    virtual TipVrijednosti& operator[](const TipKljuca &k) = 0;
    virtual const TipVrijednosti operator[](const TipKljuca &k) const = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual void obrisi() = 0;
    Mapa(const Mapa &m) = delete;
    Mapa& operator = (const Mapa &m) = delete;
};

template <typename TipKljuca, typename TipVrijednosti>
class BSCvor
{
    public:

    BSCvor *LDijete;
    BSCvor *DDijete;
    BSCvor *roditelj;

    TipKljuca kljuc;
    TipVrijednosti vrijednost;

    BSCvor()
    {
        LDijete = nullptr;
        DDijete = nullptr;
        roditelj = nullptr;

        kljuc = TipKljuca();
        vrijednost = TipVrijednosti();
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca, TipVrijednosti>
{
    BSCvor <TipKljuca, TipVrijednosti> *korijen;
    int brElemeUStablu;
    void PomObrisi(BSCvor <TipKljuca, TipVrijednosti> *tempKorijen);
    BSCvor <TipKljuca, TipVrijednosti> *PomKopiranje(const BSCvor <TipKljuca, TipVrijednosti> *tempKorijen);

    public:
    BinStabloMapa()
    {
        brElemeUStablu = 0;
        korijen = nullptr;
    }
    ~BinStabloMapa()
    {
        obrisi();
    }
    int brojElemenata() const override
    {
        return brElemeUStablu;
    }
    TipVrijednosti& operator [] (const TipKljuca &k) override;
    const TipVrijednosti operator [] (const TipKljuca &k) const override;
    void obrisi() override;
    void obrisi(const TipKljuca &tempKljuc) override;
    BinStabloMapa& operator= (const BinStabloMapa &m);
    BinStabloMapa(const BinStabloMapa &m);
};

template <typename TipKljuca, typename TipVrijednosti>
BSCvor <TipKljuca, TipVrijednosti> *BinStabloMapa<TipKljuca, TipVrijednosti> :: PomKopiranje(const BSCvor<TipKljuca, TipVrijednosti> *tempKorijen)
{
    if(!tempKorijen)
        return nullptr;

    BSCvor <TipKljuca, TipVrijednosti> *tempCvor(new BSCvor <TipKljuca, TipVrijednosti> ());

    tempCvor -> kljuc = tempKorijen -> kljuc;
    tempCvor -> vrijednost = tempKorijen -> vrijednost;

    tempCvor -> LDijete = PomKopiranje(tempKorijen -> LDijete);
    tempCvor -> DDijete = PomKopiranje(tempKorijen -> DDijete);

    return tempCvor;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa <TipKljuca, TipVrijednosti> :: PomObrisi(BSCvor <TipKljuca, TipVrijednosti> *tempKorijen)
{
    if(tempKorijen)
    {
        PomObrisi(tempKorijen -> LDijete);
        PomObrisi(tempKorijen -> DDijete);

        delete tempKorijen;
        brElemeUStablu--;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa <TipKljuca, TipVrijednosti>& BinStabloMapa <TipKljuca, TipVrijednosti> :: operator= (const BinStabloMapa &m)
{
    if(this == &m)
        return *this;

    obrisi();

    korijen = PomKopiranje(m.korijen);
    brElemeUStablu = m.brElemeUStablu;

    return *this;
}

template <typename TipKljuca, typename TipVrijednosti>
BinStabloMapa <TipKljuca, TipVrijednosti> :: BinStabloMapa(const BinStabloMapa &m)
{
    korijen = PomKopiranje(m.korijen);
    brElemeUStablu = m.brElemeUStablu;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa <TipKljuca, TipVrijednosti> :: obrisi (const TipKljuca &tempKljuc)
{
    BSCvor <TipKljuca, TipVrijednosti> *tempKorijen(korijen), *tempDijete(nullptr), *tempRoditelj(nullptr), *tempCvor(nullptr);

    while(tempKorijen != nullptr && tempKljuc != tempKorijen -> kljuc)
    {
        if(tempKorijen -> kljuc > tempKljuc)
        {
            tempKorijen = tempKorijen -> LDijete;
        }
        else
        {
            tempKorijen = tempKorijen -> DDijete;
        }
    }

    if(!tempKorijen)
        return;

    if(!tempKorijen -> LDijete)
    {
        tempDijete = tempKorijen -> DDijete;
    }
    else if(!tempKorijen -> DDijete)
    {
        tempDijete = tempKorijen -> LDijete;
    }
    else
    {
        tempRoditelj = tempKorijen;
        tempDijete = tempKorijen -> LDijete;
        tempCvor = tempDijete -> DDijete;

        while(tempCvor)
        {
            tempRoditelj = tempDijete;
            tempDijete = tempCvor;
            tempCvor = tempDijete -> DDijete;
        }

        if(tempRoditelj != tempKorijen)
        {
            tempRoditelj -> DDijete = tempDijete -> LDijete;
            tempDijete -> LDijete = tempKorijen -> LDijete;

            tempDijete -> LDijete -> roditelj = tempRoditelj;
            tempKorijen -> LDijete -> roditelj = tempDijete;
        }
        tempDijete -> DDijete = tempKorijen -> DDijete;
        tempKorijen -> DDijete -> roditelj = tempDijete;
    }
    if(!tempKorijen -> roditelj)
    {
        korijen = tempDijete;
        if(korijen)
            korijen -> roditelj = nullptr;
    }
    else if(tempKorijen == tempKorijen -> roditelj -> LDijete)
    {
        tempKorijen -> roditelj -> LDijete = tempDijete;
        if(tempDijete)
            tempDijete ->roditelj = tempKorijen ->roditelj;
    }
    else
    {
        tempKorijen -> roditelj -> DDijete = tempDijete;
        if(tempDijete)
            tempDijete -> roditelj = tempKorijen -> roditelj;
    }
    brElemeUStablu--;
    delete tempKorijen;
}

template <typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa <TipKljuca, TipVrijednosti> :: obrisi()
{
    if(korijen)
    {
        PomObrisi(korijen);
        korijen = nullptr;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti BinStabloMapa <TipKljuca, TipVrijednosti> :: operator [] (const TipKljuca &k) const
{
    BSCvor <TipKljuca, TipVrijednosti> *tempCvor(korijen);

    while(tempCvor != nullptr)
    {
        if(k == tempCvor -> kljuc)
        {
            return tempCvor -> vrijednost;
        }
        if(tempCvor -> kljuc > k)
        {
            tempCvor = tempCvor -> LDijete;
        }
        else
        {
            tempCvor = tempCvor -> DDijete;
        }
    }
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& BinStabloMapa <TipKljuca, TipVrijednosti> :: operator [] (const TipKljuca &k)
{
    BSCvor<TipKljuca, TipVrijednosti> *tempCvor(korijen);
    BSCvor<TipKljuca, TipVrijednosti> *predzadnji(korijen);

    while(tempCvor)
    {
        if(k == tempCvor -> kljuc)
        {
            return tempCvor -> vrijednost;
        }
        if(tempCvor -> kljuc > k)
        {
            predzadnji = tempCvor;
            tempCvor = tempCvor -> LDijete;
        }
        else
        {
            predzadnji = tempCvor;
            tempCvor = tempCvor -> DDijete;
        }
    }

    if(!tempCvor)
    {
        if(!korijen)
        {
            korijen = new BSCvor<TipKljuca, TipVrijednosti> ();
            korijen -> kljuc = k;
            brElemeUStablu++;
            return korijen -> vrijednost;
        }
        if(predzadnji -> kljuc > k)
        {
            predzadnji -> LDijete = new BSCvor<TipKljuca, TipVrijednosti> ();
            predzadnji -> LDijete -> roditelj = predzadnji;
            predzadnji -> LDijete -> kljuc = k;
            brElemeUStablu++;
            return predzadnji -> LDijete -> vrijednost;
        }
        else
        {
            predzadnji -> DDijete = new BSCvor<TipKljuca, TipVrijednosti> ();
            predzadnji -> DDijete -> roditelj = predzadnji;
            predzadnji -> DDijete -> kljuc = k;
            brElemeUStablu++;
            return predzadnji -> DDijete -> vrijednost;
        }
    }
}

int main() {
    //std::cout << "Pripremna Zadaca Za Vjezbu 8, Zadatak 1";

    BinStabloMapa <int, int> m;
    m[100] = 200;
    m[200] = 300;
    cout << m.brojElemenata() << endl;

    BinStabloMapa<int, int> m2 (m);
    cout << m2.brojElemenata() << endl;

    m.obrisi(2);
    cout << m.brojElemenata() << endl;
    return 0;
}
