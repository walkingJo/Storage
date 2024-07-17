#define _CRT_SECURE_NO_WARNINGS
#include "RenderEngine.h"

int main(void) {
	string fileName[5] = {
		"menger_sponge9x9x9.txt",
		"menger_sponge27x27x27.txt",
		"menger_sponge81x81x81.txt",
		"test4x4x4.txt",
		"test16x16x16.txt",
	};
	int choice = 0;
	while(true) {
		printf("1~4 �� �ϳ��� ���� �����ϼ���.\n");
		printf("\t1 : 9x9x9 ��Ž�����\n");
		printf("\t2 : 27x27x27 ��Ž�����\n");
		printf("\t3 : 81x81x81 ��Ž�����\n");
		printf("\t4 : �簢��(�׽�Ʈ��)\n");
		printf("�Է� : ");
		scanf("%d", &choice);
		if (1 <= choice && choice <= 4)
			break;
		else
			printf("���� ��Ż. �ٽ� �Է��ϼ���.\n\n");
	}
	if (choice == 3)
		printf("| 3,840,000���� �������� ������ ������ �Ǿ� �������� �Ͻ��ϴ�. |\n");

	RenderEngine rEngine = RenderEngine();
	rEngine.init(fileName[choice - 1]);
	while (rEngine.isRunning())
		rEngine.update();
	rEngine.release();

	return 0;
}