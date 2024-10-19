#pragma once

#include <QFile>
#include <QFileDevice>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>
#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QVariantMap>
#include <QRGB>
#include <QColor>
#include <QMessageBox>
#include <initializer_list>

const QString configPath = ".\\ICSettings.json";
constexpr int VERSION_ID = 2;
const QString VERSION_STR = "1.0.0 Beta";

namespace ConfigConstants {
    const QStringList Languages = { "zh_cn", "en_us" };
    const QList<int> RefreshRates = { 15, 20, 30, 60 };
    const QList<int> ISOValues = { -1, 50, 100, 200, 400, 800, 1600, 3200 };
    const QList<int> Rotations = { -180, -135, -90, -45, 0, 45, 90, 135, 180 };
    const double MaxExposureComponsation = 3.0;
    const double MinExposureComponsation = -3.0;
    const int MinPenWidth = 1;
    const int MaxPenWidth = 25;
    const int MinEraserWidth = 60;
    const int MaxEraserWidth = 150;
    const int MinColorTemperature = 500;
    const int MaxColorTemperature = 7500;
}

const QVariantMap defaultConfig = QMap<QString, QVariant>({
    {"language", "en_us"},
    {"preferredCameraDevice", ""},
    {"preferredCameraResolution", 0},
    {"cameraRefreshRate", 30},
    {"clickToScreenshotWidget", true},
    {"holdingLineConvertion", true},
    {"longHoldingConvertionMenu", true},
    {"autoHidePartToolBar", true},
    {"showCurrentFrameName", true},
    {"ISO", -1},
    {"exposureComponsation", 0.0},
    {"rotation", 0},
    {"magnify", 0},
    {"freezeCamera", false},
    {"antialiasing", false},
    {"whiteBalance", 0},
    {"defaultPenWidth", 4},
    {"defaultEraserWidth", 60},
    {"color1", (255 << 16) + (255 << 8) + 255},
    {"color2", (0 << 16) + (0 << 8) + 0},
    {"color3", (255 << 16) + (30 << 8) + 50},
    {"color4", (240 << 16) + (255 << 8) + 0},
    {"color5", (20 << 16) + (230 << 8) + 100},
    {"color6", (50 << 16) + (90 << 8) + 255},
    {"defaultColor", 0},
    {"keepImageSettings", false}
});

class Configuration: public QObject
{
    Q_OBJECT
public:
    enum LoadState{LSSucceed, LSUpgrade, LSLaterVersion, LSRecreatedFile, LSError};
    LoadState loadState;
    Configuration(QObject* parent = nullptr);
    ~Configuration();
    bool initailizeConfig();
    LoadState loadConfig();
    bool saveConfig();
    inline void resetConfigKey(const QString key) { Config[key] = defaultConfig[key]; };
    inline QVariant getConfig(const QString key) { return Config[key]; };
    inline void modifyConfig(const QString key, const QVariant value) { Config[key] = value; };

private:
    QVariantMap Config;
    
};

