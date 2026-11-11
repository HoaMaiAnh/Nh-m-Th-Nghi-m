# TH3 – Kết nối không dây trên ESP32

# 1. Nội dung thực hành

## BÀI 1 – Wi-Fi: Access Point và Station

**Phần 1: Access Point**

- Cấu hình ESP32 hoạt động ở chế độ AP.
- Đặt tên và mật khẩu cho mạng Wi-Fi.
- Sử dụng điện thoại hoặc máy tính để tìm kiếm mạng Wi-Fi do ESP32 phát ra.
- Kiểm tra địa chỉ IP của Access Point trên Serial Monitor.

**Phần 2: Station**

- Chuyển ESP32 sang chế độ STA.
- Cấu hình SSID và Password của mạng Wi-Fi cần kết nối.
- Kiểm tra trạng thái kết nối.
- In địa chỉ IP mà ESP32 nhận được.

### Kết quả

- Thiết bị khác có thể tìm thấy và kết nối vào Wi-Fi do ESP32 phát.
- ESP32 có thể kết nối thành công với mạng Wi-Fi.
- Serial Monitor hiển thị địa chỉ IP tương ứng.

## BÀI 2 – BLE: ESP32 làm Bluetooth Server

### Thực hành

- Khởi tạo BLE Device với tên thiết bị riêng.
- Tạo BLE Server.
- Tạo Service và Characteristic.
- Thiết lập UUID cho Service và Characteristic.
- Gán giá trị ban đầu cho Characteristic.
- Bật chế độ Advertising để điện thoại có thể tìm thấy ESP32.

### Kết quả

- Điện thoại tìm thấy thiết bị BLE của ESP32.
- Kết nối thành công với BLE Server.
- Đọc được dữ liệu từ Characteristic.
- Nếu hỗ trợ WRITE, có thể ghi và đọc lại giá trị mới.

## BÀI 3 – ESP-NOW: Truyền nhận dữ liệu giữa hai ESP32

### Mô hình hoạt động

Bài thực hành sử dụng hai kit ESP32:

- **Transmitter:** Thiết bị gửi dữ liệu.
- **Receiver:** Thiết bị nhận dữ liệu.

Transmitter gửi một cấu trúc dữ liệu (struct) chứa số đếm và thông điệp tới Receiver.

### Thực hành

**Receiver:**

1. Khởi tạo Wi-Fi ở chế độ STA.
2. In địa chỉ MAC ra Serial Monitor.
3. Khởi tạo ESP-NOW.
4. Đăng ký callback nhận dữ liệu.
5. Khi nhận được dữ liệu, in số đếm và thông điệp ra Serial Monitor.

**Transmitter:**

1. Khởi tạo Wi-Fi ở chế độ STA.
2. Khởi tạo ESP-NOW.
3. Khai báo MAC Address của Receiver.
4. Đăng ký Receiver làm Peer.
5. Tăng biến đếm sau mỗi lần gửi.
6. Gửi dữ liệu định kỳ tới Receiver.

### Thông tin MAC

| Thiết bị | MAC Address |
|---|---|
| Receiver | `70:4B:CA:03:49:54` |
| Transmitter | `C8:8A:7B:8C:41:44` |

### Kết quả

- Transmitter gửi dữ liệu thành công.
- Receiver nhận được thông điệp.
- Số đếm tăng dần theo mỗi lần gửi

## BÀI 4 – HTTP: Web Server điều khiển LED

### Thực hành

1. ESP32 kết nối Wi-Fi ở chế độ Station.
2. Khởi tạo Web Server
4. Người dùng truy cập địa chỉ IP của ESP32.
5. Khi nhấn nút trên trang web, ESP32 nhận yêu cầu và thay đổi trạng thái LED.

### Kết quả

- ESP32 kết nối Wi-Fi thành công.
- Serial Monitor hiển thị địa chỉ IP.
- Truy cập được Web Server bằng trình duyệt.
- Nhấn BAT LED: LED chuyển sang trạng thái bật.
- Nhấn TAT LED: LED chuyển sang trạng thái tắt.
- Trang web hiển thị đúng trạng thái LED.


## BÀI 5 – MQTT: Publish/Subscribe và điều khiển LED

### Cấu hình MQTT

**Broker:**

```text
IP: localhost
Port: 1883
```

**Topic sử dụng:**

| Topic | Chức năng |
|---|---|
| `bcse/th3/nhom08/data` | Gửi số đếm và trạng thái LED |
| `bcse/th3/nhom08/led` | Nhận lệnh điều khiển LED |

**Payload điều khiển:**

| Payload | Chức năng |
|---|---|
| `1` | Bật LED |
| `0` | Tắt LED |

### Thực hành

1. Kết nối ESP32 với Wi-Fi.
2. Cấu hình địa chỉ MQTT Broker và cổng 1883.
4. Kết nối ESP32 với MQTT Broker.
5. Subscribe topic điều khiển LED.
8. Publish lệnh `1` hoặc `0` để điều khiển LED.

### Kết quả

- ESP32 kết nối thành công với MQTT Broker.
- MQTT Subscriber nhận được số đếm từ ESP32.
- Nhận payload `1` thì LED bật.
- Nhận payload `0` thì LED tắt.

# 2. Kết luận

Qua 5 bài thực hành, tìm hiểu được các phương thức giao tiếp không dây trên ESP32.

| Công nghệ | Kiến thức đạt được |
|---|---|
| Wi-Fi | Cấu hình Access Point và Station |
| BLE | Xây dựng BLE Server, đọc và ghi Characteristic |
| ESP-NOW | Truyền nhận dữ liệu trực tiếp thông qua MAC Address |
| HTTP | Xây dựng Web Server và điều khiển LED bằng trình duyệt |
| MQTT | Publish/Subscribe thông qua Broker và điều khiển LED từ xa |
