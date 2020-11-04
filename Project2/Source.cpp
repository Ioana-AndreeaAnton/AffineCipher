#include<iostream>
#include<fstream>

using namespace std;

#define MyInt int

char caracter[100] = { 0 };
int N = 0;

struct Bloc {
	char c;//caracterul
	int nr;//numarul de aparitii ale caracterului
};

int modulo(MyInt k, MyInt n) {
	if (k < 0) k = n - (-k) % n;
	if (k >= n) return k % n;
	return k;
}

int invers(MyInt a, MyInt n) {
	MyInt q, r, x0 = 1, x1 = 0, copy_n = n;
	//ne folosim de Extended Euclidean Algorithm in care am initializat x0=1 si x1=0
	a = modulo(a, n);
	while (n != 0) {
		r = n;
		q = a / n;
		n = a % n;
		a = r;

		r = x1;
		x1 = x0 - q * x1;
		x0 = r;
	}
	if (a == 1)
		return modulo(x0, copy_n);
	return -1;
}

//functie pentru codul caracterului c din tabloul character
int da_cod(char c) {
	for (int i = 0; i < N; i++)
		if (caracter[i] == c) return i;
	return -1;
}

//da caracterul corespunzator codului dat
char da_caracter(int cod) {
	return caracter[modulo(cod, N)];
}



//functie pentru citirea din fisierul alfabet.txt
void citeste_alfabet() {
	N = 0;
	ifstream in("alfabet.txt");
	if (!in.good()) //the stream is good enoght to work
		perror("Fisier inexistent");//print an error message
	char c;
	while (in >> noskipws >> c) {
		//noskipws=do not skip whitespaces
		caracter[N] = c;
		N++;
	}
	if (N == 0)
		cout << "Alfabetul dat are 0 caractere" << endl;
}

//analiza frecventelor pentru blocuri de lungime 1
void frecvente(ifstream &in, Bloc bloc[]) {
	//Nu uita! Structura Bloc are un char si un int
	//pentru fiecare caracter exista un numar care indica frecventa lui de aparitie 
	for (int i = 0; i < N; i++) {
		bloc[i].c = da_caracter(i);
		bloc[i].nr = 0;//initializam fiecare frecventa cu 0
	}
	char c;
	while (in >> noskipws >> c) {
		int cod = da_cod(c);
		if (cod >= 0)
			bloc[cod].nr++;
	}
	int ordonat = 0;//ordonam descrescator dupa numarul de aparitii
	while (!ordonat) {
		ordonat = 1;
		for (int i = 0; i < N - 1; i++) {
			if (bloc[i].nr < bloc[i + 1].nr) {
				ordonat = 0;
				Bloc temp = bloc[i];
				bloc[i] = bloc[i + 1];
				bloc[i + 1] = temp;
			}
		}
	}
}
void blocuri1(int s1, int s2, int d1, int d2,int &a, int &b) {
	int inv = invers(s1 - s2, N);
	if (inv == 0) return;
	a = modulo(inv*(d1 - d2), N);
	b = modulo(-s1*a+d1, N);
}

void main() {
	int a, b;
	int cod_a, cod_e, cod_sp, cod_P, cod_pct, cod_q;
	Bloc bloc[100];
	citeste_alfabet();
	ifstream in("sursa.txt");
	if (!in.good())
		perror("Fisier inexistent");
	
	//analizam frecventele
	frecvente(in, bloc);
	for (int i = 0; i < N; i++) {
		cout << bloc[i].c << " " << bloc[i].nr << endl;
	}
	cout << endl;
	
	char ch;
	ifstream alf("alfabet.txt");
	while (alf >> noskipws >> ch){
		if (ch == 'a') cod_a = da_cod(ch);
		if (ch == 'e') cod_e = da_cod(ch);
		if (ch == ' ') cod_sp = da_cod(ch);
		if (ch == 'P') cod_P = da_cod(ch);
		if (ch == '.') cod_pct = da_cod(ch);
		if (ch == 'q') cod_q = da_cod(ch);
	}
	//de verificare
	cout << "a are codul " << cod_a << endl;
	cout << "e are codul " << cod_e << endl;
	cout << "  are codul " << cod_sp << endl;
	cout << "P are codul " << cod_P << endl;
	cout << ". are codul " << cod_pct << endl;
	cout << "q are codul " << cod_q << endl;

	blocuri1(cod_P, cod_pct, cod_sp, cod_a, a, b);
	cout << endl;
	cout << "Cheia de criptare" << endl;
	cout << "a= " << a << endl;
	cout << "b= " << b << endl;
	cout << endl;
	
	ifstream citire("sursa.txt");
	ofstream out("destinatie.txt");
	if (!citire.good())
		perror("Fisier inexistent");
	char c;
	while (citire >> noskipws >> c) {
		out << da_caracter(da_cod(c)*a + b);
	}

	in.close();
	out.close();

	cout << "Gata" << endl;

	system("pause");
}