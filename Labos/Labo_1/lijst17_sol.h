//C++17-compatibele lijst. Ontbrekende elementen: move en copy, iterator
#ifndef __LIJST17_H
#define __LIJST17_H
//
// speciale code voor debuggen
//
#define DEBUG
//
//
//speciale code voor iteratoren
#define ITERATOR

#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
using std::string;
using std::endl;
using std::ostream;
using std::ofstream;

template<class T>
class Lijstknoop;
template<class T>
class Lijst;

template<class T>
using Lijstknoopptr=std::unique_ptr<Lijstknoop<T> >;
template<class T>
ostream& operator<<(ostream& os, const Lijst<T>& l);

template<class T>
class Lijst: private Lijstknoopptr<T>{
public:

	//Default constructor
	Lijst();

	//Copy constructor (MOET GEDELETE WORDEN OM COPY NA RETURN TE VERMIJDEN)
	Lijst(const Lijst<T> &) = delete;

	//Move constructor
	Lijst(Lijst<T> &&);

	//Copy operator
	Lijst<T> & operator=(const Lijst<T> & );

	//Move operator
	Lijst<T> & operator=(Lijst<T> && );
	Lijst<T> & operator=(Lijstknoopptr<T> &&);

//operaties
//duplicaten zijn toegelaten.
    public: void voegToe(const T&);
//geefaantal geeft het aantal keer dat de sleutel voorkomt.
//gebruikt de zoekfunctie om door de lijst te lopen!
//zonder argument: geef lengte lijst
    public: int geefAantal(const T&) const;
    public: int geefAantal() const;
//verwijder verwijdert slechts het eerste exemplaar met de gegeven
//T, en geeft geen fout als de T niet gevonden wordt.
//gebruik de zoekfunctie om door de lijst te lopen!
    public: void verwijder(const T&);
//verwijder eerste knoop.
    public: void verwijderEerste();
    public: void insertionsort();
    public: bool isClone(const Lijst<T>&) const;
// zoek geeft een pointer naar de Lijst die de sleutelwaarde bevat,
// en geeft een pointer naar de lege lijst op het einde als de sleutel niet
// voorkomt.
    protected: const Lijst* zoek(const T&) const;
    protected: Lijst* zoek(const T&);
//preconditie zoekgesorteerd: lijst is gesorteerd
//teruggeefwaarde: wijst naar Lijst waar sleutel staat/zou moeten staan.
    protected: Lijst<T>* zoekGesorteerd(const T& sleutel);

//uitschrijf- en tekenoperaties
//dotformaat:
    public: void teken(const char * bestandsnaam)const;
//uitschrijven: voor elke knoop de T-waarde, gescheiden door komma's
    friend ostream& operator<<(ostream& os, const Lijst& l);
    public: void schrijf(ostream & os) const;

//iterator; gaat ervan uit dat alles const is
	public: class iterator {
		public:
			typedef std::forward_iterator_tag iterator_category;
			typedef Lijstknoop<T> value_type;
			typedef std::ptrdiff_t difference_type;
			typedef Lijstknoop<T>* pointer;
			typedef Lijstknoop<T>& reference;

		private:
			Lijstknoop<T> * knoop;
			
		public:
			iterator(Lijstknoop<T> * l = 0) {
				this->knoop = l;
			}

			const T& operator*() const {
				return knoop->sleutel;
			}

			const iterator& operator++() {
				knoop = (knoop->volgend).get();
				return *this;
			}

			bool operator==(const iterator& i) {
				return knoop == i.knoop;
			}

			bool operator!=(const iterator& i) {
				return knoop != i.knoop;
			}
    };

	iterator begin() const {
		iterator start(this->get());
		return start;
	}
	iterator end() const {
		Lijstknoop<T> * hulpknoop = this->get();
		while (hulpknoop != nullptr) {
			hulpknoop = (hulpknoop->volgend).get();
		}
		iterator final(hulpknoop);
		return final;
	}
};

template<class T>
class Lijstknoop{
    friend class Lijst<T>;
    public:
        Lijst<T> volgend;
        Lijstknoop(const T&);
#ifdef DEBUG
        ~Lijstknoop();
#endif
    protected:
        T sleutel;
#ifdef DEBUG
public:
        static bool controle (int gemaakt, int verwijderd);
protected:
        static int aantalGemaakt;
        static int aantalVerwijderd;
#endif
};

template<class T>
int Lijstknoop<T>::aantalGemaakt=0;

template<class T>
int Lijstknoop<T>::aantalVerwijderd=0;

template<class T>
Lijstknoop<T>::Lijstknoop(const T& _sl):sleutel(_sl){
//    std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt gemaakt\n";
    aantalGemaakt++;
}
#ifdef DEBUG

template<class T>
Lijstknoop<T>::~Lijstknoop(){
//    std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt verwijderd\n";
    aantalVerwijderd++;
}

template<class T>
bool Lijstknoop<T>::controle (int gemaakt, int verwijderd){
	if (aantalGemaakt == gemaakt && aantalVerwijderd == verwijderd){
		std::cerr << "Correcte controle\n";
		return true;
	}
    else{
        std::cerr<<"Fout bij controle:\n";
        std::cerr<<"Aantal gemaakte knopen   : "<<aantalGemaakt<<" (moet zijn: "<<gemaakt<<")\n";
        std::cerr<<"Aantal verwijderde knopen: "<<aantalVerwijderd<<" (moet zijn: "<<verwijderd<<")\n";
        throw "Mislukte controle";
    }
};
#endif

template<class T>
ostream& operator<<(ostream& os,const Lijst<T>& l){
#ifdef ITERATOR
    for (auto&& sleutel: l)
        os<<sleutel<<", ";
#else
    if (l.get()){
        os<<l.get()->sleutel<<", ";
        os<<l.get()->volgend;
    }
#endif
    return os;
}

template<class T>
void Lijst<T>::schrijf(ostream & os) const{
#ifdef ITERATOR
    if (this->get()!=0){
        os<<this->get()->sleutel;
        std::for_each (++begin(),end(),[&](const T& sleutel){ os<<" . "<<sleutel;} );
    }
#else
	{
        Lijstknoop<T> * kn=this->get();
        if (kn!=0){
            os<<kn->sleutel;
            kn=kn->volgend.get();
        };
        while (kn != 0){
            os<<" . "<<kn->sleutel;
            kn=kn->volgend.get();
        };
    }
#endif
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------

//oplossing:

template<class T>
bool Lijst<T>::isClone(const Lijst<T>& ander) const{
    const Lijst<T>* l1=this, *l2=&ander;//twee lopers
    while (*l1 && *l2 && (*l1)->sleutel == (*l2)->sleutel){
        l1=&((*l1)->volgend);
        l2=&((*l2)->volgend);
    };
    return(!(*l1) && !(*l2));
};

template<class T>
const Lijst<T>* Lijst<T>::zoek(const T& sleutel) const{
    const Lijst<T>* pl=this;
    while (*pl && pl->get()->sleutel !=sleutel)
        pl=&(pl->get()->volgend);
    return pl;
}

template<class T>
int Lijst<T>::geefAantal(const T& sleutel) const{
    int aantal=0;
    const Lijst<T>* pl=this;
    while (*pl){
        if (sleutel=pl->sleutel)
            ++aantal;
        pl=&(pl->get()->volgend);
    };
    return aantal;
};

template<class T>
int Lijst<T>::geefAantal() const{
    int aantal=0;
    const Lijst<T>* pl=this;
    while (*pl){
        ++aantal;
        pl=&(pl->get()->volgend);
    };
    return aantal;
};

template<class T>
Lijst<T>* Lijst<T>::zoek(const T& sleutel){
    Lijst* pl=this;
    while (*pl && pl->get()->sleutel !=sleutel)
        pl=&(pl->get()->volgend);
    return pl;
}

//Eigen code!! -----------------------------------------------------------------------------------------------------

template<class T>
Lijst<T>::Lijst() : std::unique_ptr<Lijstknoop<T>>()
{
	//cerr << "Usage of default constructor\n";
	//Empty body
}

template<class T>
Lijst<T>::Lijst(Lijst<T> && lijst) : std::unique_ptr<Lijstknoop<T>>(move(lijst))
{
	//cerr << "Usage of move constructor\n";
	//Empty body
}

template<class T>
Lijst<T> & Lijst<T>::operator=(const Lijst<T> & lijst)
{
	//cerr << "Usage of copy operator\n";
	if (&lijst != this) {
		this->reset();
		Lijst<T> * hulp = this;
		#ifdef ITERATOR
		for (auto&& sleutel : lijst) {
			hulp->voegToe(sleutel);
			hulp = &((hulp->get())->volgend);
		}
		#else
		Lijstknoop<T> * hulpknoop = lijst.get();
		while (hulpknoop != nullptr) {
			hulp->voegToe(hulpknoop->sleutel); //Voeg knoop toe
			hulp = &((hulp->get())->volgend); //Zet hulppointer opnieuw achteraan
			hulpknoop = (hulpknoop->volgend).get(); //Neem volgende knoop van de te copiëren lijst
		}
		#endif
	}
	return *this;
}

template<class T>
Lijst<T> & Lijst<T>::operator=(Lijst<T> && lijst)
{
	//cerr << "Usage of move operator 1\n";
	Lijstknoopptr<T>::swap(lijst);
	return *this;
}

template<class T>
Lijst<T>& Lijst<T>::operator=(Lijstknoopptr<T>&& lijst)
{
	//cerr << "Usage of move operator 2\n";
	Lijstknoopptr<T>::swap(lijst);
	return *this;
}

//----------------------------------------------------------------------------------------------------------------

template<class T>
void Lijst<T>::voegToe(const T& sleutel){
    Lijstknoopptr<T> nieuw=std::make_unique<Lijstknoop<T>>(sleutel);
    Lijstknoopptr<T>::swap(nieuw->volgend);
    *this = std::move(nieuw);
}

template<class T>
void Lijst<T>::verwijderEerste(){
    if (this->get()!=0){
        Lijstknoopptr<T> staart(std::move(this->get()->volgend));
        this->reset();
        Lijstknoopptr<T>::swap(staart);
    }
}

template<class T>
void Lijst<T>::verwijder(const T& sleutel){
    zoek(sleutel)->verwijderEerste();
}

template<class T>
Lijst<T>* Lijst<T>::zoekGesorteerd(const T& sleutel){
    Lijst* plaats=this;
    while (*plaats && plaats->get()->sleutel < sleutel)
        plaats=&plaats->get()->volgend;
    return plaats;
};

template<class T>
void Lijst<T>::insertionsort(){
    Lijstknoopptr<T> ongesorteerd=std::move(*this);
    while (ongesorteerd){
        Lijst *plaats=zoekGesorteerd(ongesorteerd.get()->sleutel);
        Lijstknoopptr<T> dummy=std::move(ongesorteerd);
        //vermits ongesorteerd een nullpointer is, is het equivalent van volgende lijnen ongeveer
        //ongesorteerd=std::move(dummy.get()->volgend);
        //std::swap(*plaats,dummy.get()->volgend);
        std::swap(ongesorteerd,dummy.get()->volgend);
        dummy.get()->volgend=std::move(*plaats);
        *plaats=std::move(dummy);
    };
};

template<class T>
void Lijst<T>::teken(const char * bestandsnaam) const{
    ofstream uit(bestandsnaam);
    assert(uit);
    uit<<"digraph {\nrankdir=\"LR\";\n\"0\"[label=\"\",shape=diamond];\n\"0\" -> \"1\";\n";
    int knoopteller=1;//knopen moeten een eigen nummer krijgen.
    const Lijst<T>* loper=this;
    while (*loper){
        uit<<"subgraph cluster_"<<knoopteller<<" {\nrankdir=\"LR\";\n";
        uit<<"\""<<knoopteller<<"\" [label=\""<<(*loper)->sleutel<<"\",color=white];\n";
        uit<<"\""<<knoopteller<<"v\" [shape=diamond,label=\"\"];\n";
        uit<<"\""<<knoopteller<<"\" -> \""<<knoopteller<<"v\" [color=white];\n";

        uit<<"}\n";
        uit<<"\""<<knoopteller<<"v\" -> \""<<knoopteller+1
           <<"\" [lhead=cluster_"<<knoopteller<<" ltail=cluster_"<<knoopteller+1<<"];\n";
        loper=&((*loper)->volgend);
        knoopteller++;
    };
    uit<<"\""<<knoopteller<<"\" [shape=point];\n";
    uit<<"}";
};

#endif
