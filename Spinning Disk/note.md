# 2024-05-15

프로젝트 몸체 구성

기획
- 처음에는 마우스로 직접 원판을 돌리는 방향으로 기획
- 현실적으로 어려워 레버를 당기는 방식으로 변경

렌더링
- 원판은 직접 찍는 것이 아닌, 이미지를 불러오는 것이 가장 합리적
  - 그러려면 이미지를 만들어야 하는데, 이 부분이 난관
- 숫자 출력은 5*3의 행렬 꼴로 형태를 저장해 두고, 격자 형태로 출력해내는 것이다.

```cpp
const char numberDrawPoint[10][5][4]{
    {
        "###",
        "# #",
        "# #",
        "# #",
        "###",
    }, //0
    {
        " # ",
        "## ",
        " # ",
        " # ",
        "###",
    }, //1
    ...
};

// 10 | x : 1017~1047
for (int h = 0; h < 5; ++h) for (int w = 0; w < 3; ++w)
	if (numberDrawPoint[num10][h][w] != ' ')
		for (int x = 967; x < 977; ++x) for (int y = 150; y < 160; ++y)
			SDL_RenderDrawPoint(renderer, x + 10 * w, y + 10 * h);
// 01 | x : 1053~1083
for (int h = 0; h < 5; ++h) for (int w = 0; w < 3; ++w)
	if (numberDrawPoint[num01][h][w] != ' ')
		for (int x = 1003; x < 1013; ++x) for (int y = 150; y < 160; ++y)
			SDL_RenderDrawPoint(renderer, x + 10 * w, y + 10 * h);
```

# 2024-05-16

현 시점에서 남은 것은 이미지를 만들고, 회전시키는 것뿐이다.

이미지를 회전시켜 출력하는 것은 아래의 코드로 가능하다.

```cpp
//아래의 함수 활용
//SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
void stretchTextureEx(SDL_Renderer* renderer, int x, int y, int w, int h, SDL_Texture* texture,
	float angle, SDL_RendererFlip flip = SDL_FLIP_NONE) {
	SDL_Rect src, dst;
	SDL_Point center;

	src.x = src.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);

	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;

	center.x = w / 2;
	center.y = h / 2;

	SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, &center, flip);
}
```

전체적인 코드는 아래와 같이 하면 되는데, 진짜 이미지 생성을 어떻게 하냐

```cpp
buildImage(objectCount);
texture = IMG_LoadTexture(renderer, "smile.png");
stretchTextureEx(renderer, 450-300, 450-300, 600, 600, texture, objectAngle[0]);
```
