#include <iostream>
#include <ctime>
using namespace std;
int main() {
	//v�ltoz�k l�trehoz�sa
	const char* hellovilag = "Hello World!\n"; //printf
	const char* _pause = "pause"; //system
	int _ESP;
	_asm{
	mov _ESP, esp
	}
	//hello vil�g ki�r�sa
	_asm {
		push hellovilag;
		call dword ptr printf; //printf f�ggv�ny h�v�s
	}
	/*
	Hosszunk l�tre egy 100 elem� t�mb�t
	Adjuk meg a v�letlen sz�m gener�tor korl�tj�t 0-t�l meddig gener�ljon
	Sz�moljuk ki a t�mb �sszeg�t, �tlag�t, minimum�t.
	�rjuk ki az eredm�nyt a k�perny�re.
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
		call dword ptr time; //eax-ben lesz az eredm�ny
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
		//regiszterbe menti a v�ltoz� c�m�t
		//scanf(scanf_szam, &randmax);
		lea eax, [randmax];
		push eax;
		//form�z� karakter a verembe
		push scanf_szam;
		call dword ptr scanf;
		//ellen�rz�s
		/*push randmax;
		push scanf_szam;
		call dword ptr printf;
		*/	
	}
	//t�mb felt�lt�s v�letlen sz�mokkal
	_asm {
		mov esi, 1; //ciklusv�ltoz� (1-t�l 100-ig fog menni)
		mov edi, 0; //t�mbindex
	ciklus_feltolt:
		//v�letlen sz�m gener�l�s
		//rand()%randmax;
		//1.rand megh�v�sa
		call dword ptr rand;
		//2.l�p�s: oszt�s el�k�sz�t�s
		mov edx, 0;
		//3.l�p�s
		div randmax; //marad�k az EDX
		//t�mbfel�lt�s
		mov tomb[edi], edx;
		//ciklus v�ge r�sz
		add edi, 4; //int 4 b�jtos, k�v. t�mbelem 4 b�jtra van
		inc esi;
		cmp esi, 101;
		jne ciklus_feltolt;
		NOP;
	}

	//�sszegz�s
	_asm {
		mov osszeg, 0;
		mov ecx, 100; //100 elem� a t�mb
		mov edi, 0; //t�mbindex
	ciklus_osszeg:
		mov eax, tomb[edi];
		add osszeg, eax;
		add edi, 4;
		loop ciklus_osszeg;
	}
	//�tlag kisz�m�t�sa
	_asm {
		//v�ltoz� null�z�sa
		fldz;
		fstp int_atlag;
		//�tlag sz�m�t�s
		fild osszeg;
		fild osszeg;
		mov osszeg, 100; //t�mb m�ret elt�rol�sa
		fild osszeg;
		fdivp ST(1), ST(0);
		fstp int_atlag;
		fistp osszeg; //�sszeg vissza�ll�t�sa
	}

	//minimumkeres�s
	_asm {
		mov edx, 99999; //min keres�shez
		mov ecx, 100; //ciklusv�ltoz�
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
	//eredm�nyek ki�rat�sa
	_asm {
	//osszeg
		push osszeg;
		push printf_osszeg;
		call dword ptr printf;
		//�tlag
		//fload(double) �rt�k a mem�ria verem tetej�re
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

	//ellen�rz�s c++-ban
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

	//system("pause") megh�v�sa
	__asm {
		push _pause;
		call dword ptr system;
	}
	//ESP vissza�ll�t�sa
	_asm {
		mov esp, _ESP;
	}
	return 0;
}