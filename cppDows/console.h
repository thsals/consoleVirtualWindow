#pragma once

#include <iostream>
#include <Windows.h>

#include "consoleDefine.h"

class CConsoleScreen {
public:
	CHAR** m_ArrayScreenCharBuffer[MAX_NUM_OF_SCREEN];
	WORD** m_ArrayScreenColorBuffer[MAX_NUM_OF_SCREEN];
	WORD			m_DefaultColor;

	int				m_LastScreenIndex;
	int				m_LastBufferIndex;

	int				m_NextScreenIndex;
	int				m_NextBufferIndex;
	HANDLE m_hWnd;
	HANDLE m_hSBuf[MAX_NUM_OF_SCREEN];

	SHORT m_iWidth;
	SHORT m_iHeight;
	SMALL_RECT m_ScreenSizeRect;
	COORD m_ScreenSizeCoord;

	CConsoleScreen() {
		m_hWnd = INVALID_HANDLE_VALUE;

		m_iWidth = 0;
		m_iHeight = 0;

		ZeroMemory(&m_ScreenSizeCoord, sizeof(m_ScreenSizeCoord));
		ZeroMemory(&m_ScreenSizeRect, sizeof(m_ScreenSizeRect));

		m_LastScreenIndex = SCREEN_01;
		m_LastBufferIndex = SCREEN_01;
		m_NextScreenIndex = SCREEN_02;
		m_NextBufferIndex = SCREEN_02;

		for (int screen = 0; screen < MAX_NUM_OF_SCREEN; ++screen)
		{
			m_hSBuf[screen] = INVALID_HANDLE_VALUE;
			m_ArrayScreenCharBuffer[screen] = NULL;
			m_ArrayScreenColorBuffer[screen] = NULL;
		}
	}

	~CConsoleScreen()
	{

	}

	BOOL Create(SHORT iWidth, SHORT iHeight, LPCSTR Title, COLOR_LIST DefaultFontColor, COLOR_LIST DefaultBackColor)
	{
		// Window
		SetConsoleTitleA(Title);
		m_hWnd = GetStdHandle(STD_OUTPUT_HANDLE);

		HWND hConsole = GetConsoleWindow();
		EnableMenuItem(GetSystemMenu(hConsole, FALSE), SC_CLOSE, MF_GRAYED);
		DrawMenuBar(hConsole);
		MoveWindow(hConsole, 0, 0, 0, 0, TRUE);

		char buffer[1024];
		sprintf_s(buffer, "mode con cols=%d lines=%d", iWidth, iHeight);
		system(buffer);

		// Init Default
		m_iWidth = iWidth;
		m_iHeight = iHeight;

		m_ScreenSizeCoord.X = m_iWidth;
		m_ScreenSizeCoord.Y = m_iHeight;

		m_ScreenSizeRect.Left = 0;
		m_ScreenSizeRect.Right = m_iWidth - 1;
		m_ScreenSizeRect.Top = 0;
		m_ScreenSizeRect.Bottom = m_iHeight - 1;

		m_DefaultColor = DefaultBackColor << 4 | DefaultFontColor;

		// Init Buffer
		for (int screen = 0; screen < MAX_NUM_OF_SCREEN; ++screen)
		{
			m_ArrayScreenCharBuffer[screen] = new CHAR * [m_iHeight + 1];
			m_ArrayScreenColorBuffer[screen] = new WORD * [m_iHeight + 1];

			for (int height = 0; height <= m_iHeight; ++height)
			{
				m_ArrayScreenCharBuffer[screen][height] = new CHAR[m_iWidth + 1];
				m_ArrayScreenColorBuffer[screen][height] = new WORD[m_iWidth + 1];

				for (int width = 0; width < m_iWidth; ++width)
				{
					m_ArrayScreenCharBuffer[screen][height][width] = ' ';
					m_ArrayScreenColorBuffer[screen][height][width] = m_DefaultColor;
				}

				m_ArrayScreenCharBuffer[screen][height][m_iWidth] = NULL;
				m_ArrayScreenColorBuffer[screen][height][m_iWidth] = NULL;
			}

			m_ArrayScreenCharBuffer[screen][m_iHeight][m_iWidth] = NULL;
			m_ArrayScreenColorBuffer[screen][m_iHeight][m_iWidth] = NULL;
		}

		// Init Device
		for (int screen = 0; screen < MAX_NUM_OF_SCREEN; ++screen)
		{
			HANDLE& activeHandle = m_hSBuf[screen];

			activeHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
			SetConsoleScreenBufferSize(activeHandle, m_ScreenSizeCoord);
			SetConsoleWindowInfo(activeHandle, TRUE, &m_ScreenSizeRect);
		}

		return TRUE;
	}

	BOOL Destroy()
	{
		for (int screen = 0; screen < MAX_NUM_OF_SCREEN; ++screen)
		{
			CloseHandle(m_hSBuf[screen]);

			for (int height = 0; height < m_iHeight; ++height)
			{
				SAFE_DELETE_ARRAY(m_ArrayScreenCharBuffer[screen][height]);
				SAFE_DELETE_ARRAY(m_ArrayScreenColorBuffer[screen][height]);
			}

			SAFE_DELETE_ARRAY(m_ArrayScreenCharBuffer[screen]);
			SAFE_DELETE_ARRAY(m_ArrayScreenColorBuffer[screen]);
		}

		return TRUE;
	}

	BOOL SetCursorState(BOOL bState)
	{
		CONSOLE_CURSOR_INFO tempConsoleCursorInfo;

		if (bState == TRUE)
		{
			tempConsoleCursorInfo.bVisible = TRUE;
			tempConsoleCursorInfo.dwSize = 1;
		}
		else
		{
			tempConsoleCursorInfo.bVisible = FALSE;
			tempConsoleCursorInfo.dwSize = 1;
		}

		BOOL result = TRUE;
		for (int screen = 0; screen < MAX_NUM_OF_SCREEN; ++screen)
			result &= SetConsoleCursorInfo(m_hSBuf[screen], &tempConsoleCursorInfo);

		return result;
	}

	VOID Rander()
	{
		// Set Buffer	
		CHAR** backCharBuffer = m_ArrayScreenCharBuffer[m_LastBufferIndex];
		WORD** backColorBuffer = m_ArrayScreenColorBuffer[m_LastBufferIndex];

		CHAR** frontCharBuffer = m_ArrayScreenCharBuffer[m_NextScreenIndex];
		WORD** frontColorBuffer = m_ArrayScreenColorBuffer[m_NextScreenIndex];
		const HANDLE& activeHandle = m_hSBuf[m_NextScreenIndex];

		// Init Buffer
		for (int height = 0; height < m_iHeight; ++height)
		{
			for (int width = 0; width < m_iWidth; ++width)
			{
				backCharBuffer[height][width] = ' ';
				backColorBuffer[height][width] = m_DefaultColor;
			}
		}

		// Draw
		DWORD tempWriten;
		COORD tempCoord;
		tempCoord.X = 0;
		tempCoord.Y = 0;

		for (int height = 0; height < m_iHeight; ++height)
		{
			
			tempCoord.Y = height;
			CHAR* outCharBuffer = frontCharBuffer[height];
			WORD* outColorBuffer = frontColorBuffer[height];
			WriteConsoleOutputCharacterA(activeHandle, outCharBuffer, m_iWidth, tempCoord, &tempWriten);
			WriteConsoleOutputAttribute(activeHandle, outColorBuffer, m_iWidth, tempCoord, &tempWriten);
		}

		tempCoord.X = 0;
		tempCoord.Y = 0;

		// Rander
		SetConsoleActiveScreenBuffer(activeHandle);
		SetConsoleCursorPosition(activeHandle, tempCoord);

		m_LastScreenIndex = m_NextScreenIndex;
		m_LastBufferIndex = m_NextScreenIndex;

		m_NextScreenIndex = (m_NextScreenIndex + 1) % MAX_NUM_OF_SCREEN;
		m_NextBufferIndex = (m_NextBufferIndex + 1) % MAX_NUM_OF_SCREEN;
	}

	VOID DrawPoint(SHORT X, SHORT Y, COLOR_LIST FontColor, COLOR_LIST BackColor, CHAR Char)
	{
		if( X >= m_iWidth || Y >= m_iHeight || X < 0 || Y < 0 )
			return;

		// NextBuffer ¿¡ ±â·Ï
		m_ArrayScreenCharBuffer[m_NextBufferIndex][Y][X] = Char;
		m_ArrayScreenColorBuffer[m_NextBufferIndex][Y][X] = BackColor << 4 | FontColor;
	}

	VOID DrawFont(SHORT X, SHORT Y, COLOR_LIST FontColor, COLOR_LIST BackColor, LPCSTR String, ...)
	{
		CHAR TempBuffer[1024];
		ZeroMemory(TempBuffer, sizeof(CHAR) * 1024);

		va_list ap;
		va_start(ap, String);
		vsprintf_s(TempBuffer, String, ap);
		va_end(ap);

		SHORT Length = (SHORT)strlen(TempBuffer);

		for (SHORT i = 0; i < Length; i++)
			DrawPoint(X + i, Y, FontColor, BackColor, TempBuffer[i]);
	}

};
