# MyTetris — STM32F429I-DISC1

## Yêu cầu
- STM32CubeIDE >= 1.19.0
- TouchGFX Designer 3.0.25
- Board STM32F429I-DISC1
- 6 nút tactile switch + breadboard + dây jumper

## Cài đặt
1. Clone repo: git clone <https://github.com/khoichim/MyTetris.git>
2. Mở CubeIDE → File → Open Projects from File System
3. Chọn thư mục MyTetris → Finish
4. Project → Build All (Ctrl+B)
5. Kết nối board qua USB (cổng CN1 ST-Link)
6. Run → Debug (F11) → Resume (F8)

## Đấu nối nút
PG2→LEFT, PG3→RIGHT, PF6→DROP
PG9→ROT_L, PB6→ROT_R, PG14→PAUSE
Tất cả nối qua breadboard → GND
