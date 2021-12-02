#include <Windows.h>
#include <stdio.h>
#define SIZE_BUFFER 140
#define _CRT_SECURE_NO_WARNINGS

DWORD WINAPI Soedin(char* param[2])
{
	HANDLE hNamedPipe;
	LPSTR lpszPipeName = L"\\\\.\\pipe\\MyPipe";
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL isSuccess;
	BOOL Zap = TRUE;
	DWORD actualwriten;
	BOOL SuccessRead;
	DWORD actualreaden;
	LPWSTR buffer = calloc(SIZE_BUFFER, sizeof(CHAR));
	LPWSTR cc = (CHAR*)calloc(500, sizeof(CHAR));
	int i = 6;
	while (TRUE)
	{
		hNamedPipe = CreateFile(lpszPipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		isSuccess = SetNamedPipeHandleState(hNamedPipe, &dwMode, NULL, NULL);
		if (hNamedPipe == INVALID_HANDLE_VALUE) //если сервер не отвечает
		{
			if (i > 2) //если не отвечает больше 5сек
			{
				printf("\nСервер не отвечает!\n");

				Zap = TRUE;
				param[1] = "0"; //соединение на нуле
				param[0] = "";
			}
			i++;
			Sleep(1000); //ждемс
		}
		else
		{
			i = 0;
			if (Zap) //если до этого было отключение/это первое подключение
			{
				printf("\nСоединение установлено!\n");

				Zap = FALSE;
				param[1] = "1"; //нужно записать число
			}
			if (param[0] != "" && param[1] == "1") //если число записано и нужно было его записать
			{
				WriteFile(hNamedPipe, param[0], SIZE_BUFFER, &actualwriten, NULL); //то отправляем
				param[1] = "0"; //число записывать больше не нужно
			}
			if (param[0] != "" && param[1] == "0") //если число записано и его не нужно записывать
			{
				if (ReadFile(hNamedPipe, buffer, SIZE_BUFFER, &actualreaden, NULL))//то пытаемся прочитать ответ
				{
					wsprintf(cc, L"Квадрат = %s\0", buffer); //если прочитали, то выводим
					MessageBox(NULL, cc, L"Ответ", NULL);
					buffer = (CHAR*)calloc(SIZE_BUFFER, sizeof(CHAR));
					cc = (CHAR*)calloc(SIZE_BUFFER, sizeof(CHAR));
					param[0] = ""; //число больше не введено
					param[1] = "1"; //нужно ввести новое
				}
			}
			Sleep(50);
		}
		CloseHandle(hNamedPipe);
	}
	ExitThread(0);
}

int main()
{
	system("chcp 1251>nul");

	HANDLE hNamedPipe; //дескриптор канала
	LPSTR lpszPipeName = L"\\\\.\\pipe\\MyPipe";
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL Zap = TRUE;
	DWORD sizeBuffer = SIZE_BUFFER;




	BOOL otvet = FALSE;



	wchar_t mes = (CHAR*)calloc(500, sizeof(CHAR));
	char message[SIZE_BUFFER];
	BOOL ustanovl = FALSE;
	HANDLE T;
	char* param[2];
	param[0] = calloc(sizeBuffer, sizeof(char));
	param[0] = ""; //тут хранится число, которое передадим на сервер
	param[1] = "0"; //состояние записи
	BOOL zapis = FALSE;
	T = CreateThread(NULL, 0, Soedin, param, 0, 0);
	while (TRUE)
	{
		if (param[1] == "1" && param[0] == "") //если нужно записать число и число ещё не записано
		{
			Sleep(3000); //не знаю почему, но так при одновременном запуске сервера и клиента - не зависает после ввода
						 //не спрашивайте как я до этого дошел...
			printf("Введите цифру:\n"); //записываем
			gets(message);
			param[0] = message;
		}
		Sleep(50);
	}
}