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
