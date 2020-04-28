///From Javidx9

#include <iostream>
using namespace std;
#include <Windows.h>
#include <chrono>


int nScreenWidth = 120;
int nScreenHeight = 40;

float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;

int nMapHeight = 16;
int nMapWidth = 16;

float fFOV = 3.14159 / 4.0f;
float fDepth = 16.0f;

int main()
{
	TCHAR *screen = new TCHAR[nScreenWidth*nScreenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	wstring map;

	map += L"################";
	map += L"#..............#";
	map += L"#.....###......#";
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

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	//Game loop
	while (true)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();


		//Controls
		//Handle CCW Rotation
		if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		{
			fPlayerA -= (0.1f)*fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			fPlayerA += (0.1f)*fElapsedTime;
		}

		for (int x = 0; x < nScreenWidth; x++)
		{
			//for each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth)*fFOV;

			float fDistanceToWall = 0.0f;
			bool bHitWall = false;

			float fEyeX = sinf(fRayAngle); //Unti vector for ray in player space
			float fEyeY = cosf(fRayAngle);
			while (!bHitWall && fDistanceToWall <= fDepth)
			{
				fDistanceToWall += 0.1f;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				//Test if array is out of bounds
				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth; //Just set distance to maximum depth
				}
				else 
				{
					if (map[nTestY*nMapWidth + nTestX] == '#')
					{
						bHitWall = true;
					}
				}
			}

			//Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight / 2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y < nCeiling)
				{
					screen[y*nScreenWidth + x] = ' ';
				}
				else if (y > nCeiling && y <= nFloor)
				{
					screen[y*nScreenWidth + x] = '#';
				}
				else
				{
					screen[y*nScreenWidth + x] = ' ';
				}
				
				
				
			}
		}


	screen[nScreenWidth*nScreenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, { 0,0 }, &dwBytesWritten);

	}

	

	return 0;
}