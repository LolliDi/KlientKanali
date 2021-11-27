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
    BOOL otvet=FALSE;
    LPWSTR buffer = (CHAR*)calloc(sizeBuffer, sizeof(CHAR));
    BOOL SuccessRead;
    char message[SIZE_BUFFER];
    while (TRUE)
    {
        hNamedPipe = CreateFile(lpszPipeName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
       
        isSuccess = SetNamedPipeHandleState(hNamedPipe, &dwMode, NULL, NULL);
        if (!isSuccess)
        {
            printf("\nСервер не отвечает!\n");
            Sleep(1000);
            
            Zap = TRUE;
        }
        else 
        {
            if (Zap)
            {
                printf("\nСоединение установлено!\n");
                Zap = FALSE;
                otvet = TRUE;
            }
            if (otvet)
            {
                printf("Введите сообщение для сервера:\n");
                gets(message);
                buffer = &message;
                WriteFile(hNamedPipe, buffer, sizeBuffer, &actualwriten, NULL);
                otvet = FALSE;
            }
            SuccessRead = ReadFile(hNamedPipe, buffer, sizeBuffer, &actualwriten, NULL);
            if (SuccessRead)
            {
                printf("Сервер пишет: ");
                printf(buffer);
                printf("\n");
                otvet = TRUE;
            }

        }
        Sleep(50);
        CloseHandle(hNamedPipe);
    }
}
