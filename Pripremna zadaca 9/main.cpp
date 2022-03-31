#include <iostream>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

template <typename TipKljuca, typename TipVrijednosti>
class Mapa
{
    public:
    Mapa(){}
    virtual ~Mapa(){}
    virtual int brojElemenata () const = 0;
    virtual TipVrijednosti& operator[] (const TipKljuca &k) = 0;
    virtual const TipVrijednosti& operator[] (const TipKljuca &k) const = 0;
    virtual void obrisi(const TipKljuca &kljuc) = 0;
    virtual void obrisi() = 0;
    Mapa(const Mapa &m) = delete;
    Mapa& operator= (const Mapa &m) = delete;
};

//////////////////////////////////////////////////////////////////////////////////////////////

template<typename TipKljuca, typename TipVrijednosti>
class NizMapa : public Mapa<TipKljuca, TipVrijednosti>{

    int trenutniBrojElemenata, kapacitet;
    TipVrijednosti *nizTV;
    TipKljuca *nizTK;
    TipVrijednosti tmpTipVrijednosti;

    public:
    NizMapa(int tmpKapacitet = 10): kapacitet(tmpKapacitet),trenutniBrojElemenata(0), tmpTipVrijednosti(TipVrijednosti()){
        nizTV = new TipVrijednosti[tmpKapacitet];
        nizTK = new TipKljuca[tmpKapacitet];
    }
    ~NizMapa(){
        delete [] nizTV;
        delete [] nizTK;
        //brojElemenata = trenutniBrojElemenata =0;
    }
    int brojElemenata () const override { return trenutniBrojElemenata; }
    TipVrijednosti& operator [](const TipKljuca &k) override;
    const TipVrijednosti& operator [](const TipKljuca &k) const override;

    void obrisi() override{
        /*
        delete [] nizTK;
        delete [] nizTV;
        kapacitet = trenutniBrojElemenata = 0;*/
        //bolje reci da trenutnih elemenata ima 0 pa da pisemo
        //jedan preko drugog nego ovo
        trenutniBrojElemenata=0;
    }

    void obrisi(const TipKljuca &kljuc) override;
    NizMapa(const NizMapa &m) ;
    NizMapa& operator = (const NizMapa &m);

};

//Operator dodjele
template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca,TipVrijednosti>& NizMapa<TipKljuca,TipVrijednosti>::operator =(const NizMapa &m){
    if(this == &m) return *this;
    delete [] nizTK;
    delete [] nizTV;

    kapacitet=m.kapacitet;
    trenutniBrojElemenata = m.trenutniBrojElemenata;

    nizTK = new TipKljuca[kapacitet];
    nizTV = new TipVrijednosti[kapacitet];

    for(int i(0); i<trenutniBrojElemenata; i++){
        nizTK[i] = m.nizTK[i];
        nizTV[i] = m.nizTV[i];
    }

    return *this;
}

//konstruktor kopije
template<typename TipKljuca, typename TipVrijednosti>
NizMapa<TipKljuca,TipVrijednosti>::NizMapa(const NizMapa &m){
    kapacitet = m.kapacitet;
    trenutniBrojElemenata=m.trenutniBrojElemenata;

    nizTK = new TipKljuca[kapacitet];
    nizTV = new TipVrijednosti [kapacitet];


    for(int i(0); i<trenutniBrojElemenata; i++){
        nizTK[i] = m.nizTK[i];
        nizTV[i] = m.nizTV[i];
    }
}

//BRISANJE JEDNOG ELEMENTA
template<typename TipKljuca, typename TipVrijednosti>
void NizMapa<TipKljuca, TipVrijednosti>::obrisi(const TipKljuca &kljuc){
    for(int i(0); i<trenutniBrojElemenata; i++)
     if(nizTK[i]==kljuc){
         nizTK[i] = nizTK[trenutniBrojElemenata-1];
         nizTV[i] = nizTV[trenutniBrojElemenata-1];
         trenutniBrojElemenata--;
         return;
     }

     throw "nema tog elementa";
}


template<typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti& NizMapa<TipKljuca,TipVrijednosti>::operator [](const TipKljuca &k) const{

    for(int i(0); i<trenutniBrojElemenata; i++)
        if(nizTK[i]==k)
         return nizTV[i];


    return tmpTipVrijednosti;
}

template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& NizMapa<TipKljuca,TipVrijednosti>::operator[](const TipKljuca &k){
    for(int i=0; i<trenutniBrojElemenata; i++)
     if(nizTK[i]==k)
       return nizTV[i];

     //po defaultu kod mapa ukoliko ne postoji trazeni element on se dodaje
     if(trenutniBrojElemenata<kapacitet){
         //ovdje je greska sto uopste nismo alocirali prostor za ova dva pokazivaca nizova
         nizTK[trenutniBrojElemenata] = k;
         nizTV[trenutniBrojElemenata] = TipVrijednosti();
         trenutniBrojElemenata++;
         return nizTV[trenutniBrojElemenata-1];
     }


      //sada ukoliko nema dovoljno prostora treba proširiti i kopirat postojece za te potrebe
     //korisno je napraviti pomocnu funkciju.. konkretno cu napisat samo kod operatora dodjele ako
     //bude potrebe za jos necim napisat cu da bi se izbjegla nepreglednost

     if(trenutniBrojElemenata==kapacitet){
         kapacitet*=2;

         TipKljuca *tmpNizKljuceva = new TipKljuca[kapacitet];
         TipVrijednosti *tmpNizVrjednosti = new TipVrijednosti[kapacitet];

         for(int i(0); i<trenutniBrojElemenata; i++){
             tmpNizKljuceva[i] = nizTK[i];
             tmpNizVrjednosti[i] = nizTV[i];
         }

         delete [] nizTK;
         delete [] nizTV;
         tmpNizKljuceva[trenutniBrojElemenata] = k;
         tmpNizVrjednosti[trenutniBrojElemenata] = TipVrijednosti();
         trenutniBrojElemenata++;

         nizTK=tmpNizKljuceva;
         nizTV=tmpNizVrjednosti;

         //tmpNizKljuceva = tmpNizVrjednosti = nullptr;

         return nizTV[trenutniBrojElemenata-1];
     }



}

//----------------------BINARNOOOO STABLLOOOOO-------------------------------
template<typename TipKljuca, typename TipVrijednosti>
class BSCvor{ //ova klasa nam sluzi za predstavljanje cvora
 public:

 BSCvor *lijevoDijete, *desnoDijete, *roditelj;
 TipKljuca kljuc;
 TipVrijednosti vrijednost;

 BSCvor(){
     lijevoDijete=desnoDijete=roditelj=nullptr;
     kljuc=TipKljuca();
     vrijednost=TipVrijednosti();
 }

};

template<typename TipKljuca, typename TipVrijednosti>
class BinStabloMapa: public Mapa<TipKljuca,TipVrijednosti> {
    BSCvor<TipKljuca,TipVrijednosti> *korjen;
    int brElemenataUStablu;
    void PomObrisi(BSCvor<TipKljuca, TipVrijednosti> *tmpKorjen);
    BSCvor<TipKljuca,TipVrijednosti> * PomKopiranje(const BSCvor<TipKljuca, TipVrijednosti> *tmpKorjen);

    public:
    BinStabloMapa(){
     brElemenataUStablu=0;
     korjen = nullptr;
    }
    ~BinStabloMapa(){
        obrisi();
    }
    int brojElemenata () const override { return brElemenataUStablu; }
    TipVrijednosti& operator[](const TipKljuca &k) override;
    const TipVrijednosti& operator[](const TipKljuca &k) const override;
    void obrisi() override;
    void obrisi(const TipKljuca &tmpKljuc) override;
    BinStabloMapa& operator= (const BinStabloMapa &m);
    BinStabloMapa(const BinStabloMapa &m);

};

template<typename TipKljuca, typename TipVrijednosti>
BSCvor<TipKljuca,TipVrijednosti> * BinStabloMapa<TipKljuca,TipVrijednosti>:: PomKopiranje(const BSCvor<TipKljuca, TipVrijednosti> *tmpKorjen){
    //izlaz iz rekurzije
    if(!tmpKorjen) return nullptr;

    BSCvor<TipKljuca, TipVrijednosti> *tmpCvor(new BSCvor<TipKljuca,TipVrijednosti> ());
    tmpCvor->kljuc = tmpKorjen->kljuc;
    tmpCvor->vrijednost = tmpKorjen->vrijednost;


    tmpCvor->lijevoDijete = PomKopiranje(tmpKorjen->lijevoDijete);
    tmpCvor->desnoDijete = PomKopiranje(tmpKorjen->desnoDijete);
    return tmpCvor;
}

// KONSTRUKTOR KOPIJE
template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>::BinStabloMapa(const BinStabloMapa &m){
    korjen = PomKopiranje(m.korjen);
    brElemenataUStablu = m.brElemenataUStablu;
}

//OPERATOR DODJELE
template<typename TipKljuca, typename TipVrijednosti>
BinStabloMapa<TipKljuca, TipVrijednosti>& BinStabloMapa<TipKljuca, TipVrijednosti>::operator = (const BinStabloMapa &m){
    if(this == &m) return *this;

    obrisi();

    korjen = PomKopiranje(m.korjen);
    brElemenataUStablu = m.brElemenataUStablu;

    return *this;
}


template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::PomObrisi(BSCvor<TipKljuca, TipVrijednosti> *tmpKorjen){
    if(tmpKorjen){
        PomObrisi(tmpKorjen->lijevoDijete);
        PomObrisi(tmpKorjen->desnoDijete);
        delete tmpKorjen;
        brElemenataUStablu--;
    }
}

template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca,TipVrijednosti>::obrisi(const TipKljuca &tmpKljuc){

   BSCvor<TipKljuca,TipVrijednosti> *tmpKorjen(korjen), *tmpDijete(nullptr);
   BSCvor<TipKljuca,TipVrijednosti> *tmpRoditelj(nullptr), *tmpCvor(nullptr);

   while(tmpKorjen!= nullptr and tmpKljuc!=tmpKorjen->kljuc){
       if(tmpKorjen->kljuc>tmpKljuc)
         tmpKorjen=tmpKorjen->lijevoDijete;
       else
         tmpKorjen=tmpKorjen->desnoDijete;
   }
   if(!tmpKorjen) return;
   //sad obradjujemo situaciju kad ima samo jedno dijete..
   if(!tmpKorjen->lijevoDijete) tmpDijete=tmpKorjen->desnoDijete;
   else if(!tmpKorjen->desnoDijete) tmpDijete=tmpKorjen->lijevoDijete;
   else{
       tmpRoditelj = tmpKorjen;
       tmpDijete = tmpKorjen->lijevoDijete;
       tmpCvor = tmpDijete->desnoDijete;
       while(tmpCvor){
           tmpRoditelj=tmpDijete;
           tmpDijete=tmpCvor;
           tmpCvor=tmpDijete->desnoDijete;
       }//do ovog dijela smo trebali naci najdesniji cvor
       if(tmpRoditelj!=tmpKorjen){
           tmpRoditelj->desnoDijete = tmpDijete->lijevoDijete;
           tmpDijete->lijevoDijete=tmpKorjen->lijevoDijete;
           //sada moramo orediti ko je kome roditelj jer za razliku od primjera iz knjige mi imamo cvor koji pokazuje na roditelja
           tmpDijete->lijevoDijete->roditelj = tmpRoditelj;
           tmpKorjen->lijevoDijete->roditelj = tmpDijete;

       }
       tmpDijete->desnoDijete=tmpKorjen->desnoDijete;
       //izmjenimo jos roditelja..
       tmpKorjen->desnoDijete->roditelj=tmpDijete;
   }//prvi if ispod je stajalo if(!tmpKorjen->roditelj) ali kako
   if(!tmpKorjen->roditelj) { korjen=tmpDijete; if(korjen)korjen->roditelj = nullptr; }
   else if(tmpKorjen==tmpKorjen->roditelj->lijevoDijete){
             tmpKorjen->roditelj->lijevoDijete = tmpDijete;
             // da djetetu odredimo roditelja..
             if(tmpDijete)
             tmpDijete->roditelj = tmpKorjen->roditelj;

            }
   else{

       tmpKorjen->roditelj->desnoDijete=tmpDijete;
       //i ovdje takodjer
       if(tmpDijete)
       tmpDijete->roditelj =tmpKorjen->roditelj;
   }

   brElemenataUStablu--;
   delete tmpKorjen;
}


/*
 ne treba 2x puta prolaziti kada dodajemo novi cvor
 ideno od korjena prema cvoru gdje treba biti ako ga nema onda ga dodamo...
*/


template<typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& BinStabloMapa<TipKljuca,TipVrijednosti>::operator[](const TipKljuca &k){
    BSCvor<TipKljuca, TipVrijednosti> *tmpCvor(korjen), *predzadnji(korjen);

    while(tmpCvor){
        if(k==tmpCvor->kljuc) return tmpCvor->vrijednost;
        if(tmpCvor->kljuc > k) {predzadnji = tmpCvor; tmpCvor=tmpCvor->lijevoDijete; }
        else
        { predzadnji = tmpCvor; tmpCvor=tmpCvor->desnoDijete;}
    }
    if(!tmpCvor){
        if(!korjen){
            korjen = new BSCvor<TipKljuca, TipVrijednosti>();
            korjen->kljuc = k;
            brElemenataUStablu++;
            return korjen->vrijednost;
        }
        if(predzadnji->kljuc > k){
            predzadnji->lijevoDijete = new BSCvor<TipKljuca, TipVrijednosti> ();
            predzadnji->lijevoDijete->roditelj = predzadnji;
            predzadnji->lijevoDijete->kljuc = k;
            brElemenataUStablu++;
            return predzadnji->lijevoDijete->vrijednost;
        }else{
            predzadnji->desnoDijete = new BSCvor<TipKljuca, TipVrijednosti>();
            predzadnji->desnoDijete->roditelj = predzadnji;
            predzadnji->desnoDijete->kljuc = k;
            brElemenataUStablu++;
            return predzadnji->desnoDijete->vrijednost;
        }
    }
}

template<typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti& BinStabloMapa<TipKljuca,TipVrijednosti>::operator[](const TipKljuca &k) const{
    //konstantna verzija ne moze vrsiti nikakve izmjene moze samo vratit ako postoji ta vrijednost..
   BSCvor<TipKljuca, TipVrijednosti> *tmpCvor(korjen);

    while(tmpCvor!=nullptr){
        if(k==tmpCvor->kljuc) return tmpCvor->vrijednost;
        if(tmpCvor->kljuc > k) tmpCvor=tmpCvor->lijevoDijete;
        else
         tmpCvor=tmpCvor->desnoDijete;
    }


}



template<typename TipKljuca, typename TipVrijednosti>
void BinStabloMapa<TipKljuca, TipVrijednosti>::obrisi(){
    if(korjen){
       PomObrisi(korjen);
       korjen = nullptr;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename TipKljuca, typename TipVrijednosti>
class HashMapa : public Mapa <TipKljuca, TipVrijednosti>
{
    int trenutniBrojElemenata, max;
    TipKljuca *nizTK;
    TipVrijednosti *nizTV, tempTipVrijednosti;
    unsigned int (*hashFunkcija)(TipKljuca, unsigned int);

    public:

    HashMapa(int tempMax = 10)
    {
        max = tempMax;
        nizTK = new TipKljuca[max];
        nizTV = new TipVrijednosti[max];
        tempTipVrijednosti = TipVrijednosti();
        hashFunkcija = nullptr;
        trenutniBrojElemenata = 0;

        for(int i=0; i<max; i++)
        {
            nizTK[i] = TipKljuca();
            nizTV[i] = TipVrijednosti();
        }
    }
    ~HashMapa()
    {
        delete [] nizTK;
        delete [] nizTV;
    }
    int brojElemenata () const override
    {
        return trenutniBrojElemenata;
    }
    void obrisi() override
    {
        trenutniBrojElemenata = 0;

        for(int i=0; i<max; i++)
        {
            nizTK[i] = TipKljuca();
            nizTV[i] = TipVrijednosti();
        }
    }
    TipVrijednosti& operator[] (const TipKljuca &k) override;
    const TipVrijednosti& operator[] (const TipKljuca &k) const override;
    HashMapa(const HashMapa &h);
    HashMapa& operator= (const HashMapa &h);
    void obrisi(const TipKljuca &kljuc) override;
    void definisiHashFunkciju(unsigned int (*f) (TipKljuca k, unsigned int i))
    {
        hashFunkcija = f;
    }
};

unsigned int hashf(string ulaz, unsigned int max)
{
    unsigned int suma = 5381;

    for(int i = 0; i<ulaz.length(); i++)
    {
        suma = suma*33 + ulaz[i];
    }

    return suma %max;
}

template <typename TipKljuca, typename TipVrijednosti>
void HashMapa <TipKljuca, TipVrijednosti> :: obrisi (const TipKljuca &kljuc)
{
    if(!hashFunkcija) throw "Nema hash funkcije";

        unsigned int tempIndex(hashFunkcija(kljuc, max));
        int i=1;

        if(nizTK[tempIndex] == kljuc)
        {
            nizTK[tempIndex] = -1;
            nizTV[tempIndex] = TipVrijednosti();
            trenutniBrojElemenata--;
            return;
        }

        while(i < max && nizTK[(tempIndex + i) % max] != TipKljuca())
        {
            if(nizTK[(tempIndex + i) % max] == kljuc)
            {
                nizTK[(tempIndex + i) % max] = -1;
                nizTV[(tempIndex + i) % max] = TipVrijednosti();
                trenutniBrojElemenata--;
                return;
            }
            i++;
        }
        throw "Nema tog elementa";
}

template <typename TipKljuca, typename TipVrijednosti>
HashMapa <TipKljuca, TipVrijednosti> :: HashMapa(const HashMapa &h)
{
    trenutniBrojElemenata = h.trenutniBrojElemenata;
    tempTipVrijednosti = h.tempTipVrijednosti;
    hashFunkcija = h.hashFunkcija;
    max = h.max;

    nizTV = new TipVrijednosti[max];
    nizTK = new TipKljuca[max];

    for(int i=0; i<max; i++)
    {
        nizTV[i] = h.nizTV[i];
        nizTK[i] = h.nizTK[i];
    }
}

template <typename TipKljuca, typename TipVrijednosti>
TipVrijednosti& HashMapa<TipKljuca, TipVrijednosti> :: operator[] (const TipKljuca & k)
{
    if(!hashFunkcija) throw "Nema hash Funkcije.";

    int j = 1;
    unsigned int tempIndex (hashFunkcija(k, max));

    if(nizTK[tempIndex] == k)
    {
        if(trenutniBrojElemenata == 0)
            trenutniBrojElemenata++;

        return nizTV[tempIndex];
    }

    if(nizTK[tempIndex] == TipKljuca())
    {
        nizTK[tempIndex] = k;
        trenutniBrojElemenata++;
        return nizTV[tempIndex];
    }

    if(trenutniBrojElemenata >= max)
    {
        max = max * 2;
        TipKljuca *tempNizKljuceva = new TipKljuca[max];
        TipVrijednosti *tempNizVrijednosti = new TipVrijednosti[max];

        for(int i = 0; i<max; i++)
        {
            tempNizKljuceva[i] = TipKljuca();
        }

        for(int i=0; i<trenutniBrojElemenata; i++)
        {
            unsigned int tempIndex2 (hashFunkcija(nizTK[i], max));

            if(tempNizKljuceva[tempIndex2] == TipKljuca())
            {
                tempNizKljuceva[tempIndex2] = nizTK[i];
                tempNizVrijednosti[tempIndex2] = nizTV[i];
            }
            else
            {
                int k=1;
                while(k < max && nizTK[(tempIndex2 + k) % max] != TipKljuca())
                    k++;

                tempNizKljuceva[(tempIndex2 + k) % max] = nizTK[i];
                tempNizVrijednosti[(tempIndex2 + k) % max] = nizTV[i];
            }
        }
        delete [] nizTV;
        delete [] nizTK;

        nizTK = tempNizKljuceva;
        nizTV = tempNizVrijednosti;
    }
    while(j < max && nizTK[(tempIndex + j) % max] != TipKljuca())
    {
        if(nizTK[(tempIndex + j) % max] == k)
        {
            return nizTV[(tempIndex + j) % max];
        }
        j++;
    }
    nizTK[(tempIndex + j) % max] = k;
    trenutniBrojElemenata++;

    return nizTV[(tempIndex + j) % max];
}

template <typename TipKljuca, typename TipVrijednosti>
const TipVrijednosti& HashMapa<TipKljuca, TipVrijednosti> :: operator [] (const TipKljuca &k) const
{
    if(!hashFunkcija) throw "Nema hash Funkcije.";

    int i=1;
    unsigned int tempIndex (hashFunkcija(k,max));

    if(nizTK[tempIndex] == k)
        return nizTV[tempIndex];

    while(i<max && nizTK[(tempIndex + i) % max] != TipKljuca())
    {
        if(nizTK[(tempIndex + i) % max] == k)
        {
            return nizTV[(tempIndex + i) % max];
        }
        i++;
    }
    return tempTipVrijednosti;
}


template <typename TipKljuca, typename TipVrijednosti>
HashMapa <TipKljuca, TipVrijednosti> & HashMapa <TipKljuca, TipVrijednosti> :: operator = (const HashMapa &h)
{
    if(this == &h) return *this;

        delete [] nizTV;
        delete [] nizTK;

        hashFunkcija = h.hashFunkcija;
        tempTipVrijednosti = h.tempTipVrijednosti;
        trenutniBrojElemenata = h.trenutniBrojElemenata;
        max = h.max;

        nizTV = new TipVrijednosti[max];
        nizTK = new TipKljuca[max];

        for(int i=0; i<max; i++)
        {
            nizTV[i] = h.nizTV[i];
            nizTK[i] = h.nizTK[i];
        }
        return *this;
}

unsigned int djbhash(std::string ulaz, unsigned int max)
{
    unsigned int suma = 5381;

    for(int i=0; i<ulaz.length(); i++)
        suma = suma *33 + ulaz[i];

    return suma % max;
}

unsigned int mojahash3 (int ulaz, unsigned int max)
{
    unsigned int suma = 23;
    suma = suma * 31 + ulaz;
    return suma % max;
}

int main() {
   // std::cout << "Pripremna Zadaca Za Vjezbu 9, Zadatak 1";

   HashMapa<int, int> hm;
hm.definisiHashFunkciju(mojahash3);

NizMapa<int, int> nm(123);

BinStabloMapa<int, int> bs;

    clock_t vrijemePocetka, vrijemeKraja;

    vrijemePocetka=clock_t();

    for(int i(1); i<250000; i++)
     hm[i] = i;

     vrijemeKraja=clock_t();
      //cout<<vrijemePocetka;
     cout<<"Dodavanje u hash mapu je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;


     //sada cemo ispitati niz mapu

     vrijemePocetka=clock_t();

     for(int i(1); i<250000; i++)
        nm[i] = i;
       vrijemeKraja=clock_t();
        cout<<"Dodavanje u niz mapu je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;


        //Binarno stablo..
            vrijemePocetka=clock_t();

         for(int i(1); i<250000; i++)
            bs[i] = i;
            vrijemeKraja=clock_t();

        cout<<"Dodavanje u binarno stablo mapu je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;
   //test brisanja...
   vrijemePocetka=clock_t();
   for(int i(1); i<250000; i++)
     hm.obrisi(i);
    vrijemeKraja=clock_t();
    cout<<"Brisanje iz hash mape je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;

    vrijemePocetka=clock_t();
   for(int i(1); i<250000; i++)
     nm.obrisi(i);
    vrijemeKraja=clock_t();
    cout<<"Brisanje iz niz mape je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;


    vrijemePocetka=clock_t();
   for(int i(1); i<250000; i++)
     bs.obrisi(i);
    vrijemeKraja=clock_t();
    cout<<"Brisanje iz binStablo mape je trajalo: "<<(vrijemeKraja-vrijemePocetka)/CLOCKS_PER_SEC<<" sekundi"<<endl;

    return 0;
}
