# 2024-06-05

## 게임의 기본적인 틀 완성
- 플레이어 조종
- 애니메이션 시스템
- 플레이어 움직임(달리기, 점프, 더블점프 등)
- 충돌 판정 함수들
- 맵 출력

## 버그 수정
- 애니메이션 텍스처 시작 좌표 계산 오류
  - 연산 우선순위를 고려하지 않아 발생
  - `A + B % C` 의 값은 `A + (B % C)` 가 아닌 `(A + B) % C` 이다.
  - ```cpp
    //최종 코드
    SDL_Rect srcRect = {
        16 * (7 * platformThema / 3 + platformType % 6),
        16 * (5 * platformThema % 3 + platformType / 6),
        16, 16
    };
    ```
- constexpr 변수의 선언 위치 오류
  - *.cpp 파일이 아닌 *.h 파일에 선언
  - projectTitle, ScreenWidth, ScreenHeight 의 경우에는 RenderEngine 클래스에 static 타입으로 부속
- Fall 에서 WallJump 로 변환 직후 모션 위치가 한 칸 밀리는 오류
  - WallJump + space 직후에 남아있던 xSpeed 때문에 밀려 발생
  - WallJump 로의 변환과 동시에 xSpeed=0 으로 설정하면 됨
  - ```cpp
    if (!isHeightEnoughToHangOnWall() || (!isPlayerTouchedOnWallRightSide() && !isPlayerTouchedOnWallLeftSide()));
    else if (isLeftKeyPressed && isPlayerTouchedOnWallRightSide() && isHeightEnoughToHangOnWall()) {
        setState(PlayerStateType::WALL_JUMP);
        flipState = FlipStateType::LOOK_AT_RIGHT_ON_WALL;
        xCoord = 0;
        xSpeed = 0;
    }
    else if (isRightKeyPressed && isPlayerTouchedOnWallLeftSide() && isHeightEnoughToHangOnWall()) {
        setState(PlayerStateType::WALL_JUMP);
        flipState = FlipStateType::LOOK_AT_LEFT_ON_WALL;
        xCoord = RenderEngine::ScreenWidth - TextureXYSize;
        xSpeed = 0;
    }
    ``` 

## 이후 계획
- 플레이어 시작 지점
- 맵과 플레이어의 연동
  - 플레이어 콜라이더 지정
- 카메라
- 장애물
  - 점수 시스템

# 2024-06-10

## 수정사항

카메라 기능
- player.draw() 재정의
- ```cpp
  void Player::draw(int screenXCoord, int screenYCoord);
  void Player::drawWithCameraCoord(int centorXCoord, int centorYCoord) {
	  draw(
		  xCoord - (centorXCoord - RenderEngine::centorXCoord),
		  (centorYCoord - RenderEngine::centorYCoord) - yCoord
	  );
  }
  ```
- 지상에서 '↑', '↓'키를 누르면 시야를 위, 아래로 확장할 수 있도록 함

맵 확장
- 황금 블록 텍스쳐 다시 만들기 - 반사 부분을 중점적으로

## 버그 수정

조작감의 개선
- `isPlayerTouchedOnWallRightSide`와 `isPlayerTouchedOnWallLeftSide`를 `isPlayerTouchedGround`와 `isPlayerTouchedOnCeiling`보다 앞에 뒀더니, 원하는 방식대로 (wall jump를 부드럽게) 움직일 수 있게 되었다.

fall 상태일 때 r 로 원위치 시켰을 때, 상태가 유지되는 문제
- 조작감 개선과 함께 해결함

## 깨달음

변수 기본값 설정은 선언 단계에서만 해도 된다.
- 그러니까 *.cpp파일이 아닌 *.h파일에만 적용하면 된다
- ex)
  - ```cpp
    void setCameraCoord(int xCoord = centorXCoord, int yCoord = centorYCoord);
    void RenderEngine::setCameraCoord(int xCoord, int yCoord) {
	    this->cameraXCoord = xCoord;
  	  this->cameraYCoord = yCoord;
    }
    ```

## 추가로 할 것

맵 - 텍스처 자동 설정 기능 추가

애니메이션 일반화
- 플레이어 캐릭터 외에도 지형, 적 등에도 활용하기 위함
  - 충돌 역시 SDL_Rect를 사용해 일반화하자
- 클래스에 담을 정보
  - 이미지 프레임의 수
    - 나중에 이미지 크기 정보를 통해 자동화가 가능한지 확인하
  - 파일명
  - 프레임별 시간
- 필요한 함수
  - 텍스처 반환
  - 애니메이션 인덱스 반환
  - 애니메이션을 특정 장소에 출력?
- 클래스화
  - ```cpp
    class AnimationData {
    public:
        std::string fileName;
        short imgCount;
        clock_t frameTime;

        SDL_Texture* getTexture();
        static int getFrameIndex(int maxIdx);
        void drawAnimation(SDL_Rect* dstRect);
    }
    ```
