///From Javidx9

#include <iostream>
using namespace std;
#include <Windows.h>
#include "Source.h"


int iScreenWidth = 120;
int iScreenHeight = 40;

float fPlayerX = 0.0f;
float fPlayerY = 0.0f;
float fPlayerAngle = 0.0f;

int iMapWidth = 16;
int iMapHeight = 16;

int main()
{
	//Create Screen Buffer
	TCHAR *screen = new TCHAR[iScreenWidth*iScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"################";

	//Game Loop
	while (true)
	{
		for (int x = 0; x < iScreenWidth; x++)
		{

		}

		//Drawnings
		screen[iScreenWidth*iScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, iScreenWidth*iScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}