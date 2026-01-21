# 🚁 Chamchi GCS

**Chamchi Ground Control Station** - QGroundControl 기반 커스텀 GCS

---

## 📋 프로젝트 소개

QGroundControl을 기반으로 개발된 커스텀 지상 관제 시스템입니다.

---

## ✨ 주요 기능

- 🎮 MAVLink 드론 비행 제어
- 🗺️ 미션 플래닝
- 📡 커스텀 MAVLink 메시지 지원
- 🎨 커스터마이즈된 FlyView UI

---

## 🛠️ 빌드 방법

```bash
# 빌드 디렉토리 생성
mkdir build && cd build

# CMake 설정
cmake ..

# 빌드
cmake --build .
```

---

## 📁 프로젝트 구조

```
chamchi_GCS/
├── custom/                 # 커스텀 플러그인 및 UI
│   ├── src/               # 커스텀 소스 코드
│   └── res/               # 커스텀 리소스
├── src/                   # QGC 소스 코드
└── libs/                  # 라이브러리
```

---

## 📜 라이선스

Apache 2.0 License (QGroundControl 기반)

---

## 🔗 참고

- [QGroundControl](https://github.com/mavlink/qgroundcontrol) - 원본 프로젝트
