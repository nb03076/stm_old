임종수의 STM32 Cortex-M 완벽활용서 예제문제를 stm32f429zi 디스커버리 보드로 재구성해서 실험해보았음

동기화 순서 Tim1 > Tim2 > Tim3
Tim1 자신이 Tim2한테 동기화 해주고 자신이 받는게 없으니깐 slave mode 는 구성할 필요가 없다
TRGO는 output compare 2를 사용하였고, 채널1은 pwm파형 채널2는 동기화용으로 두었다.
채널 2 ccr값을 이용해서 3ms마다 Tim2 동기화 시켜준다
채널1을 통해 원하는 파형 나오게끔 그냥 설정해둔거...

Tim2는 자신이 Tim1의 trgo로 부터 동기화 받으니깐 slave mode를 trigger로 둔다
그외에는 tim1이랑 동일

Tim3은 자신이 Tim2의 trgo로 부터 동기화 받지만 동기화를 해줄 타이머가 더이상 없으니
채널2는 사용하지 않는다.


결론은 output compare no ch과 TRGO, trigger 모드를 이용하여 동기화를 해줄수있다.