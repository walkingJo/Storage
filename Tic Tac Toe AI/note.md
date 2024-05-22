# 2024-05-20

개발 시작.

게임의 전반적인 틀을 만들었으며, AI가 입력 가능한 형태로 만들었다.

게임 시스템에서 아직 점수 표기 기능을 완성하지 못했는데, 시간 낭비인 듯 해서 `printf()`로 때웠다.

이제 `AI`클래스에 지성을 불어넣기만 하면 된다.

## 자잘한 버그들

```
1>C:\Users\rererererelo\Desktop\Projects\C++ Projects\Project Tic Tac Toe AI\Project Tic Tac Toe AI\AI.h(25,6): error C2143: 구문 오류: ';'이(가) '*' 앞에 없습니다.
1>(소스 파일 'Game.cpp'을(를) 컴파일하는 중)
1>C:\Users\rererererelo\Desktop\Projects\C++ Projects\Project Tic Tac Toe AI\Project Tic Tac Toe AI\AI.h(25,6): error C4430: 형식 지정자가 없습니다. int로 가정합니다. 참고: C++에서는 기본 int를 지원하지 않습니다.
1>(소스 파일 'Game.cpp'을(를) 컴파일하는 중)
1>C:\Users\rererererelo\Desktop\Projects\C++ Projects\Project Tic Tac Toe AI\Project Tic Tac Toe AI\AI.h(25,21): error C2238: ';' 앞에 예기치 않은 토큰이 있습니다.
1>(소스 파일 'Game.cpp'을(를) 컴파일하는 중)
```

두 클래스 A와 B가 서로를 참조할 경우 생기는 에러다.

정확한 원인은 아직 파악하지 못했으나, 해결책은 마련했다.

1. 클래스를 `*.h`파일과 `*.cpp`로 파일을 분리하여 개발한다.
2. 다른 클래스 선언 앞에 `class`를 붙인다.

# 2024-05-21

1. 점수 표시 기능을 추가했다.  

각각의 문자를 이미지 파일로서 만들어 출력하도록 했다.

2. 판에 사인이 새겨질 수 있는 모든 경우를 프로그램을 통해 구하고, 파일로 저장했다.

0~(3^9-1)의 3진법 표기에서 (1의 개수)가 (2의 개수)와 같거나 1개 많은 경우에만 출력

```cpp
for (int i = 0; i < 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 * 3 - 1; ++i) {
    short n[9] = {};
    int I = i;
    for (int j = 0; j < 9; ++j) {
        n[j] = I % 3;
        I /= 3;
    }
        
    short oCount = 0;
    short xCount = 0;
    for (int j = 0; j < 9; ++j) {
        switch (n[j]) {
        case 0: break;
        case 1: oCount++; break;
        case 2: xCount++; break;
        }
    }
    if (oCount == xCount || oCount - 1 == xCount) {
        char buff[10] = { '_', '_', '_', '_', '_', '_', '_', '_', '_', '\0' };
        for(int j=0;j<9;++j)
            switch (n[j]) {
            case 0: break;
            case 1: buff[j] = 'O'; break;
            case 2: buff[j] = 'X'; break;
            }
        printf("%s 0.5\n", buff);
    }
}
```

프로그램을 통해 계산한 전체 경우의 수는 6046으로, 계산한 것과 같다.

$$
\begin{aligned}
& 0 : 1			= & 1 \\
& 1 : 9			= & 9 \\
& 2 : _9C_1 \times _8C_1		= & 72   \\
& 3 : _9C_2 \times _7C_1		= & 252  \\
& 4 : _9C_2 \times _7C_2		= & 756  \\
& 5 : _9C_3 \times _6C_2		= & 1260 \\
& 6 : _9C_3 \times _6C_3		= & 1680 \\
& 7 : _9C_4 \times _5C_3		= & 1260 \\
& 8 : _9C_4 \times _5C_4		= & 630  \\
& 9 : _9C_5 \times _4C_4		= & 126  \\
& \\
& 1+9+72+252+756+1260+1680+1260+630+126 \\
& \= 6046 \\
\end{aligned}
$$

# 2024-05-22

AI는 기본적으로 하나의 클래스로서 작용한다.
