timer1을 이용하였음
timer1 slave mode : gated
trigger : TI1FP input capture falling edge를 이용하였음
input capture 핀과 풀업저항이 달린 입력이랑 연결하고 
10ms마다 led 토글링 시킨 뒤, led 토글링 파형을 관찰하였음

버튼을 누르면  버튼의 파형이 3.3 > 0v falling edge가 되고
input capture에서 이를 감지한뒤에 타이머가 동작되는걸 볼 수 있다.
level이 0v라면 타이머가 동작하지만 버튼을 뗐을때는 level이 3.3v여서
타이머가 멈추는것 을 볼 수있다.

gated 모드는 전압이 low level일때 타이머가 동작하거나 혹은 동작을 멈추게 할 수 있다.
reset처럼 prescaler, counter 값이 0으로 초기화 되진않고 그냥 계속 동작하는 것