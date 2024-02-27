//#include "stdafx.h"
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <cmath>
#include <string>
#include <time.h>

#pragma warning(disable: 4996)
using namespace std;

SOCKET Connection;
const int n = 20;   //размерность игрового поля
const int m = 20;
int A[n][m];
int Player_Field[n][m];     //поле данного клиента
int bombs = 50;     //количествово бомб
int X_offset = 100;     //отступ от верхнего края окна(в пикселях) для функция SetPixel
int Z_offset = 200;     //отступ от левого края окна(в пикселях) для функция SetPixel
int X_offset_2 = 700;   //отступ от левого краф окна(в пикселях) для функция SetPixel
int Player_Field_Two[20][20];   //поле второго клиента
int Rand_Val = 0;   //число содержащее значение,получаемое функцией srand()
char lose[256] = "YOULOSE";     //месседж, сигнализирующий о поригрыше клиента
char win[256] = "YOUWON";       //месседж, сигнализирующий о победе клиента
int status = 0;     //значение этой переменной оаределяет статус основного массива клиента(является условием выхода из цикла)
int LiveCounter = 3;    //количествово жизней клиента
int Not_Bomb_Counter = 0;   //счётчик, являющийся условием выхода из цикла при победе(считает раскрытые клетки клиента) 

HWND hwnd = GetConsoleWindow();                     //полученки дескриптора окна
LPPOINT pPnt;
HDC hdc = GetDC(hwnd);

char Button_0[9][10]{   //массив, по которому отображаются символы в консоли
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '1', '.', '.', '.', '1', '.', '.',  },
{'.', '.', '1', '.', '.', '.', '1', '.', '.',  },
{'.', '.', '1', '.', '.', '.', '1', '.', '.',  },
{'.', '.', '1', '.', '.', '.', '1', '.', '.',  },
{'.', '.', '1', '.', '.', '.', '1', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_1[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '1', '1', '.', '.', '.', },
{'.', '.', '.', '1', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '1', '1', '1', '1', '.', '.', },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },

};
char Button_2[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '1', '.', '.', '.', '.', },
{'.', '.', '1', '.', '.', '1', '.', '.', '.', },
{'.', '.', '1', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '.', '1', '.', '.', '.', },
{'.', '.', '.', '.', '1', '.', '.', '.', '.', },
{'.', '.', '.', '1', '.', '.', '.', '.', '.', },
{'.', '.', '1', '1', '1', '1', '1', '.', '.', },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_3[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '1', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_4[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_5[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_6[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_7[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '1', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '1', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '1', '1', '1', '1', '1', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '1', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_Flag[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '2', '1', '1', '1', '1', '.', '.',  },
{'.', '.', '2', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '2', '1', '1', '.', '.', '.', '.',  },
{'.', '.', '2', '1', '1', '1', '.', '.', '.',  },
{'.', '.', '2', '1', '1', '1', '1', '.', '.',  },
{'.', '.', '2', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '2', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};
char Button_B[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '.', '1', '.', '.', '.', '.', },
{'.', '.', '1', '.', '1', '.', '1', '.', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.', },
{'.', '1', '1', '1', '1', '1', '1', '1', '.', },
{'.', '.', '.', '1', '1', '1', '.', '.', '.', },
{'.', '.', '1', '.', '1', '.', '1', '.', '.', },
{'.', '.', '.', '.', '1', '.', '.', '.', '.', },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },

};
char Button_Clear[9][10]{
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.',  },
{'.', '.', '.', '.', '.', '.', '.', '.', '.', },
};

int Drowing_Button_0(int i, int j, int X_offset, int Z_offset) {    //функция отрисовки клетки 0 на поле

    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_0[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_0[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 255, 255));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 255, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 255, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 255, 255));
            }
        }
    }
    return 1;
}
int Drowing_Button_1(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {               //
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_1[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_1[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 0, 0));
            }
        }
    }
    return 1;
}
int Drowing_Button_2(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {               //
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_2[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_2[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(127, 255, 50));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(127, 255, 50));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(127, 255, 50));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(127, 255, 50));
            }
        }
    }
    return 1;
}
int Drowing_Button_3(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_3[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_3[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 255, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 255, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 255, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 255, 0));
            }
        }
    }
    return 1;
}
int Drowing_Button_4(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {               //
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_4[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_4[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(0, 255, 255));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 255, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 255, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(0, 255, 255));
            }
        }
    }
    return 1;
}
int Drowing_Button_5(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {               //
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_5[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_5[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 0, 255));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 0, 255));
            }
        }
    }
    return 1;
}
int Drowing_Button_6(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_6[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_6[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 69, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 69, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 69, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 69, 0));
            }
        }
    }
    return 1;
}
int Drowing_Button_7(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {               //
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_7[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_7[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(0, 0, 255));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 255));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(0, 0, 255));
            }
        }
    }
    return 1;
}
int Drowing_Button_Flag(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_Flag[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_Flag[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(255, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(255, 0, 0));
            }
            if (Button_Flag[k][l] == '2') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
            }
        }
    }
    return 1;
}
int Drowing_Button_B(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_B[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_B[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
            }
        }
    }
    return 1;
}
int Drowing_Button_Clear(int i, int j, int X_offset, int Z_offset) {
    HWND hwnd = GetConsoleWindow();
    HDC hdc = GetDC(hwnd);
    for (int k = 0; k < (m / 2) - 1; k++) {
        for (int l = 0; l < (m / 2) - 1; l++) {
            if (Button_Clear[k][l] == '.') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(100, 150, 200));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(100, 150, 200));
            }
            if (Button_Clear[k][l] == '1') {
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + 1 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i, k * 2 + 1 + Z_offset + 20 * j, RGB(0, 0, 0));
                SetPixel(hdc, l * 2 + X_offset + 20 * i + 1, k * 2 + Z_offset + 20 * j, RGB(0, 0, 0));
            }
        }
    }
    return 1;
}

int Field_Show_2() {        //функция отображения всего поля(другого игрока)

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (Player_Field_Two[j][i] == 8) {
                for (int k = 0; k < 18; k++) {
                    for (int l = 0; l < 18; l++) {
                        SetPixel(hdc, k + 20 * j + X_offset_2, l + 20 * i + Z_offset, RGB(100, 150, 200));
                    }
                }

            }
            if (Player_Field_Two[i][j] == 0) {
                Drowing_Button_0(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 1) {
                Drowing_Button_1(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 2) {
                Drowing_Button_2(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 3) {
                Drowing_Button_3(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 4) {
                Drowing_Button_4(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 5) {
                Drowing_Button_5(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 6) {
                Drowing_Button_6(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 7) {
                Drowing_Button_7(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 9) {
                Drowing_Button_B(i, j, X_offset_2, Z_offset);
            }
            if (Player_Field_Two[i][j] == 12) {
                Drowing_Button_Flag(i, j, X_offset_2, Z_offset);
            }
        }
    }
    return 1;
}

int Field_Show() {        //функция отображения всего поля

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            if (Player_Field[i][j] == 8) {
                for (int k = 0; k < 18; k++) {
                    for (int l = 0; l < 18; l++) {
                        SetPixel(hdc, k + 20 * i + X_offset, l + 20 * j + Z_offset, RGB(100, 150, 200));
                    }
                }

            }
            if (Player_Field[i][j] == 0) {
                Drowing_Button_0(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 1) {
                Drowing_Button_1(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 2) {
                Drowing_Button_2(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 3) {
                Drowing_Button_3(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 4) {
                Drowing_Button_4(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 5) {
                Drowing_Button_5(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 6) {
                Drowing_Button_6(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 7) {
                Drowing_Button_7(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 9) {
                Drowing_Button_B(i, j, X_offset, Z_offset);
            }
            if (Player_Field[i][j] == 12) {
                Drowing_Button_Flag(i, j, X_offset, Z_offset);
            }
        }
    }
    return 1;
}

int Opening(int a, int b) {     //рекурсивная функция для открытия пустых клеток(часть процесса игры)
    if (a != 0) {
        if (A[a - 1][b] == 0) {
            if (Player_Field[a - 1][b] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a - 1][b] = 0;

            A[a - 1][b] = 11;
            Opening(a - 1, b);
            Drowing_Button_0(a - 1, b, X_offset, Z_offset);
        }
        else
            if (A[a - 1][b] >= 0 && A[a - 1][b] <= 7) {
                if (Player_Field[a - 1][b] != A[a - 1][b]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a - 1][b] = A[a - 1][b];
                if (A[a - 1][b] == 1) {
                    Drowing_Button_1(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 2) {
                    Drowing_Button_2(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 3) {
                    Drowing_Button_3(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 4) {
                    Drowing_Button_4(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 5) {
                    Drowing_Button_5(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 6) {
                    Drowing_Button_6(a - 1, b, X_offset, Z_offset);
                }
                if (A[a - 1][b] == 7) {
                    Drowing_Button_7(a - 1, b, X_offset, Z_offset);
                }
            }
    }
    if (a != 0 && b != m - 1) {
        if (A[a - 1][b + 1] == 0) {
            if (Player_Field[a - 1][b + 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a - 1][b + 1] = 0;
            A[a - 1][b + 1] = 11;
            Opening(a - 1, b + 1);
            Drowing_Button_0(a - 1, b + 1, X_offset, Z_offset);
        }
        else
            if (A[a - 1][b + 1] >= 0 && A[a - 1][b + 1] <= 7) {
                if (Player_Field[a - 1][b + 1] != A[a - 1][b + 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a - 1][b + 1] = A[a - 1][b + 1];
                if (A[a - 1][b + 1] == 1) {
                    Drowing_Button_1(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 2) {
                    Drowing_Button_2(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 3) {
                    Drowing_Button_3(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 4) {
                    Drowing_Button_4(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 5) {
                    Drowing_Button_5(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 6) {
                    Drowing_Button_6(a - 1, b + 1, X_offset, Z_offset);
                }
                if (A[a - 1][b + 1] == 7) {
                    Drowing_Button_7(a - 1, b + 1, X_offset, Z_offset);
                }
            }
    }
    if (b != m - 1) {
        if (A[a][b + 1] == 0) {
            if (Player_Field[a][b + 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a][b + 1] = 0;
            A[a][b + 1] = 11;
            Opening(a, b + 1);
            Drowing_Button_0(a, b + 1, X_offset, Z_offset);
        }
        else
            if (A[a][b + 1] >= 0 && A[a][b + 1] <= 7) {
                if (Player_Field[a][b + 1] != A[a][b + 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a][b + 1] = A[a][b + 1];
                if (A[a][b + 1] == 1) {
                    Drowing_Button_1(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 2) {
                    Drowing_Button_2(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 3) {
                    Drowing_Button_3(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 4) {
                    Drowing_Button_4(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 5) {
                    Drowing_Button_5(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 6) {
                    Drowing_Button_6(a, b + 1, X_offset, Z_offset);
                }
                if (A[a][b + 1] == 7) {
                    Drowing_Button_7(a, b + 1, X_offset, Z_offset);
                }
            }
    }
    if (b != m - 1 && a != n - 1) {
        if (A[a + 1][b + 1] == 0) {
            if (Player_Field[a + 1][b + 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a + 1][b + 1] = 0;
            A[a + 1][b + 1] = 11;
            Opening(a + 1, b + 1);
            Drowing_Button_0(a + 1, b + 1, X_offset, Z_offset);
        }
        else
            if (A[a + 1][b + 1] >= 0 && A[a + 1][b + 1] <= 7) {
                if (Player_Field[a + 1][b + 1] != A[a + 1][b + 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a + 1][b + 1] = A[a + 1][b + 1];
                if (A[a + 1][b + 1] == 1) {
                    Drowing_Button_1(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 2) {
                    Drowing_Button_2(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 3) {
                    Drowing_Button_3(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 4) {
                    Drowing_Button_4(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 5) {
                    Drowing_Button_5(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 6) {
                    Drowing_Button_6(a + 1, b + 1, X_offset, Z_offset);
                }
                if (A[a + 1][b + 1] == 7) {
                    Drowing_Button_7(a + 1, b + 1, X_offset, Z_offset);
                }
            }
    }
    if (a != n - 1) {
        if (A[a + 1][b] == 0) {
            if (Player_Field[a + 1][b] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a + 1][b] = 0;
            A[a + 1][b] = 11;
            Opening(a + 1, b);
            Drowing_Button_0(a + 1, b, X_offset, Z_offset);
        }
        else
            if (A[a + 1][b] >= 0 && A[a + 1][b] <= 7) {
                if (Player_Field[a + 1][b] != A[a + 1][b]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a + 1][b] = A[a + 1][b];
                if (A[a + 1][b] == 1) {
                    Drowing_Button_1(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 2) {
                    Drowing_Button_2(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 3) {
                    Drowing_Button_3(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 4) {
                    Drowing_Button_4(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 5) {
                    Drowing_Button_5(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 6) {
                    Drowing_Button_6(a + 1, b, X_offset, Z_offset);
                }
                if (A[a + 1][b] == 7) {
                    Drowing_Button_7(a + 1, b, X_offset, Z_offset);
                }
            }
    }
    if (a != n - 1 && b != 0) {
        if (A[a + 1][b - 1] == 0) {
            if (Player_Field[a + 1][b - 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a + 1][b - 1] = 0;
            A[a + 1][b - 1] = 11;
            Opening(a + 1, b - 1);
            Drowing_Button_0(a + 1, b - 1, X_offset, Z_offset);
        }
        else
            if (A[a + 1][b - 1] >= 0 && A[a + 1][b - 1] <= 7) {
                if (Player_Field[a + 1][b - 1] != A[a + 1][b - 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a + 1][b - 1] = A[a + 1][b - 1];
                if (A[a + 1][b - 1] == 1) {
                    Drowing_Button_1(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 2) {
                    Drowing_Button_2(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 3) {
                    Drowing_Button_3(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 4) {
                    Drowing_Button_4(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 5) {
                    Drowing_Button_5(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 6) {
                    Drowing_Button_6(a + 1, b - 1, X_offset, Z_offset);
                }
                if (A[a + 1][b - 1] == 7) {
                    Drowing_Button_7(a + 1, b - 1, X_offset, Z_offset);
                }
            }
    }
    if (b != 0) {
        if (A[a][b - 1] == 0) {
            if (Player_Field[a][b - 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a][b - 1] = 0;
            A[a][b - 1] = 11;
            Opening(a, b - 1);
            Drowing_Button_0(a, b - 1, X_offset, Z_offset);
        }
        else
            if (A[a][b - 1] >= 0 && A[a][b - 1] <= 7) {
                if (Player_Field[a][b - 1] != A[a][b - 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a][b - 1] = A[a][b - 1];
                if (A[a][b - 1] == 1) {
                    Drowing_Button_1(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 2) {
                    Drowing_Button_2(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 3) {
                    Drowing_Button_3(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 4) {
                    Drowing_Button_4(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 5) {
                    Drowing_Button_5(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 6) {
                    Drowing_Button_6(a, b - 1, X_offset, Z_offset);
                }
                if (A[a][b - 1] == 7) {
                    Drowing_Button_7(a, b - 1, X_offset, Z_offset);
                }
            }
    }
    if (a != 0 && b != 0) {
        if (A[a - 1][b - 1] == 0) {
            if (Player_Field[a - 1][b - 1] != 0) {
                Not_Bomb_Counter++;
            }
            Player_Field[a - 1][b - 1] = 0;
            A[a - 1][b - 1] = 11;
            Opening(a - 1, b - 1);
            Drowing_Button_0(a - 1, b - 1, X_offset, Z_offset);
        }
        else
            if (A[a - 1][b - 1] >= 0 && A[a - 1][b - 1] <= 7) {
                if (Player_Field[a - 1][b - 1] != A[a - 1][b - 1]) {
                    Not_Bomb_Counter++;
                }
                Player_Field[a - 1][b - 1] = A[a - 1][b - 1];
                if (A[a - 1][b - 1] == 1) {
                    Drowing_Button_1(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 2) {
                    Drowing_Button_2(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 3) {
                    Drowing_Button_3(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 4) {
                    Drowing_Button_4(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 5) {
                    Drowing_Button_5(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 6) {
                    Drowing_Button_6(a - 1, b - 1, X_offset, Z_offset);
                }
                if (A[a - 1][b - 1] == 7) {
                    Drowing_Button_7(a - 1, b - 1, X_offset, Z_offset);
                }
            }
    }
    return (A[a][b]);

}

void SecondEditionUltraThread() {       //функция, вызывающаяся как второй поток, для общения с сервером
    char msgr[512];
    for (int i = 0; i < 400; i++) {
        msgr[i] = 8;
    }

    char Temp_Arr[512];

    Sleep(3000);
    while (true) {

        for (int i = 0; i < 400; i++) {
            msgr[i] = 8;
        }

        Sleep(50);
        for (int i = 0, count = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                Temp_Arr[count] = Player_Field[i][j];
                count++;
            }
        }
        send(Connection, Temp_Arr, sizeof(Temp_Arr), 0);
        Sleep(50);
        recv(Connection, msgr, sizeof(msgr), 0);

        if (strcmp(msgr, "YOULOSE") == 0) { //условия выхода из цикла при получения сообщения о проигрыше
            status = 3;
            break;
        }
        if (strcmp(msgr, "YOUWON") == 0) {  //условия выхода из цикла при получения сообщения о победе
            status = 2;
            break;
        }

        for (int i = 0; i < 20; i++) {
            for (int j = 0; j < 20; j++) {
                if (Player_Field_Two[j][i] != int(msgr[j * 20 + i])) {
                    Player_Field_Two[j][i] = int(msgr[j * 20 + i]);
                    if (Player_Field_Two[j][i] == 8) {
                        for (int k = 0; k < 18; k++) {
                            for (int l = 0; l < 18; l++) {
                                SetPixel(hdc, k + 20 * j + X_offset_2, l + 20 * i + Z_offset, RGB(100, 150, 200));
                            }
                        }
                    }
                    if (Player_Field_Two[j][i] == 0) {
                        Drowing_Button_0(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 1) {
                        Drowing_Button_1(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 2) {
                        Drowing_Button_2(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 3) {
                        Drowing_Button_3(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 4) {
                        Drowing_Button_4(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 5) {
                        Drowing_Button_5(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 6) {
                        Drowing_Button_6(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 7) {
                        Drowing_Button_7(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 9) {
                        Drowing_Button_B(j, i, X_offset_2, Z_offset);
                    }
                    if (Player_Field_Two[j][i] == 12) {
                        Drowing_Button_Flag(j, i, X_offset_2, Z_offset);
                    }

                }
            }
        }
    }
}

int mains() {   //главный поток, вызывающийся как функция в главной функции main при правильном подключении к серверу
    srand(time(NULL));
    setlocale(LC_ALL, "Ru");
    CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SecondEditionUltraThread, NULL, NULL, NULL);
    HANDLE hin = GetStdHandle(STD_INPUT_HANDLE); // получаем дескриптор
    SetConsoleMode(hin, ENABLE_MOUSE_INPUT); // разрешаем обработку мыши(чтобы при нажатии ПКМ не вызывалось меню)

    //меняем размер консоли 
    system("mode con cols=200 lines=70"); //размер окна, вывод нужного количества строк в консоль
    HANDLE  hout = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD  size{ 100,100 };//символов строки, строк
    SetConsoleScreenBufferSize(hout, size);//размер буфера
    CONSOLE_FONT_INFOEX cfi;                    //Меняем шрифт для отображения символов Unicode
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 10;
    cfi.dwFontSize.Y = 10;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy_s(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(hout, false, &cfi);

    for (int i = 0; i < 20; i++) {  //заполнение миссива "игрока" восьмёрками
        for (int j = 0; j < 20; j++) {
            Player_Field_Two[i][j] = 8;
        }
    }

    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            A[i][j] = 0;
        }
    }

    for (int i = 0; i < bombs; i++) {   //раскидывание бомб
        int val = (((rand() + Rand_Val) * 17) % (n * m) + 1);
        if (A[((val - (val % n)) / n)][val % m - 1] == 9) {
            i--;
        }
        A[((val - (val % n)) / n)][val % m - 1] = 9;  // 9 здесь - обозначение бомбы в консольном приложении

    }

    for (int i = 0; i < n; i++) {   //добавление значений в клетки
        for (int j = 0; j < m; j++) {
            if (A[i][j] == 9) {
                if (i != 0 && j != 0 && A[i - 1][j - 1] != 9) { A[i - 1][j - 1] ++; }
                if (i != 0 && A[i - 1][j] != 9) { A[i - 1][j] ++; }
                if (i != 0 && j != m - 1 && A[i - 1][j + 1] != 9) { A[i - 1][j + 1] ++; }
                if (j != 0 && A[i][j - 1] != 9) { A[i][j - 1] ++; }
                if (j != m - 1 && A[i][j + 1] != 9) { A[i][j + 1] ++; }
                if (i != m - 1 && j != 0 && A[i + 1][j - 1] != 9) { A[i + 1][j - 1] ++; }
                if (i != m - 1 && A[i + 1][j] != 9) { A[i + 1][j] ++; }
                if (i != m - 1 && j != m - 1 && A[i + 1][j + 1] != 9) { A[i + 1][j + 1] ++; }

            }
        }
    }

    //отображение координат, клик мышкой
    HWND hwnd = GetConsoleWindow();
    LPPOINT pPnt;
    pPnt = static_cast<LPPOINT>(malloc(sizeof(*pPnt)));
    GetCursorPos(pPnt);
    ScreenToClient(hwnd, pPnt);

    for (int i = 0; i < n; i++) {                                                           //заполнение миссива "игрока" восьмёрками
        for (int j = 0; j < m; j++) {
            Player_Field[i][j] = 8;
        }
    }

    bool Choose_Lbutton = 0;    //переменные для проверки нажатия лкм/пкм
    bool Choose_Rbutton = 0;
    Field_Show();
    Field_Show_2();
    do {    //основной цикл "игры" (взаимодействие клисента со своим полем)
        GetCursorPos(pPnt);
        ScreenToClient(hwnd, pPnt);
        if ((GetKeyState(VK_LBUTTON) < 0) && (Choose_Lbutton == 0)) {   //реакция на клик лкм
            SetConsoleMode(hin, ENABLE_MOUSE_INPUT);
            Choose_Lbutton = 1;

            if (pPnt[0].x < X_offset + 20 * m && pPnt[0].y < Z_offset + 20 * m && pPnt[0].x > X_offset && pPnt[0].y > Z_offset)   //процесс игры(открытие клеток на поле)
            {
                int Xx = ((pPnt[0].y - Z_offset) - ((pPnt[0].y - Z_offset) % 20)) / 20;
                int Yy = ((pPnt[0].x - X_offset) - ((pPnt[0].x - X_offset) % 20)) / 20;

                if (Player_Field[Yy][Xx] == 8) {
                    if (A[Yy][Xx] >= 1 && A[Yy][Xx] <= 7) {     //если в клетке цифра, он её открывает
                        Player_Field[Yy][Xx] = A[Yy][Xx];

                        if (Player_Field[Yy][Xx] == 1) {
                            Drowing_Button_1(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 2) {
                            Drowing_Button_2(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 3) {
                            Drowing_Button_3(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 4) {
                            Drowing_Button_4(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 5) {
                            Drowing_Button_5(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 6) {
                            Drowing_Button_6(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                        if (Player_Field[Yy][Xx] == 7) {
                            Drowing_Button_7(Yy, Xx, X_offset, Z_offset);
                            Not_Bomb_Counter++;
                        }
                    }
                    if (A[Yy][Xx] == 0) {
                        Player_Field[Yy][Xx] = A[Yy][Xx];
                        Opening(Yy, Xx);
                        Drowing_Button_0(Yy, Xx, X_offset, Z_offset);
                        Not_Bomb_Counter++;
                    }
                    if (A[Yy][Xx] == 9) {
                        Player_Field[Yy][Xx] = A[Yy][Xx];
                        Drowing_Button_B(Yy, Xx, X_offset, Z_offset);
                        LiveCounter--;
                    }
                }
            }
        }
        if (GetKeyState(VK_LBUTTON) >= 0) {
            Choose_Lbutton = 0;
        }
        if ((GetKeyState(VK_RBUTTON) < 0) && (Choose_Rbutton == 0)) {   //реакция на клик пкм
            Choose_Rbutton = 1;

            if (pPnt[0].x <= X_offset + 20 * m && pPnt[0].y <= Z_offset + 20 * m && pPnt[0].x > X_offset && pPnt[0].y > Z_offset) {
                int Xx = ((pPnt[0].y - Z_offset) - ((pPnt[0].y - Z_offset) % 20)) / 20;
                int Yy = ((pPnt[0].x - X_offset) - ((pPnt[0].x - X_offset) % 20)) / 20;

                if (Player_Field[Yy][Xx] == 8) {
                    Player_Field[Yy][Xx] = 12;
                    Drowing_Button_Flag(Yy, Xx, X_offset, Z_offset);
                }
                else
                    if (Player_Field[Yy][Xx] == 12) {
                        Player_Field[Yy][Xx] = 8;
                        Drowing_Button_Clear(Yy, Xx, X_offset, Z_offset);
                    }
            }
        }
        if (GetKeyState(VK_RBUTTON) >= 0) {
            Choose_Rbutton = 0;
        }
        if (status == 3 || status == 2 || LiveCounter == 0) {
            break;
        }
    } while ((Not_Bomb_Counter < (n * m - bombs)));

    if (status != 3 && LiveCounter != 0) {
        send(Connection, lose, sizeof(lose), NULL);
        cout << "Вы выйграли!" << endl;
        system("title Окно победителя");
        Field_Show_2();
    }
    else {
        send(Connection, win, sizeof(win), NULL);
        system("title Окно проигравшего");
        cout << "Вы проиграли(" << endl;
        Field_Show_2();
    }
    return 1;
}

void ClientHandler() {
    char msg[512];
    while (true) {
        recv(Connection, msg, sizeof(msg), NULL);   //Получаем сообщение от сервера
        if (strcmp(msg, "chats") == 0) {    //Если сообщение - chats, то мы запускаем сервер чата и сам чат
            system("start chatserver.exe");
            Sleep(200);
            system("start chat.exe");
        }
        if (strcmp(msg, "chat") == 0) {    //Если сообщение - chat, то мы запускаем только чат
            system("start chat.exe");
            Sleep(200);
        }
        if (strcmp(msg, "start") == 0) {    //Если сообщение - start, то мы запускаем функцию mains
            mains();
        }
    }
}

int main(int argc, char* argv[]) {
    system("title Окно игры Сапёр");
    srand(time(NULL));
    WSAStartup;
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        cout << "Error" << endl;
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
        cout << "Error: failed connect to server.\n";
        return 1;
    }
    cout << "Connected!\n";
    Rand_Val = rand();
    ClientHandler();

    system("pause");
    return 0;
}