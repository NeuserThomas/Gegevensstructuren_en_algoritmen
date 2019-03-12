#ifndef __SORTVECTOR
#define __SORTVECTOR
/**
 \class sortvector
 \brief is een klasse van sorteerbare vectoren
 Bovenop de vectoreigenschappen zijn er hulpfuncties om sorteervergelijkingen
 te vergemakkelijken.
*/
#include <iostream>
   using std::istream;
   using std::ostream;
    using std::move;
    using std::swap;
#include <iomanip>   // voor setw
#include <cstdlib>   // voor rand - opletten!! 
#include <vector>
    using std::vector;

template<class T>
class Sortvector:public vector<T>{
  public:

    /// \fn Constructor: het argument geeft de grootte aan
    /// bij constructie zal de tabel opgevuld worden met
    /// n verschillende Ts in random volgorde
    /// (zie hulplidfuncties)
    Sortvector(int);
    
    Sortvector(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(Sortvector<T>&& v) = delete;
    Sortvector(Sortvector<T>&& v) = delete;

    /// \fn vul_range vul vector met de waarden T(0)...T(size()-1) in volgorde
    void vul_range(); //oke
    void draai_om(); //oke
    void vul_omgekeerd(); //oke
    void shuffle(); //oke
    void vul_random_zonder_dubbels();//oke
    void vul_random(); //< nog niet implementeren
    
    bool is_gesorteerd() const;//oke
    /// \fn is_range controleert of *this eruit ziet als het resultaat van vul_range(), d.w.z.
    /// dat, voor alle i, (*this)[i]==T(i);
    bool is_range() const;//oke

    friend ostream& operator<<(ostream& os, const Sortvector<T>& s){
        s.schrijf(os);
        return os;
    }

  private: 
    void schrijf(ostream & os)const;
    
};


template <class T>
void Sortvector<T>::schrijf(ostream & os)const{
    for(auto&& t : *this){
        os<<t<<" ";
    }
    os<<"\n";
}


template <class T>
Sortvector<T>::Sortvector(int l){//l is lengte
        this->reserve(l);
        for (int i=0; i<l; i++ ){
            this->emplace_back(i);
        }
        shuffle();
    };
    

template <class T>
void Sortvector<T>::shuffle(){
	
//	std::random_shuffle(this->begin(), this->end());

for(int i=0; i<this->size()-2; i++){
	int j = i + rand()%(this->size()-i);
	swap(this->at(i), this->at(j));
}
	
}

template <class T>
void Sortvector<T>::vul_range(){
	
	int n = this->size();
	
	for(int i=0; i< n; i++){
		this->at(i) = i;
	}
	
}

template <class T>
void Sortvector<T>::vul_omgekeerd(){
	
	int n = this->size();
	for(int i= 0; i<n ; i++){
		this->at(i) = n-i-1;
	}
	
}

template <class T>
void Sortvector<T>::draai_om(){
	
	int n = this->size();
	
	for(int i=0; i<n/2; i++){
		
		swap(this->at(i), this->at(n-1-i));
		
	}
	
}

template <class T>
bool Sortvector<T>::is_gesorteerd() const{

	int n = this->size();
	for(int i = 1; i<n; i++){
		
		if(this->at(i-1) > this->at(i)){
			return false;
		}
		return true;
		
	}

}

template <class T>
bool Sortvector<T>::is_range() const{
	
	int n = this->size();
	
	for(int i = 0; i<n ; i++){
		
		if(this->at(i) != i){
			return false;
		}
		return true;
		
	}
	
}

template <class T>
void Sortvector<T>::vul_random(){
	
}

template <class T>
void Sortvector<T>::vul_random_zonder_dubbels(){
	
	this->vul_range();
	this->shuffle();
	
}


#endif
