timer1을 이용하였음
timer1 slave mode : reset
trigger : TI1FP input capture falling edge를 이용하였음
input capture 핀과 풀업저항이 달린 입력이랑 연결하고 
10ms마다 led 토글링 시킨 뒤, led 토글링 파형을 관찰하였음

버튼을 누르면  버튼의 파형이 3.3 > 0v falling edge가 되고
input capture에서 이를 감지한뒤 timer reset을 시킨다
이 때, 타이머 프리스케일러, 카운터 값이 0으로 초기화하고, 타이머를 다시 동작시킨다
