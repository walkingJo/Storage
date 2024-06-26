# 2024-04-27

드디어 오늘, 게임 엔진 프로젝트를 본격적으로 시작한다!

~~↑↑ 이거 왜 써놓고 작업 안했는지 나는 아직도 모른다.~~

# 2024-04-30

## 게임 엔진의 필요 기능들
게임엔진의 전체적인 틀을 짜기 위해 어떤 기능들일 필요한지 나열해보았다.
- 객체 시스템
   - 움직임 및 회전
   - 애니메이션
   - AI 상태 변화 ← 이거는 객체&시스템 중 어디다 전가할까
- 게임 시스템(`GameEngine`, `GameManager`)
   - 충돌 판정 ← `GameManager`에게의 정보 전달을 어떻게 할 것인지?
   - 객체 지정
   - 점수 등 계산
- 전투 시스템
   - 무기 장착 & 교체
   - 공격
      - 투사체 공격
      - 근접 공격
   - 특수 판정
      - 그로기
      - 잡기
      - 무적
- 카메라 조작

---

## 핵심 클래스 연결도
이 프로젝트의 구조적인 축은 `Game`, `GameEngine`, `GameManager`이고, 기능적인 축은 `RenderEngine`, `PhysicsEngine`, `Object`이다.
위 클래스들은 아래와 같이 연결된다.
```
[구조 연결도]
    > 조작 연결도 참고

[기능 연결도]
                    +--------+
                    | Object |
                    +--------+
                         ↑
           +-------------+-------------+
           |                           |
    +------+-------+           +-------+-------+
    | RenderEngine |           | PhysicsEngine |
    +--------------+           +---------------+
           ↑                           ↑
           +-------------+-------------+
                         |
                   +-----+------+
                   | GameEngine |
                   +------------+
```
각각의 기능은 아래와 같다.
1. `Game`
   - 아래 클래스들에 접속해 동작시키는 역할
   - 초기화 외에 특수하게 조작하는 기능은 없다
2. `GameEngine`
   - 게임의 연산들을 실행시키는 역할
   - 이 클래스는 별도의 조작이 전혀 없을 예정이다
3. `GameManager`
   - 게임을 실질적으로 진행시키는 역할
   - 이 클래스는 무조건 상속을 통해 플레이어가 재정의해야 한다

---

# 2024-05-01

아래 클래스들의 연결을 수행했다.
각각의 관계에 근거하여 생성자 및 `init()`함수, 소멸자를 정의했으며, `GameEngine`과 `GameManager`의 경우에는 특별히 `setManager()`과 `setEngine()`를 만들어 서로를 필드 값으로 지정할 수 있도록 했다.
```
[조작 연결도]
    +------+     +------------+           +---------------+
    | Game +----→| GameEngine |←----+----→| GraphicEngine |
    +------+     +--+---------+     |     +---------------+
                    |      ↑        |     +---------------+
                    ↓      |        +----→| PhysicsEngine |
                 +---------+---+          +---------------+
                 | GameManager |
                 +-------------+
```

## 추가할 것 :
- `GameEngine`클래스에 `GetInput()`함수 추가 및 **입력 전달 방법** 고민
- `Vector3`의 `rotate(axis, radian)`함수를 쿼터니언을 통해 구현

---

## `Model`사이의 연결.

사람처럼 관절이 존재하는 모델을 만들기 위해서는 여러 모델을 상하관계를 두어 연결해야 한다.
1. 모델의 정보는 ***최소한으로 수정***하는 것을 기본으로 한다.
   - 평행이동에 대한 정보는 모델이 아닌 `Object`의 `location`에 저장한다.
   - 회전에 대한 정보는 `Object`와 `Model`모두에 저장한다.
2. 부모 `Model`과의 연결
   - 고려사항 :
     - 인간의 관절처럼 작용해야 한다.
     - 부모 모델이 회전하면 자식 모델 또한 회전해야 한다.
     - 각각의 자식 모델은 방향과 중심점을 가져야 한다.
   - 제안(이지만 사실상 이 방법으로 갈 것 같다.) :
     - `centor`가 갖던 본래의 의미를 버리고, `(0,0,0)`을 중심점으로 갖도록 한다.
     - 모델의 좌표들은 기본적으로 움직이지 않는다. 렌더링 때 사용하는 좌표는 객체를 새로 생성해 사용한다.~~(<<실행 시간이 조금 걱정되는데 일단 적용시키고 보자)~~
     - `centor`와 `direction`을 선언해 **최상위 모델에 대한** 위치, 방향을 저장한다.
       - 상위 모델을 회전시킬 때 수정되는 것은 이 정보들 뿐이다.
       - 최상위 모델의 `centor`는 `(0,0,0)`로 고정된다.

3. `Object`가 움직일 때, 부속된 `Model`도 움직이게 만들기 | **출력 직전에 계산해서 만들기**
   - 후자의 경우 `Model`의 `Center`필드를 제거하는 것이 가능하다.

4. 오브젝트 회전의 중심점 확장
   - 부모 오브젝트의 회전과 자유로운 회전을 위해 회전 중심점을 가변적으로 설정할 수 있어야 한다.
   - 이때, 중심 이외의 회전점을 지정할 수 있도록 함수를 정의해야 한다.

---

# 2024-05-03

## 입력과 관련된 모든 처리는 `InputProcessor` 클래스를 만들어 활용하도록 한다.
기본적으로는 마우스와 키보드 입력만을 저장한다.
기존 버전의 차이점은 다음과 같다.
- 화면(카메라) 회전에 대한 코드는 manager로 이양한다.
- ESC 등의 입력으로 프로세스를 종료할 때, 즉각적으로 종료되지 않고 **딜레이**가 생긴다.
- 마우스의 위치 정보는 `MouseCoord`라는 클래스를 새로 만들어 전달하는걸로

또한 현재의 마우스의 움직임 인식은 클릭 도중에만 적용된다.
이는 마우스를 중앙으로 옮길 수 없기 때문에 쓰는 일종의 꼼수이다.
따라서 마우스를 중앙으로 옮길 수 있다면 이 문제는 해결되지만, **마우스를 중앙으로 옮기는 코드를 어디에 삽입해야 하는지**도 문제다.

```
[InputProcessor 선언에서의 오류]

error C2027: 정의되지 않은 형식 'InputProcessor'을(를) 사용했습니다.

InputProcessor 의 타입으로 선언하는 모든 변수에서 C2027 의 오류가 발생함.
    GameEngine 의 부속 클래스인 InputProcessor 가 그 아래에 있어서 생긴 문제로 추정되며,
    마치 함수 정의를 아래에 둘 때, 프로토타입을 선언하는 것과 같은 이치라고 판단됨.
```

---

## `Update`의 순서?
- `pEngine`, `gEngine`, `manager`, `object`의 업데이트 순서를 어떻게 정해야 하는가?
- 일단 `gEngine`가 가장 마지막에 실행되는 것은 고정이다.
- `manager`를 계산하기 위해서는 충돌 관련 정보가 필요하므로 `pEngine`가 선행되어야 한다.
- `object`의 업데이트는 적 인식 등이 포함되므로 `pEngine`가 선행되어야 한다.
- 그리고 자잘한 것만 하면서도 다른 영향을 받는 것은 좋지 않으니 `object`→`manager`로 한다.

따라서 최종적인 순서는 `pEngine`→`object`→`manager`→`gEngine` 이다.

---

## `GameManager`의 활용!
- 게임 엔진을 조작해 게임을 진행하는 코드를 업데이트에 넣도록 한다.
- 마치 **유티니의 `Scene`과 같은** 활용법도 가능할듯.
  - GameManager를 다수 생성해서 특정 상황에서 교체해 사용

---

## `RenderEngine` 구현

`update()`는 다음의 순서로 구현된다.
```cpp
void update(Object** objects, int objectCount) {
    clear(0, 0, 0);
    //drawAllObject();
    for (int i = 0; i < objectCount; ++i)
        drawObject(objects[i]);
    renderPresent();
}
```
본래대로라면 `GameEngine*`를 받아서 진행해야 하지만, `C2027`오류 때문에 위와 같이 진행하게 됐다.

---

## `Model::rotate()` 구현

`rotate()`는 아래와 같이 구현했다. 변수의 값을 바꾸는 동시에 반환값이 존재한다.
```cpp
Vector3 rotate(const Vector3& axis, float radian) {
    //https://jebae.github.io/quaternion-rotation
    Vector3 qv = sin(radian / 2.0f) * axis;
    float   qw = cos(radian / 2.0f);
    *this += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, *this) + qw * *this);

    return *this;
}
```

---

## 현재 남은 작업들

- `Model`의 부모-자식간 연결의 정하기
- `Object`세부 사항 구현
- `GameManager`의 테스트 케이스 구성
  - 카메라 1개 | 오브젝트 3개 | 광원 1개
- `GraphicEngine`의 렌더링 기능 구현
  - 기저변환, 사영변환
  - 깊이 버퍼 활용
  - 광원 효과 구현
- `PhysicsEngine`의 충돌 판정 및 중력 등 구성
  - 충돌 정보 전달 방식 정하기

---

# 2024-05-04

## 오브젝트의 상태 저장 방식
- 주 상태와 보조 상태를 구분지어 저장하는 기능이 필요하다.
  - `enum class`의 각각의 요소가 하위 개체를 갖는 것이 가능한가?
  - 보조 상태가 얼마 되지 않는다면 `BATTLE_FIGHT`, `BATTLE_GROGGY`처럼 하나하나 다 만드는 것도 상관은 없을 것 같다.
  - 아니면 상태 변수를 2개 만들어 활용하는 것도 고려해볼 만 하다.

---

## manager의 역할
FPS같은 경우는 대부분 객체의 프로세스를 따른다.
그렇다면 `manager`가 하는 일은 무엇인가?
- 카메라 조작
- 배그 : 점수 저장 및 자기장 관리(이동, 피해 등)
- 카드게임류 : GM의 그것
- `Dark Souls` : 진행도 저장 및 NPC 이벤트 관리
`manager`를 단순한 오브젝트에서 격상시킨 것은 의미 있는 일이다.

---

# 2024-05-05

## 오브젝트 회전에 대해서

이전까지는 회전에 의해 좌표 정보가 소실되는 것을 막기 위해 회전 정보를 따로 만들어두고 `PhysicsEngine`, `GraphicEngine`의 계산 직전에 **그때그때** 계산해 쓰는 방식을 생각했다.
앞서 말한 대로 3~4시간 프로그램을 실행시켜도 오브젝트가 깨지지 않을 것이고, 또 자식 모델이 줄줄이 딸린 촉수 같은 경우에는 이쪽이 비용이 덜 들 수도 있기 때문이다.

하지만 중요한 사실을 잊은 것이 있었으니, 물리 시뮬레이션이 추가될 예정이라는 것이다.
물리 시뮬레이션을 하게 된다면 회전된 오브젝트를 참조하는 일이 훨씬 많아질 것이고, 구조도 복잡해질 것이다.

그렇기 때문에 아래와 같이 변경하게 되었다.
- `centor`는 모델의 회전중심
- `vertices`는 `centor`에 대한 상대좌표
  - `vertices`를 절대좌표로 저장하던 이전과는 다르다!
    - 또한 덕분에 `rotate()`의 구현이 한결 편해졌다.
- `Model::rotate()`는 모델의 정보 자체를 수정한다.
- `Model::move()`에 더해, 회전중심을 옮기는 함수도 필요하다.

중요한 점을 일깨워 주신 아버지께 감사를.

### `Object::rotateX()`, `Object::rotateY()`, `Object::rotateZ()`를 위한 `Model`의 필드...?

이를 위해서는 `Model`에 방향벡터가 존재해야 하는 것 아닌가 싶다.

모든 `Model`에 방향벡터가 있으면 골치아프니까 `Object`에만 부여할까?
- 이거 좋은 듯
- 근데 방향벡터 하나만 만들면 의미가 없고, 그렇다고 3개를 만들기에는 36byte라는 메모리가 너무 부담된다...

---

## 마우스 입력

전투 중에는 마우스가 중앙에 고정될 수 있도록 방법을 찾아보자.(`Windows API`를 참고해보자.)
- 이 방식을 사용하면 `SDL_MOUSEMOTION`에 해당하는 연산을 더욱 자연스럽고 직관적으로 작성할 수 있다.

마우스 좌우 키를 구분할 수 있어야 한다.
- 현재는 좌우 구분 없이 입력을 받는다.
- FPS류의 발사, 조준과 같은 기능을 구현하려면 필수적으로 필요하다.

---

## `Game`내부에 선언된 `GameManager`변수의 쓸모?

애초에 `GameManager`의 업데이트는 `GameEngine`에서 책임지고 있고, 그럼 `GameManager`은 특별히 가지고 있을 필요 없는것이 아닌가 라는 생각이 든다.

근거는 아래와 같다.
- `Game`은 `GameManager`를 전혀 조작하지 않는다.
- `Game`은 상속을 전혀 고려하지 않았는데 `GameManager`를 고정적인 변수로 가지고 있는 것은 부자연스럽다.
-  더군다나 `GameManager`는 상속을 무조건 고려하고 만들어졌다.

생각해보니 어차피 `GameManager`를 상속받은 클래스를 여러 개 사용해야 한다면, 이를 통제할 장소가 필요하다.
`main()`에서 하는 건 당연히 미친 짓이고, 차라리 `Game`을 상속받은 클래스가 이를 통제하도록 만들자.

물론 기본 `Game`클래스에서 `GameManager`를 지우는 건 바뀌지 않는다.
~~↑바로 아랫줄에서 부정해버리기~~

### `GameEngine`내부의 `GameManager::Update()`문제

`Game`에서 `GameEngine`의 `GameManager`를 지정해주지 않으면 `nullptr`로 남게 되는데, 문제는 `update()`실행 도중에 `GameManager`에 대한 `NllPointException`이 발생한다는 것이다.

따라서, `Game`내부에 `defaultManager`라는 이름으로 `GameManager`변수를 놓기로 했다.

~~NULL인지 확인하겠답시고 while안에 if를 넣을 수는 없는 노릇이지 않은가~~

---

## `livingObjects`와 `deadObjects`의 관리

이 둘을 왜 만들었는지를 생각해보면, 모든 오브젝트들을 `GameEngine`내부에 선언해두고 활용 및 관리하기 위함이었다.

그러나, 실제로 이들을 활용하기는 매우 어려울 듯 하다.
- 원하는 오브젝트가 어디에 있는지 찾기 힘들다.(사실 이 이유가 가장 크다.)
  - ID를 부여하는 방법도 있긴 하지만, ID 관리 시스템도 만들어야 한다는 것이 무척이나 번거롭다.
- `deadObjects`를 선언하는 의미가 없다.

하지만 `GraphicEngine`에 접근하기 위해 오브젝트의 목록 역시 필요하기도 하다.

따라서 `deadObjects`는 없대더라도, `GameEngine::objects`에서 원하는 오브젝트를 제할 수만 있다면 아마 괜찮을 듯 하다

---

~~## 예상 오류 발생 지점!~~

~~이걸 왜 이제야 생각해냈는지는 모르겠는데 암튼 중요하다.~~

~~0으로 나누는 버그~~
~~- 이전 프로젝트에 분명 뭔가 있었다. 반드시 생각해내기를~~

---

# 2024-05-06

## 클래스 간의 `Objects`의 전달

클래스들 사이에서 `Objects`를 전달하는 것은 아래의 조건을 만족해야 한다.
1. 연결되는 클래스는 `GameManager`, `GameEngine`, `GraphicEngine`, `RenderEngine`이다.
2. `Objects`를 총괄해서 저장할 곳이 필요하다.
   - 일단 `GameManager`과 `GameEngine`중 하나다.
3. 전달 과정은 됟록이면 깔끔하기를 원하기 때문에, 2)의 총괄 클래스를 인자로 맏도록 한다.

# 2024-05-13

## objects 관련

어느 한 곳에 `objects`배열을 선언해 사용할 수 없는 것은 내부에서 원하는 오브젝트를 찾기 힘들기 때문이다.

그렇기 때문에 ID를 부여하여 활용한다면 충분히 써먹을 만 하다고 생각했다.

### Object에 ID 추가

~~가장 간단하면서도 겹칠 우려가 없는 `clock()`으로 초기화하기로 했다.~~

확인 결과, clock()은 숫자가 겹칠 가능성이 있기 때문에 그보다 더 작은 시간을 다루는 `chrono`를 사용하기로 했다.

시간을 확인하는 함수는 다음과 같다.

```cpp
#include <chrono>
std::chrono::system_clock::now();
```

---

### GameManager의 objects 관련

관련 필드를 모두 public으로 처리했다.
- friend 선언 시 정체불명의 오류가 발생하기 때문
  - Object에서 Model 필드를 선언할 때 역시 비슷한 문제가 발생 -> friend에 대해 더 공부하자
 
pEngine, gEngine의 update()의 인자로 manager를 받게 했다.

---

## graphic 관련

drawModel() 및 drawTriangle()의 내용을 대략적으로 작성
- 이 이상을 작성하기 위해서는 기저변환 코드가 필요하다.

---

## Vector3 관련 수정사항

direction을 directionX, directionY, directionZ로 분리해 사용하는 것으로 결정했다.
하지만 이렇게 한다면 메모리 낭비가 되기 때문에 추후에 해결책을 찾아야 한다.

기저변환, rotate(), lookAt(), scale()의 내용 수정.

---

# 2024-05-14

## 기저변환에 대한 오늘의 이해

기저변환은 원론적으로 벡터의 값은 놔둔 채, 표현만을 바꾸는 것이다.

에를 들어 기저 $\alpha$, $\beta$에 대해서

$$
\begin{pmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1 \\
\end{pmatrix}
\= [\alpha],
\begin{pmatrix}
\\
\hat{i} & \hat{j} & \hat{k} \\
\\
\end{pmatrix}
\= [\beta]
$$

벡터 v는 $\alpha$와 $\beta$에 대해 아래와 같이 표현할 수 있다.

$$
v \=
\begin{pmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1 \\
\end{pmatrix}
\begin{pmatrix}
x \\
y \\
z \\
\end{pmatrix}
\=
\begin{pmatrix}
\\
\hat{i} & \hat{j} & \hat{k} \\
\\
\end{pmatrix}
\begin{pmatrix}
a \\
b \\
c \\
\end{pmatrix}
$$

이를 간추리면 아래처럼도 표현할 수 있을 듯 하다.

$$
\begin{pmatrix}
x \\
y \\
z \\
\end{pmatrix} _{[\alpha]}
\=
\begin{pmatrix}
a \\
b \\
c \\
\end{pmatrix} _{[\beta]}
$$

T가 $\alpha$에서 $\beta$로의 기저변환행렬이라 할 때, (기저변환은 단순히 표현의 변환에 지나지 않으므로) 아래와 같이 볼 수 있다.

$$
\begin{aligned}
&
T
\begin{pmatrix}
x \\
y \\
z \\
\end{pmatrix}
\=
\begin{pmatrix}
a \\
b \\
c \\
\end{pmatrix} \\
&
\begin{pmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1 \\
\end{pmatrix}
\begin{pmatrix}
x \\
y \\
z \\
\end{pmatrix}
\=
\begin{pmatrix}
\\
\hat{i} & \hat{j} & \hat{k} \\
\\
\end{pmatrix}
\begin{pmatrix}
a \\
b \\
c \\
\end{pmatrix} \\
&
\begin{pmatrix}
\\
\hat{i} & \hat{j} & \hat{k} \\
\\
\end{pmatrix}^{-1}
\begin{pmatrix}
1 & 0 & 0 \\
0 & 1 & 0 \\
0 & 0 & 1 \\
\end{pmatrix}
\begin{pmatrix}
x \\
y \\
z \\
\end{pmatrix}
\=
\begin{pmatrix}
a \\
b \\
c \\
\end{pmatrix} \\
&
T \=
\begin{pmatrix}
\\
\hat{i} & \hat{j} & \hat{k} \\
\\
\end{pmatrix}^{-1} \\
\end{aligned}
$$

~~아무리 생각해봐도 이 식에서 문제점은 없는데, 도대체 왜 3D renderer에서는 T의 역원을 T 대신 사용했냔 말이다!!!!~~
