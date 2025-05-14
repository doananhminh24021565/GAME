**Zoombie_Slayer**

**Tên:** Đoàn Anh Minh 

**Mã SV:** 24021565

**Ý tưởng game:** Dựa trên game EVOWAR. 

**Cách chơi game:** Có thể chuyển đổi 2 kiểu nhân vật bằng cách nhấn phím 1 (nhân vật warrior), phím 2 (nhân vật archer). Bằng cách click chuột trái nhân vật warrior sẽ tấn công theo cơ chế, xoay kiếm thành một vòng tròn còn nhân vật archer tấn công theo cơ chế kéo cung và mũi tên sẽ bay ra theo hướng con trỏ chuột. Ngoài ra có tính năng bổ trợ là tăng gấp 3 tốc độ trong khoảng thời gian 2 giây và hồi chiêu mất 5 giây, để kích hoạt cần nhấn phím F

**Các tính năng và thuật toán được sử dụng:**

•	Thư viện SDL.h, SDL_image.h, SDL_mixer.h, SDL_ttf.h - Các struct khác nhau được sử dụng để quản lí các đối tượng game khác nhau

•	3 dạng nhân vật: warrior, archer, enemies (zombie)

•	4 loại màn hình chủ yếu ở trong game: menu chính, màn hình dừng, màn hình game và màn hình sau khi kết thúc game

•	Có sử dụng minimap – phiên bản thu nhỏ dựa trên kích cỡ từ background(1).png. Nhân vật và zombies sẽ bị hạn chế ở trong minimap, cố gắng ra ngoài phạm vi sẽ xuất hiện cơ chế chặn

•	Xử lý fps

•	Xử lý va chạm 

**Chi tiết về từng file trong chương trình:**

•	characters.h + characters.cpp: file xử lí tất cả mọi thứ liên quan đến nhân vật chính của game

•	enemies.h: file xử lí cơ chế di chuyển của nhân vật zombie 

•	spawnenemies.h: file xử lý việc xuất hiện và biến mất (gồm va chạm, nổ) của nhân vật zombie

•	arrow.h: file xử  lý mọi thứ liên quan đến mũi tên

•	main_menu.h: file xử lý mọi thứ liên quan đến main menu

•	mini_map.h: file xử lý mọi thứ liên quan đến mini map

•	scrollingbackground.h: file xử lý việc cuộn nền, cải tiến dựa trên file: https://docs.google.com/document/d/1FZ3jTqHxtyZznNWiJmmve0zYu_aSliUqLP2OsMcdehQ/edit?tab=t.0#heading=h.g69r5ua9lzeh. Có thêm tính năng màn hình trôi theo các phím left, right, up, down

•	graphics.h + graphics.cpp: Sử dụng toàn bộ hàm trong file: https://docs.google.com/document/d/1FZ3jTqHxtyZznNWiJmmve0zYu_aSliUqLP2OsMcdehQ/edit?tab=t.0#heading=h.g69r5ua9lzeh

•	sprite.h: file xử lý fps https://docs.google.com/document/d/1FZ3jTqHxtyZznNWiJmmve0zYu_aSliUqLP2OsMcdehQ/edit?tab=t.0#heading=h.g69r5ua9lzeh. Thêm hàm act, updateAct có các hàm động chuyên biệt như slash, shoot của nhân vật. Cải tiến hàm render, mặt của nhân vật luôn hướng về phía con trỏ chuột

•	function.h: file sinh có công việc giảm bớt độ dài cho file main.cpp

**Các nguồn tham khảo cho từng phần của game:**

•	Hàm xử lý va chạm: Tham khảo ý tưởng của ChatGPT

•	Hàm nhân vật di chuyển, màn hình di chuyển, âm thanh, hình ảnh, font chữ,..: https://docs.google.com/document/d/1FZ3jTqHxtyZznNWiJmmve0zYu_aSliUqLP2OsMcdehQ/edit?tab=t.0#heading=h.g69r5ua9lzeh. Phát triển thêm

•	Đồ hoạ: ngoài background.png nhờ ChatGPT vẽ, còn lại tự vẽ

•	Âm thanh và font chữ: link tải âm thanh https: //pixabay.com/. Font chữ dựa trên file bài giảng

**Checklisk khi chấm bài (danh sách dưới đây liệt kê tính năng để giáo viên dễ chấm bài chứ không có nghĩa các bạn cần làm hết mới được 10 nhé)**

Tự làm những phần nào, phần nào lấy từ các kiểu tutorial/code mẫu: Hàm xử lý va chạm, cuộn nền, file arrow.h tham khảo ChatGPT, các hàm slash, shoot, boost trong characters.h cải tiến dựa trên hàm tick() trong file bài giảng, còn lại tự code

Liệt kê các nguồn tham khảo (code, hình ảnh, âm thanh...) (đã liệt kê ở trên)

Dùng các lệnh vẽ hình: có (SDL_RenderCopyEx)

Texture: có

Background: có

Event bàn phím: có (trong main.cpp, character.h)

Event chuột: có (slash, shoot)

Animation (hoạt hình) có

Xử lý va chạm: có

Score (có tính điểm): có 

Lưu bảng điểm: có (highest score)

Sound: có 

Sound on off: có

Background music: có

Font: có 

Menu: có 

Pause/Resume:có

Status bar / lives (mạng/máu...): Có

Các điểm nổi bật khác: các trạng thái khác nhau của nhân vật, xây dựng map, minimap, nhiều hiệu ứng

Code viết dễ đọc (gv đánh giá)

Hiểu rõ code (gv đánh giá)
