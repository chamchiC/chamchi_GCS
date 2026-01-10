# FIRE_MISSION_START 메시지 송신 기능 완료

## ✅ 완료된 작업

### 1. Vehicle 클래스에 송신 메서드 추가
- ✅ `sendFireMissionStart()` - 특정 좌표로 메시지 송신
- ✅ `sendFireMissionStartAtCurrentPosition()` - 현재 위치로 메시지 송신
- ✅ Q_INVOKABLE로 QML에서 호출 가능하도록 설정

### 2. UI 버튼 추가
- ✅ `FlyViewMap.qml` - 지도 클릭 메뉴에 "Fire Mission Start" 버튼 추가
- ✅ `FlyViewAdditionalActionsList.qml` - 추가 액션 목록에 버튼 추가
- ✅ `FlyViewCustomLayer.qml` - 우측 하단에 고정 버튼 추가

## 📍 버튼 위치

### 1. 지도 클릭 메뉴
- 지도를 우클릭하면 나타나는 메뉴에 "Fire Mission Start" 버튼이 추가됨
- 클릭한 위치를 목표로 메시지 송신

### 2. 추가 액션 목록
- FlyView의 추가 액션 패널에 "Fire Mission Start" 항목 추가
- 현재 드론 위치를 목표로 메시지 송신

### 3. 우측 하단 고정 버튼
- FlyViewCustomLayer에 "Fire Mission" 버튼이 우측 하단에 고정
- 드론이 연결되고 유효한 좌표가 있을 때만 표시

## 🔧 사용 방법

### 방법 1: 지도에서 위치 지정
1. FlyView에서 지도를 우클릭
2. "Fire Mission Start" 버튼 클릭
3. 클릭한 위치로 메시지 송신

### 방법 2: 현재 위치로 송신
1. FlyView의 추가 액션 버튼 클릭
2. "Fire Mission Start" 선택
3. 현재 드론 위치로 메시지 송신

### 방법 3: 우측 하단 버튼
1. FlyView 우측 하단의 "Fire Mission" 버튼 클릭
2. 현재 드론 위치로 메시지 송신

## 📝 송신되는 메시지 내용

- `target_system`: 드론의 시스템 ID
- `target_component`: 드론의 컴포넌트 ID
- `target_lat`: 목표 위도 (deg * 1E7)
- `target_lon`: 목표 경도 (deg * 1E7)
- `target_alt`: 목표 고도 (미터)
- `auto_fire`: 자동 발사 활성화 (1 = 활성화)
- `max_projectiles`: 최대 발사체 수 (10)

## 🎯 테스트 방법

1. QGC 빌드 및 실행
2. 드론/시뮬레이터 연결
3. FlyView로 이동
4. 버튼 클릭하여 메시지 송신
5. 로그에서 "sendFireMissionStart:" 메시지 확인
6. 드론에서 메시지 수신 확인

## 📦 완료된 파일

- `src/Vehicle/Vehicle.h` - 송신 메서드 선언 추가
- `src/Vehicle/Vehicle.cc` - 송신 메서드 구현 추가
- `src/FlightDisplay/FlyViewMap.qml` - 지도 클릭 메뉴에 버튼 추가
- `src/FlightDisplay/FlyViewAdditionalActionsList.qml` - 추가 액션에 버튼 추가
- `src/FlightDisplay/FlyViewCustomLayer.qml` - 고정 버튼 추가

---

**완료일**: 2026-01-10
**상태**: ✅ 송신 기능 완료 및 UI 버튼 추가 완료
