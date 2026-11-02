#TH2

## 1. So sánh delay(), millis() và Hardware Timer

### 1.1. Mục tiêu
So sánh delay(), millis() và Hardware Timer trên ESP32.

### 1.2. Thông số thử nghiệm
| Thông số | Giá trị |
|---|---|
| Chu kỳ mục tiêu | 100 ms |
| Thời gian thử nghiệm | 3000 ms |
| Baud rate Serial | 115200 |

### 1.3. Kết quả thực nghiệm
| Phương pháp | Số lần | Độ lệch trung bình (ms) | Độ lệch lớn nhất (ms) |
|---|---:|---:|---:|
| delay() | 29 | 0.000 | 0.000 |
| millis() | 29 | 0.001 | 0.010 |
| Hardware Timer | 28 | 0.001 | 0.001 |

### 1.4. Nhận xét và kết luận
- **delay():** Độ lệch đo được rất nhỏ, tuy nhiên chương trình bị blocking trong thời gian chờ.
- **millis():** Không gây blocking, độ lệch lớn nhất 0.010 ms
- **Hardware Timer:** Độ lệch lớn nhất 0.001 ms, duy trì chu kỳ ổn định.

## 2. Deep Sleep định giờ (Timer Wakeup)

### 2.1. Mục tiêu
Tìm hiểu chế độ Deep Sleep của ESP32 và cách sử dụng Timer Wakeup để tự động đánh thức thiết bị sau một khoảng thời gian xác định.

### 2.2. Kết quả quan sát

Chạy chương trình trên ESP32 với thời gian Deep Sleep được thiết lập là 10 giây, ta thấy:

- ESP32 khởi động và hiển thị thông báo trên Serial Monitor.
- Biến `dem` được khai báo bằng `RTC_DATA_ATTR` và tăng thêm 1 sau mỗi lần thức dậy.
- ESP32 tự động đi vào Deep Sleep sau khi in thông báo.
- Sau khoảng 10 giây, ESP32 thức dậy và chạy lại chương trình từ hàm `setup()`.

**Kết quả Serial Monitor:**

```text
ESP32 da thuc day!
So lan thuc day: 1
Ngu 10 giay...

ESP32 da thuc day!
So lan thuc day: 2
Ngu 10 giay...

ESP32 da thuc day!
So lan thuc day: 3
Ngu 10 giay...
```

### 2.3. Nhận xét và kết luận

ESP32 có thể sử dụng Timer Wakeup để tự động thức dậy sau khoảng thời gian Deep Sleep đã thiết lập.

Chương trình được khởi động lại từ hàm `setup()` sau khi ESP32 thức dậy.

Có thể dùng Deep Sleep kết hợp Timer Wakeup và RTC Memory để tiết kiệm năng lượng và lưu trữ dữ liệu qua các lần ngủ.
