# OpenGL Avoid Balls

Avoid Balls - Né Tránh Những Quả Bóng

## Content:

Written by C programming language, use graphic library OpenGL/GLUT

### Setup:

- Setup MinGW/GCC
- Open main.c
- Only build main.c

Linker:

- Window 32 bit: -lopengl32 -lglu32 -lglut32 (for headers gl.h, glu.h và glut.h)
- Ubuntu: -lGL -lGLU -lglut -lm (for headers gl.h, glu.h, glut.h và math.h)

### Update:

```
v0.2
- Remake all code, image
- Better looking

v0.3
- Add a simple AI
```

### Game:

Your mission is control The Boy avoid all the balls appear randomly each interval time from the left or right. Try to get score as high as possible. You can get the items to make your game easily

The balls will be created every 5 seconds with random velocity and acceleration

Your score will increase sequentially

The Item also appear sequentially but exist only one item at a time

More and more, the game will become more difficult because of many balls

You will gain the invincible ability in short time after touch the ball

Items:

- Shield: Eat 3 shields will give you an ability to block the ball once, and you will gain 2 points score if you eat a shield when you have it
- Coin: Gain 2 points score
- Bomb: 1 Ball will be destroyed each 2 bombs you eat


### Nhiệm vụ:

Giúp cậu bé né tránh các quả bóng, thu thập các vật phẩm với mục đích điểm càng cao càng tốt

Mỗi 5 giây sẽ có một quả bóng mới xuất hiện ngẫu nhiên từ một trong 2 phía (tốc độ rơi và góc rơi ngẫu nhiên)

Các quả bóng sẽ liên tục nảy qua nảy lại, càng lâu đương nhiên càng nhiều bóng, càng khó chơi

Điểm số sẽ tăng liên tục

Các vật phẩm cũng liên tục xuất hiện nhưng mỗi thời điểm chỉ tồn tại một vật phẩm, nghĩa là phải ăn xong mới xuất hiện tiếp

Khi cậu bé bị va trúng bóng sẽ được bất tử trong một thời gian ngắn

Các loại vật phẩm:
- Khiên: Ăn 3 khiên sẽ đỡ được một quả bóng, nếu đang có khiên mà nhặt được vật phẩm này sẽ được cộng thêm 2 điểm
- Đồng xu: Tăng ngay 2 điểm
- Bom: Ăn 2 quả bom sẽ phá hủy được một quả bóng, nếu không có bóng sẽ được tăng 2 điểm

## Youtube:

Game: https://youtu.be/BMMJN_dT2-Q

Auto Play: https://youtu.be/lpj_Kmmz3M0

--- End ---
