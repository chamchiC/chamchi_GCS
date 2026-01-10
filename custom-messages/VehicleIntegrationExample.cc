/****************************************************************************
 *
 * Vehicle 클래스에 커스텀 메시지 핸들러를 통합하는 예제
 * 
 * 이 파일은 src/Vehicle/Vehicle.cc에 추가할 코드 예제를 보여줍니다.
 * 실제 Vehicle.cc 파일을 직접 수정하세요.
 *
 ****************************************************************************/

// ============================================================================
// Vehicle.cc의 include 섹션에 추가
// ============================================================================

#include "CustomMessageHandler.h"

// ============================================================================
// Vehicle 생성자에 추가할 코드
// ============================================================================

Vehicle::Vehicle(LinkInterface* link, FirmwarePlugin* firmwarePlugin, Vehicle* parent)
    : VehicleFactGroup(this)
    // ... 기존 초기화 리스트 ...
{
    // ... 기존 초기화 코드 ...
    
    // 커스텀 메시지 핸들러 초기화
    _customMessageHandler = new CustomMessageHandler(this);
    
    // 커스텀 메시지 핸들러 시그널 연결 (선택사항)
    connect(_customMessageHandler, &CustomMessageHandler::customStatusReceived,
            this, [this](uint32_t timestamp_ms, uint8_t statusFlag, 
                         float value1, float value2, const QString& statusText) {
        qCDebug(VehicleLog) << "Custom status received:"
                            << "timestamp:" << timestamp_ms
                            << "flag:" << statusFlag
                            << "value1:" << value1
                            << "value2:" << value2
                            << "text:" << statusText;
        
        // Vehicle 시그널 발생 (필요한 경우)
        emit customStatusMessageReceived(timestamp_ms, statusFlag, value1, value2, statusText);
    });
    
    connect(_customMessageHandler, &CustomMessageHandler::customSensorDataReceived,
            this, [this](uint32_t timeBootMs, float sensor1, float sensor2, 
                         float sensor3, int16_t temperature, uint8_t sensorId) {
        qCDebug(VehicleLog) << "Custom sensor data received:"
                            << "time_boot_ms:" << timeBootMs
                            << "sensor1:" << sensor1
                            << "sensor2:" << sensor2
                            << "sensor3:" << sensor3
                            << "temperature:" << temperature
                            << "sensor_id:" << sensorId;
        
        // Vehicle 시그널 발생 (필요한 경우)
        emit customSensorDataMessageReceived(timeBootMs, sensor1, sensor2, sensor3, temperature, sensorId);
    });
    
    // ... 기존 코드 계속 ...
}

// ============================================================================
// Vehicle 소멸자에 추가할 코드 (필요한 경우)
// ============================================================================

Vehicle::~Vehicle()
{
    // CustomMessageHandler는 QObject의 자식이므로 자동으로 삭제됩니다.
    // 명시적으로 삭제할 필요는 없지만, 필요하다면:
    // delete _customMessageHandler;
    // _customMessageHandler = nullptr;
}

// ============================================================================
// _mavlinkMessageReceived 메서드에 추가할 코드
// ============================================================================

void Vehicle::_mavlinkMessageReceived(LinkInterface* link, mavlink_message_t message)
{
    // ... 기존 코드 (버전 확인, 링크 확인 등) ...
    
    // Give the plugin a change to adjust the message contents
    if (!_firmwarePlugin->adjustIncomingMavlinkMessage(this, &message)) {
        return;
    }

    // Give the Core Plugin access to all mavlink traffic
    if (!QGCCorePlugin::instance()->mavlinkMessage(this, link, message)) {
        return;
    }

    if (!_terrainProtocolHandler->mavlinkMessageReceived(message)) {
        return;
    }
    _ftpManager->_mavlinkMessageReceived(message);
    _parameterManager->mavlinkMessageReceived(message);
    (void) QMetaObject::invokeMethod(_imageProtocolManager, "mavlinkMessageReceived", Qt::AutoConnection, message);
    _remoteIDManager->mavlinkMessageReceived(message);

    _waitForMavlinkMessageMessageReceivedHandler(message);

    // Battery fact groups are created dynamically as new batteries are discovered
    VehicleBatteryFactGroup::handleMessageForFactGroupCreation(this, message);

    // Let the fact groups take a whack at the mavlink traffic
    for (FactGroup* factGroup : factGroups()) {
        factGroup->handleMessage(this, message);
    }

    this->handleMessage(this, message);

    // ========================================================================
    // 커스텀 메시지 핸들러 처리 추가 (switch 문 전에)
    // ========================================================================
    if (_customMessageHandler && _customMessageHandler->mavlinkMessageReceived(message)) {
        // 메시지가 처리되었지만, 다른 핸들러도 메시지를 볼 수 있도록 계속 진행
        // (필요한 경우 여기서 return 할 수 있음)
    }

    switch (message.msgid) {
    case MAVLINK_MSG_ID_HOME_POSITION:
        _handleHomePosition(message);
        break;
    case MAVLINK_MSG_ID_HEARTBEAT:
        _handleHeartbeat(message);
        break;
    // ... 기존 케이스들 ...
    
    // ========================================================================
    // 커스텀 메시지 케이스 추가
    // ========================================================================
    case MAVLINK_MSG_ID_CUSTOM_STATUS:
    case MAVLINK_MSG_ID_CUSTOM_SENSOR_DATA:
    case MAVLINK_MSG_ID_CUSTOM_COMMAND:
        // CustomMessageHandler에서 이미 처리됨
        // 추가 처리가 필요한 경우 여기에 코드 추가
        break;
    
    // ... 기존 케이스들 계속 ...
    }

    // This must be emitted after the vehicle processes the message. This way the vehicle state is up to date when anyone else
    // does processing.
    emit mavlinkMessageReceived(message);
}

// ============================================================================
// 메시지 송신 메서드 구현 (선택사항)
// ============================================================================

void Vehicle::sendCustomStatus(uint32_t timestamp_ms, uint8_t statusFlag, 
                                float value1, float value2, const QString& statusText)
{
    // 참고: MAVLink 라이브러리를 재생성한 후 아래 코드의 주석을 해제하고 사용하세요.
    /*
    if (!_priorityLink) {
        qCWarning(VehicleLog) << "sendCustomStatus: No priority link available";
        return;
    }
    
    mavlink_message_t msg;
    mavlink_custom_status_t customStatus;
    
    customStatus.timestamp_ms = timestamp_ms;
    customStatus.status_flag = statusFlag;
    customStatus.value1 = value1;
    customStatus.value2 = value2;
    
    // 문자열 복사 (널 종료 문자 포함)
    QByteArray textBytes = statusText.toUtf8();
    int copyLen = qMin(textBytes.length(), static_cast<int>(sizeof(customStatus.status_text) - 1));
    memcpy(customStatus.status_text, textBytes.constData(), copyLen);
    customStatus.status_text[copyLen] = '\0';
    
    mavlink_msg_custom_status_encode(_id, _compID, &msg, &customStatus);
    sendMessageOnLinkThreadSafe(_priorityLink, msg);
    */
    
    qCDebug(VehicleLog) << "sendCustomStatus called (implementation pending MAVLink library regeneration)";
}

void Vehicle::sendCustomSensorData(uint32_t timeBootMs, float sensor1, float sensor2, 
                                    float sensor3, int16_t temperature, uint8_t sensorId)
{
    // 참고: MAVLink 라이브러리를 재생성한 후 아래 코드의 주석을 해제하고 사용하세요.
    /*
    if (!_priorityLink) {
        qCWarning(VehicleLog) << "sendCustomSensorData: No priority link available";
        return;
    }
    
    mavlink_message_t msg;
    mavlink_custom_sensor_data_t sensorData;
    
    sensorData.time_boot_ms = timeBootMs;
    sensorData.sensor1 = sensor1;
    sensorData.sensor2 = sensor2;
    sensorData.sensor3 = sensor3;
    sensorData.temperature = temperature;
    sensorData.sensor_id = sensorId;
    
    mavlink_msg_custom_sensor_data_encode(_id, _compID, &msg, &sensorData);
    sendMessageOnLinkThreadSafe(_priorityLink, msg);
    */
    
    qCDebug(VehicleLog) << "sendCustomSensorData called (implementation pending MAVLink library regeneration)";
}
