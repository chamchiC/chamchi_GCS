# QGroundControl (QGC) 통합 워크 플랜
## Humiro Fire Suppression 프로젝트 - QGC 기능 구현

**작성일**: 2026-01-10  
**버전**: v1.0  
**상태**: 초안 작성 완료

---

## 개요

이 문서는 Humiro Fire Suppression 프로젝트의 드론 자동 소화 시스템과 QGroundControl(QGC) 간의 통합을 위한 워크 플랜입니다.  
QGC는 GCS(Ground Control Station) 역할을 수행하며, 드론 제어, 상태 모니터링, 화재 진압 시나리오 관리 기능을 제공합니다.

**참조 문서**: [프로젝트 마스터 플랜](https://github.com/kiy0518/humiro_fire_suppression/blob/main/work-plan/001_PROJECT_MASTER_PLAN.md)

---

## 현재 구현 상태

### ✅ 완료된 기능
- **FIRE_MISSION_START 메시지** (MSG_ID: 50000)
  - 위치 기반 화재 진압 임무 시작 명령
  - QML UI 통합 완료 (`FlyViewCustomLayer.qml`, `FlyViewMap.qml`)
  - 상세 로깅 및 디버깅 기능

---

## QGC 구현 필요한 기능 목록

### Phase 1: 기본 통신 및 상태 모니터링

#### 1.1 화재 진압 임무 상태 메시지 수신 및 표시
**우선순위**: 🔴 높음  
**예상 작업량**: 3-5일

**설명**: 드론이 보내는 화재 진압 임무의 현재 상태를 QGC에서 실시간으로 확인

**구현 내용**:
- **FIRE_MISSION_STATUS 메시지 정의** (MSG_ID: 50001)
  ```xml
  <message id="50001" name="FIRE_MISSION_STATUS">
      <field type="uint8_t" name="mission_id">Mission ID</field>
      <field type="uint8_t" name="phase">Phase (1=접근, 2=대기, 3=Targeting, 4=발사)</field>
      <field type="uint8_t" name="progress" units="%">Progress percentage (0-100)</field>
      <field type="float" name="distance_to_target" units="m">Distance to target (meters)</field>
      <field type="uint8_t" name="hotspot_tracking_active">Hotspot tracking active (0/1)</field>
      <field type="float" name="hotspot_angle_x" units="deg">Hotspot angle X (degrees)</field>
      <field type="float" name="hotspot_angle_y" units="deg">Hotspot angle Y (degrees)</field>
      <field type="uint8_t" name="locked">Locked status (0/1)</field>
  </message>
  ```

- **CustomMessageHandler 확장**
  - `fireMissionStatusReceived` 시그널 추가
  - `Vehicle` 클래스에 상태 업데이트 로직 통합

- **QML UI 구현**
  - Fire Mission Status Panel 추가
  - Phase 표시 (접근/대기/Targeting/발사)
  - 진행률 프로그레스 바
  - 거리 및 각도 정보 표시
  - Locked 상태 표시

**파일**:
- `custom-messages/custom_messages.xml` (메시지 정의 추가)
- `custom-messages/CustomMessageHandler.h/cc` (핸들러 확장)
- `src/Vehicle/Vehicle.h/cc` (상태 관리)
- `src/FlightDisplay/FireMissionStatusPanel.qml` (새 파일)

---

#### 1.2 발사 제어 메시지
**우선순위**: 🔴 높음  
**예상 작업량**: 2-3일

**설명**: Phase 3 (Targeting) 완료 후 발사 명령 전송

**구현 내용**:
- **FIRE_LAUNCH_CONTROL 메시지 정의** (MSG_ID: 50002)
  ```xml
  <message id="50002" name="FIRE_LAUNCH_CONTROL">
      <field type="uint8_t" name="target_system">Target System ID</field>
      <field type="uint8_t" name="target_component">Target Component ID</field>
      <field type="uint8_t" name="command">Command (0=CONFIRM, 1=ABORT, 2=REQUEST_STATUS)</field>
      <field type="float" name="delay" units="s">Launch delay in seconds (0=immediate)</field>
  </message>
  ```

- **QML UI 구현**
  - Fire Launch Control Panel
  - CONFIRM/ABORT 버튼
  - 지연 시간 설정 옵션
  - 안전 확인 다이얼로그 (발사 전 확인)

**파일**:
- `custom-messages/custom_messages.xml` (메시지 정의 추가)
- `src/Vehicle/Vehicle.h/cc` (발사 명령 전송 함수)
- `src/FlightDisplay/FireLaunchControlPanel.qml` (새 파일)

---

#### 1.3 발사 결과 메시지 수신
**우선순위**: 🟡 중간  
**예상 작업량**: 2-3일

**설명**: 발사 완료 후 결과 및 효과 정보 수신

**구현 내용**:
- **FIRE_SUPPRESSION_RESULT 메시지 정의** (MSG_ID: 50003)
  ```xml
  <message id="50003" name="FIRE_SUPPRESSION_RESULT">
      <field type="uint8_t" name="shot_number">Shot number (1-based)</field>
      <field type="int16_t" name="temp_before" units="0.1degC">Temperature before shot (0.1°C)</field>
      <field type="int16_t" name="temp_after" units="0.1degC">Temperature after shot (0.1°C)</field>
      <field type="uint8_t" name="success">Success status (0/1)</field>
      <field type="float" name="accuracy_error" units="m">Accuracy error in meters</field>
  </message>
  ```

- **QML UI 구현**
  - 발사 결과 로그 표시
  - 온도 변화 그래프
  - 정확도 통계

**파일**:
- `custom-messages/custom_messages.xml` (메시지 정의 추가)
- `src/FlightDisplay/FireSuppressionResultPanel.qml` (새 파일)

---

### Phase 2: 영상 스트리밍 통합

#### 2.1 열화상/RGB 영상 스트리밍 수신 및 표시
**우선순위**: 🟡 중간  
**예상 작업량**: 5-7일

**설명**: 드론에서 전송하는 RTSP/HTTP 스트리밍 영상을 QGC에서 표시

**구현 내용**:
- **Video Streaming 파이프라인 구현**
  - RTSP 클라이언트 통합
  - HTTP 스트리밍 지원
  - QML Video 요소 활용

- **QML UI 구현**
  - Fire Mission Video Panel
  - 열화상/RGB 영상 전환 탭
  - 핫스팟 오버레이 표시 (드론에서 전송하는 좌표 기반)
  - 거리 정보 오버레이

**기술 스택**:
- Qt Multimedia (QML Video)
- GStreamer (RTSP 스트리밍, 선택사항)

**파일**:
- `src/VideoStreaming/VideoStreamManager.h/cc` (새 파일)
- `src/FlightDisplay/FireMissionVideoPanel.qml` (새 파일)

---

#### 2.2 영상 오버레이 정보 통합
**우선순위**: 🟡 중간  
**예상 작업량**: 3-4일

**설명**: 드론에서 전송하는 핫스팟 위치, 거리 등 정보를 영상에 오버레이

**구현 내용**:
- **오버레이 데이터 구조**
  - FIRE_MISSION_STATUS의 hotspot_angle_x/y 활용
  - distance_to_target 정보 표시

- **QML Canvas 활용**
  - 핫스팟 위치 마커
  - 거리 텍스트
  - 트래킹 상태 표시

**파일**:
- `src/FlightDisplay/VideoOverlayRenderer.qml` (새 파일)

---

### Phase 3: 고급 제어 및 모니터링

#### 3.1 AI 화재 감지 결과 표시 (Phase 5 연동)
**우선순위**: 🟢 낮음 (추후)  
**예상 작업량**: 4-5일

**설명**: 드론의 AI 화재 감지 결과를 QGC에서 확인

**구현 내용**:
- **AI 화재 감지 메시지 정의** (MSG_ID: 50004, 예정)
  ```xml
  <message id="50004" name="FIRE_DETECTION_AI">
      <field type="uint8_t" name="detection_confidence" units="%">Confidence (0-100)</field>
      <field type="float" name="fire_probability">Fire probability (0.0-1.0)</field>
      <field type="int32_t" name="detected_lat" units="degE7">Detected fire latitude</field>
      <field type="int32_t" name="detected_lon" units="degE7">Detected fire longitude</field>
      <field type="float" name="bounding_box_x">Bounding box X (normalized 0-1)</field>
      <field type="float" name="bounding_box_y">Bounding box Y (normalized 0-1)</field>
      <field type="float" name="bounding_box_w">Bounding box width (normalized 0-1)</field>
      <field type="float" name="bounding_box_h">Bounding box height (normalized 0-1)</field>
  </message>
  ```

- **QML UI 구현**
  - AI 감지 결과 표시
  - 바운딩 박스 오버레이
  - 확률 및 신뢰도 표시
  - 지도에 감지 위치 표시

**파일**:
- `custom-messages/custom_messages.xml` (메시지 정의 추가)
- `src/FlightDisplay/AIFireDetectionPanel.qml` (새 파일)

---

#### 3.2 자율 비행 경로 계획 연동
**우선순위**: 🟢 낮음 (추후)  
**예상 작업량**: 5-7일

**설명**: AI 화재 감지 결과를 기반으로 자율 비행 경로를 QGC에서 설정 및 모니터링

**구현 내용**:
- **Mission Planning 통합**
  - Waypoint 추가 (화재 위치 기반)
  - 경로 시각화
  - 장애물 회피 경로 표시

- **QML UI 구현**
  - Mission Editor 확장
  - 화재 위치 기반 자동 경로 생성
  - 실시간 경로 업데이트

**파일**:
- `src/MissionManager/FireMissionPlanner.h/cc` (새 파일)
- `src/MissionEditor/FireMissionEditor.qml` (새 파일)

---

#### 3.3 LTE 통신 이중화 모니터링
**우선순위**: 🟢 낮음 (추후)  
**예상 작업량**: 2-3일

**설명**: WiFi/LTE 이중화 통신 상태 모니터링

**구현 내용**:
- **통신 상태 메시지 정의** (MSG_ID: 50005, 예정)
  ```xml
  <message id="50005" name="COMMUNICATION_STATUS">
      <field type="uint8_t" name="active_link">Active link (0=WiFi, 1=LTE)</field>
      <field type="uint8_t" name="wifi_quality" units="%">WiFi signal quality (0-100)</field>
      <field type="uint8_t" name="lte_quality" units="%">LTE signal quality (0-100)</field>
      <field type="uint8_t" name="failover_count">Failover count</field>
  </message>
  ```

- **QML UI 구현**
  - 통신 상태 표시
  - 활성 링크 표시
  - 신호 품질 그래프

**파일**:
- `custom-messages/custom_messages.xml` (메시지 정의 추가)
- `src/FlightDisplay/CommunicationStatusPanel.qml` (새 파일)

---

### Phase 4: UI/UX 개선

#### 4.1 Fire Mission 전용 뷰
**우선순위**: 🟡 중간  
**예상 작업량**: 5-7일

**설명**: 화재 진압 임무에 특화된 전용 뷰 생성

**구현 내용**:
- **Fire Mission View 생성**
  - 영상 스트리밍 영역 (대형)
  - 상태 패널 (측면)
  - 제어 패널 (하단)
  - 맵 오버레이 (작은 창)

- **레이아웃 구성**
  ```
  +------------------+------------------+
  |                  |   Status Panel   |
  |   Video Stream   |   - Phase        |
  |   (Main Area)    |   - Progress     |
  |                  |   - Distance     |
  |                  |   - Hotspot      |
  +------------------+------------------+
  |   Control Panel  |   Map Overlay    |
  |   - Launch       |   (Mini)         |
  |   - Abort        |                  |
  +------------------+------------------+
  ```

**파일**:
- `src/FlightDisplay/FireMissionView.qml` (새 파일)
- `src/FlightDisplay/FireMissionViewManager.h/cc` (새 파일)

---

#### 4.2 알림 및 경고 시스템
**우선순위**: 🟡 중간  
**예상 작업량**: 2-3일

**설명**: 화재 진압 임무 중 중요한 이벤트에 대한 알림

**구현 내용**:
- **알림 타입**
  - Phase 변경 알림
  - 10m 도착 알림
  - Locked 상태 알림
  - 발사 완료 알림
  - 오류/경고 알림

- **QML 구현**
  - Toast 알림 시스템
  - 사운드 알림 (선택사항)
  - 알림 로그 패널

**파일**:
- `src/FlightDisplay/FireMissionNotificationManager.qml` (새 파일)

---

#### 4.3 통계 및 리포트 기능
**우선순위**: 🟢 낮음  
**예상 작업량**: 3-4일

**설명**: 화재 진압 임무 수행 통계 및 리포트

**구현 내용**:
- **통계 데이터 수집**
  - 임무 수행 시간
  - 발사 횟수
  - 온도 변화 추이
  - 정확도 통계

- **QML UI 구현**
  - 통계 대시보드
  - 리포트 생성 (PDF/CSV)
  - 그래프 표시 (온도, 정확도 등)

**파일**:
- `src/FlightDisplay/FireMissionStatisticsPanel.qml` (새 파일)
- `src/FlightDisplay/FireMissionReportGenerator.h/cc` (새 파일)

---

## 구현 우선순위

### 🔴 즉시 구현 (1-2주)
1. **FIRE_MISSION_STATUS 메시지 수신 및 표시** (Phase 1.1)
2. **FIRE_LAUNCH_CONTROL 메시지 전송** (Phase 1.2)
3. **기본 Fire Mission UI 패널** (Phase 4.1 기본 버전)

### 🟡 단기 구현 (2-4주)
4. **FIRE_SUPPRESSION_RESULT 메시지 수신** (Phase 1.3)
5. **영상 스트리밍 통합** (Phase 2.1)
6. **영상 오버레이 정보** (Phase 2.2)
7. **알림 시스템** (Phase 4.2)

### 🟢 중장기 구현 (1-3개월)
8. **AI 화재 감지 결과 표시** (Phase 3.1)
9. **자율 비행 경로 계획 연동** (Phase 3.2)
10. **LTE 통신 모니터링** (Phase 3.3)
11. **통계 및 리포트** (Phase 4.3)

---

## 기술 스택 및 의존성

### 필수 의존성
- **Qt 6.8.3** (현재 사용 중)
- **QML/Quick** (UI 프레임워크)
- **MAVLink 2.0** (통신 프로토콜)

### 추가 의존성 (선택사항)
- **Qt Multimedia** (영상 스트리밍)
- **GStreamer** (고급 RTSP 스트리밍, 선택사항)
- **Qt Charts** (통계 그래프, 이미 포함됨)

---

## 파일 구조

```
qgroundcontrol/
├── custom-messages/
│   ├── custom_messages.xml              # 메시지 정의 (확장)
│   ├── CustomMessageHandler.h/cc        # 메시지 핸들러 (확장)
│   └── QGC_WORK_PLAN.md                 # 이 문서
├── src/
│   ├── Vehicle/
│   │   ├── Vehicle.h/cc                 # Vehicle 클래스 (확장)
│   │   └── FireMissionManager.h/cc      # 새 파일 (임무 관리)
│   ├── FlightDisplay/
│   │   ├── FlyViewCustomLayer.qml       # 기존 (확장)
│   │   ├── FireMissionView.qml          # 새 파일 (전용 뷰)
│   │   ├── FireMissionStatusPanel.qml   # 새 파일
│   │   ├── FireLaunchControlPanel.qml   # 새 파일
│   │   ├── FireMissionVideoPanel.qml    # 새 파일
│   │   └── FireMissionNotificationManager.qml  # 새 파일
│   ├── VideoStreaming/                  # 새 디렉토리
│   │   ├── VideoStreamManager.h/cc
│   │   └── RTSPClient.h/cc
│   └── MissionManager/
│       └── FireMissionPlanner.h/cc      # 새 파일
```

---

## 통합 테스트 계획

### 테스트 시나리오

1. **Phase 1 테스트**
   - FIRE_MISSION_START 전송 → 드론 수신 확인
   - FIRE_MISSION_STATUS 수신 → UI 업데이트 확인
   - FIRE_LAUNCH_CONTROL 전송 → 드론 응답 확인

2. **Phase 2 테스트**
   - 영상 스트리밍 연결 → 영상 표시 확인
   - 오버레이 정보 표시 → 정확도 확인

3. **전체 시나리오 테스트**
   - 임무 시작 → 10m 도착 → Targeting → Locked → 발사 → 결과 확인
   - 모든 Phase 전환 확인
   - 오류 처리 확인

---

## 참고 자료

- [프로젝트 마스터 플랜](https://github.com/kiy0518/humiro_fire_suppression/blob/main/work-plan/001_PROJECT_MASTER_PLAN.md)
- [MAVLink 메시지 정의 가이드](https://mavlink.io/en/guide/xml_schema.html)
- [QGroundControl 개발 가이드](https://dev.qgroundcontrol.com/en/)
- [Qt QML 문서](https://doc.qt.io/qt-6/qtqml-index.html)

---

## 다음 단계

1. **현재 워크 플랜 검토 및 승인**
2. **Phase 1.1 구현 시작** (FIRE_MISSION_STATUS)
3. **드론 측 메시지 정의 확인** (호환성 검증)
4. **통합 테스트 환경 구축**

---

**작성자**: AI Assistant  
**버전**: v1.0  
**작성일**: 2026-01-10  
**최종 수정일**: 2026-01-10
