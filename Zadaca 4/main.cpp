#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    Mapa () {}
    virtual ~Mapa () {}
    virtual int brojElemenata () const = 0;
    virtual TipVrijednosti& operator[] (const TipKljuca &k) = 0;
    virtual const TipVrijednosti operator[] (const TipKljuca &k) const = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual void obrisi() = 0;
    Mapa(const Mapa &m) = delete;
    Mapa& operator= (const Mapa &m) = delete;
};

template <typename TipKljuca, typename TipVrijednosti>
class BSCvor
{
    public:
    BSCvor *lijevoDijete;
    BSCvor *desnoDijete;
    BSCvor *roditelj;
    TipKljuca kljuc;
    TipVrijednosti vrijednost;
    int balans;

    BSCvor()
    {
        lijevoDijete = nullptr;
        desnoDijete = nullptr;
        roditelj = nullptr;
        kljuc = TipKljuca();
        vrijednost = TipVrijednosti();
        balans = 0;
    }
};

template <typename TipKljuca, typename TipVrijednosti>
class AVLStabloMapa: public Mapa<TipKljuca, TipVrijednosti>
{
    BSCvor<TipKljuca, TipVrijednosti> *korijen;
    int brojElemenataUStablu;
    void PomObrisi(BSCvor<TipKljuca, TipVrijednosti> *tempKorijen);
    BSCvor<TipKljuca, TipVrijednosti> *PomKopiranje(const BSCvor<TipKljuca, TipVrijednosti> *tempKorijen);

    public:
    AVLStabloMapa()
    {
        korijen = nullptr;
        brojElemenataUStablu = 0;
    }

    ~AVLStabloMapa()
    {
        obrisi();
    }

    int brojElemenata() const override
    {
        return brojElemenataUStablu;
    }

    TipVrijednosti& operator[] (const TipKljuca &k) override;
    const TipVrijednosti operator[] (const TipKljuca &k) const override;
    void obrisi(const TipKljuca &tempKljuc) override;
    void obrisi() override;
    AVLStabloMapa& operator= (const AVLStabloMapa &m);
    AVLStabloMapa(const AVLStabloMapa &m);

    void azurirajBalans(BSCvor<TipKljuca, TipVrijednosti> *noviCvor);
    void azurirajBalansPoslijeBrisanja(BSCvor<TipKljuca, TipVrijednosti> *noviCvor);
    void lijevaRotacija(BSCvor<TipKljuca, TipVrijednosti> *cvor);
    void desnaRotacija(BSCvor<TipKljuca, TipVrijednosti> *cvor);
};

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti> :: lijevaRotacija(BSCvor <TipKljuca, TipVrijednosti> *cvor)
{
    BSCvor <TipKljuca, TipVrijednosti> *cvorKojiNarusavaBalans(cvor);
    BSCvor <TipKljuca, TipVrijednosti> *njegovoDesnoDijete(cvor -> desnoDijete);
    BSCvor <TipKljuca, TipVrijednosti> *njegovoLijevoDijete(cvor -> lijevoDijete);
    BSCvor <TipKljuca, TipVrijednosti> *desnoDijeteDesnogDjeteta(njegovoDesnoDijete -> desnoDijete);
    BSCvor <TipKljuca, TipVrijednosti> *lijevoDijeteDesnogDjeteta(njegovoDesnoDijete -> lijevoDijete);

    if(!cvorKojiNarusavaBalans -> roditelj)
    {
        korijen = njegovoDesnoDijete;
    }
    else
    {
        if(cvorKojiNarusavaBalans -> roditelj -> desnoDijete == cvorKojiNarusavaBalans)
        {
            cvorKojiNarusavaBalans -> roditelj -> desnoDijete = cvorKojiNarusavaBalans;
        }
        else
        {
            cvorKojiNarusavaBalans -> roditelj -> lijevoDijete = cvorKojiNarusavaBalans;
        }
    }

    cvorKojiNarusavaBalans -> roditelj = njegovoDesnoDijete;

    if(njegovoLijevoDijete)
    {
        njegovoLijevoDijete -> roditelj = cvorKojiNarusavaBalans;
    }

    if(lijevoDijeteDesnogDjeteta)
    {
        lijevoDijeteDesnogDjeteta -> roditelj = cvorKojiNarusavaBalans;
    }

    if(desnoDijeteDesnogDjeteta)
    {
        desnoDijeteDesnogDjeteta -> roditelj = njegovoDesnoDijete;
    }

    cvorKojiNarusavaBalans -> desnoDijete = lijevoDijeteDesnogDjeteta;
    njegovoDesnoDijete -> lijevoDijete = cvorKojiNarusavaBalans;

    if(cvorKojiNarusavaBalans -> balans == -2)
    {
        njegovoDesnoDijete -> balans = 0;
    }
    njegovoDesnoDijete -> balans = 1;
    cvorKojiNarusavaBalans -> balans = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa<TipKljuca, TipVrijednosti> :: desnaRotacija(BSCvor<TipKljuca, TipVrijednosti> *cvor)
{
    BSCvor<TipKljuca, TipVrijednosti> *cvorKojiNarusavaBalans(cvor);
    BSCvor<TipKljuca, TipVrijednosti> *njegovoLijevoDijete(cvor -> lijevoDijete);
    BSCvor<TipKljuca, TipVrijednosti> *lijevoDijeteLijevogDjeteta(njegovoLijevoDijete -> lijevoDijete);
    BSCvor<TipKljuca, TipVrijednosti> *desnoDijeteLijevogDjeteta(njegovoLijevoDijete -> desnoDijete);
    BSCvor<TipKljuca, TipVrijednosti> *njegovoDesnoDijete(cvor -> desnoDijete);

    if(!cvorKojiNarusavaBalans -> roditelj)
    {
        korijen = njegovoLijevoDijete;
    }
    else
    {
        if(cvorKojiNarusavaBalans -> roditelj -> lijevoDijete == cvorKojiNarusavaBalans)
            cvorKojiNarusavaBalans -> roditelj -> lijevoDijete = njegovoLijevoDijete;
        else
            cvorKojiNarusavaBalans -> roditelj -> desnoDijete = njegovoLijevoDijete;
    }
    cvorKojiNarusavaBalans -> roditelj = njegovoLijevoDijete;

    if(lijevoDijeteLijevogDjeteta)
    {
        lijevoDijeteLijevogDjeteta -> roditelj = njegovoLijevoDijete;
    }
    if(desnoDijeteLijevogDjeteta)
    {
        desnoDijeteLijevogDjeteta -> roditelj = cvorKojiNarusavaBalans;
    }
    if(njegovoDesnoDijete)
    {
        njegovoDesnoDijete -> roditelj = cvorKojiNarusavaBalans;
    }

    njegovoLijevoDijete -> desnoDijete = cvorKojiNarusavaBalans;
    cvorKojiNarusavaBalans -> lijevoDijete = desnoDijeteLijevogDjeteta;

    if(cvorKojiNarusavaBalans -> balans == 2)
    {
        njegovoLijevoDijete -> balans = 0;
    }
    else
    {
        njegovoLijevoDijete -> balans = -1;
    }

    cvorKojiNarusavaBalans -> balans = 0;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti> :: azurirajBalans(BSCvor <TipKljuca, TipVrijednosti> *noviCvor)
{
    while(noviCvor -> roditelj)
    {
        if(!noviCvor) return;
        if(noviCvor -> roditelj -> lijevoDijete == noviCvor)
        {
            noviCvor -> roditelj -> balans++;
        }
        else if(noviCvor -> roditelj -> desnoDijete == noviCvor)
        {
            noviCvor -> roditelj -> balans--;
        }
        if(noviCvor -> roditelj -> balans == 0) break;


        if(noviCvor -> roditelj -> balans > 1 || noviCvor -> roditelj -> balans < -1)
        {
            if(noviCvor -> roditelj -> balans == -2 && noviCvor -> roditelj -> desnoDijete -> balans == -1)
            {
                lijevaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans == -2 && noviCvor -> roditelj -> desnoDijete -> balans == 1)
            {
                desnaRotacija(noviCvor -> roditelj -> desnoDijete);
                lijevaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans = 2 && noviCvor -> roditelj -> lijevoDijete -> balans == 1)
            {
                desnaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans == 2 && noviCvor -> roditelj -> lijevoDijete -> balans == -1)
            {
                lijevaRotacija(noviCvor -> roditelj -> lijevoDijete);
                desnaRotacija(noviCvor -> roditelj);
            }
        }
        noviCvor = noviCvor -> roditelj;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti> :: azurirajBalansPoslijeBrisanja(BSCvor <TipKljuca, TipVrijednosti> *noviCvor)
{
    if(!noviCvor) return;

    while(noviCvor -> roditelj)
    {
        if(noviCvor -> roditelj -> lijevoDijete == noviCvor)
        {
            noviCvor -> roditelj -> balans--;
        }
        else if(noviCvor -> roditelj -> desnoDijete == noviCvor)
        {
            noviCvor -> roditelj -> balans++;
        }

        if(noviCvor -> roditelj -> balans == 0) break;

        if(noviCvor -> roditelj -> balans > 1 || noviCvor -> roditelj -> balans < -1)
        {
            if(noviCvor -> roditelj -> balans == -2 && noviCvor -> roditelj -> desnoDijete -> balans == -1)
            {
                lijevaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans == -2 && noviCvor -> roditelj -> desnoDijete -> balans == 1)
            {
                desnaRotacija(noviCvor -> roditelj ->desnoDijete);
                lijevaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans = 2 && noviCvor -> roditelj -> lijevoDijete -> balans == 1)
            {
                desnaRotacija(noviCvor -> roditelj);
            }
            else if(noviCvor -> roditelj -> balans == 2 && noviCvor -> roditelj -> lijevoDijete -> balans == -1)
            {
                lijevaRotacija(noviCvor -> roditelj -> lijevoDijete);
                desnaRotacija(noviCvor -> roditelj);
            }
        }
        noviCvor = noviCvor -> roditelj;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
BSCvor <TipKljuca, TipVrijednosti> *AVLStabloMapa <TipKljuca, TipVrijednosti> :: PomKopiranje(const BSCvor<TipKljuca, TipVrijednosti> *tempKorijen)
{
    if(!tempKorijen) return nullptr;

    BSCvor <TipKljuca, TipVrijednosti> *tempCvor(new BSCvor <TipKljuca, TipVrijednosti> ());
    tempCvor -> kljuc = tempKorijen -> kljuc;
    tempCvor -> vrijednost = tempKorijen -> vrijednost;

    tempCvor -> lijevoDijete = PomKopiranje(tempKorijen -> lijevoDijete);
    tempCvor -> desnoDijete = PomKopiranje(tempKorijen -> desnoDijete);

    return tempCvor;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti> :: PomObrisi(BSCvor <TipKljuca, TipVrijednosti> *tempKorijen)
{
    if(tempKorijen)
    {
        PomObrisi(tempKorijen -> lijevoDijete);
        PomObrisi(tempKorijen -> desnoDijete);

        delete tempKorijen;

        brojElemenataUStablu--;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa <TipKljuca, TipVrijednosti> & AVLStabloMapa <TipKljuca, TipVrijednosti> :: operator= (const AVLStabloMapa &m)
{
    if(this == &m) return *this;

    obrisi();
    brojElemenataUStablu = m.brojElemenataUStablu;
    korijen = PomKopiranje(m.korijen);

    return *this;
}


template <typename TipKljuca, typename TipVrijednosti>
AVLStabloMapa <TipKljuca, TipVrijednosti> :: AVLStabloMapa(const AVLStabloMapa &m)
{
    brojElemenataUStablu = m.brojElemenataUStablu;
    korijen = PomKopiranje(m.korijen);
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti>:: obrisi(const TipKljuca &tempKljuc)
{
    BSCvor <TipKljuca, TipVrijednosti> *tempKorijen(korijen), *tempDijete(nullptr), *tempRoditelj(nullptr), *tempCvor(nullptr);

    while(tempKorijen != nullptr && tempKljuc != tempKorijen -> kljuc)
    {
        if(tempKorijen -> kljuc > tempKljuc)
        {
            tempKorijen = tempKorijen -> lijevoDijete;
        }
        else
        {
            tempKorijen = tempKorijen -> desnoDijete;
        }
    }
    if(!tempKorijen) return;

    azurirajBalansPoslijeBrisanja(tempKorijen);

    if(!tempKorijen -> lijevoDijete)
    {
        tempDijete = tempKorijen -> desnoDijete;
    }
    else if(!tempKorijen -> desnoDijete)
    {
        tempDijete = tempKorijen -> lijevoDijete;
    }
    else
    {
        tempRoditelj = tempKorijen;
        tempDijete = tempKorijen -> lijevoDijete;
        tempCvor = tempDijete -> desnoDijete;

        while (tempCvor)
        {
            tempRoditelj = tempDijete;
            tempDijete = tempCvor;
            tempCvor = tempDijete -> desnoDijete;
        }

        if(tempRoditelj != tempKorijen)
        {
            tempRoditelj -> desnoDijete = tempDijete -> lijevoDijete;
            tempDijete -> lijevoDijete = tempKorijen -> lijevoDijete;

            tempDijete -> lijevoDijete -> roditelj = tempRoditelj;
            tempKorijen -> lijevoDijete -> roditelj = tempDijete;
        }

        tempDijete -> desnoDijete = tempKorijen -> desnoDijete;

        tempKorijen -> desnoDijete -> roditelj = tempDijete;
    }
    if(!tempKorijen -> roditelj)
    {
        korijen = tempDijete;
        if(korijen) korijen -> roditelj = nullptr;
    }
    else if(tempKorijen == tempKorijen -> roditelj -> lijevoDijete)
    {
        tempKorijen -> roditelj -> lijevoDijete = tempDijete;
        if(tempDijete)
        {
            tempDijete -> roditelj = tempKorijen -> roditelj;
        }
    }
    else
    {
        tempKorijen -> roditelj ->desnoDijete = tempDijete;

        if(tempDijete)
        {
            tempDijete -> roditelj = tempKorijen -> roditelj;
        }
    }
    azurirajBalansPoslijeBrisanja(tempDijete);
    brojElemenataUStablu--;

    delete tempKorijen;
}

template <typename TipKljuca, typename TipVrijednosti>
void AVLStabloMapa <TipKljuca, TipVrijednosti> :: obrisi()
{
    if(korijen)
    {
        PomObrisi(korijen);
        korijen = nullptr;
    }
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti AVLStabloMapa <TipKljuca, TipVrijednosti> :: operator[] (const TipKljuca &k) const
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
            tempCvor = tempCvor -> lijevoDijete;
        }
        else
        {
            tempCvor = tempCvor -> desnoDijete;
        }
    }
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& AVLStabloMapa <TipKljuca, TipVrijednosti> :: operator [](const TipKljuca &k)
{
    BSCvor <TipKljuca, TipVrijednosti> *tempCvor(korijen), *pz(korijen);

    while(tempCvor)
    {
        if(k == tempCvor -> kljuc)
        {
            return tempCvor -> vrijednost;
        }
        if(tempCvor -> kljuc > k)
        {
            pz = tempCvor;
            tempCvor = tempCvor -> lijevoDijete;
        }
        else
        {
            pz = tempCvor;
            tempCvor = tempCvor -> desnoDijete;
        }
    }

    if(!tempCvor)
    {
        if(!korijen)
        {
            korijen = new BSCvor <TipKljuca, TipVrijednosti> ();
            korijen -> kljuc = k;
            brojElemenataUStablu++;
            return korijen -> vrijednost;
        }

        if(pz -> kljuc > k)
        {
            pz -> lijevoDijete = new BSCvor<TipKljuca, TipVrijednosti>();
            pz -> lijevoDijete -> roditelj = pz;
            pz -> lijevoDijete -> kljuc = k;
            brojElemenataUStablu++;

            azurirajBalans(pz -> lijevoDijete);

            return pz -> lijevoDijete -> vrijednost;
        }
        else
        {
            pz -> desnoDijete = new BSCvor <TipKljuca, TipVrijednosti> ();
            pz -> desnoDijete -> roditelj = pz;
            pz -> desnoDijete -> kljuc = k;
            brojElemenataUStablu++;

            azurirajBalans(pz -> desnoDijete);

            return pz -> desnoDijete -> vrijednost;

        }
    }
}



int main() {
    //std::cout << "Zadaća 4, Zadatak 1";
    AVLStabloMapa <int, int> mm;
mm[100] = 200;
const AVLStabloMapa <int, int>& mm2(mm);
mm[200] = 300;
cout << mm2.brojElemenata() << " ";
cout << mm2[200] << " ";


cout << mm2[300] << " ";
cout << mm2.brojElemenata() << " ";
mm[300] = 400;
cout << mm2[300] << " ";

cout << mm2.brojElemenata();

AVLStabloMapa <int, int> m;
int vel(2000000);
for (int i(0); i<vel; i++)
    m[rand()%(2*vel)-vel] = i;
for (int i(-vel); i<vel; i++)
    m[i] = i;
for (int i(-vel); i<vel; i++) {
    if (m[i] != i) { cout << i << " NOT "; break; }
    m.obrisi(i);
}
cout << "OK";
    return 0;
}
