# TH5 – Tích hợp end-to-end (thiết bị → broker → dashboard)

## 1. Nội dung thực hành

### REST API

Xây dựng HTTP Server trên ESP32.

- Kết nối ESP32 với Wi-Fi.
- Xây dựng endpoint `GET /api/sensor`.
- Trả dữ liệu cảm biến dưới định dạng JSON.

Ví dụ JSON:

```json
{
  "id": "nhom08",
  "temp": 29.7,
  "hum": 76.0,
  "uptime": 120
}
```

### MQTT Publish

ESP32 thu thập dữ liệu và gửi lên MQTT Broker định kỳ.

- Đọc nhiệt độ và độ ẩm từ cảm biến AHT20.
- Publish dữ liệu JSON.
- Sử dụng namespace topic riêng.
- Bổ sung thông tin trạng thái LED và nút nhấn.

Broker: `172.16.6.164`

### Dashboard

Các chức năng:
- Hiển thị nhiệt độ realtime.
- Hiển thị độ ẩm.
- Theo dõi trạng thái nút nhấn.
- Hiển thị trạng thái LED.
- Điều khiển bật/tắt LED từ Dashboard.
- Gửi lệnh MQTT ngược về ESP32.

## 2. Kết quả thực nghiệm

Hệ thống hướng tới các chức năng:

1. ESP32 thu thập dữ liệu cảm biến.
2. Dữ liệu được gửi lên MQTT Broker.
3. Node-RED nhận dữ liệu và hiển thị trên Dashboard.
4. Người dùng có thể điều khiển LED từ Dashboard.
5. ESP32 nhận lệnh và cập nhật trạng thái thiết bị.

## 3. Kết luận

Dữ liệu được truyền từ ESP32 lên MQTT Broker.
Dùng Node-RED để xây dựng dashboard theo dõi các thông số cảm biến và điều khiển LED từ xa.

