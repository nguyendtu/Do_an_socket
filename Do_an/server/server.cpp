// 123.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "server.h"
#include <afxsock.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// The one and only application object

CWinApp theApp;

using namespace std;



int search(char command[50]){
	if(strlen(command) == 1)
		if(command[0] == 'h')
			return 1;
		else return 0;
	else{
		char cat[] = " ";
		char *str;
		int dem = -1;
		if(!strcmp(command, "exit"))
			return 0;
		str = strtok(command, cat);
		while(str){
			dem++;
			str = strtok(NULL, cat);
		}

		if(dem == 0)
			return 2;
		else if(dem == 1)
			return 3;
		else return 0;
	}

	return 0;
 }



struct goal{
	char fPrize[20];
	char nPrize[50];
	char number[7];
};

struct town{
	char name[50];
	goal FPrize[3];
};

town Town[7] = {
					{"TP HCM", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Ca Mau", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Kien Giang", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Binh THuan", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Tien GIang", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Bac Binh", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
					{"Vung Tau", {{"10000", "Giai Nhat", "123456"}, {"8000", "Giai Nhi", "456789"}, {"5000", "Giai Ba", "123789"}}},
				};



int searchTwn(char command[50]){
	char *str, *str1;
	char str2[50];
	for(int j = 0; j < 7; j++){
		strcpy(str2, Town[j].name);
		str = strtok(str2, " ");
		//printf("\n%s\n", str);
		str1 = strtok(NULL, " ");
		strcat(str, str1);
		if(!strcmp(str, strtok(command, " "))){
			return j;
		}
	}

	return -1;
}



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
			CSocket server;
			unsigned int port = 1234;

			AfxSocketInit(NULL);

			server.Create(port);
			server.Listen(5);

			int numClient;
			printf("Nhap vao so luong client: ");
			scanf("%d", &numClient);

			printf("\n Dang lang nghe ket noi tu client......\n");

			CSocket *sockClient = new CSocket[numClient];

			for(int i = 0; i < numClient; i++){
				server.Accept(sockClient[i]);
				printf("\nDa nhan ket noi tu client %d...\n", i + 1);
				
				sockClient[i].Send((char*)&i, sizeof(int), 0);

				int dem = 0;
				bool end = true;

				do{
					char command[50];
					sockClient[i].Receive(command, 50, 0);
					printf("\n%s\n", command);

					// xử lý câu truy vấn.
       				char *st = new char[50];
					strcpy(st, command);
       				int index = search(st);

					// gởi kết quả đến client.
					int twn = -1;
					char *stt = new char[50];
					switch(index){

					case 1:

						// gởi hướng dẫn câu truy vấn.
						sockClient[i].Send(" --<TenTinhThanh> --Tim tinh thanh mo thuong (neu co)---", 100, 0);
						sockClient[i].Send(" --<TenTinhThanh><spcae><So_ve> --He thong tu dong do---", 100, 0);
						sockClient[i].Send(" --exit --Thoat---", 100, 0);
						
						// gởi thông tin các tỉnh mở thưởng.
						for(int j = 0; j < 7; j++){
							sockClient[i].Send(Town[j].name, 100, 0);
						}
						sockClient[i].Send("exit", 100, 0);
						break;
					case 2:
						twn = searchTwn(command);
						if(twn == -1)
							sockClient[i].Send("Tinh hien khong mo thuong..", 100, 0);
						else{
							sockClient[i].Send("Cac giai thuong cua tinh..", 100, 0);

							for(int j = 0; j < 3; j++){
								sockClient[i].Send(Town[twn].FPrize[j].nPrize, 100, 0);
								sockClient[i].Send(Town[twn].FPrize[j].number, 100, 0);
								sockClient[i].Send(Town[twn].FPrize[j].fPrize, 100, 0);
							}
						}
						sockClient[i].Send("exit", 100, 0);
						break;
					case 3:
						printf("333\n");
						strcpy(stt, command);
						twn = searchTwn(stt);
						
						if(twn == -1)
							sockClient[i].Send("Tinh hien khong mo thuong..", 100, 0);
						else{
							bool plus = true;
							char *str;
							str = strtok(command, " ");
							printf("\n%s\n", str);
							str = strtok(NULL, " ");
							printf("\n%s\n", str);

							for(int j = 0; j < 3; j++){
								if(!strcmp(str, Town[twn].FPrize[j].number)){
									sockClient[i].Send("Ban da trung giai ", 100, 0);
									sockClient[i].Send(Town[twn].FPrize[j].nPrize, 100, 0);
									sockClient[i].Send(" voi so tien: ", 100, 0);
									sockClient[i].Send(Town[twn].FPrize[j].fPrize, 100, 0);
									plus = false;
									break;
								}
							}
							if(plus)
								sockClient[i].Send("Chuc ban may man lan sau.", 100, 0);
						}
						sockClient[i].Send("exit", 100, 0);
						break;
					case 0:
						sockClient[i].Send("Close", 50, 0);
						end = false;
						break;
					}
				}while(end);
			}
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
