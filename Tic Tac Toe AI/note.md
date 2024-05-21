#2024-05-20

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

#2024-05-21

AI는 기본적으로 하나의 클래스로서 작용한다.
