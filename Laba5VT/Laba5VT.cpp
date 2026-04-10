#include <iostream>
#include <ctime>

clock_t CPP_(unsigned char* buffer, unsigned width, unsigned height)
{
	clock_t time;
	time = clock();
	short val;
	unsigned char dif;
	unsigned int y;
	dif = 0;

	// Изменил начальное значение j и шаг цикла
	for (int j = height - 2; j >= 0; j -= 2) {
		y = (j * width) * 4 + 1;

		for ( int i = 0; i < width; y = y + 4, i++) {

			buffer[(j * width + i) * 4 + 0] += 40;
			buffer[(j * width + i) * 4 + 1] += 40;
		}
	}

	time = clock() - time;
	return time;
}


clock_t Cx86_(unsigned char* buffer, unsigned width, unsigned height)
{
	clock_t time;
	time = clock();
	unsigned len = width * height * 4;
	_asm 
	{
		mov edi, dword ptr[buffer] // edi = buffer
		mov ecx, dword ptr[height] // ecx = height
		sub ecx, 2
		mov esi, dword ptr[width] // esi = width
		mov bh, 0
		jmp j_firstpass
		for_j :
		sub ecx, 2
			j_firstpass :
			mov eax, ecx
			cmp eax, 0
			jle j_end //If j<0 j_end
			mov eax, Y
			//imul eax, esi // j*width
			shl eax, 2 // y = j*width*4
			add eax, edi // eax = buffer + y
			add eax, 1 // eax = buffer + y + 1
			mov edx, 0 //i = edx = 0
			jmp i_firstpass
			for_i :
			inc edx
			add eax, 4
			i_firstpass :
			cmp edx, esi //i>?<width
			jae i_end //if(i>=width) -> i_end
			mov bl, byte ptr[eax]
			sub bl, bh
			jnc not_saturated
			mov bl, 0
			not_saturated:
			mov byte ptr[eax], bl
			mov eax, ecx
			imul eax, esi
			shl eax, 2
			add eax, edi
			add eax, edx
			add byte ptr[eax], 40
			add byte ptr[eax + 1], 40
			jmp for_i
			i_end :
		cmp bh, 0xFF//if (dif != 255)
			je for_j
			inc bh
			mov edx, 0
			jmp for_j
			j_end :

		/*mov edi, dword ptr[buffer] // edi = buffer
mov ecx, dword ptr[height] // ecx = height
sub ecx, 2
mov esi, dword ptr[width] // esi = width
mov edx, 0 // обнуление счетчика edx
jmp j_firstpass
for_j:
    sub ecx, 2
    j_firstpass:
        mov eax, ecx
        cmp eax, 0
        jl j_end //If j<0 j_end
        imul eax, ecx
        shl eax, 2
        add eax, edi
        add eax, 1
        mov edx, 0
        jmp i_firstpass
    for_i:
        inc edx
        add eax, 4
        i_firstpass:
        cmp edx, esi
        jae i_end
        mov bl, byte ptr[eax]
        sub bl, bh
        jnc not_saturated
        mov bl, 0
        not_saturated:
        mov byte ptr[eax], bl
        mov eax, ecx
        imul eax, ecx
        imul eax, 4
        shl eax, 2
        add eax, edi
        add eax, edx
		mov eax, edi
        add byte ptr[eax], 40
        add byte ptr[eax + 1], 40
        jmp for_i
        i_end:
    cmp bh, 0xFF
    je for_j
    inc bh
    mov edx, 0
    jmp for_j
    j_end:
	*/


		/*mov ecx, dword ptr[height]; Загрузка значения height в регистр ecx
		sub ecx, 2; Вычитание 2 из значения в регистре ecx
		mov esi, dword ptr[buffer]; Загрузка адреса буфера изображения в регистр esi
		outer_loop :
		cmp ecx, 0; Сравнение значения в регистре ecx с 0
			jl outer_loop_end; Если значение меньше 0, переход к концу внешнего цикла
			mov eax, ecx; Перемещение значения j в регистр eax
			imul eax, dword ptr[width]; Умножение значения в регистре eax на значение width
			sar eax, 2; Умножение результата на 2 (сдвиг вправо на 2 бита)
			add eax, 1; Добавление 1 к результату
			mov ebx, eax; Сохранение результата в регистр ebx(переменная y)
			xor ecx, ecx; Обнуление счетчика i
			inner_loop :
		cmp ecx, dword ptr[width]; Сравнение значения счетчика i с width
			jge inner_loop_end; Если значение больше или равно width, переход к концу внутреннего цикла
			mov al, byte ptr[esi + edx]; Загрузка байта из буфера в регистр al
			add al, 40; Увеличение значения на 40
			mov byte ptr[esi + ebx], al; Запись из регистра al в буфер
			inc ecx; Увеличение счетчика i на 1
			; add byte ptr[esi + ebx], 40; Увеличение значения в буфере на 40 (канал R)
			; add byte ptr[esi + ebx + 1], 40; Увеличение значения в буфере на 40 (канал G)
			add ebx, 4; Увеличение значения y на 4
			inc ecx; Увеличение счетчика i на 1
			jmp inner_loop; Переход к началу внутреннего цикла
			inner_loop_end :
		sub ecx, 2; Вычитание 2 из значения в регистре ecx
			add esi, dword ptr[width]; Переход на следующую строку в буфере
			add esi, dword ptr[width]; Переход на следующую строку в буфере
			jmp outer_loop; Переход к началу внешнего цикла
			outer_loop_end : */

	}
	time = clock() - time;
	return time;
}

clock_t CmmX(unsigned char* buffer, unsigned width, unsigned height)
{
	clock_t time;
	const char* address = "%p\n";
	time = clock();
	_asm {
		/*mov ebx, 0x00010001
		movd mm3, ebx
		mov ebx, 0x00000000
		movd mm0, ebx
		punpcklbw mm0, mm3
		pxor mm1, mm1
		mov edi, dword ptr[buffer] // edi = buffer
		mov ecx, dword ptr[height] // ecx = height
		shr ecx, 1
		mov esi, dword ptr[width] // esi = width
		jmp j_firstpass
		for_j :
		dec ecx
			j_firstpass :
		mov eax, ecx
			cmp eax, 0
			jl j_end
			imul eax, esi // j*width
			shl eax, 2 // y = j*width*4
			add eax, edi // eax = buffer + y
			mov edx, 0 //i = edx = 0
			jmp i_firstpass
			for_i :
		add edx, 2
			add eax, 8
			i_firstpass :
			cmp edx, esi //i>?<width
			jae i_end //if(i>=width) -> i_end
			movq mm2, [eax]
			psubusb mm2, mm1
			movq[eax], mm2
			jmp for_i
			i_end :
		paddusb mm1, mm0
			jmp for_j
			j_end :
		emms //mmX
		*/
	}
	time = clock() - time;
	return time;
}

void SafeOpen(FILE*& f, const char* name, const char* mode)
{
	fopen_s(&f, name, mode);
	if (f == NULL) {
		printf("Error: Can't open %s with mode %s\n", name, mode);
		exit(-2);
	}
}

unsigned char* ReadFile(const char* name, unsigned char* header)
{
	FILE* in;
	unsigned char* buffer;
	unsigned int width ;
	unsigned int height;
	SafeOpen(in, name, "rb");
	fread(header, 1, 54, in);
	width = *(unsigned int*)(header + 18);
	height = *(unsigned int*)(header + 22);
	buffer = (unsigned char*)malloc(width * height * 4);
	fread(buffer, 1, width * height * 4, in);
	fclose(in);
	printf("width = %d\nheight = %d\n", width, height);
	return buffer;
}

void SaveFile(const char* name, unsigned char* header, unsigned char* buffer, unsigned int len)
{
	FILE* out;
	SafeOpen(out, name, "wb");
	fwrite(header, 1, 54, out);
	fwrite(buffer, 1, len, out);
	fclose(out);
}


int main()
{
	setlocale(LC_ALL, "Rus");
	clock_t x86 = 0, mmX = 0, CPP = 0;
	double time1, time2, time3;
	int times = 100;

	unsigned char* buffer_original, * bufferCPP;
	unsigned int width, height, len;
	unsigned char headers[54];
	buffer_original = ReadFile("C:\\Users\\qshes\\Desktop\\Laba5VT\\outNature.bmp", headers);
	width = *(unsigned int*)(headers + 18);
	height = *(unsigned int*)(headers + 22);

	len = width * height * 4;
	bufferCPP = (unsigned char*)malloc(len);
	for (int i = 0; i < times; i++) {
		memcpy(bufferCPP, buffer_original, len); //Функция memcpy копирует num байтов первого блока памяти,
		//на который ссылается указатель srcptr, во второй блок памяти, на который ссылается указатель destptr.
		CPP += CPP_(bufferCPP, width, height);

		memcpy(bufferCPP, buffer_original, len);
		x86 += Cx86_(bufferCPP, width, height);

		memcpy(bufferCPP, buffer_original, len);
		mmX += CmmX(bufferCPP, width, height);
	}

	time1 = ((double)CPP / CLOCKS_PER_SEC) / times;//avg atempts
	time2 = ((double)x86 / CLOCKS_PER_SEC) / times;
	time3 = ((double)mmX / CLOCKS_PER_SEC) / times;

	printf("\nСреднее за %d раз(-а):\nC++:%lfс x86:%lfс MMX:%lfс\n", times, time1, time2, time3);

	memcpy(bufferCPP, buffer_original, len);
	CmmX(bufferCPP, width, height);
	SaveFile("C:\\Users\\qshes\\Desktop\\Laba5VT\\outMMX.bmp", headers, bufferCPP, len);

	memcpy(bufferCPP, buffer_original, len);
	CPP_(bufferCPP, width, height);
	SaveFile("C:\\Users\\qshes\\Desktop\\Laba5VT\\outCPP.bmp", headers, bufferCPP, len);

	memcpy(bufferCPP, buffer_original, len);
	Cx86_(bufferCPP, width, height);
	SaveFile("C:\\Users\\qshes\\Desktop\\Laba5VT\\out_x86.bmp", headers, bufferCPP, len);

	//free memory
	free(bufferCPP);
	free(buffer_original);
	return 0;
}

/*
#include "stdafx.h"
#include <stdlib.h>
#include <conio.h>
#include <mmintrin.h>
#include <time.h>
#include <iostream>
#include <array>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* in, * out;
	unsigned __int8* buffer1;
	unsigned __int32 wight, height;
	fopen_s(&in, "file.bmp", "rb");
	if (in == NULL)
	{
		printf("Error: Can't open file.bmp for reading\n");
		return 0;
	}
	buffer1 = (unsigned __int8*)malloc(54);

	fread(buffer1, 1, 54, in);
	for (int i = 0; i <= 3; i++)
	{
		wight = buffer1[21 - i] | wight << 8;
		height = buffer1[25 - i] | height << 8;
	}

	unsigned __int32 size;
	for (int i = 0; i <= 3; i++)
		size = buffer1[5 - i] | size << 8;


	printf("size = %d\n", size);
	printf("wight = %d\n", wight);
	printf("height = %d\n", height);

	fopen_s(&out, "out.bmp", "wb");
	if (out == NULL)
	{
		printf("Error: Can't open file %s for writing\n", "out.bmp");
		return 0;
	}
	fwrite(buffer1, 1, 54, out);

	free(buffer1);
	unsigned __int32 MasSize = wight * height * 1.5;
	unsigned __int32 MasSize2 = wight * height * 3;

	buffer1 = (unsigned __int8*)malloc(MasSize2);
	unsigned __int8* buffer2 = (unsigned __int8*)malloc(MasSize2);
	fread(buffer1, 1, MasSize2, in);

	for (int i = 0; i <= MasSize2; i++)
	{
		buffer2[i] = buffer1[i];
	}


	//Часть 1
	time_t start;
	int time_mmx = 0;
	for (int i = 1; i <= 100; i++) //Подсчет времени
	{
		for (int i = 0; i <= MasSize2; i++)
		{
			buffer1[i] = buffer2[i];
		}

		start = clock();

		for (int i = 0; i <= MasSize; i += 3)
		{
			buffer1[i + 1] /= 2;
		}

		time_mmx += clock() - start;
	}
	cout << endl << "\tExecution time: " << time_mmx << endl;
	//___________________________

	//Часть 2
	time_t start;
	int time_mmx = 0;
	for (int i = 1; i <= 100; i++) //Подсчет времени
	{

		for (int i = 0; i <= MasSize2; i++)
		{
			buffer1[i] = buffer2[i];
		}

		start = clock();

		__asm
		{
			mov esi, buffer1;
			add esi, 1;
			mov ecx, MasSize;
		FOR:
			mov al, byte ptr[esi];
			shr al, 1;
			mov byte ptr[esi], al
				add esi, 3;
			sub ecx, 2;
			loop FOR;
		}
		time_mmx += clock() - start;
	}
	cout << endl << "\tExecution time: " << time_mmx << endl;
	//________________________________

	//Часть 3
	time_t start;
	__int64 mask = 0xFF0000FF0000FF00;
	int time_mmx = 0;
	for (int i = 1; i <= 100; i++) //Подсчет времени
	{

		for (int i = 0; i <= MasSize2; i++)
		{
			buffer1[i] = buffer2[i];
		}

		start = clock();

		__asm
		{
			mov esi, buffer1;
			mov ecx, MasSize;

		FOR:
			movq mm0, [esi];
			pand mm0, mask;

			movq mm1, [esi];

			psrl mm0, 1;
			pand mm0, mask;

			psubusb mm1, mm0;

			movq[esi], mm1;

			add esi, 9;
			sub ecx, 9;
			cmp ecx, 0;
			jge FOR;
			jmp END;

		END:
		}

		time_mmx += clock() - start;

	}
	cout << endl << "\tExecution time: " << time_mmx << endl;
	//______________________________

	fclose(in);
	fwrite(buffer1, 1, wight * height * 3, out);
	fclose(out);
	free(buffer1);
	free(buffer2);
	_getch();
	return 0;
}
*/