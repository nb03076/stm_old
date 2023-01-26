infinitime org 소스코드 분석

main

systemtask.start

display app 하고 system task보자...
싱글톤패턴(객체 인스턴스 한번만 하는것... 동적할당 한번만 해주면 되니깐 좋아.. 전역으로 사용)

main함수에서 싹다 객체 인스턴스 함

start(task/queue create) > process(객체인스턴스) > work(실질적인 동작)
여기서 핀 설정이라던지 이것저것함...

spi는 인터럽트핸들러에서 전송 끝나고 나면 notify give from isr 사용
sleep : pin config default
wakeup : pin init

i2c나 spi나 slave device는 버스 마스터 + 주소?(기본파라미터?)가 필요한듯
파라미터는 enum class 이용해서 설정해줌(msb first / mode / spi0,1이런것들)
enum class Gestures : uint8_t { 정의...} 이런식


배터리 감시는 periodic timer 사용(10분....)

버튼 / 터치 gpio 인터럽트




system task
드라이버 클래스는 namespace이용해서 class stmpe811 이렇게만 정의해두고 인스턴스화는 안함
system task 클래스
gpio 같은거만 변수로 넣어둠


system task
start : xQueueCreate(10,1)
process : app->work(app은 this 이용해서 처리)
work : 실제로 일하는것...
일단 각종 클래스 init
필요한 것들은 태스크 생성

while(1)
xQueueReceive(큐,메시지,100)
state machine 이용해서 처리함

push message는
인터럽트일때는 xQueueSendFromISR
이용해서 portYIELD_FROM_ISR사용
그게 아니면 그냥 send

main
button debounce timer
charge debounce charge timer


512바이트씩 읽고
32바이트씩 전송


클록 데이터 같은것들은 따로 구조체 만들어서 넣어버림
screen = lv_scr_act()
style은 필요시에 사용하자.. 일단은 냅둬... 나중에 꾸밀때 바꾸자

화면 밖에 터치시 드롭다운 리스트 삭제
드롭다운 터치시 드롭다운 리스트 관련 설정



ili9341 backlight 설정하자!
brightness low mid high 이렇게 설정해보자



display app
enum으로 page 지정함
load app으로 페이지 불러옴 + 컨트롤러 불러옴
메세지에 따라서 이벤트 처리


lv_task_create 이거로 특정 부분 refresh함


littlevgl 부분


==============================================
spi master : 레지스터 초기화
init : 뮤텍스 / 레지스터 초기화
read/write polling

spi : spi선택 cs핀
read/write 따로 api만듦(cs핀땜에)
cs핀 따로 정해주는 용도로 만든 객체임 별다른건없음


st7789 : spi / 핀
해당 lcd에 맞게끔 read /write 하는거는 spi.read 이런거 래핑함

spiNorFlash : spi 

TWI master : i2c선택 / sck sda 핀

cst816s 터치패널 : i2c / slave addr

littlevgl : lcd / touch panel

controller 부분은 static으로 두어서 싱글톤 유지함




==========================================================
crazyflie

