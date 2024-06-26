## 세 점을 지나는 평면의 방정식

일반적으로 평면은 법선과 평면 위의 한 점을 통해서 정의된다.

한 점은 임의의 한 점 `P0`로 정하고, 법선은 `P0P1`, `P0P2` 사이의 외적을 통해서 얻을 수 있다.

`P0P1`, `P0P2` 사이의 외적이자, 평면의 법서벡터를 n이라고 할 때, 평면의 방정식은 다음과 같다.

$$
n•(x-x_0, y-y_0, z-z_0)=0
$$

여기서 `(x, y)`값은 고정이기 때문에 식을 전개하고 대입하면 z를 얻을 수 있다.

```
$$
n•(x-x0, y-y0, z-z0)=0, n.x(x-p.x)+n.y(y-p.y)+n.z(z-p.z)=0

n.z(z-p.z) = -\{ n.x(x-p.x)+n.y(y-p.y) \}

z = -\{ n.x(x-p.x)+n.y(y-p.y) \} / n.z + p.z
$$
```

위의 방식으로 사각형 위의 특정한 한 점의 깊이는 알 수 있게 되었다.

## 그럼 이제 어떻게 그릴 좌표들을 특정할 수 있는지 알아보자

다만, 문제는 세 점이 주어졌을 때, 그 내부의 점을 어떻게 그릴 것인지...?

1. 어떻게 삼각형의 내부임을 알 수 있는가? >> 굳이 삼각형의 내부임을 알 필요는 없다

2. 삼각형의 외곽선을 먼저 그린 후에, 그 내부를 세로선을 통해 그리는 방식으로
	- 원래 좌표를 참고해야 하는 정보가 있음(해당 좌표점의 색, 해당 좌표점의 깊이)
	- 

선은 그을 수 있다. 실수 변수 써서 그으면 편하게 그릴 수 있음
