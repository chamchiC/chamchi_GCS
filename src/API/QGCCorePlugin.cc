/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#include "QGCCorePlugin.h"
#include "QGCLogging.h"
#include "AppSettings.h"
#include "MavlinkSettings.h"
#include "FactMetaData.h"
#include "QGCMAVLink.h"
#ifdef QGC_GST_STREAMING
#include "GStreamer.h"
#endif
#include "HorizontalFactValueGrid.h"
#include "InstrumentValueData.h"
#include "JoystickManager.h"
#include "LogDownloadController.h"
#include "MAVLinkLib.h"
#include "QGCLoggingCategory.h"
#include "QGCOptions.h"
#include "QmlComponentInfo.h"
#include "QmlObjectListModel.h"
#ifdef QGC_QT_STREAMING
#include "QtMultimediaReceiver.h"
#endif
#include "SettingsManager.h"
#include "VideoReceiver.h"

#ifdef QGC_CUSTOM_BUILD
#include CUSTOMHEADER
#endif

#include <QtCore/qapplicationstatic.h>
#include <QtCore/QFile>
#include <QtQml/qqml.h>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>

QGC_LOGGING_CATEGORY(QGCCorePluginLog, "qgc.api.qgccoreplugin");

#ifndef QGC_CUSTOM_BUILD
Q_APPLICATION_STATIC(QGCCorePlugin, _qgcCorePluginInstance);
#endif

QGCCorePlugin::QGCCorePlugin(QObject *parent)
    : QObject(parent)
    , _defaultOptions(new QGCOptions(this))
    , _emptyCustomMapItems(new QmlObjectListModel(this))
{
    // qCDebug(QGCCorePluginLog) << Q_FUNC_INFO << this;
}

QGCCorePlugin::~QGCCorePlugin()
{
    // qCDebug(QGCCorePluginLog) << Q_FUNC_INFO << this;
}

QGCCorePlugin *QGCCorePlugin::instance()
{
#ifndef QGC_CUSTOM_BUILD
    return _qgcCorePluginInstance();
#else
    return CUSTOMCLASS::instance();
#endif
}

void QGCCorePlugin::registerQmlTypes()
{
    (void) qmlRegisterUncreatableType<QGCCorePlugin>("QGroundControl", 1, 0, "QGCCorePlugin", QStringLiteral("Reference only"));
    (void) qmlRegisterUncreatableType<QGCOptions>("QGroundControl", 1, 0, "QGCOptions", QStringLiteral("Reference only"));
    (void) qmlRegisterUncreatableType<QGCFlyViewOptions>("QGroundControl", 1, 0, "QGCFlyViewOptions", QStringLiteral("Reference only"));
}

const QVariantList &QGCCorePlugin::analyzePages()
{
    static const QVariantList analyzeList = {
        QVariant::fromValue(new QmlComponentInfo(
            tr("Log Download"),
            QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/AnalyzeView/LogDownloadPage.qml")),
            QUrl::fromUserInput(QStringLiteral("qrc:/qmlimages/LogDownloadIcon.svg")))),
#if !defined(Q_OS_ANDROID) && !defined(Q_OS_IOS)
        QVariant::fromValue(new QmlComponentInfo(
            tr("GeoTag Images"),
            QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/AnalyzeView/GeoTagPage.qml")),
            QUrl::fromUserInput(QStringLiteral("qrc:/qmlimages/GeoTagIcon.svg")))),
#endif
        QVariant::fromValue(new QmlComponentInfo(
            tr("MAVLink Console"),
            QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/AnalyzeView/MAVLinkConsolePage.qml")),
            QUrl::fromUserInput(QStringLiteral("qrc:/qmlimages/MAVLinkConsoleIcon.svg")))),
#ifndef QGC_DISABLE_MAVLINK_INSPECTOR
        QVariant::fromValue(new QmlComponentInfo(
            tr("MAVLink Inspector"),
            QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/AnalyzeView/MAVLinkInspectorPage.qml")),
            QUrl::fromUserInput(QStringLiteral("qrc:/qmlimages/MAVLinkInspector.svg")))),
#endif
        QVariant::fromValue(new QmlComponentInfo(
            tr("Vibration"),
            QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/AnalyzeView/VibrationPage.qml")),
            QUrl::fromUserInput(QStringLiteral("qrc:/qmlimages/VibrationPageIcon")))),
    };

    return analyzeList;
}

QGCOptions *QGCCorePlugin::options()
{
    return _defaultOptions;
}

const QmlObjectListModel *QGCCorePlugin::customMapItems()
{
    return _emptyCustomMapItems;
}

bool QGCCorePlugin::adjustSettingMetaData(const QString &settingsGroup, FactMetaData &metaData)
{
    if (settingsGroup == AppSettings::settingsGroup) {
        if (metaData.name() == AppSettings::indoorPaletteName) {
            QVariant outdoorPalette;
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
            outdoorPalette = 0;
#else
            outdoorPalette = 1;
#endif
            metaData.setRawDefaultValue(outdoorPalette);
            return true;
        }
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
        else if (metaData.name() == MavlinkSettings::telemetrySaveName) {
            metaData.setRawDefaultValue(false);
            return true;
        }
#endif
#ifndef Q_OS_ANDROID
        else if (metaData.name() == AppSettings::androidSaveToSDCardName) {
            return false;
        }
#endif
        // Chamchi GCS: Default offline editing settings
        else if (metaData.name() == AppSettings::offlineEditingFirmwareClassName) {
            metaData.setRawDefaultValue(QGCMAVLink::FirmwareClassPX4);
            return false;
        } else if (metaData.name() == AppSettings::offlineEditingVehicleClassName) {
            metaData.setRawDefaultValue(QGCMAVLink::VehicleClassMultiRotor);
            return false;
        }
    }

    return true;
}

QString QGCCorePlugin::showAdvancedUIMessage() const
{
    return tr("WARNING: You are about to enter Advanced Mode. "
              "If used incorrectly, this may cause your vehicle to malfunction thus voiding your warranty. "
              "You should do so only if instructed by customer support. "
              "Are you sure you want to enable Advanced Mode?");
}

void QGCCorePlugin::factValueGridCreateDefaultSettings(FactValueGrid* factValueGrid)
{
    if (factValueGrid->specificVehicleForCard()) {
        bool includeFWValues = factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassFixedWing || factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassVTOL || factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassAirship;

        factValueGrid->setFontSize(FactValueGrid::LargeFontSize);
        factValueGrid->appendColumn();
        factValueGrid->appendColumn();

        int rowIndex = 0;
        int colIndex = 0;

        // first cell
        QmlObjectListModel* column = factValueGrid->columns()->value<QmlObjectListModel*>(colIndex++);
        InstrumentValueData* value = column->value<InstrumentValueData*>(rowIndex);
        value->setFact("Vehicle", "AltitudeRelative");
        value->setIcon("arrow-thick-up.svg");
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);

        // second cell
        column = factValueGrid->columns()->value<QmlObjectListModel*>(colIndex++);
        value = column->value<InstrumentValueData*>(rowIndex);
        if (includeFWValues) {
            value->setFact("Vehicle", "AirSpeed");
            value->setText("AirSpd");
            value->setShowUnits(true);
        } else {
            value->setFact("Vehicle", "GroundSpeed");
            value->setIcon("arrow-simple-right.svg");
            value->setText(value->fact()->shortDescription());
            value->setShowUnits(true);
        }
    } else {
        const bool includeFWValues = ((factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassFixedWing) || (factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassVTOL) || (factValueGrid->vehicleClass() == QGCMAVLink::VehicleClassAirship));

        factValueGrid->setFontSize(FactValueGrid::LargeFontSize);

        (void) factValueGrid->appendColumn();
        (void) factValueGrid->appendColumn();
        (void) factValueGrid->appendColumn();
        if (includeFWValues) {
            (void) factValueGrid->appendColumn();
        }
        factValueGrid->appendRow();

        int rowIndex = 0;
        QmlObjectListModel *column = factValueGrid->columns()->value<QmlObjectListModel*>(0);

        InstrumentValueData *value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("AltitudeRelative"));
        value->setIcon(QStringLiteral("arrow-thick-up.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);

        value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("DistanceToHome"));
        value->setIcon(QStringLiteral("bookmark copy 3.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);

        rowIndex = 0;
        column = factValueGrid->columns()->value<QmlObjectListModel*>(1);

        value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("ClimbRate"));
        value->setIcon(QStringLiteral("arrow-simple-up.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);

        value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("GroundSpeed"));
        value->setIcon(QStringLiteral("arrow-simple-right.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);

        if (includeFWValues) {
            rowIndex = 0;
            column = factValueGrid->columns()->value<QmlObjectListModel*>(2);

            value = column->value<InstrumentValueData*>(rowIndex++);
            value->setFact(QStringLiteral("Vehicle"), QStringLiteral("AirSpeed"));
            value->setText(QStringLiteral("AirSpd"));
            value->setShowUnits(true);

            value = column->value<InstrumentValueData*>(rowIndex++);
            value->setFact(QStringLiteral("Vehicle"), QStringLiteral("ThrottlePct"));
            value->setText(QStringLiteral("Thr"));
            value->setShowUnits(true);
        }

        rowIndex = 0;
        column = factValueGrid->columns()->value<QmlObjectListModel*>(includeFWValues ? 3 : 2);

        value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("FlightTime"));
        value->setIcon(QStringLiteral("timer.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(false);

        value = column->value<InstrumentValueData*>(rowIndex++);
        value->setFact(QStringLiteral("Vehicle"), QStringLiteral("FlightDistance"));
        value->setIcon(QStringLiteral("travel-walk.svg"));
        value->setText(value->fact()->shortDescription());
        value->setShowUnits(true);
    }
}

QQmlApplicationEngine *QGCCorePlugin::createQmlApplicationEngine(QObject *parent)
{
    QQmlApplicationEngine *const qmlEngine = new QQmlApplicationEngine(parent);
    qmlEngine->addImportPath(QStringLiteral("qrc:/qml"));
    qmlEngine->rootContext()->setContextProperty(QStringLiteral("joystickManager"), JoystickManager::instance());
    qmlEngine->rootContext()->setContextProperty(QStringLiteral("debugMessageModel"), QGCLogging::instance());
    qmlEngine->rootContext()->setContextProperty(QStringLiteral("logDownloadController"), LogDownloadController::instance());
    return qmlEngine;
}

void QGCCorePlugin::createRootWindow(QQmlApplicationEngine *qmlEngine)
{
    qmlEngine->load(QUrl(QStringLiteral("qrc:/qml/QGroundControl/MainWindow/MainWindow.qml")));
}

VideoReceiver *QGCCorePlugin::createVideoReceiver(QObject *parent)
{
#ifdef QGC_GST_STREAMING
    return GStreamer::createVideoReceiver(parent);
#elif defined(QGC_QT_STREAMING)
    return QtMultimediaReceiver::createVideoReceiver(parent);
#else
    return nullptr;
#endif
}

void *QGCCorePlugin::createVideoSink(QQuickItem *widget, QObject *parent)
{
#ifdef QGC_GST_STREAMING
    return GStreamer::createVideoSink(widget, parent);
#elif defined(QGC_QT_STREAMING)
    return QtMultimediaReceiver::createVideoSink(widget, parent);
#else
    Q_UNUSED(widget); Q_UNUSED(parent);
    return nullptr;
#endif
}
void QGCCorePlugin::releaseVideoSink(void *sink)
{
#ifdef QGC_GST_STREAMING
    GStreamer::releaseVideoSink(sink);
#elif defined(QGC_QT_STREAMING)
    QtMultimediaReceiver::releaseVideoSink(sink);
#else
    Q_UNUSED(sink);
#endif
}

const QVariantList &QGCCorePlugin::toolBarIndicators()
{
    static const QVariantList toolBarIndicatorList = QVariantList(
        {
            QVariant::fromValue(QUrl::fromUserInput(QStringLiteral("qrc:/qml/QGroundControl/Toolbar/RTKGPSIndicator.qml"))),
        }
    );

    return toolBarIndicatorList;
}

QVariantList QGCCorePlugin::firstRunPromptsToShow()
{
    QList<int> rgIdsToShow;

    rgIdsToShow.append(firstRunPromptStdIds());
    rgIdsToShow.append(firstRunPromptCustomIds());

    const QList<int> rgAlreadyShownIds = AppSettings::firstRunPromptsIdsVariantToList(SettingsManager::instance()->appSettings()->firstRunPromptIdsShown()->rawValue());
    for (int idToRemove: rgAlreadyShownIds) {
        (void) rgIdsToShow.removeOne(idToRemove);
    }

    QVariantList rgVarIdsToShow;
    for (int id: rgIdsToShow) {
        rgVarIdsToShow.append(id);
    }

    return rgVarIdsToShow;
}

QString QGCCorePlugin::firstRunPromptResource(int id) const
{
    switch (id) {
    case kUnitsFirstRunPromptId:
        return QStringLiteral("/FirstRunPromptDialogs/UnitsFirstRunPrompt.qml");
    case kOfflineVehicleFirstRunPromptId:
        return QStringLiteral("/FirstRunPromptDialogs/OfflineVehicleFirstRunPrompt.qml");
    default:
        return QString();
    }
}

void QGCCorePlugin::_setShowTouchAreas(bool show)
{
    if (show != _showTouchAreas) {
        _showTouchAreas = show;
        emit showTouchAreasChanged(show);
    }
}

void QGCCorePlugin::_setShowAdvancedUI(bool show)
{
    if (show != _showAdvancedUI) {
        _showAdvancedUI = show;
        emit showAdvancedUIChanged(show);
    }
}

// Chamchi GCS: Custom Neumorphism Dark Cyan Theme
void QGCCorePlugin::paletteOverride(const QString &colorName, QGCPalette::PaletteColorInfo_t& colorInfo)
{
    if (colorName == QStringLiteral("window")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#2d3446");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#2d3446");
    }
    else if (colorName == QStringLiteral("windowShade")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#252a3a");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#343b4f");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#343b4f");
    }
    else if (colorName == QStringLiteral("windowShadeDark")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#141824");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#141824");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#1e2433");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#1e2433");
    }
    else if (colorName == QStringLiteral("text")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ffffff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#e8eaed");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("warningText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ff6b6b");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#ff6b6b");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#ff6b6b");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#ff6b6b");
    }
    else if (colorName == QStringLiteral("button")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#2d3446");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#1e2433");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#343b4f");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#252a3a");
    }
    else if (colorName == QStringLiteral("buttonText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#e8eaed");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#e8eaed");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("buttonHighlight")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#2d3446");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00d4ff");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#343b4f");
    }
    else if (colorName == QStringLiteral("buttonHighlightText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("primaryButton")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#2d3446");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#343b4f");
    }
    else if (colorName == QStringLiteral("primaryButtonText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("textField")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#2d3446");
    }
    else if (colorName == QStringLiteral("textFieldText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ffffff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#e8eaed");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("mapButton")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#343b4f");
    }
    else if (colorName == QStringLiteral("mapButtonHighlight")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#2d3446");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#343b4f");
    }
    else if (colorName == QStringLiteral("mapIndicator")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00e5ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00d4ff");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("mapIndicatorChild")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00b8d4");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00acc1");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("colorGreen")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00e5c7");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#00b89c");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#00b89c");
    }
    else if (colorName == QStringLiteral("colorOrange")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ffab40");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#ff9100");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#ffab40");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#ff9100");
    }
    else if (colorName == QStringLiteral("colorRed")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ff5252");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#ff1744");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#ff5252");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#ff1744");
    }
    else if (colorName == QStringLiteral("colorGrey")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#8b90a0");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#8b90a0");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("colorBlue")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#00a8cc");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00d4ff");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#00a8cc");
    }
    else if (colorName == QStringLiteral("alertBackground")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ffab00");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#ffab00");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#ffab00");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#ffab00");
    }
    else if (colorName == QStringLiteral("alertBorder")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#ff8f00");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#ff8f00");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#ff8f00");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#ff8f00");
    }
    else if (colorName == QStringLiteral("alertText")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#1a1f2e");
    }
    else if (colorName == QStringLiteral("missionItemEditor")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#141824");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#1e2433");
    }
    else if (colorName == QStringLiteral("hoverColor")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#00a8cc");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#00b89c");
    }
    else if (colorName == QStringLiteral("mapWidgetBorderLight")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#5a6070");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#5a6070");
    }
    else if (colorName == QStringLiteral("mapWidgetBorderDark")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#141824");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#141824");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#1a1f2e");
    }
    else if (colorName == QStringLiteral("brandingPurple")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00d4ff");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#00d4ff");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00d4ff");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#00d4ff");
    }
    else if (colorName == QStringLiteral("brandingBlue")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#00e5c7");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#00b89c");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#00e5c7");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#00b89c");
    }
    else if (colorName == QStringLiteral("toolbarBackground")) {
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupEnabled]   = QColor("#1a1f2e");
        colorInfo[QGCPalette::Dark][QGCPalette::ColorGroupDisabled]  = QColor("#1a1f2e");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupEnabled]  = QColor("#252a3a");
        colorInfo[QGCPalette::Light][QGCPalette::ColorGroupDisabled] = QColor("#252a3a");
    }
}
