/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include <QtCore/QObject>
#include <QtCore/QLoggingCategory>
#include "MAVLinkLib.h"

Q_DECLARE_LOGGING_CATEGORY(CustomMessageHandlerLog)

/**
 * @brief 커스텀 MAVLink 메시지 핸들러 클래스
 * 
 * 이 클래스는 Vehicle 클래스에서 커스텀 메시지를 처리하기 위한 틀을 제공합니다.
 * QGC 5.0.8과 Qt 6.8.3에서 안정적으로 동작하도록 설계되었습니다.
 */
class CustomMessageHandler : public QObject
{
    Q_OBJECT

public:
    explicit CustomMessageHandler(QObject* parent = nullptr);
    ~CustomMessageHandler();

    /**
     * @brief MAVLink 메시지를 처리합니다.
     * @param message 수신된 MAVLink 메시지
     * @return true: 메시지가 처리됨, false: 메시지가 처리되지 않음
     */
    bool mavlinkMessageReceived(const mavlink_message_t& message);

signals:
    /**
     * @brief FIRE_MISSION_START 메시지가 수신되었을 때 발생하는 시그널
     * @param targetSystem 대상 시스템 ID
     * @param targetComponent 대상 컴포넌트 ID
     * @param targetLat 목표 위도 (deg * 1E7)
     * @param targetLon 목표 경도 (deg * 1E7)
     * @param targetAlt 목표 고도 (미터)
     * @param autoFire 자동 발사 활성화 (0: False, 1: True)
     * @param maxProjectiles 최대 발사체 수
     */
    void fireMissionStartReceived(uint8_t targetSystem, uint8_t targetComponent,
                                 int32_t targetLat, int32_t targetLon, float targetAlt,
                                 uint8_t autoFire, uint8_t maxProjectiles);

private:
    /**
     * @brief FIRE_MISSION_START 메시지를 처리합니다.
     * @param message MAVLink 메시지
     */
    void _handleFireMissionStart(const mavlink_message_t& message);

    // 메시지 ID 상수 정의 (MAVLink 헤더에 정의된 값 사용)
    enum { MSG_ID_FIRE_MISSION_START = 60000 };
};
