/****************************************************************************
 *
 * Vehicle 클래스에 커스텀 메시지 핸들러를 통합하는 예제
 * 
 * 이 파일은 src/Vehicle/Vehicle.h에 추가할 코드 예제를 보여줍니다.
 * 실제 Vehicle.h 파일을 직접 수정하세요.
 *
 ****************************************************************************/

// ============================================================================
// Vehicle.h의 private 멤버 섹션에 추가할 코드
// ============================================================================

// 1. 전방 선언 추가 (이미 다른 클래스들처럼 선언되어 있을 수 있음)
class CustomMessageHandler;

// 2. private 멤버 변수 추가
private:
    // ... 기존 멤버 변수들 ...
    
    CustomMessageHandler* _customMessageHandler = nullptr;

// ============================================================================
// Vehicle.h의 signals 섹션에 추가할 시그널 (선택사항)
// ============================================================================

signals:
    // ... 기존 시그널들 ...
    
    // 커스텀 메시지 관련 시그널 (필요한 경우)
    void customStatusMessageReceived(uint32_t timestamp_ms, uint8_t statusFlag, 
                                      float value1, float value2, const QString& statusText);
    void customSensorDataMessageReceived(uint32_t timeBootMs, float sensor1, float sensor2, 
                                         float sensor3, int16_t temperature, uint8_t sensorId);

// ============================================================================
// Vehicle.h의 public slots 섹션에 추가할 메서드 (메시지 송신용, 선택사항)
// ============================================================================

public slots:
    // ... 기존 슬롯들 ...
    
    /**
     * @brief 커스텀 상태 메시지 송신
     */
    void sendCustomStatus(uint32_t timestamp_ms, uint8_t statusFlag, 
                          float value1, float value2, const QString& statusText);
    
    /**
     * @brief 커스텀 센서 데이터 메시지 송신
     */
    void sendCustomSensorData(uint32_t timeBootMs, float sensor1, float sensor2, 
                              float sensor3, int16_t temperature, uint8_t sensorId);
