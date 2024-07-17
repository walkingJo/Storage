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
		printf("1~4 중 하나의 수를 선택하세요.\n");
		printf("\t1 : 9x9x9 멩거스펀지\n");
		printf("\t2 : 27x27x27 멩거스펀지\n");
		printf("\t3 : 81x81x81 멩거스펀지\n");
		printf("\t4 : 사각형(테스트용)\n");
		printf("입력 : ");
		scanf("%d", &choice);
		if (1 <= choice && choice <= 4)
			break;
		else
			printf("범위 이탈. 다시 입력하세요.\n\n");
	}
	if (choice == 3)
		printf("| 3,840,000개의 폴리곤을 감당할 각오가 되어 있으리라 믿습니다. |\n");

	RenderEngine rEngine = RenderEngine();
	rEngine.init(fileName[choice - 1]);
	while (rEngine.isRunning())
		rEngine.update();
	rEngine.release();

	return 0;
}