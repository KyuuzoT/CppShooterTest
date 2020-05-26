///From Javidx9

#include <iostream>
using namespace std;
#include <io.h>
#include <Windows.h>
#include <chrono>
#include <fcntl.h>


enum MovementMode
{
	Forwards,
	Backwards
};

MovementMode mmMode = Forwards;

void onCollision(MovementMode mode, wstring map, float fTime);

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
	_setmode(_fileno(stdout), _O_U8TEXT);
	_setmode(_fileno(stdin), _O_U8TEXT);
	setlocale(LC_ALL, "RUS");
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
			fPlayerA -= (0.8f)*fElapsedTime;
		}
		if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		{
			fPlayerA += (0.8f)*fElapsedTime;
		}
		//forward
		if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
		{
			fPlayerX += sinf(fPlayerA)*5.0f*fElapsedTime;
			fPlayerY += cosf(fPlayerA)*5.0f*fElapsedTime;

			onCollision(Forwards, map, fElapsedTime);
		}
		//backward
		if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
		{
			fPlayerX -= sinf(fPlayerA)*5.0f*fElapsedTime;
			fPlayerY -= cosf(fPlayerA)*5.0f*fElapsedTime;

			onCollision(Backwards, map, fElapsedTime);
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

			short nShade = ' ';

			if (fDistanceToWall <= fDepth / 4.0f) //Very close
			{
				nShade = 0xDB;
			}
			else if (fDistanceToWall <= fDepth / 3.0f)
			{
				nShade = 0xB2;
			}else if (fDistanceToWall <= fDepth / 2.0f)
			{
				nShade = 0xB1;
			}else if (fDistanceToWall <= fDepth)
			{
				nShade = 0xB0;
			}
			else //Very distant
			{
				nShade = ' ';
			}

			for (int y = 0; y < nScreenHeight; y++)
			{
				if (y <= nCeiling)
				{
					screen[y*nScreenWidth + x] = ' ';
				}
				else if (y > nCeiling && y <= nFloor)
				{
					screen[y*nScreenWidth + x] = nShade;
				}
				else
				{
					//Shading the floor based on distance
					float b = 1.0f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
					if (b < 0.25f)
					{
						nShade = '#';
					}
					else if (b < 0.5f)
					{
						nShade = 'x';
					}
					else if (b < 0.75f)
					{
						nShade = '.';
					}
					else if (b < 0.9f)
					{
						nShade = '-';
					}
					else
					{
						nShade = ' ';
					}
					screen[y*nScreenWidth + x] = /*0xFD*/ nShade;
				}
				
				
				
			}
		}


	screen[nScreenWidth*nScreenHeight - 1] = '\0';
	WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth*nScreenHeight, { 0,0 }, &dwBytesWritten);

	}

	
	

	return 0;
}


void onCollision(MovementMode mode, wstring map, float fTime)
{
	switch (mode)
	{
	case Forwards:
		if (map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX -= sinf(fPlayerA)*5.0f*fTime;
			fPlayerY -= cosf(fPlayerA)*5.0f*fTime;
		}
		break;
	case Backwards:
		if (map[(int)fPlayerY*nMapWidth + (int)fPlayerX] == '#')
		{
			fPlayerX += sinf(fPlayerA)*5.0f*fTime;
			fPlayerY += cosf(fPlayerA)*5.0f*fTime;
		}
		break;
	default:
		break;
	}
}