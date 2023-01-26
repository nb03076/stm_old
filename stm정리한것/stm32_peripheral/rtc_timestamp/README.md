PC13 rtc 출력 핀에다 푸시버튼을 이용하여 파형이 rising edge로 변할때의 시간을 측정함

edge는 rising/falling으로 설정 가능
timestamp 기능 이용시 캘린더는 작동하나 wakeup / 알람은 사용이 불가능해짐.
만약 사용한다면 알람/타임스탬프/wakeup 기능 다 작동 안함. conflict

타임스탬프 설정할때 콜백함수만 적어주면 끝임. 별도의  api가 필요없음