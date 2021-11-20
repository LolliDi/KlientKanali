#include <Windows.h>
#include <stdio.h>
#define SIZE_BUFFER 140
#define _CRT_SECURE_NO_WARNINGS

int main()
{
    system("chcp 1251>nul");
    HANDLE hNamedPipe; //дескриптор канала
    LPSTR lpszPipeName = L"\\\\.\\pipe\\MyPipe";//имя канала
    BOOL Zap = TRUE;
    DWORD sizeBuffer = SIZE_BUFFER;
    DWORD actualwriten;
    DWORD dwMode = PIPE_READMODE_MESSAGE;
    BOOL isSuccess;
    LPWSTR buffer;
    while (TRUE)
    {
        hNamedPipe = CreateFile(lpszPipeName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
       
        isSuccess = SetNamedPipeHandleState(hNamedPipe, &dwMode, NULL, NULL);
        if (!isSuccess)
        {
            printf("\nСервер не отвечает!\n");
            Sleep(1000);
        }
        else 
        {
            if (Zap)
            {
                printf("\nСоединение установлено!\n");
                Zap = FALSE;
            }
           
            char message[SIZE_BUFFER];
            printf("Введите сообщение для сервера:\n");
            gets(message);
            LPWSTR buffer = &message;
            WriteFile(hNamedPipe, buffer, sizeBuffer, &actualwriten, NULL);

        }
        Sleep(50);
        CloseHandle(hNamedPipe);
    }
}
