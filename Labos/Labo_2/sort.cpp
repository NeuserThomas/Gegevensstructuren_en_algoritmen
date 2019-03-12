#include "sortvector.h"
#include "sorteermethode.h"



using std::cout;
using std::endl;
using std::cin;


int main(int argc, char** argv){
	
/*	Sortvector<double> s(10);
	s.vul_random_zonder_dubbels();
	cout << s << endl;
	cout << s.is_range() << endl;
*/	

	Sortvector<int> s(30);
	s.vul_random_zonder_dubbels();
	
	//cout << "Beginvector: " << s << endl;
	STLSort<int> sorter;
	sorter(s);
	//cout<< "Na STLSort: " << s << endl;
	//InsertionSort<int> sorter2;
	//s.vul_random_zonder_dubbels();
	//sorter2(s);
	//cout<<"Na insertion sort: " << s << endl;
	
	sorter.meet(10, 10000000000, std::cout);
	
	return 0;
	
	
}
