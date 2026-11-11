# TH4 - ESP32 - REST API - MQTT - Node-RED

## 1. Nội dung thực hành

### Bài 1: REST API

- Kết nối ESP32 với mạng Wi-Fi.
- Trả về dữ liệu cảm biến dưới định dạng JSON.
- Dữ liệu bao gồm nhiệt độ, độ ẩm, thời gian hoạt động và thông tin Wi-Fi.

### Bài 2: MQTT Publish

- Kết nối ESP32 với MQTT Broker.
- Đọc dữ liệu từ cảm biến AHT20.
- Publish dữ liệu định kỳ 2 giây.

### Bài 3: Node-RED Dashboard và MQTT Subscribe

- Hiển thị nhiệt độ và độ ẩm từ ESP32.
- Tạo nút điều khiển LED thông qua MQTT.
- ESP32 subscribe topic điều khiển và xử lý lệnh nhận được.
- LED được điều khiển thông qua GPIO14.
- Payload `1`: Bật LED.
- Payload `0`: Tắt LED.

### Bài 4: MQTT Explorer

- Kết nối MQTT Explorer với MQTT Broker.
- Theo dõi cây topic của nhóm.
- Kiểm tra payload JSON được ESP32 publish.

## 2. Kết quả đạt được

- Kết nối ESP32 với Wi-Fi thành công.
- Đọc dữ liệu nhiệt độ và độ ẩm từ cảm biến AHT20.
- Publish dữ liệu cảm biến lên MQTT Broker.
- Hiển thị dữ liệu trên Node-RED
- Theo dõi và kiểm tra dữ liệu bằng MQTT Explorer.
