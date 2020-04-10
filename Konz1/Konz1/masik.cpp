#include <iostream>
using namespace std;

int main()
{
	//ciklusok
	const int elemszam = 10;
	int tomb[elemszam];
	//feladat: töltsük fel és írassuk ki a tömb elemeit a képernyõre
	const char* _szoveg = "%d\n";//printf-hez
	//1.rész: tömb feltöltése ciklussal
	_asm {
		mov ecx, elemszam;
		mov eax, 0;//tömb index
	ciklus_feltolt_1:
		mov tomb[eax], ecx;//feltöltés
		add eax, 4;//következõ tömbelem
		loop ciklus_feltolt_1;//ciklus vége
	}
	//2.rész: kiírás a képernyõre
	_asm {
		mov ecx, elemszam;
		mov esi, 0;//tömbindex
	ciklus_kiir_1:
		push ecx;//ciklusváltozó a veremben
		mov edi, esp;
		//kiírás
		push tomb[esi];
		push _szoveg;
		call dword ptr printf;
		//ciklus vége
		add esi, 4;
		mov esp, edi;
		pop ecx;//ecx visszaallítasa
		loop ciklus_kiir_1;
	}
	//Lebegõpontos 
	//Feladat: Számoljuk ki a kör kerületét és területét
	float kerulet, terulet;
	int sugar;
	const char* be_printf = "Kerem a kor sugarat! : ";
	const char* be_scanf = "%d";
	const char* ki_printf = "A kor kerulete: %f és terulete %f\n";
	//bekérés
	_asm {
		//esp mentés
		mov edi,esp
		//printf
		push be_printf;
		call dword ptr printf;
		//scanf
		lea eax, [sugar];
		push eax;
		push be_scanf;
		call dword ptr scanf;
		//esp visszaállítás
		mov esp, edi;
	}
	//kerület kiszámítása
	_asm {
	//kerület változó nullázása
		FLDZ;// st(0)-ba 0.0 kerül
		FSTP kerulet; //ST0 kimentése a kerületbe, majd pop st0
		//kerület kiszámítása
		fild sugar;//ST0-ba a sugár
		FADD sugar;//ST0=ST0+sugár
		FLDPI;
		FMULP ST(1), ST(0);
		//eredmény kimentése
		fstp kerulet;
	}
	//terulet
	_asm {
		//terület kinullázás
		fldz;
		fstp terulet;
		//terület kiszámítás
		FLDPI; //ST0-ba kerül a PI értéke
		FIMUL sugar;
		FIMUL sugar;
		//változó kimentése
		fstp terulet;
	}
	//eredmények kiírása
	_asm {
		mov edi, ESP;//ESP mentés
		//printf(ki_printf,kerulet,terulet);		
		//lebegõpontos érték a verembe 1-terület
		fld terulet;
		sub esp, 8;
		fstp qword ptr[esp];
		//lebegõpontos érték a verembe 2.a kerület
		fld kerulet;
		sub esp, 8;
		fstp qword ptr[esp];
		//formázó string
		push ki_printf;
		//printf meghívása
		call dword ptr printf;
		mov esp, edi;//ESP visszaálíltás
	}

	system("pause");
	return 0;
}