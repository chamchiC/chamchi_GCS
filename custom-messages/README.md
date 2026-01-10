# QGroundControl 커스텀 메시지 구현 가이드

이 가이드는 QGC 5.0.8과 Qt 6.8.3 환경에서 커스텀 MAVLink 메시지를 추가하는 안정적인 방법을 제공합니다.

## 목차

1. [개요](#개요)
2. [사전 요구사항](#사전-요구사항)
3. [구현 단계](#구현-단계)
4. [Vehicle 클래스 통합](#vehicle-클래스-통합)
5. [MAVLink 라이브러리 재생성](#mavlink-라이브러리-재생성)
6. [테스트](#테스트)
7. [주의사항](#주의사항)

## 개요

QGroundControl은 MAVLink 프로토콜을 사용하여 드론과 통신합니다. 커스텀 메시지를 추가하려면:

1. MAVLink 메시지 정의 XML 파일 생성
2. MAVLink 라이브러리 재생성
3. QGC 소스 코드에 메시지 핸들러 추가
4. Vehicle 클래스에 통합

## 사전 요구사항

- QGC 5.0.8 소스 코드
- Qt 6.8.3 설치
- Python 3.x (MAVLink 코드 생성용)
- MAVLink 도구 (mavgen)

## 구현 단계

### 1단계: 커스텀 메시지 정의

`custom_messages.xml` 파일에 원하는 메시지를 정의합니다. 메시지 ID는 256-511 범위를 사용하세요 (커스텀 메시지 범위).

**중요**: 메시지 ID가 다른 메시지와 충돌하지 않도록 주의하세요.

### 2단계: MAVLink 라이브러리 재생성

#### 방법 A: CMake를 통한 자동 생성 (권장)

1. `cmake/CustomOptions.cmake` 파일을 수정하여 커스텀 메시지 XML 파일을 포함:

```cmake
# CustomOptions.cmake에 추가
set(QGC_MAVLINK_CUSTOM_MESSAGES "${CMAKE_SOURCE_DIR}/custom-messages/custom_messages.xml" CACHE STRING "Custom MAVLink messages XML file")
```

2. MAVLink 라이브러리 재생성:
   - QGC 빌드 시 자동으로 처리됩니다.

#### 방법 B: 수동 생성

1. MAVLink 저장소 클론:
```bash
git clone https://github.com/mavlink/mavlink.git
cd mavlink
```

2. 커스텀 메시지 XML을 message_definitions/v1.0/ 디렉토리에 복사

3. C 라이브러리 생성:
```bash
python3 -m pymavlink.tools.mavgen --lang=C --wire-protocol=2.0 --output=generated/include/mavlink/v2.0 custom_messages.xml
```

4. 생성된 파일을 QGC의 MAVLink 라이브러리 디렉토리에 복사

### 3단계: Vehicle 클래스 통합

#### 3.1 CustomMessageHandler 추가

1. `src/Vehicle/Vehicle.h`에 CustomMessageHandler 멤버 추가:

```cpp
// Vehicle.h의 private 멤버 섹션에 추가
CustomMessageHandler* _customMessageHandler = nullptr;
```

2. `src/Vehicle/Vehicle.cc`의 생성자에서 초기화:

```cpp
// Vehicle 생성자에 추가
_customMessageHandler = new CustomMessageHandler(this);
```

3. `src/Vehicle/Vehicle.cc`의 `_mavlinkMessageReceived` 메서드에 통합:

```cpp
// _mavlinkMessageReceived 메서드 내부, switch 문 전에 추가
if (_customMessageHandler->mavlinkMessageReceived(message)) {
    return; // 메시지가 처리되었으면 여기서 반환
}
```

4. `src/Vehicle/Vehicle.cc`의 switch 문에 케이스 추가:

```cpp
// switch (message.msgid) 문 내부에 추가
case MAVLINK_MSG_ID_CUSTOM_STATUS:
case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
case MAVLINK_MSG_ID_CUSTOM_COMMAND:
    // CustomMessageHandler에서 이미 처리됨
    break;
```

#### 3.2 CMakeLists.txt 수정

`src/Vehicle/CMakeLists.txt`에 CustomMessageHandler 소스 파일 추가:

```cmake
set(VEHICLE_SOURCES
    # ... 기존 파일들 ...
    ${CMAKE_SOURCE_DIR}/custom-messages/CustomMessageHandler.cc
    ${CMAKE_SOURCE_DIR}/custom-messages/CustomMessageHandler.h
)
```

### 4단계: 메시지 송신 (선택사항)

커스텀 메시지를 송신하려면:

```cpp
// Vehicle 클래스에 메서드 추가
void Vehicle::sendCustomStatus(uint32_t timestamp_ms, uint8_t statusFlag, 
                                float value1, float value2, const QString& statusText)
{
    mavlink_message_t msg;
    mavlink_custom_status_t customStatus;
    
    customStatus.timestamp_ms = timestamp_ms;
    customStatus.status_flag = statusFlag;
    customStatus.value1 = value1;
    customStatus.value2 = value2;
    strncpy((char*)customStatus.status_text, statusText.toUtf8().constData(), 
            sizeof(customStatus.status_text));
    
    mavlink_msg_custom_status_encode(_id, _compID, &msg, &customStatus);
    sendMessageOnLinkThreadSafe(_priorityLink, msg);
}
```

## Vehicle 클래스 통합 예제

### Vehicle.h 수정

```cpp
// Vehicle.h의 private 멤버 섹션에 추가
#include "CustomMessageHandler.h"

private:
    CustomMessageHandler* _customMessageHandler = nullptr;
```

### Vehicle.cc 수정

```cpp
// Vehicle 생성자에 추가
Vehicle::Vehicle(LinkInterface* link, FirmwarePlugin* firmwarePlugin, Vehicle* parent)
    : VehicleFactGroup(this)
    // ... 기존 초기화 코드 ...
{
    // ... 기존 코드 ...
    
    // 커스텀 메시지 핸들러 초기화
    _customMessageHandler = new CustomMessageHandler(this);
    
    // 커스텀 메시지 핸들러 시그널 연결 (선택사항)
    connect(_customMessageHandler, &CustomMessageHandler::customStatusReceived,
            this, [this](uint32_t timestamp_ms, uint8_t statusFlag, 
                         float value1, float value2, const QString& statusText) {
        qCDebug(VehicleLog) << "Custom status received:" << statusText;
        // 여기에 추가 처리 로직 추가
    });
}

// _mavlinkMessageReceived 메서드 수정
void Vehicle::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
{
    // ... 기존 코드 ...
    
    // 커스텀 메시지 핸들러 처리 (switch 문 전에 추가)
    if (_customMessageHandler && _customMessageHandler->mavlinkMessageReceived(message)) {
        // 메시지가 처리되었으면 여기서 반환하지 않고 계속 진행
        // (다른 핸들러도 메시지를 볼 수 있도록)
    }
    
    // ... 기존 코드 ...
    
    switch (message.msgid) {
    // ... 기존 케이스들 ...
    
    // 커스텀 메시지 케이스 추가
    case MAVLINK_MSG_ID_CUSTOM_STATUS:
    case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
    case MAVLINK_MSG_ID_CUSTOM_COMMAND:
        // CustomMessageHandler에서 이미 처리됨
        break;
    
    // ... 기존 코드 ...
    }
    
    emit mavlinkMessageReceived(message);
}
```

## MAVLink 라이브러리 재생성

### CMake를 통한 자동 재생성

QGC는 빌드 시 MAVLink 라이브러리를 자동으로 다운로드하고 빌드합니다. 커스텀 메시지를 추가하려면:

1. `cmake/CustomOptions.cmake`에서 MAVLink 저장소와 태그를 수정하여 커스텀 메시지가 포함된 버전을 사용
2. 또는 로컬 MAVLink 디렉토리를 사용하도록 설정:

```cmake
# CustomOptions.cmake 또는 CustomOverrides.cmake에 추가
set(CPM_mavlink_SOURCE "${CMAKE_SOURCE_DIR}/path/to/custom/mavlink" CACHE PATH "Local MAVLink source directory")
```

### 수동 재생성

1. MAVLink 메시지 정의에 커스텀 메시지 추가
2. MAVLink C 라이브러리 재생성
3. 생성된 파일을 QGC 프로젝트에 통합

## 테스트

1. QGC 빌드 및 실행
2. 드론 또는 시뮬레이터에 연결
3. 커스텀 메시지 수신 확인 (로그 확인)
4. 필요시 UI에 메시지 표시 추가

## 주의사항

1. **메시지 ID 충돌**: 메시지 ID가 기존 메시지와 충돌하지 않도록 주의하세요. 커스텀 메시지는 256-511 범위를 사용하세요.

2. **MAVLink 버전**: QGC 5.0.8은 MAVLink v2.0을 사용합니다. 메시지 정의 시 올바른 버전을 사용하세요.

3. **펌웨어 동기화**: 펌웨어와 QGC 모두에서 동일한 메시지 정의를 사용해야 합니다.

4. **빌드 순서**: MAVLink 라이브러리를 먼저 재생성한 후 QGC를 빌드해야 합니다.

5. **Qt 버전**: QGC 5.0.8은 Qt 6.8.3과 호환됩니다. 다른 버전을 사용하면 안정성 문제가 발생할 수 있습니다.

## 추가 리소스

- [MAVLink 공식 문서](https://mavlink.io/en/)
- [QGC 개발자 가이드](https://docs.qgroundcontrol.com/Stable_V5.0/en/qgc-dev-guide/)
- [MAVLink 메시지 정의 가이드](https://mavlink.io/en/guide/define_xml_element.html)

## 문제 해결

### 메시지가 수신되지 않는 경우

1. 메시지 ID가 올바른지 확인
2. MAVLink 라이브러리가 재생성되었는지 확인
3. Vehicle 클래스에 핸들러가 올바르게 통합되었는지 확인
4. 로그에서 오류 메시지 확인

### 빌드 오류가 발생하는 경우

1. MAVLink 헤더 파일이 올바르게 포함되었는지 확인
2. 메시지 구조체 이름이 올바른지 확인 (소문자, 언더스코어 사용)
3. CMakeLists.txt에 소스 파일이 추가되었는지 확인

## 라이선스

이 코드는 QGroundControl 프로젝트의 라이선스를 따릅니다.
