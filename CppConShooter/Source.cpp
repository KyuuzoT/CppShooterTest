///From Javidx9

#include <iostream>
using namespace std;
#include <Windows.h>
#include <chrono>


int iScreenWidth = 120;
int iScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerAngle = 0.0f;

int iMapWidth = 16;
int iMapHeight = 16;

float fFOV = 3.1415f / 4.0f; //Поле зрения
float fDepthOfView = 16.0f;

int main()
{
	float fRayAngle = 0.0f; //Угол отклонения выпускаемого луча от нормали, проведенной от игрока
	float fDisctanceToWall = 0.0f; //Расстояние до стены в пределах видимости
	float fEyeX = 0.0f;
	float fEyeY = 0.0f;
	bool bHitWall = false; //Произошло ли столкновение луча и стены (объекта)?
	int iTestX = 0;
	int iTestY = 0;

	int iCeiling = 0;
	int iFloor = 0;

	chrono::duration<float> elapsedTime;
	float fElapsedTime;

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


	auto timePoint1 = chrono::system_clock::now();
	auto timePoint2 = chrono::system_clock::now();
	
	//Game Loop
	while (true)
	{
		timePoint2 = chrono::system_clock::now();
		elapsedTime = timePoint2 - timePoint1;
		fElapsedTime = elapsedTime.count();
		
		//movement control
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			fPlayerAngle += -0.1f*fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			fPlayerAngle += 0.1f*fElapsedTime;
		}


		for (int x = 0; x < iScreenWidth; x++)
		{
			//For each column on screen
			fRayAngle = (fPlayerAngle - fFOV / 2) + ((float)x / (float)iScreenWidth) * fFOV;
			
			fEyeX = sinf(fRayAngle);
			fEyeY = sinf(fRayAngle);
			

			while (!bHitWall && fDisctanceToWall < fDepthOfView)
			{
				fDisctanceToWall += 0.1f;

				iTestX = (int)(fPlayerX + fEyeX * fDisctanceToWall);
				iTestY = (int)(fPlayerY + fEyeY * fDisctanceToWall);

				if (iTestX < 0 || iTestX >= iMapWidth || iTestY < 0 || iTestY >= iMapHeight)
				{
					bHitWall = true;
					fDisctanceToWall = fDepthOfView;
				}
				else
				{
					if (map[iTestY*iMapWidth + iTestX] == '#')
					{
						bHitWall = true;
					}
				}
			}

			iCeiling = (float)(iScreenHeight / 2.0) - iScreenHeight / fDisctanceToWall;
			iFloor = iScreenHeight - iCeiling;

			for (int y = 0; y < iScreenHeight; y++)
			{
				if (y < iCeiling)
				{
					screen[y*iScreenWidth + x] = ' ';
				}
				else if (y > iCeiling && y <= iFloor)
				{
					screen[y*iScreenWidth + x] = '#';
				}
				else
				{
					screen[y*iScreenWidth + x] = ' ';
				}
			}
			
		}

		//Drawnings
		screen[iScreenWidth*iScreenHeight - 1] = '\0';
		WriteConsoleOutputCharacter(hConsole, screen, iScreenWidth*iScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	return 0;
}