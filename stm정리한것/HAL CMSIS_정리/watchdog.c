IWDG : LSI 즉 부정확함. 수십 ms 오차 발생가능성
prescaler / reload 설정

여기서 LSI는 보통 32나 40 khz임 이거 감안해서 prescaler / reload 설정해야함
HAL_IWDG_Refresh 이거 이용해서 counter reload


WWDG : tWWDG = tPCLK1 × 4096 × 2^WDGTB[1:0] × (T[5:0] + 1) (ms)
즉 PCLK1 클럭을 이용한다.