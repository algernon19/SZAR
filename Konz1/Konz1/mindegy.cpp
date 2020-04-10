#include <iostream>
#include <ctime>
using namespace std;
int main() {
	//változók létrehozása
	const char* hellovilag = "Hello World!\n"; //printf
	const char* _pause = "pause"; //system
	int _ESP;
	_asm{
	mov _ESP, esp
	}
	//hello világ kiírása
	_asm {
		push hellovilag;
		call dword ptr printf; //printf függvény hívás
	}
	/*
	Hosszunk létre egy 100 elemû tömböt
	Adjuk meg a véletlen szám generátor korlátját 0-tól meddig generáljon
	Számoljuk ki a tömb összegét, átlagát, minimumát.
	Írjuk ki az eredményt a képernyõre.
	*/
	int tomb[100];
	int randmax, osszeg, minimum;
	float int_atlag;
	const char* printf_beker = "Kerem a veletlen maximumat: ";
	const char* scanf_szam = "%d";
	const char* printf_osszeg = "A tomb osszege: %d\n";
	const char* printf_atlag = "A tomb atlaga: %f\n";
	const char* printf_minimum = "A tomb minimuma: %d\n";
	//veletlen szam inicializalas
	//srand
	_asm {
		push 0;
		call dword ptr time; //eax-ben lesz az eredmény
		push eax;
		call dword ptr srand;
		pop eax;
		pop eax;
	}
	//veletlen generalas2.verzio
	_asm {
		RDTSC; //Read time stamp counter - idoszamlalo EDX:EAX
		push eax;
		call dword ptr srand;
		pop eax;
	}
	_asm {
	//printf
		push printf_beker;
		call dword ptr printf;
		//scanf
		//LEA Load Effective Address
		//regiszterbe menti a változó címét
		//scanf(scanf_szam, &randmax);
		lea eax, [randmax];
		push eax;
		//formázó karakter a verembe
		push scanf_szam;
		call dword ptr scanf;
		//ellenõrzés
		/*push randmax;
		push scanf_szam;
		call dword ptr printf;
		*/	
	}
	//tömb feltöltés véletlen számokkal
	_asm {
		mov esi, 1; //ciklusváltozó (1-tõl 100-ig fog menni)
		mov edi, 0; //tömbindex
	ciklus_feltolt:
		//véletlen szám generálás
		//rand()%randmax;
		//1.rand meghívása
		call dword ptr rand;
		//2.lépés: osztás elõkészítés
		mov edx, 0;
		//3.lépés
		div randmax; //maradék az EDX
		//tömbfelöltés
		mov tomb[edi], edx;
		//ciklus vége rész
		add edi, 4; //int 4 bájtos, köv. tömbelem 4 bájtra van
		inc esi;
		cmp esi, 101;
		jne ciklus_feltolt;
		NOP;
	}

	//összegzés
	_asm {
		mov osszeg, 0;
		mov ecx, 100; //100 elemû a tömb
		mov edi, 0; //tömbindex
	ciklus_osszeg:
		mov eax, tomb[edi];
		add osszeg, eax;
		add edi, 4;
		loop ciklus_osszeg;
	}
	//átlag kiszámítása
	_asm {
		//változó nullázása
		fldz;
		fstp int_atlag;
		//átlag számítás
		fild osszeg;
		fild osszeg;
		mov osszeg, 100; //tömb méret eltárolása
		fild osszeg;
		fdivp ST(1), ST(0);
		fstp int_atlag;
		fistp osszeg; //összeg visszaállítása
	}

	//minimumkeresés
	_asm {
		mov edx, 99999; //min kereséshez
		mov ecx, 100; //ciklusváltozó
		mov esi, 0;
	ciklus_minimum:
		mov eax, tomb[esi];
		cmp eax, edx;
		jl felvesz;
		jmp tovabb;
	felvesz:
		mov edx, eax;
	tovabb:
		add esi, 4;
		loop ciklus_minimum;
		mov minimum, edx; //edx-ben a minimum
	}
	//eredmények kiíratása
	_asm {
	//osszeg
		push osszeg;
		push printf_osszeg;
		call dword ptr printf;
		//átlag
		//fload(double) érték a memória verem tetejére
		fld int_atlag;
		sub esp, 8;
		fstp qword ptr[esp];
		push printf_atlag;
		call dword ptr printf;

		//minimum
		push minimum;
		push printf_minimum;
		call dword ptr printf;
	}

	//ellenõrzés c++-ban
	osszeg = 0; minimum = 999999;
	for (size_t i = 0; i < 100; i++)
	{
		osszeg += tomb[i];
		if (tomb[i] < minimum) minimum = tomb[i];
	}
	int_atlag = (float)(osszeg / 100.0);
	cout << osszeg << endl;
	cout << int_atlag << endl;
	cout << minimum << endl;

	//system("pause") meghívása
	__asm {
		push _pause;
		call dword ptr system;
	}
	//ESP visszaállítása
	_asm {
		mov esp, _ESP;
	}
	return 0;
}