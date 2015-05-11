// c123.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "client.h"
#include <afxsock.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
		}
		CSocket client;
		
		AfxSocketInit(NULL);

		client.Create();
		
		unsigned int port = 1234;
		printf("Nhap dia chi ket noi: ");
		char ip[13];
		gets(ip);
		 
		if(client.Connect(CA2W(ip), port)){
			printf("\nDa nhan ket noi tu server\n");
			int id;
			client.Receive((char*)&id, sizeof(id), 0);
			printf("\nclient thu %d\n", id + 1);

			
			// nhập câu truy vấn.
			int dem = 0;
			bool end = true;
			do{
				printf("Nhap cau truy van: ");
				char command[100];
				gets(command);
				client.Send(command, 50, 0);
				
				char connect[100];
				do{
					client.Receive(connect, 100, 0);
					printf("\n %d \n", strlen(connect));
					if(!strcmp(connect, "exit"))
						break;
					else if(!strcmp(connect, "Close")){
						end = false;
						break;
					}
					else{
						printf("\n%s\n", connect);		/// //////////////...................................//////
					}
				}while(true);
			}while(end);

			printf("\nKet thuc ket noi toi server...\n");
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
