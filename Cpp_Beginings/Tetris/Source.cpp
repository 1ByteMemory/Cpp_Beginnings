#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <vector>
#include <cstdlib>
using namespace std;

wstring tetromino[7];

//  playinng feild vars
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;

// screen vars
int nScreenWidth = 120;
int nScreenHeight = 30;



int Rotate(int px, int py, int r)
{
	switch (r % 4)
	{
		case 0: return py * 4 + px;			// 0 Degrees
		case 1: return 12 + py - (px * 4);	// 90 Degrees
		case 2: return 15 - (py * 4) - px;	// 180 Degrees
		case 3: return 3 - py + (px * 4);	// 270 Degrees
	}
	return 0;
}

bool DoesPieceFit(int nTetromino, int nRotation, int nPosX, int nPosY)
{
	for (int px = 0; px < 4 ; px++)
		for (int py = 0; py < 4; py++)
		{
			// Get index into piece
			int pi = Rotate(px, py, nRotation);

			// Get index into field
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);

			if (nPosX + px >= 0 && nPosX + px < nFieldWidth)
			{
				if (nPosY + py >= 0 && nPosY + py < nFieldHeight)
				{
					if (tetromino[nTetromino][pi] == L'X' && pField[fi] != 0)
						return false; // fail on first hit
				}
			}

		}


	return true;
}


int main()
{
	// Create assets
#pragma region Teterominos
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L"..X.");
	tetromino[1].append(L"....");

	tetromino[2].append(L"..X.");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L".X..");
	tetromino[2].append(L"....");

	tetromino[3].append(L".X..");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"..X.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"....");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"....");

	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"....");

	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");
	tetromino[6].append(L"....");
#pragma endregion

	// Create the playing field
#pragma region Playing Field
	pField = new unsigned char[nFieldWidth * nFieldHeight];
	for (int x = 0; x < nFieldWidth; x++)
	{
		for (int y = 0; y < nFieldHeight; y++)
		{
			pField[y * nFieldWidth + x] = (x == 0 || x == nFieldWidth - 1 || y == nFieldHeight - 1) ? 9 : 0;
		}
	}
	

	// Screen (include Windows.h)
	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

#pragma endregion

	// Game Logic stuff
	bool bGameOver = false;

	int nCurrentPiece = rand() % 7;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	// input keys
	bool bKey[4];
	bool bRotateHold = false;

	int nSpeed = 20;
	int nSpeedCounter = 0; // counting game ticks
	bool bForceDown = false;
	int nPieceCount = 0;
	int nScore = 0;

	vector<int> vLines;

	while (!bGameOver)
	{
		// GAME TIMING ===================================
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		nSpeedCounter++;
		bForceDown = nSpeed == nSpeedCounter;


		// INPUT =========================================
		// Piece movement
		for (int k = 0; k < 4; k++)								  // R   L   D    U
			bKey[k] = (0x8000 & GetAsyncKeyState(( unsigned char )("\x27\x25\x28\x26"[k]))) != 0;

		
		// GAME LOGIC ====================================
		if (GetAsyncKeyState(VK_ESCAPE) & 0x01)
			return 0;


		/*Right*/ nCurrentX += bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY);
		
		/*Left*/  nCurrentX -= bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY);
		
		/*Down*/  nCurrentY += bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1);

		/*Rotation*/ 
		if (bKey[3])
		{
			nCurrentRotation += !bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY);
			bRotateHold = true;
		}
		else
		{
			bRotateHold = false;
		}

		if (bForceDown)
		{
			if (DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			{
				nCurrentY++;
			}
			else
			{
				// Lock current piece in the field
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
					{
						if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
							pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
					}

				// Increase difficulty every 10 pieces
				nPieceCount++;
				if (nPieceCount % 10 == 0)
					if (nSpeed >= 10) nSpeed--;

				// Check have created any lines
				for (int py = 0; py < 4; py++)
				{
					if (nCurrentY + py < nFieldHeight - 1)
					{
						bool bLine = true;
						for (int px = 1; px < nFieldWidth - 1; px++)
							bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;

						if (bLine)
						{
							// Remove Line, set to =
							for (int px = 1; px < nFieldWidth; px++)
							{
								pField[(nCurrentY + py) * nFieldWidth + px] = 8;
							}

							vLines.push_back(nCurrentY + py);
						}
					}
				}

				nScore += 25;
				if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;


				// Chose the next piece
				nCurrentPiece = rand() % 7;
				nCurrentRotation = 0;
				nCurrentX = nFieldWidth / 2;
				nCurrentY = 0;

				// if piece does not fit
				bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);

			}

			nSpeedCounter = 0;
		}


		// RENDER OUTPUT =================================


		// Draw Field
		for (int x = 0; x < nFieldWidth; x++)
		{
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			}
		}

		// Draw Current Piece
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
			{
				if (tetromino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
				{
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
				}
			}

		// Draw Score
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		if (!vLines.empty())
		{
			// Display Frame
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(chrono::milliseconds(400));

			for (auto &v : vLines)
			{
				for (int px = 1; px < nFieldWidth - 1; px++)
				{
					for (int py = v; py > 0; py--)
					{
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					}
					pField[px] = 0;
				}
			}
			vLines.clear();
		}
		
		// Display Frame
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}

	CloseHandle(hConsole); // Cannot use cout unless console handle is closed
	cout << "Game Over!! Score: " << nScore << endl;
	system("pause");

	return 0;
}