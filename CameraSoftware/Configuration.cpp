#include "Configuration.h"

Configuration::Configuration(QObject* parent) 
{
	setParent(parent);
	loadState = loadConfig();
}

Configuration::~Configuration() 
{
	
}

bool Configuration::initailizeConfig()
{
	Config = defaultConfig;
	Config["versionId"] = VERSION_ID;
	return saveConfig();
}

Configuration::LoadState Configuration::loadConfig()
{
	using namespace ConfigConstants;
	bool fileExist = QFile::exists(configPath);
	if (not fileExist) {
		if (initailizeConfig()) {
			return LSRecreatedFile;
		}
		else {
			return LSError;
		}
	}
	QFile configFile = QFile(configPath);
	configFile.open(QIODeviceBase::ReadOnly);
	QJsonDocument jsonObj = QJsonDocument::fromJson(configFile.readAll());
	configFile.close();
	Config = jsonObj.object().toVariantMap();

	// Detect missing keys
	auto configKeys = Config.keys();
	for (auto& configKey : defaultConfig.keys()) {
		if (not configKeys.contains(configKey)) {
			Config[configKey] = defaultConfig[configKey];
		}
	}
	// Detect wrong keys
	if (not Languages.contains(Config["language"].toString()))
		resetConfigKey("language");
	if (Config["preferredCameraDevice"].type() != QVariant::String)
		resetConfigKey("preferredCameraDevice");
	if (Config["preferredCameraResolution"].type() != QVariant::Int or Config["preferredCameraResolution"].toInt() < 0)
		resetConfigKey("preferredCameraResolution");
	if (not RefreshRates.contains(Config["cameraRefreshRate"].toInt()))
		resetConfigKey("cameraRefreshRate");
	if (Config["clickToScreenshotWidget"].type() != QVariant::Bool)
		resetConfigKey("clickToScreenshotWidget");
	if (Config["holdingLineConvertion"].type() != QVariant::Bool)
		resetConfigKey("holdingLineConvertion");
	if (Config["longHoldingConvertionMenu"].type() != QVariant::Bool)
		resetConfigKey("longHoldingConvertionMenu");
	if (not ISOValues.contains(Config["ISO"].toInt()))
		resetConfigKey("ISO");
	if (Config["exposureComponsation"].type() != QVariant::Double or Config["exposureComponsation"].toDouble() < -2.0 or Config["exposureComponsation"].toDouble() > 2.0)
		resetConfigKey("exposureComponsation");
	if (Config["rotation"].type() != QVariant::Int or not Rotations.contains(Config["rotation"]))
		resetConfigKey("rotation");
	if (Config["antialiasing"].type() != QVariant::Bool)
		resetConfigKey("antialiasing");
	if (Config["freezeCamera"].type() != QVariant::Bool)
		resetConfigKey("antialiasing");

	//Todo

	//Check version
	if (not configKeys.contains("versionId"))
		Config["versionId"] = QVariant(VERSION_ID);
	if (Config["versionId"].toInt() < VERSION_ID) {
		Config["versionId"] = QVariant(VERSION_ID);
		return LSUpgrade;
	}
	else if (Config["versionId"].toInt() > VERSION_ID) {
		Config["versionId"] = QVariant(VERSION_ID);
		return LSLaterVersion;
	}
	else
		return LSSucceed;
}

bool Configuration::saveConfig()
{
	QFile configFile = QFile(configPath);
	if (!configFile.open(QIODeviceBase::WriteOnly)) {
		return false;
	}
	auto JsonText = QJsonObject::fromVariantMap(Config);
	configFile.write(QJsonDocument(JsonText).toJson());
	configFile.close();
	return true;
}
