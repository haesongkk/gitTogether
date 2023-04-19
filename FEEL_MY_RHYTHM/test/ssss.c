#include <stdio.h>
#include <windows.h>
#include <stdbool.h>
#include <string.h>

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
	for (int i = 0; i < 4; i++) {
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
//void PrintAsciiArt(char* asciiArt[], int x, int y, int artCount)
//{
//
//	HANDLE hConsole = GetScreenHandle();
//	COORD cursorPos;
//
//	for (int i = 0; i < artCount; i++)
//	{
//		int line = 0;
//		int index = 0;
//		int posX = x;
//		int posY = y;   // posX�� posY�� ���� ��� ��ġ ����
//		//printf("\033[%d;%dH", y + line++, x); // Ŀ�� ��ġ ����
//		while (asciiArt[i][index] != '\0') {
//			//putchar(asciiArt[i][index++]);
//			ScreenPrint(y + line++, x, asciiArt[i][index++], 1);
//			if (asciiArt[i][index] == '\n') {
//				index++;
//				posY++;   // ���� �ٷ� �̵��� ������ ����
//				printf("\033[%d;%dH", posY, posX); // ���� �ٷ� �̵�
//			}
//		}
//		//Sleep(delay); // ������ �ð���ŭ ���
//	}
//
//
//	// �����Ҵ� ����
//	for (int i = 0; i < 4; i++) {
//		free(asciiArt[i]);
//	}
//	free(asciiArt);
//
//}

int main()
{
	
		system("cls");

		while (1)
		{
			char* asciiArtFilePath = "C:\\Users\\User\\Documents\\GitHub\\gitTogether\\FEEL_MY_RHYTHM\\x64\\Debug\\ascii_art_"; // �ƽ�Ű ���� ���
			int x = 120;
			int y = 10;
			int delay = 100;

			FindAsciiArt(asciiArtFilePath);
		}
	
}