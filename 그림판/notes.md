
## 2024-04-06

1. Ctrl + x, c, v :
   - x, v에만 코드를 추가하면 된다. 다만 `area`의 사용 방법을 다시 이해할 필요가...

2. 이미지 드래그 횟수의 연장 :
   - `pressed` → `dragged` → `released` → `draw`
   1. `area0`일 때, 드래그 완료 시 `area1`로 전환(기존, 변화X)
   2. `area1`일 때, 영역 안에 마우스를 올려 드래그하면 드래그되도록(일부 기능 유지)
   3. `area1`일 때, 영역 밖에 마우스를 올려 마우스를 누르면 `area0`으로 전환되고 종료되도록(코드 추가, 다만 종료 방식이 애매하다)
      - → 입력이 취소될 경우, `area9`로 바꿔 진행. 문제는 그림판 본체에서 타입을 변환했을 때, 마무리 작업이 실행되지 않고 끝난다는 것.
      - → 캡슐화로 해결

3. 추가로 할 것 :
   1. 커서 이미지 추가
   2. ~~영역 선택 시 출력되는 `drawDottedRect`의 점선이 `area`의 상대죄표를 기준으로 하도록~~ >> 실제로는 별 의미 없음
   3. 베지에 곡선을  미분이든 뭐든 계산해서 연산량을 최소화시키고 균등해지도록 함

## 2024-04-13

1. 이미지 영역선택 디테일 추가
   - 영역선택 후 영역 밖에서 드래그하면 영역선택 기능이 실행되어야 함
   - 점선은 드래그 도중에는 보이지 않도록

2. 클립보드 기능 추가
   - 클립보드로 이미지 붙여넣기
      - ```java
        BufferedImage img = ...;
        TransferableImage trans = new TransferableImage(img);

        Clipboard c = Toolkit.getDefaultToolkit().getSystemClipboard();
        c.setContents(trans, this); //setContents(Transferable contents, ClipboardOwner owner)
        ```
      - ```java
        private class TransferableImage implements Transferable {
            Image i;
            public TransferableImage(Image i) {
                this.i = i;
            }
            @Override
            public Object getTransferData(DataFlavor flavor) throws UnsupportedFlavorException, IOException {
                if (flavor.equals(DataFlavor.imageFlavor) && i != null) {
                    return i;
                } else {
                   throw new UnsupportedFlavorException(flavor);
                }
            }
            @Override
            public DataFlavor[] getTransferDataFlavors() {
               DataFlavor[] flavors = new DataFlavor[1];
               flavors[0] = DataFlavor.imageFlavor;
               return flavors;
            }
            @Override
            public boolean isDataFlavorSupported(DataFlavor flavor) {
                DataFlavor[] flavors = getTransferDataFlavors();
                for (int i = 0; i < flavors.length; i++) {
                    if (flavor.equals(flavors[i])) {
                        return true;
                    }
                }
                return false;
            }
        }
        ```
   - 클립보드에서 이미지 가져오기
      - ```java
        try {
            Transferable content = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null);
            if (content == null); //클립보드에 아무것도 없음 ← 실행되지 않도록 예외처리
            if (!content.isDataFlavorSupported(DataFlavor.imageFlavor)); //클립보드에 이미지가 없음 ← 실행되지 않도록 예외처리
            BufferedImage img = (BufferedImage) content.getTransferData(DataFlavor.imageFlavor);
        } catch (AWTException e) {
            e.printStackTrace();
        }
        ```

4. 추가로 할 것 :
   1. 복사, 붙여넣기 과정의 오류 해결
      - `Ctrl+X`
         - 오류 목록 분석 및 원인 파악
      - `Ctrl+V`
         - 입력 즉시 화면에 반영되지 않는 오류
         - 두 번째 입력부터는 백지가 출력되는 오류
   2. **곡선** 출력 시 찍는 **점의 수 효율적으로 줄이기**
      - 이거 어케하는데
         - 미분해서 속도을 구한다 치더라도, 이것만으론 뭘 어찌할 수는 없을 것 같은데
         - for(t=0; t<=1; t+=**1/a**)면 되려나
   3. `canvas` 기능에 대응하도록 **커서 이미지** 부여
      - 이미지 만드는건 따로
      - 커서 위치에 `button` 만들어서 사용?
   4. 외부 파일을 불러왔을 때, 수정후 종료하면 **경고 없이 종료**되는 것 수정
      - 수정된 이미지임을 표시하는 과정에서 문제가 생긴 듯
   5. 그림판에서, 현재 그리기 상태에 따라 숏컷의 **이미지가 눌림 상태로 고정**되도록
  
## 2024-04-20

1. 이미지가 눌림 상태로 고정되도록
   1. `isClicked` 변수를 따로 만들어서 쓰자
   2. `click`시에 다른 모든 `shortcutButtons`를 리셋한다
   3. 그리고 자신만 갱신하면 끝
   - 문제는 메뉴에서 기능을 선택하면 어떻게 반영시키는지

2. `Ctrl+X,C,V` 문제들 해결
   1. 왜 `Ctrl+X`에서 오류가 발생하는가? 그러면서도 어떻게 멀쩡히 돌아가는가?
   2. 영역선택 후 `Ctrl+V`하면 선택한 영역이 사라지는 문제 해결
      - `draw` 또는 `setdrawtype`(?) 호출로 해결하면 될듯
   3. `Ctrl+V` 이후 생성되는 이미지는 왜 바로 모습이 보이지 않는가
      - `graphics`의 문제일 가능성이 있다
      - 전에 있던 비슷한 문제도 `graphics`를 `getGraphics()`로 받아서 생긴 문제였으니
   - `Ctrl+C`를 해도 마찬가지로 존재는 하나, 화면에 출력되지 않는다.
   - `clipboard`의 오류가 `graphics`의 문제와 연결되어있는듯
      - 아닌 것 같다.
      - 차라리 더 단순한 형태인 `Ctrl+C`를 분석하는 게 더 나을듯
      - ```java
        public void ctrlC() {
            if(!canCtrlC) return;
	
            cbAdapter.CopyImagetoClipBoard(area);
	
            canCtrlX = true; 그림판app.setCtrlXEnabled(canCtrlX);
            canCtrlC = true; 그림판app.setCtrlCEnabled(canCtrlC);
            canCtrlV = true; 그림판app.setCtrlVEnabled(canCtrlV);
	
            그림판app.setFileSaved(true);
        }
        ```
      - `repaint`도 소용이 없다! 하하하하하하하핳ㅎ!

3. 추가로 할 것 :
   1. 2024-04-13의 '추가로 할 것' 참고
  
## 2024-04-27

1. 지난 수업에서 **오류가 발생해도 멀쩡히 돌아가는 것은 잘했지만** 이라고 선생님께서 말씀하셨는데, 이건 무슨 의미인가?
   - `Java` 프로그램에서 오류가 발생하고 멈추는 경우도 있던가?
      - 무한루프 외에는 잘 모르겠다

2. `Ctrl+X,C,V` 문제들 해결
   - `Ctrl+X,C`
      - 아래와 같은 오류 코드 발생
      - > __java.io.IOException__: Registered service providers failed to encode BufferedImage@54b0cb2f: type = 2 DirectColorModel: rmask=ff0000 gmask=ff00 bmask=ff amask=ff000000 IntegerInterleavedRaster: width = 392 height = 266 #Bands = 4 xOff = 0 yOff = 0 dataOffset[0] 0 to image/jpeg
      - 이거 사실 에러 아니고, 디버그 로그로 판별. 아이 좋아
   - `Ctrl+V`
      - 일단 클립보드로부터 이미지를 복사해오는 과정에는 아무 문제가 없음
         - 다만, 해당 이미지를 area에 복사했을 때, 화면에 잘 출력되게만 만들면 될듯
      - 복사 직후 `draw`를 실행해 `img`, `tempImg`에 그려봤지만, 오류 발생
         - `img` 복사는 원본을 건드리는거라 안되고,
         - `tempImg` 복사는 화면에 나타나질 않는다
      - 아니면 설마, `area`에다 이미지를 복사할 때 일어나는 버그인가?
      - 아니, 그보다 **왜** 복사를 두 번 하면 백지가 출력되는데?
         - 분석하자면, 이것은 이미지 복사 과정에서, 원본 이미지는 해당 부분이 비여있지만, 복사 과정에서 draw가 누락되어서 생기는 일인듯.
         - 다만 왜 누락되는지, 왜 draw(tempImg)가 area 출력에 효과가 없는지 파악해야 함
            - 일단 repaint()로 땜빵하긴 했지만, 버그의 원인은 **반드시 파악해야 한다!!!**

3. **그리고 `Ctrl+X,C,V`를 해결하는 도중에 생긴 문제들**
   - `Ctrl+X` 직후에 이미지가 표시되는 문제
     - 이건 우선 `Ctrl+X` 내부를 확인해보자
   - `Ctrl+C` 직후 오른쪽, 아래쪽 점선이 남아있는 문제
     - 아마 `repaint`가 이미지 위에 단순히 덛씌우는 형태이라 그런듯
     - 어떻게 해결해야 하는가?
   - `Ctrl+V` 직후 점선이 표시되지 않는 문제
     - `repaint` 내부에 `draw`를 적절히 섞어주면 해결될 듯

5. **확장 가능성을 고려한 프로그래밍**
   - 이번에 만든 프로그램은 `bufferedImage`변수를 2개 만들어서, 뒤로가기 기능을 만들어도 1회만이 가능하다.
      - 하지만, 이 횟수를 조절하려면 `bufferedImage`를 `queue` 구조로 만들어야 한다.
      - 그러나, 개발 초반에 이것을 놓쳐 지금 이 기능을 구현하려면 프로그램을 갈아엎어야 하는 상황이다.
      - 앞으로 이런 실수들을 줄이기 위해서는 계획 단계에서 어떤 방식으로 접근해야 하는가?
   1. 필드를 선언할 때, 그것의 확장 가능성을 살피자.
      - 비단 변수만이 아니라, 함수도 확장 가능해야 한다.
      - 다만, 함수는 변수에 종속되어 있으므로, 인자나 매개변수 등으로 쓰일 변수가 확장 가능해야하는지만 확인하면 될 것 같다.  

6. 추가로 할 것 :
   1. 붙여넣기를 두 번 했을 때 백지가 출력되는 오류 수정
   2. 색 추출 이후 원래 도구로 돌아가지지 않는 오류
      - 숏컷 사용시 발생하는 문제... 지만 처리는 canvas에서 할 텐데?
   3. ~~외부 파일을 불러왔을 때, 수정후 종료하면 **경고 없이 종료**되는 것 수정~~
      - 이거 사실 당연한거다. 프로그램 종료를 통제하지를 못하는데 뭘 어떻게 해
   4. **곡선** 출력 시 찍는 **점의 수 효율적으로 줄이기**
   5. canvas 기능에 대응하도록 **커서 이미지** 부여
      - 사용할 이미지는 따로 만들기

# 2024-05-04

### 커서 이미지

커서 이미지를 만들어 커서를 따라다니게 만드는 것은 굉장히 비효율적이라 생각된다.
따라서 기각.

그래도 지우개 정도는 보일만 한 것 같기도 하고?

---

### 이전 도구 복귀 관련 오류

추출을 사용했을 때, 직전에 사용한 도구가 다시 불러와지지 않는 오류가 있다.

버그 해결 도중에 `drawType`가 추출 상태라면 `mousePressed`, `mouseDragged`를 실행하지 않도록 조치했는데, 이것 때문에 마우스 좌표를 계산하는 코드까지 생략되어서 제대로 동작하지 않은 일이 있었다.
마우스 좌표만을 받고 이외에는 실행되지 않도록 다시 조치했다.

`extractor(g)`의 `setShortcutButtonOn(idx)`와 관련된 조건문의 `drawType`을 `oldDrawType`로 바꿨지만 결과의 차이점은 없었다.
따라서 이 버그의 원인은 `setDrawType(type)`또는 그 참조에 있으리라 생각된다.

`setDrawType(type)`의 타입 값을 주는 과정에서 그 값을 출력하도록 `sysout`을 추가했다.
```java
[구문 추가]
...
oldDrawType = drawType;
System.out.println("old draw type : " + oldDrawType);  //추가됨
drawType = type;
System.out.println("new draw type : " + drawType);     //추가됨
```
아니나 다를까 `shortcutButtons`를 통해 추츨을 선택했을 때, 아래와 같이 `oldDrawType`과 `newDrawType`이 같아진다.
```
[shotCut 선택 시 출력문]
old draw type : PENCIL
new draw type : EXTRACTOR
old draw type : EXTRACTOR
new draw type : EXTRACTOR
```
또한, 이를 통해 `setDrawType(type)`가 두 번 실행됨을 알 수 있다.
~~그럼 두 번째 `setDrawType(type)`는 도대체 어디에 있는건데!~~

`setDrawType(type)`가 두 번 실행되는 것이 `shortcutClicked()`가 두 번 실행되기 때문이라는 것까지 확인됐다.

`shortcutClicked()`가 두 번 실행되는 원인이 해당 함수가 `mouseClicked(e)`와 `mouseReleased(e)`모두에 포함되어 있었기 때문이라는 결론이 났다.
원인이 이러하기 때문에 드물게 버튼을 클릭했을 때에만 정상적으로 동작했던 것이다...
`mouseClicked(e)`에서 `shortcutClicked()`를 제거한 이후에는 정상적으로 동작한다.

---

### 앞으로 할 것 :

붙여넣기 두 번 실행 시 백지 출력 오류
- `ImgArea`의 초기화와 관련이 있을 것이라 생각된다.

곡선 출력 시의 찍는 점의 수 효율적으로 줄이기
- 이 부분은 외부 자료를 참고하자.
- ~~아직까진 필요한 자료를 못 찾았는데, 언젠가는 찾을 수 있겠지~~
