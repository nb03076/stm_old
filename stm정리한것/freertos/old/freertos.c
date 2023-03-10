태스크 : 이름 / 정보 / 스택 크기 / 스택포인터 등..

컨텍스트 스위칭
1. tick interrupt > R0-R3,R12,LR,return address,xPSR
2. RTOS 핸들러 > R4-R11,R14 저장
3. TCB에 스택포인터 저장
4. 복원(다른 태스크로 스위칭시..)
5. 핸들러 종료

세마포어 : 동기화 / 공유 자원 할당 관리
카운팅 세마포어 : 여러개 자원 관리
바이너리 세마포어 : 태스크간의 동기화를 위한 플래그
ex) ADC 변환 이후에만 lcd에 출력 하게끔 만드는것.
뮤텍스 : 공유 자원 동시 접근 제한 용도(lock)

태스크간 통신
- 메모리 공유
- 신호 매커니즘 : 바이너리 세마포어 / 이벤트 플래그(모든 태스크) 
이벤트 플래그 생성/삭제/설정/clear/읽기/기다리기
/ task notify(단일 태스크). 플래그 set/clear/wait
- 메시지 전달
	- 직접 전달 : xStreamBuffer
	- 간접 전달 : 메시지 큐 / 메일박스
	-메시지 큐 : 생성/삭제 /메시지 수/대기열 넣음/가져옴
	
	