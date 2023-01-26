12V pin에다가 12V 인가하여 실험해보았음
PD11 : L298 Enable Pin
PD12 : Tim4 channel1 L298 IN1
PD13 : Tim4 channel2 L298 IN2

IN1	IN2	동작방향	전류방향
1	0	forward	OUT1 > OUT2
0	1	reverse	OUT2 > OUT1

H브릿지에 대한 내용은
알기쉽게 풀어 쓴 전력전자공학 5.3절 혹은 모터제어 책 참고
https://www.basic4mcu.com/bbs/board.php?bo_table=gesiyo11&wr_id=127&sca=DC%EB%AA%A8%ED%84%B0&page=2
회로도를 보면 pole이 동시에 on될 경우 큰 단락전류가 흐르므로 and게이트를 이용하여 폴에서 동시에 on이 안되게 해둔듯
dead time이 단락되는 경우를 방지하기 위해 있는것


/* 이건 나만보자... 저작권일수도 */
https://www.studocu.com/ko/document/%EA%B8%88%EC%98%A4%EA%B3%B5%EA%B3%BC%EB%8C%80%ED%95%99%EA%B5%90/%EC%A0%84%EA%B8%B0%EC%A0%84%EC%9E%90%ED%9A%8C%EB%A1%9C/%EA%B0%95%EC%9D%98%EB%85%B8%ED%8A%B8-%EC%A0%84%EB%A0%A5%EC%A0%84%EC%9E%90-5%EC%9E%A5%EC%9D%B8%EB%B2%84%ED%84%B0-%EB%8B%A8%EC%83%81-%EC%9D%B8%EB%B2%84%ED%84%B0-%EB%B6%80%EB%B6%84/28739855


se-dm185 감속비 1/30 엔코더 기어드모터 12V 1.6A를 이용하여 테스트해보았음
분해능 13이라 1회전당 펄스 390개 나옴
https://m.blog.naver.com/PostView.naver?isHttpsRedirect=true&blogId=specialist0&logNo=220697694965
https://www.devicemart.co.kr/goods/view?no=37124
홀센서 output은 1k옴 풀업저항을 달았음
A상이 rising edge일때 B의 값이 0이라면 CW
A상이 rising edge일때 B값이 1이라면 CCW
B상이 rising edge일때 A값이 1이라면 CW
B상이 rising edge일때 A값이 0이라면 CCW

https://gdnn.tistory.com/83