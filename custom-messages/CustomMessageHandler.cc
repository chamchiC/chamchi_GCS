/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "CustomMessageHandler.h"
#include "QGCLoggingCategory.h"

// MAVLink 커스텀 메시지 헤더 포함
#include <mavlink/v2.0/custom_messages/mavlink_msg_fire_mission_start.h>

QGC_LOGGING_CATEGORY(CustomMessageHandlerLog, "gcs.custom.messagehandler")

CustomMessageHandler::CustomMessageHandler(QObject* parent)
    : QObject(parent)
{
    qCDebug(CustomMessageHandlerLog) << "CustomMessageHandler created";
}

CustomMessageHandler::~CustomMessageHandler()
{
    qCDebug(CustomMessageHandlerLog) << "CustomMessageHandler destroyed";
}

bool CustomMessageHandler::mavlinkMessageReceived(const mavlink_message_t& message)
{
    // 커스텀 메시지 ID 확인 및 처리
    switch (message.msgid) {
    case MSG_ID_FIRE_MISSION_START:
        _handleFireMissionStart(message);
        return true;
        
    default:
        // 이 핸들러가 처리하지 않는 메시지
        return false;
    }
}

void CustomMessageHandler::_handleFireMissionStart(const mavlink_message_t& message)
{
    mavlink_fire_mission_start_t fireMission;
    mavlink_msg_fire_mission_start_decode(&message, &fireMission);
    
    qCDebug(CustomMessageHandlerLog) 
        << "FireMissionStart received:"
        << "target_system:" << fireMission.target_system
        << "target_component:" << fireMission.target_component
        << "target_lat:" << fireMission.target_lat
        << "target_lon:" << fireMission.target_lon
        << "target_alt:" << fireMission.target_alt
        << "auto_fire:" << fireMission.auto_fire
        << "max_projectiles:" << fireMission.max_projectiles;
    
    emit fireMissionStartReceived(
        fireMission.target_system,
        fireMission.target_component,
        fireMission.target_lat,
        fireMission.target_lon,
        fireMission.target_alt,
        fireMission.auto_fire,
        fireMission.max_projectiles
    );
}
