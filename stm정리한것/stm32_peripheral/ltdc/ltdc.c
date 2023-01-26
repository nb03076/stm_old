http://www.lucadavidian.com/2017/10/02/stm32-using-the-ltdc-display-controller/
정리 잘되어있다..  udemy 강의도 참고하자...

===================================================
기본 파라미터 설정

GCR : sync / DE(data not enable) active low로 설정
SSCR : hsync / vsync - 1 설정
BPCR : hsync + hbp - 1
AWCR : hsync + hbp + active width(ex : 320) - 1
TWCR : hsync + hbp + active width + hfp -1

이러한 사양은 disc보드에 있는 ili9341 데이터 시트보면 나와있다
sync, porch, address 부분은 데이터시트 보고 typical 한 값 넣으면 된다.

BCCR : 백그라운드 RGB 컬러 설정

====================================================
layer configuration

WHPCR 수평 window 시작/끝점 정함
WVPCR 수직 window 시작/끝점 정함
LxCR : enable
LxCFBAR : 컬러 프레임 버퍼 메모리 주소
CFBLR : pitch는 240 * 3(rgb888기준인듯? rgb565는 2일듯)
length : Active high width x number of bytes per pixel + 3.
ex) 240*3+3
LxPFCR : 픽셀 포멧(RGB565) 설정

LTDC_LxDCCR 보통 0으로 둠(argb8888 format임)
LTDC_LxCACR 255에 가까울수록 불투명 0에 가까울수록 투명(blend)

프레임 버퍼에서 읽어들인 픽셀 데이터는 LTDC를 거쳐 32비트 ARGB8888로 변환


IER : 인터럽트 설정
ISR : 인터럽트 상태플래그
ICR : 인터럽트 플래그 클리어

LIPCR : 라인 인터럽트 위치 설정
CPSR : 현재 라인 상태(x,y)

CDSR : sync, DE 상태플래그

LxCR : 레이어 on/off 컬러 룩업테이블은 상위 rgb?에서 사용
컬러 룩업테이블은 다시보자.. 무슨 용도인지 이해 못함..
============================================================
유데미 강의 요약(ltdc 부분만) 이 부분은 ltdc 어플리케이션 노트에도 적혀있으니 참고

argb에서 a는 255에 가까울수록 투명도 100퍼 0에 가까울수록 투명도 0
rgb 0 = black
ltdc는 코어가 read할 필요없이 flash > ltdc > tft 로 감
하지만 ltdc가 없다면 flash > core > sram > tft로 감

mipi dpi : gram 지원 x host 컨트롤러가 실시간 stream
vsync hsync de pclk 데이터 라인(rgb) ltdc는 이걸 지원해줌

mipi dbi : gram(모토로라 6800 / 인텔 8080 / spi)인 경우에 사용

vsync : vertical synchronization signal / start a new frame
hsync : start new line of the frame

ex) 480*270 = 1 frame
1프레임마다 vsync(1time)
hsync는 270번(라인마다)

DE : DE = 0 doesn't read(invalid) 1 : read RGB data

vsync width + vertical back porch width + 프레임 끝에 vertical front porch width

back porch는 hsync나 vsync가 다시 high가 됐을때, DE가 high로 돌아갈때 까지의 딜레이를 말함
front porch는 반대 
