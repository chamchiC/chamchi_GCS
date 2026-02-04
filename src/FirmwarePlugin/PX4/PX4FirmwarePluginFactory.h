/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "FirmwarePluginFactory.h"
#include "QGCMAVLink.h"

class PX4FirmwarePlugin;

class PX4FirmwarePluginFactory : public FirmwarePluginFactory
{
    Q_OBJECT

public:
    PX4FirmwarePluginFactory(void);

    QList<QGCMAVLink::FirmwareClass_t>  supportedFirmwareClasses(void) const final;
    QList<QGCMAVLink::VehicleClass_t>   supportedVehicleClasses(void) const final;  // Chamchi GCS: MultiRotor only
    FirmwarePlugin*                     firmwarePluginForAutopilot  (MAV_AUTOPILOT autopilotType, MAV_TYPE vehicleType) final;

private:
    PX4FirmwarePlugin*  _pluginInstance;
};
