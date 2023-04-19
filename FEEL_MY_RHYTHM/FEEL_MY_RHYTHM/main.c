#define MAX_NOTE 6
#define ROW_SIZE 160
#define COLUMN_SIZE 50
#include "input.h"
#include "main.h"

//using namespace std;



///
/// 04. 10
/// Ŀ�� �ޱ� gotoxy, initConsole ��
/// â ������ ����
/// ***���� ���۸� ã�ƺ���***
/// 






// ȭ�� ���۰� �޾ƿ� ����ü (�簢������ ���� �����Ѵٰ� ��)
SMALL_RECT consoleScreenSize;
// UI �ִ� ������ ������
SMALL_RECT _UImaxSize;
// ����(ȭ��) 2�� ����
HANDLE hScreen[2];
bool bScreenIndex;


/// <summary>
/// �ʱ�ȭ
/// ���� �ޱ� -> ���߿� �ܼ�â ũ�� ���� �����ϱ�
/// </summary>
void initConsole()
{
	system("mode con cols=160 lines=50 | title FEEL_THE_RHYTHM");
	// ���� ���۸�.. ���� �ΰ� �����
	hScreen[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	hScreen[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	// Ŀ�� ���ܹ����� (�ȱ�����! �Ⱥ���!)
	HANDLE consonleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO consoleCursor = { 0, };
	consoleCursor.bVisible = 0;											// 0 �̸� Ŀ���� ����, 0 �� �ƴ� ��쿡 ���δ�
	consoleCursor.dwSize = 1;											// ���� ���� Ŀ���� ũ�⸦ �����Ѵ�

	// Ŀ�������� �������� �� ȭ�鿡 �Է�
	SetConsoleCursorInfo(consonleHandle, &consoleCursor);
	SetConsoleCursorInfo(hScreen[0], &consoleCursor);
	SetConsoleCursorInfo(hScreen[1], &consoleCursor);


	// ȭ�� ������ �޾ƿ��� (���۰� �޾Ƽ� ScreenSize �� ����)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	consoleScreenSize.Left = csbi.srWindow.Left;
	consoleScreenSize.Right = csbi.srWindow.Right;
	consoleScreenSize.Bottom = csbi.srWindow.Bottom;
	consoleScreenSize.Top = csbi.srWindow.Top;

	_UImaxSize.Left = consoleScreenSize.Left + 2;
	_UImaxSize.Right = consoleScreenSize.Right - 2;
	_UImaxSize.Bottom = consoleScreenSize.Bottom - 2;
	_UImaxSize.Top = consoleScreenSize.Top + 2;

	// ȭ�� �ʱ�ȭ
	//system("cls");


}

HANDLE GetScreenHandle()
{
	int index = (bScreenIndex ? 1 : 0);

	return hScreen[bScreenIndex];
}


void SetStdScreen()
{
	SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
}

void SetBackScreen()
{
	SetConsoleActiveScreenBuffer(GetScreenHandle());
}


/// ���� �ΰ��� ��ȯ���ִ� �Լ�
void ScreenFlipping()
{
	SetConsoleActiveScreenBuffer(GetScreenHandle());
	bScreenIndex = !bScreenIndex;
}



/// ȭ���� ���� �Լ�. ���� ���۷� ��ȯ�� �� ���� ��³����� �������Ѵ�
void ScreenClear()
{
	COORD coordScreen = { 0, 0 };
	DWORD dwConSize;


	// �ܼ� â�� �������� ä���
	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dwConSize, coordScreen, &dwConSize);

	// ���� �Ӽ��� �����Ͽ� �ܼ� â�� �����
	WORD wColors = ((WORD)0 << 4) | (WORD)15; // ��� ���ڻ�, ���� ����
	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dwConSize, coordScreen, &dwConSize);
}

//void ScreenClear()
//{
//	COORD coor = { 0,0 };
//	DWORD dw;
//
//	FillConsoleOutputCharacterW(GetScreenHandle(), L' ', dw, coor, &dw);
//	WORD wColors = ((WORD)0 << 4) | (WORD)15; // ��� ���ڻ�, ���� ����
//	FillConsoleOutputAttribute(GetScreenHandle(), wColors, dw, coor, &dw);
//		//���� ���� ȭ�� ���ۿ��� ������ ���� �� �̻����� Ȯ��Ǹ� �����࿡ ��ϵȴ�(�ڵ����� ���������� �Ѿ�µ�)
//		//���ۺ��� ���� ���� ū ���� ������ �� ������ ��ϵȴ�
//		//�ۼ��� ��ġ�� Ư�� ���� ������� �ʴ´�(? ? ) �� ������ �ȵǴ°ǰ�
//	FillConsoleOutputCharacter(GetScreenHandle(), 'a', 180 * 60, coor, &dw);
//
//
//	FillConsoleOutputCharacter(GetScreenHandle(), ' ', 180 * 60, coor, &dw);
//
//	 ////[�����ٿ� ���� ���� ä���] �� ������ ������ŭ ����
//		//for (int i = 0; i < COLUMN_SIZE; i++)
//		//{
//		//	coor.Y = i;
//		//	//������ ȭ��ũ�⸸ŭ �Է��������;;
//		//	for (int j = 0; j < ROW_SIZE; j++)
//		//	{
//		//		setColor(color_yellow, color_blue);
//		//		ScreenPrint(j, i, ' ', 1);
//		//	}
//
//		//	FillConsoleOutputCharacter(GetScreenHandle(), 'a', _UImaxSize.Right - _UImaxSize.Left + 5, coor, &dw);
//		//}
//
//}


/// ���ۿ� �׸��׸��� (���� �� ĭ ���)
void ScreenPrint(int x, int y, const char* str, int length)
{
	DWORD dw;	// unsigned long ����ü
	COORD CursorPosition = { x, y };
	//char buffer[10];
	//sprintf_s(buffer, "%c", c);

	SetConsoleCursorPosition(GetScreenHandle(), CursorPosition);
	// 2 �� ���ڿ� ����
	WriteFile(GetScreenHandle(), &str, length, &dw, NULL);
}




/// ���� �ݱ�
void ScreenRelease()
{
	CloseHandle(hScreen[0]);
	CloseHandle(hScreen[1]);
}







/// <summary>
/// Ŀ�� ��ġ �̵��ϴ� �Լ�
/// y ��ǥ�� �Ʒ��� ������ ����, ������ ���� ���
/// </summary>
/// <param name="x">�̵��� x��ǥ</param>
/// <param name="y">�̵��� y��ǥ</param>
void gotoXY(int x, int y)
{
	COORD cursor = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursor);
}











///
/// 04. 10
/// ��ǲ ���� ������
/// ��ĥ�� �Ǵ� UI
/// ���� ��üȭ��
/// 







// ��� ��, �۲� �� ����
void setColor(int backColor, int textColor)
{
	//textColor = color_white;
	//SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (backColor << 4) + textColor);
	SetConsoleTextAttribute(GetScreenHandle(), (backColor << 4) + textColor);
}




/// <summary>
/// ȭ��ǥ ��� ���� (UP KEY)
/// </summary>
const int arr[5][5] =
{
	{0,0,1,0,0},
	{0,1,1,1,0},
	{1,0,1,0,1},
	{0,0,1,0,0},
	{0,0,1,0,0}
};


/// <summary>
/// �ʹ� �������ϰ� �پ��� ���(���߿� ���� �ö�� ��Ʈ ��¿��� ����ϴ�)
/// ���� ���ຼ��
/// �����¿� ȭ��ǥ ��� �Լ��̴�
/// </summary>
/// <param name="pos">��ǥ ������ ����ִ� ����ü</param>
/// <param name="color">���� ������ ����ұ�?</param>
void DrawUpArrow(COORD pos, int color)
{

	// arr[i][j] up
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			// j �� ���� ��� (x), i �� ���� ����� ��� (y)
			// ���δ� ���κ��� �ι� ����ؾ���.. -> j*2
			gotoXY(pos.X + 2 * j, pos.Y + i);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

//void ScreenInfoSave(int n)
//{
//	memcpy(arr, )
//}


void ScreenDrawUpArrow(COORD pos, int color)
{
	//const char* c = '  ';
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			//ScreenPrint(pos.X + 2 * j, pos.Y + i, NULL);
			////gotoXY(pos.X + 2 * j, pos.Y + i);
			//if (arr[i][j] == 0)
			//{
			//	setColor(color_black, 15);
			//}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * j, pos.Y + i, '  ', 2);
				//printf("  ");
			}
		}
	}
}

void DrawLeftArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * i, pos.Y + j);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawLeftArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * i, pos.Y + j, '  ', 2);
			}
		}
	}
}

void DrawRightArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 8 - 2 * i, pos.Y + j);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawRightArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 8 - 2 * i, pos.Y + j, '  ', 2);
			}
		}
	}
}

void DrawDownArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			gotoXY(pos.X + 2 * j, pos.Y + 4 - i);
			if (arr[i][j] == 0)
			{
				setColor(color_black, 15);
			}
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				printf("  ");
			}
		}
	}
}

void ScreenDrawDownArrow(COORD pos, int color)
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			setColor(color_black, 15);
			if (arr[i][j] == 1)
			{
				setColor(color, 15);
				ScreenPrint(pos.X + 2 * j, pos.Y + 4 - i, '  ', 2);
			}
		}
	}
}





/// 
/// �迭�� �������
/// 0 �̸� �ѱ�, 1�̸� ������ ����ϰ� ��� ���� ����
/// �������̽� �� Ű�� �׸���! Ű�� �ԷµǸ� ������ Ȱ��ȭ ��
void ScreenDrawKeyInterface()
{
	// �������̽� ��� ��ǥ ���� 
	// 5x5 ũ���� ȭ��ǥ. ����� ������ ���� ��� ��
	const int x = 8;
	const int y = 2;

	const int padding = 16;

	const COORD posLeft = { x + padding * 0, y };	// 8
	const COORD posDown = { x + padding * 1, y };	// 24
	const COORD posUp = { x + padding * 2, y };		// 40
	const COORD posRight = { x + padding * 3, y };	// 56

	/// �� ���� �⺻ ���´� ���
	// Up
	int colorUp = color_dark_white;
	// Down
	int colorDown = color_dark_white;
	// Left
	int colorLeft = color_dark_white;
	// Right
	int colorRight = color_dark_white;


	// Ű�� ������?
	if (GetKeyTable(UP))
	{
		// �ѹ��� �Է¹ޱ� ���� false ó��
		SetKeyTable(UP, false);
		// ������ �������� �ٲ۴�
		colorUp = color_red;
	}

	if (GetKeyTable(DOWN))
	{
		SetKeyTable(DOWN, false);
		// ������ green���� �ٲ۴�
		colorDown = color_green;
	}

	if (GetKeyTable(LEFT))
	{
		SetKeyTable(LEFT, false);
		// ������ skyblue���� �ٲ۴�
		colorLeft = color_dark_blue;
	}

	if (GetKeyTable(RIGHT))
	{
		SetKeyTable(RIGHT, false);
		// ������ yellow���� �ٲ۴�
		colorRight = color_dark_yellow;
	}

	ScreenDrawUpArrow(posUp, colorUp);
	ScreenDrawRightArrow(posRight, colorRight);
	ScreenDrawDownArrow(posDown, colorDown);
	ScreenDrawLeftArrow(posLeft, colorLeft);

	/// �ٸ� �Լ��� ������
	{
		// ������ �迭�� �����ϰ� ȭ��ǥ ����� ����
	//int arr[5][5] =
	//{
	//	{0,0,1,0,0},
	//	{0,1,1,1,0},
	//	{1,0,1,0,1},
	//	{0,0,1,0,0},
	//	{0,0,1,0,0}
	//};

	// arr[i][j] up
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		// j�� ���� ��� (x), i�� ���� ����� ��� (y)
	//		// ���δ� ���κ��� �ι� ����ؾ���.. -> j*2
	//		gotoXY(x + 2 * j, y + i);
	//		//if (arr[i][j] == 0)
	//		//{
	//		//	setColor(color_black);
	//		//	//printf("  ");
	//		//}	 ��� ����
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}

	// arr[i][j] left
	// y=x ��Ī (�������� y���� �Ʒ��� ����)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x +  2*i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	// arr[i][j] right
	// left�� x= 5 ��Ī (�������� ��ĭ������!!!!!) -> �׷��� (5-1)�� �ƴ϶� (10-2)
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 8 - 2 * i, y + j);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}
	//

	// arr[i][j] 
	// �迭�� �׻� 0��°���� ��µ��� �������!! (5 - 1) = 4
	//for (int i = 0; i < 5; i++)
	//{
	//	for (int j = 0; j < 5; j++)
	//	{
	//		gotoXY(x + 2 * j, y + 4 - i);
	//		
	//		if (arr[i][j] == 1)
	//		{
	//			setColor(color);
	//			printf("  ");
	//		}
	//	}
	//}


	/*setColor(color_black);
	printf("    ");
	setColor(color, 15);
	printf("  \n");

	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("  ");
	setColor(color, 15);
	printf("  \n");

	setColor(color, 15);
	printf("  ");
	setColor(color_black);
	printf("      ");
	setColor(color, 15);
	printf("  ");

	setColor(color_black);*/
	}
}





void Interface()
{


}




///
/// ��Ʈ ���, �̵�

///
/// ������ �ð����� ������Ʈ�� �ϵ��� ����
/// deltaTime 
///

// ���� �ð�
ULONGLONG startTime;
// ���� �ð�
ULONGLONG previousTime;
// ���� �ð�
ULONGLONG currentTime;
// �����ð� - ����ð�
ULONGLONG deltaTime;


const ULONGLONG runningSpeed = 50;		// 0.01 ��
const ULONGLONG BPM = 120;				// ���� 120 bpm �����̸�?
const ULONGLONG noteSpeed = 100;


/// <summary>
/// �ð� �ʱ�ȭ
/// </summary>
void InitTime()
{
	// ��ŸŸ�� �ʱ�ȭ = 0 (�����ð� 0)
	startTime = currentTime = previousTime = GetTickCount64();
	// 1ms ������ ��ȯ
}

void UpdateTime()
{
	previousTime = currentTime;
	currentTime = GetTickCount64();

	deltaTime = currentTime - previousTime;
}

//
//ULONGLONG GetDeltaTime()
//{
//	return deltaTime;								// ���� : 1/1000 ��
//}




///
/// �ö���� ��Ʈ ���� �� ��� ���� �Լ�
/// y �� ���� �� -1 ��Ű��
/// _UIMaxSize.Bottom -1
/// L(8) D(24) U(40) R(56)
void UpdateNotePosition_left(int i)
{
	/*static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;*/

	//ULONGLONG barTime = 60000 / BPM * 4;
	//ULONGLONG noteInterval = barTime / 8;

	//ULONGLONG noteTime = noteInterval * i;
	


	if (l_note[i] == 1)
	{
		//NoteprePos_l[i] = NotecurPos_l[i];
		//ScreenDrawLeftArrow(NoteprePos_l[i], color_black);
		/*if (elapsedTime >= 50)
		{
			
			elapsedTime -= 50;
		}*/

		NotecurPos_l[i].Y --;

		// ���� ȭ��ǥ�� ȭ���� ����� �׸� ��½�Ų��
		// ���� ��� ���� ��
		// ���� �ȿ� Ÿ���ϸ� UImaxSize ���� ����, �̽��� �ֻܼ������ ����
		if (NotecurPos_l[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawLeftArrow(NotecurPos_l[i], color_blue);
	}

	if (l_note[i] == 0)
		return;
}

void UpdateNotePosition_down(int i)
{
	if (d_note[i] == 1)
	{
		NotecurPos_d[i].Y--;

		if (NotecurPos_d[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawDownArrow(NotecurPos_d[i], color_green);
	}

	if (d_note[i] == 0)
		return;
}

void UpdateNotePosition_up(int i)
{
	if (u_note[i] == 1)
	{
		NotecurPos_u[i].Y--;

		if (NotecurPos_u[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawUpArrow(NotecurPos_u[i], color_red);
	}

	if (u_note[i] == 0)
		return;
}


void UpdateNotePosition_right(int i)
{
	if (r_note[i] == 1)
	{
		NotecurPos_r[i].Y--;

		if (NotecurPos_r[i].Y <= consoleScreenSize.Top)
		{
			return;
		}
		ScreenDrawRightArrow(NotecurPos_r[i], color_dark_yellow);
	}

	if (r_note[i] == 0)
		return;
}


void GenerateNote()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	ULONGLONG barTime = 60000 / BPM * 4;
	ULONGLONG noteInterval = barTime / 16;

	int i = 0;
	int noteCount = sizeof(l_note) / sizeof(int);

	for (i = 0; i < noteCount; i++)
	{
		if (elapsedTime >= noteInterval * i)
		{
			UpdateNotePosition_left(i);
			UpdateNotePosition_down(i);
			UpdateNotePosition_up(i);
			UpdateNotePosition_right(i);
		}
	}

}


void UpdateNote()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	if (elapsedTime >= noteSpeed)
	{
		GenerateNote();
		elapsedTime -= noteSpeed;
	}

}




/// ���� (�������� ��� �κ�) �� ���� ����
void UpdateRender()
{
	static ULONGLONG elapsedTime;
	elapsedTime += deltaTime;

	if (elapsedTime >= runningSpeed)
	{
		// ���� ��� ������ �����
		ScreenClear();

		// ���� ����� ������ �ۼ��Ѵ�
		ScreenDrawKeyInterface();
		GenerateNote();
		//UpdateNote();
		PrintAsciiArt(asciiArtFilePath);


		// �� �� ���۸� �����´�
		ScreenFlipping();

		elapsedTime -= runningSpeed;
	}
	
}


///
/// �ִϸ��̼� ���
/// 

// �ƽ�Ű ��Ʈ�� �����ϴ� txt ������ ã�� ���ۿ� �����Ѵ�
void FindAsciiArt(const char* asciiArtFilePath)
{
	FILE* fp;
	char buffer[256];
	int line = 0;
	int index = 0;
	char** asciiArt = (char**)malloc(sizeof(char*) * 4); 
	int artCount = 0;

	// �� ���� �ƽ�Ű ��Ʈ ���� �б�
	for (int i = 0; i < 4; i++) 
	{
		asciiArt[i] = (char*)malloc(sizeof(char) * 5000);
		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
		errno_t err = fopen_s(&fp, buffer, "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(asciiArt[i], sizeof(char), 5000, fp);
		asciiArt[i][readSize] = '\0'; // �������� �� ���� �߰�
		fclose(fp);
		artCount++;
	}
}


// ���ۿ� ����� �ƽ�Ű ��Ʈ�� ����Ѵ�
void PrintAsciiArt(const char* asciiArtFilePath)
{

	FILE* fp;
	char buffer[256];
	int line = 0;
	int index = 0;
	char** asciiArt = (char**)malloc(sizeof(char*) * 4);
	int artCount = 0;

	// �� ���� �ƽ�Ű ��Ʈ ���� �б�
	for (int i = 0; i < 4; i++)
	{
		asciiArt[i] = (char*)malloc(sizeof(char) * 5000);
		snprintf(buffer, 256, "%s%d.txt", asciiArtFilePath, i);
		errno_t err = fopen_s(&fp, buffer, "r");
		if (err != 0) {
			//printf("�ƽ�Ű ��Ʈ ������ ã�� �� �����ϴ�.");
			exit(1);
		}
		int readSize = fread(asciiArt[i], sizeof(char), 5000, fp);
		asciiArt[i][readSize] = '\0'; // �������� �� ���� �߰�
		fclose(fp);
		artCount++;
	}

	for (int i = 0; i < artCount; i++)
	{
		int line = 0;
		int index = 0;
		int x = 80;
		int y = 1;   // posX�� posY�� ���� ��� ��ġ ����


		//printf("\033[%d;%dH", y + line++, x); // Ŀ�� ��ġ ����
		while (asciiArt[i][index] != '\0') {
			//putchar(asciiArt[i][index++]);
			setColor(color_black, color_white);
			ScreenPrint(x, y + line++, asciiArt[i][index++], 1);
			if (asciiArt[i][index] == '\n') {
				index++;
				y++;   // ���� �ٷ� �̵��� ������ ����
				//printf("\033[%d;%dH", posY, posX); // ���� �ٷ� �̵�
				//ScreenPrint(x, y + line++, NULL, 0);
			}
		}
		//Sleep(delay); // ������ �ð���ŭ ���
	}
	

	// �����Ҵ� ����
	for (int i = 0; i < 4; i++) {
		free(asciiArt[i]);
	}
	free(asciiArt);

}












///
/// 04.18
/// ���� �Լ�
/// Ư�� ������ �Է��� �޾Ҵ°�,, �� �����Ѵ�
/// 



int main()
{
	initConsole();
	InitTime();
	system("cls");

	while (1)
	{
		UpdateTime();
		UpdateInput();
		

		UpdateRender();

		

	}
}