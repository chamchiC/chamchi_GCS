target_sources(${CMAKE_PROJECT_NAME}
    PRIVATE
        Autotune.cpp
        Autotune.h
        FTPManager.cc
        FTPManager.h
        InitialConnectStateMachine.cc
        InitialConnectStateMachine.h
        MAVLinkLogManager.cc
        MAVLinkLogManager.h
        MultiVehicleManager.cc
        MultiVehicleManager.h
        RemoteIDManager.cc
        RemoteIDManager.h
        StandardModes.cc
        StandardModes.h
        TerrainProtocolHandler.cc
        TerrainProtocolHandler.h
        TrajectoryPoints.cc
        TrajectoryPoints.h
        Vehicle.cc
        Vehicle.h
        VehicleLinkManager.cc
        VehicleLinkManager.h
        VehicleObjectAvoidance.cc
        VehicleObjectAvoidance.h
        ${CMAKE_SOURCE_DIR}/custom/src/CustomMessageHandler.cc
        ${CMAKE_SOURCE_DIR}/custom/src/CustomMessageHandler.h
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/libs/mavlink/include  # 커스텀 메시지 헤더 경로 추가
    ${CMAKE_SOURCE_DIR}/custom/src  # CustomMessageHandler.h 경로 추가
)

qt_add_library(VehicleModule STATIC)

qt_add_qml_module(VehicleModule
    URI QGroundControl.Vehicle
    VERSION 1.0
    RESOURCE_PREFIX /qml
    NO_PLUGIN # Required by Qt 6.6.3. TODO: Remove when no longer supported.
)

add_subdirectory(Actuators)
add_subdirectory(ComponentInformation)
add_subdirectory(FactGroups)
add_subdirectory(VehicleSetup)
