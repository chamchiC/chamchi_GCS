# 빌드 및 테스트 가이드

## 사전 요구사항 확인

1. ✅ 커스텀 메시지 XML 파일: `custom-messages/custom_messages.xml`
2. ✅ 생성된 MAVLink 헤더: `libs/mavlink/include/mavlink/v2.0/custom_messages/`
3. ✅ CustomMessageHandler 소스: `custom-messages/CustomMessageHandler.h/cc`
4. ✅ Vehicle 클래스 통합 완료
5. ✅ CMakeLists.txt 설정 완료

## 빌드 단계

### 1. 빌드 디렉토리 생성
```bash
mkdir build
cd build
```

### 2. CMake 설정
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 3. 빌드
```bash
cmake --build . -j$(nproc)  # Linux/Mac
cmake --build . -j%NUMBER_OF_PROCESSORS%  # Windows
```

## 빌드 확인 사항

### 컴파일 단계에서 확인
- ✅ CustomMessageHandler.cc 컴파일 성공
- ✅ Vehicle.cc 컴파일 성공 (CustomMessageHandler 포함)
- ✅ 커스텀 메시지 헤더 파일 인식 확인

### 링크 단계에서 확인
- ✅ CustomMessageHandler 심볼 링크 성공
- ✅ Vehicle 모듈 링크 성공

## 실행 테스트

### 1. QGC 실행
```bash
./qgroundcontrol  # Linux/Mac
qgroundcontrol.exe  # Windows
```

### 2. 로그 확인
QGC 실행 후 다음 로그가 출력되어야 합니다:
```
CustomMessageHandler created
```

### 3. 메시지 수신 테스트
드론/시뮬레이터에 연결 후 `FIRE_MISSION_START` 메시지(ID: 50000)를 수신하면:
```
FireMissionStart received: target_system:X target_component:X ...
```

## 문제 해결

### 컴파일 오류: "mavlink_msg_fire_mission_start.h"를 찾을 수 없음
→ `src/Vehicle/CMakeLists.txt`와 `src/MAVLink/CMakeLists.txt`에 include 경로가 추가되었는지 확인

### 링커 오류: CustomMessageHandler 심볼을 찾을 수 없음
→ `src/Vehicle/CMakeLists.txt`에 CustomMessageHandler 파일이 추가되었는지 확인

### 런타임 오류: CustomMessageHandler가 초기화되지 않음
→ `Vehicle.cc`의 생성자에서 `_customMessageHandler` 초기화 코드 확인

## Git 커밋 체크리스트

다른 사람과 공유하기 전에 다음 파일들이 커밋되어 있는지 확인:

- [ ] `custom-messages/custom_messages.xml`
- [ ] `custom-messages/CustomMessageHandler.h`
- [ ] `custom-messages/CustomMessageHandler.cc`
- [ ] `libs/mavlink/include/mavlink/v2.0/custom_messages/` (전체 디렉토리)
- [ ] `src/Vehicle/Vehicle.h` (수정된 부분)
- [ ] `src/Vehicle/Vehicle.cc` (수정된 부분)
- [ ] `src/Vehicle/CMakeLists.txt` (수정된 부분)
- [ ] `src/MAVLink/CMakeLists.txt` (수정된 부분)

## 완료 확인

모든 단계가 완료되면:
1. ✅ 빌드 성공
2. ✅ 실행 성공
3. ✅ 로그에서 CustomMessageHandler 초기화 확인
4. ✅ 메시지 수신 시 로그 출력 확인

이 모든 것이 확인되면 통합이 완료된 것입니다!
