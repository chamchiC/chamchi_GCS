# 커스텀 메시지 빠른 시작 가이드

이 가이드는 QGC 5.0.8과 Qt 6.8.3에서 커스텀 메시지를 빠르게 추가하는 방법을 설명합니다.

## 5단계 빠른 시작

### 1단계: 메시지 정의 수정

`custom_messages.xml` 파일을 열어 원하는 메시지를 정의하거나 수정합니다.

```xml
<message id="256" name="CUSTOM_STATUS">
    <description>내 커스텀 메시지</description>
    <field type="uint32_t" name="timestamp_ms">타임스탬프</field>
    <field type="float" name="my_value">내 값</field>
</message>
```

**중요**: 메시지 ID는 256-511 범위를 사용하세요.

### 2단계: MAVLink 라이브러리 재생성

#### 옵션 A: CMake 자동 빌드 (권장)

1. `cmake/CustomOptions.cmake` 또는 `custom/cmake/CustomOverrides.cmake` 파일 생성/수정:

```cmake
# 커스텀 메시지 XML 파일 경로 설정
set(QGC_MAVLINK_CUSTOM_MESSAGES "${CMAKE_SOURCE_DIR}/custom-messages/custom_messages.xml" CACHE STRING "Custom MAVLink messages")
```

2. QGC 빌드 시 자동으로 처리됩니다.

#### 옵션 B: 수동 생성

```bash
# MAVLink 저장소 클론
git clone https://github.com/mavlink/mavlink.git
cd mavlink

# 커스텀 메시지 XML 복사
cp /path/to/qgroundcontrol/custom-messages/custom_messages.xml message_definitions/v1.0/

# C 라이브러리 생성
python3 -m pymavlink.tools.mavgen --lang=C --wire-protocol=2.0 \
    --output=generated/include/mavlink/v2.0 \
    message_definitions/v1.0/custom_messages.xml

# 생성된 파일을 QGC에 복사
cp -r generated/include/mavlink/v2.0/* /path/to/qgroundcontrol/libs/mavlink/include/mavlink/v2.0/
```

### 3단계: Vehicle 클래스 통합

#### 3.1 Vehicle.h 수정

```cpp
// include 섹션에 추가
#include "CustomMessageHandler.h"

// private 멤버에 추가
private:
    CustomMessageHandler* _customMessageHandler = nullptr;
```

#### 3.2 Vehicle.cc 수정

```cpp
// include 섹션에 추가
#include "CustomMessageHandler.h"

// 생성자에 추가
Vehicle::Vehicle(...)
{
    // ... 기존 코드 ...
    
    _customMessageHandler = new CustomMessageHandler(this);
    
    // 시그널 연결 (선택사항)
    connect(_customMessageHandler, &CustomMessageHandler::customStatusReceived,
            this, [this](uint32_t timestamp_ms, uint8_t statusFlag, 
                         float value1, float value2, const QString& statusText) {
        qCDebug(VehicleLog) << "Custom status:" << statusText;
    });
}

// _mavlinkMessageReceived 메서드에 추가
void Vehicle::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
{
    // ... 기존 코드 ...
    
    // 커스텀 메시지 핸들러 처리 (switch 문 전에)
    if (_customMessageHandler && _customMessageHandler->mavlinkMessageReceived(message)) {
        // 메시지 처리됨
    }
    
    switch (message.msgid) {
    // ... 기존 케이스들 ...
    
    // 커스텀 메시지 케이스 추가
    case MAVLINK_MSG_ID_CUSTOM_STATUS:
    case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
    case MAVLINK_MSG_ID_CUSTOM_COMMAND:
        break;
    }
    
    emit mavlinkMessageReceived(message);
}
```

### 4단계: CMakeLists.txt 수정

`src/Vehicle/CMakeLists.txt`에 추가:

```cmake
list(APPEND VEHICLE_SOURCES
    ${CMAKE_SOURCE_DIR}/custom-messages/CustomMessageHandler.cc
    ${CMAKE_SOURCE_DIR}/custom-messages/CustomMessageHandler.h
)
```

### 5단계: CustomMessageHandler 활성화

`CustomMessageHandler.cc` 파일을 열어 주석 처리된 코드를 활성화합니다:

```cpp
void CustomMessageHandler::_handleCustomStatus(const mavlink_message_t& message)
{
    // 주석 해제
    mavlink_custom_status_t customStatus;
    mavlink_msg_custom_status_decode(&message, &customStatus);
    
    // ... 나머지 코드 활성화 ...
}
```

## 빌드 및 테스트

```bash
# 빌드 디렉토리 생성
mkdir build
cd build

# CMake 설정
cmake .. -DCMAKE_BUILD_TYPE=Release

# 빌드
cmake --build . -j$(nproc)

# 실행
./qgroundcontrol
```

## 확인 사항

1. ✅ 메시지 ID가 256-511 범위인가?
2. ✅ MAVLink 라이브러리가 재생성되었는가?
3. ✅ Vehicle 클래스에 핸들러가 통합되었는가?
4. ✅ CMakeLists.txt에 파일이 추가되었는가?
5. ✅ CustomMessageHandler 코드가 활성화되었는가?

## 문제 해결

### 빌드 오류: "mavlink_custom_status_t"를 찾을 수 없음

→ MAVLink 라이브러리가 재생성되지 않았습니다. 2단계를 다시 확인하세요.

### 메시지가 수신되지 않음

→ Vehicle 클래스 통합을 확인하세요. 로그에서 메시지 ID를 확인하세요.

### 링커 오류: CustomMessageHandler 심볼을 찾을 수 없음

→ CMakeLists.txt에 파일이 추가되었는지 확인하세요.

## 다음 단계

- [README.md](README.md)에서 상세한 설명 확인
- [VehicleIntegrationExample.h](VehicleIntegrationExample.h)에서 통합 예제 확인
- UI에 메시지 표시 추가
- 메시지 송신 기능 추가

## 도움말

자세한 내용은 [README.md](README.md)를 참조하세요.
