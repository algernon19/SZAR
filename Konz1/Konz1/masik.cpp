#include <iostream>
using namespace std;

int main()
{
	//ciklusok
	const int elemszam = 10;
	int tomb[elemszam];
	//feladat: t�lts�k fel �s �rassuk ki a t�mb elemeit a k�perny�re
	const char* _szoveg = "%d\n";//printf-hez
	//1.r�sz: t�mb felt�lt�se ciklussal
	_asm {
		mov ecx, elemszam;
		mov eax, 0;//t�mb index
	ciklus_feltolt_1:
		mov tomb[eax], ecx;//felt�lt�s
		add eax, 4;//k�vetkez� t�mbelem
		loop ciklus_feltolt_1;//ciklus v�ge
	}
	//2.r�sz: ki�r�s a k�perny�re
	_asm {
		mov ecx, elemszam;
		mov esi, 0;//t�mbindex
	ciklus_kiir_1:
		push ecx;//ciklusv�ltoz� a veremben
		mov edi, esp;
		//ki�r�s
		push tomb[esi];
		push _szoveg;
		call dword ptr printf;
		//ciklus v�ge
		add esi, 4;
		mov esp, edi;
		pop ecx;//ecx visszaall�tasa
		loop ciklus_kiir_1;
	}
	//Lebeg�pontos 
	//Feladat: Sz�moljuk ki a k�r ker�let�t �s ter�let�t
	float kerulet, terulet;
	int sugar;
	const char* be_printf = "Kerem a kor sugarat! : ";
	const char* be_scanf = "%d";
	const char* ki_printf = "A kor kerulete: %f �s terulete %f\n";
	//bek�r�s
	_asm {
		//esp ment�s
		mov edi,esp
		//printf
		push be_printf;
		call dword ptr printf;
		//scanf
		lea eax, [sugar];
		push eax;
		push be_scanf;
		call dword ptr scanf;
		//esp vissza�ll�t�s
		mov esp, edi;
	}
	//ker�let kisz�m�t�sa
	_asm {
	//ker�let v�ltoz� null�z�sa
		FLDZ;// st(0)-ba 0.0 ker�l
		FSTP kerulet; //ST0 kiment�se a ker�letbe, majd pop st0
		//ker�let kisz�m�t�sa
		fild sugar;//ST0-ba a sug�r
		FADD sugar;//ST0=ST0+sug�r
		FLDPI;
		FMULP ST(1), ST(0);
		//eredm�ny kiment�se
		fstp kerulet;
	}
	//terulet
	_asm {
		//ter�let kinull�z�s
		fldz;
		fstp terulet;
		//ter�let kisz�m�t�s
		FLDPI; //ST0-ba ker�l a PI �rt�ke
		FIMUL sugar;
		FIMUL sugar;
		//v�ltoz� kiment�se
		fstp terulet;
	}
	//eredm�nyek ki�r�sa
	_asm {
		mov edi, ESP;//ESP ment�s
		//printf(ki_printf,kerulet,terulet);		
		//lebeg�pontos �rt�k a verembe 1-ter�let
		fld terulet;
		sub esp, 8;
		fstp qword ptr[esp];
		//lebeg�pontos �rt�k a verembe 2.a ker�let
		fld kerulet;
		sub esp, 8;
		fstp qword ptr[esp];
		//form�z� string
		push ki_printf;
		//printf megh�v�sa
		call dword ptr printf;
		mov esp, edi;//ESP vissza�l�lt�s
	}

	system("pause");
	return 0;
}