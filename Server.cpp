//#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>

#pragma warning(disable: 4996)
using namespace std;

SOCKET Connections[2];
int Counter = 0;
char msgstart[512] = "start";
char msgstartchats[512] = "chats";
char msgstartchat[512] = "chat";

void ClientHandler(int index) { //функция принимает index подключения
	char msg[512];

	while (true)
	{
		recv(Connections[index], msg, sizeof(msg), NULL); //Принимаем сообщения клиента
		
		if (index == 1) { //Проверяем индекс клиента и отправляем данные другому клиенту
			send(Connections[0], msg, sizeof(msg), NULL);
		}
		else {
			send(Connections[1], msg, sizeof(msg), NULL);
		}

	}
}

int main(int argc, char* argv[]) {
	system("title Cервер");
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) { //Инициализация winsock
		cout << "Error WSA" << endl;
		exit(1);
	}

	SOCKADDR_IN addr;   //Переменная для хранения адреса
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //адресс подключения 4байта
	addr.sin_port = htons(1111); //порт
	addr.sin_family = AF_INET; //ipv4

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Создание прослушки
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //объеденение
	listen(sListen, SOMAXCONN); //Прослушиваем

	SOCKET newConnection; //Создаем сокет
	for (int i = 0; i < 2; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr); //Принимаем прослушиваемые подключения

		if (newConnection == 0) {
			cout << "Error Connection\n";
		}
		else {
			cout << "Client Connected!\n";

			Connections[i] = newConnection; //заполняем массив подключений
			Counter++;
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);//Создаем поток и передаём i
			
			if (Counter == 1) {
				send(Connections[0], msgstartchats, sizeof(msgstartchats), NULL); //отправляем сообщение о запуске чата и сервера 1 клиенту
			}
			else if (Counter == 2) {
				send(Connections[1], msgstartchat, sizeof(msgstartchat), NULL);//отправляем сообщение о запуске чата 2 клиенту
			}
			if (Counter == 2) {
				for (int i = 0; i < Counter; i++) {
					send(Connections[i], msgstart, sizeof(msgstart), NULL); //Отправляем сообщение о начале игры
				}
			}
			
		}
	}



	system("pause");
	return 0;
}