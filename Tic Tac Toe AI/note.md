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

필요한 필드들
- 확률의 저장 방식 : container

- 연결 가능한 수들의 모음 : process
  - 수의 종류는 9-rank로 생각하면 된다
  - ~~그래도 안전하게 vector로 전달할까~~

- 한 경기에서 둔 수의 기록 : container
  - 반추 : process

- 확률은 어떻게 계산할까?
  - 선택할 수 있는 경우가 중복되어 있으니 가중치로 가자(되도록이면 $0\le t\le1$로)

# 2024-05-23

### string out of range 에러 :
- if (!~) return false 하면 될걸 assert 박아서 문제됨
  - 처음에는 assert 자체를 삭제함
  - 그러니까 랭크가 +1만큼 차이나는 경우 외의 경우를 거르지 못해 오류남
  - 원래 코드에서 assert를 if (!~) return false 로 대체함
- ```cpp
  //최종 코드의 일부
  bool AI::isCaseLinked(string srcCase, string dstCase) {
      short srcRank = 0, dstRank = 0;
      for (int i = 0; i < 9; ++i) {
          if (srcCase[i] != '_') srcRank++;
          if (dstCase[i] != '_') dstRank++;
      }
      if (srcRank + 1 != dstRank) //←본래는 이 부분에 assert 가 있었음
          return false;
      ...
  }
  ```

### vector out of range 에러 :
- 처음에 각각의 weight에 1 이상의 가중치 k를 곱하지 않고 비교해 weightSum에서 모든 weight 만큼을 빼도 값이 남아 인덱스 범위를 벗어남
  - 수정 후 선택 좌표값이 (가능한 한) 가장 작은 수가 출력됨 :
    - k = 1000 * weightSum 으로 해야 하는데, k = 10000 * weight 로 함
- ```cpp
  //최종 코드
  Coord AI::selectBestCoordWithRandom() {
      vector<Case> linkedCases = getLinkedCasesWith(fieldToStr());

      float weightSum = 0;
      for (Case singleCase : linkedCases)
          weightSum += singleCase.weight;
      float k = 1000 * 1.0f / weightSum; //k 선언 관련 오류 지점

      float randVal1000 = (float)(rand() % 999);
      for (int i = 0; i < linkedCases.size(); ++i) {
          if (randVal1000 > linkedCases[i].weight * k) { //k를 곱하지 않아 오류가 생긴 지점
              randVal1000 -= linkedCases[i].weight * k;  //
          }
          else {
              for (short x = 0; x < 3; ++x)
                  for (short y = 0; y < 3; ++y)
                      if (fieldToStr()[3 * y + x] != linkedCases[i].strCode[3 * y + x])
                          return Coord(x, y);
          }
      }
  }
  ```
