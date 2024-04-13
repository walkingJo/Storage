
### 2024-04-06

1. Ctrl + x, c, v :
   - x, v에만 코드를 추가하면 된다. 다만 area의 사용 방법을 다시 이해할 필요가...

2. 이미지 드래그 횟수의 연장 :
   - pressed → dragged → released → draw
   1. area0일 때, 드래그 완료 시 area1로 전환(기존, 변화X)
   2. area1일 때, 영역 안에 마우스를 올려 드래그하면 드래그되도록(일부 기능 유지)
   3. area1일 때, 영역 밖에 마우스를 올려 마우스를 누르면 area0으로 전환되고 종료되도록(코드 추가, 다만 종료 방식이 애매하다)
      - → 입력이 취소될 경우, area9로 바꿔 진행. 문제는 그림판 본체에서 타입을 변환했을 때, 마무리 작업이 실행되지 않고 끝난다는 것.
      - → 캡슐화로 해결

3. 추가로 할 것 :
   1. 커서 이미지 추가
   2. ~~영역 선택 시 출력되는 drawDottedRect의 점선이 area의 상대죄표를 기준으로 하도록~~ >> 실제로는 별 의미 없음
   3. 베지에 곡선을  미분이든 뭐든 계산해서 연산량을 최소화시키고 균등해지도록 함

### 2024-04-13

1. 이미지 영역선택 디테일 추가
   - 영역선택 후 영역 밖에서 드래그하면 영역선택 기능이 실행되어야 함
   - 점선은 드래그 도중에는 보이지 않도록

2. 클립보드 기능 추가
```java
//클립보드로 이미지 붙여넣기
BufferedImage img = ...;
TransferableImage trans = new TransferableImage(img);

Clipboard c = Toolkit.getDefaultToolkit().getSystemClipboard();
c.setContents(trans, this); //setContents(Transferable contents, ClipboardOwner owner)
```
```java
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
```java
//클립보드에서 이미지 가져오기
try {
    Transferable content = Toolkit.getDefaultToolkit().getSystemClipboard().getContents(null);
    if (content == null); //클립보드에 아무것도 없음 ← 실행되지 않도록 예외처리
    if (!content.isDataFlavorSupported(DataFlavor.imageFlavor)); //클립보드에 이미지가 없음 ← 실행되지 않도록 예외처리
    BufferedImage img = (BufferedImage) content.getTransferData(DataFlavor.imageFlavor);
} catch (AWTException e) {
    e.printStackTrace();
}
```

3. 추가로 할 것 :
   1. 복사, 붙여넣기 과정의 오류 해결
      - Ctrl+X
         - 오류 목록 분석 및 원인 파악
      - Ctrl+V
         - 입력 즉시 화면에 반영되지 않는 오류
         - 두 번째 입력부터는 백지가 출력되는 오류
   3. 곡선 출력 시 찍는 점의 수 효율적으로 줄이기
      - 이거 어케하는데
         - 미분해서 속도을 구한다 치더라도, 이것만으론 뭘 어찌할 수는 없을 것 같은데
         - for(t=0; t<=1; t+=**1/a**)면 되려나
   4. canvas 기능에 대응하도록 커서 이미지 부여 
      - 이미지 만드는건 따로
      - 커서 위치에 button 만들어서 사용?
   5. 외부 파일을 불러왔을 때, 수정후 종료하면 경고 없이 종료되는 것 수정
      - 수정된 이미지임을 표시하는 데에 문제가 생긴 듯
   6. 그림판에서, 현재 그리기 상태에 따라 숏컷의 이미지가 눌림 상태로 고정되도록
