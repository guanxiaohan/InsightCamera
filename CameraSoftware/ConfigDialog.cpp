#include "ConfigDialog.h"

ConfigDialog::ConfigDialog(Configuration* configObj, CameraManager* cameraObj, QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ConfigDialogUi())
{
	ui->setupUi(this);
	configObject = configObj;
	cameraObject = cameraObj;

	setLayout(ui->gridLayout);
	ui->deviceTab->setLayout(ui->deviceTabLayout);
	ui->graphicsTab->setLayout(ui->graphicsTabLayout);
	ui->presetsTab->setLayout(ui->presetsTabLayout);

	setWindowModality(Qt::WindowModal);
	show();
}

ConfigDialog::~ConfigDialog()
{
	delete ui;
}

void ConfigDialog::initailizeUi()
{
	using namespace ConfigConstants;

	ui->configTabWidget->setCurrentIndex(0);

	ui->languageComboBox->setCurrentIndex(
		Languages.indexOf(configObject->getConfig("language")));
	connect(ui->languageComboBox, &QComboBox::currentIndexChanged, [=](int index){
		configObject->modifyConfig("language", Languages[index]);
		QMessageBox::information(this, tr("Restart to Apply"), tr("The Changes of Language Settings need to be effected by restarting InsightCamera."));
	});

	initCameraDeviceComboBox();
	connect(ui->cameraDeviceComboBox, &QComboBox::currentIndexChanged, [=](int index) {
		if (index == 0) {
			configObject->modifyConfig("preferredCameraDevice", "");
			emit switchCamera(-1);
		}
		else {
			auto cameras = cameraObject->getAvailableCameras();
			if (cameras.count() != ui->cameraDeviceComboBox->count() - 1 or cameras[index - 1].description() != ui->cameraDeviceComboBox->itemText(index)) {
				emit switchCamera(-2);
				initCameraDeviceComboBox();
			}
			else {
				configObject->modifyConfig("preferredCameraDevice", cameras[index - 1].description());
				emit switchCamera(114514);
			}
		}
		initResolutionLabel();
	});

	initResolutionLabel();

	ui->refreshRateComboBox->setCurrentIndex(RefreshRates.indexOf(configObject->getConfig("cameraRefreshRate").toInt()));
	connect(ui->refreshRateComboBox, &QComboBox::currentIndexChanged, [=](int index) {
		configObject->modifyConfig("cameraRefreshRate", RefreshRates[index]);
		emit changeRefreshRate(RefreshRates[index]);
	});

	ui->clickToScreenshotCheckBox->setChecked(configObject->getConfig("clickToScreenshotWidget").toBool());
	connect(ui->clickToScreenshotCheckBox, &QCheckBox::clicked, [=](bool checked) {
		configObject->modifyConfig("clickToScreenshotWidget", checked);
	});

	ui->holdConvertionCheckBox->setChecked(configObject->getConfig("holdingLineConvertion").toBool());
	connect(ui->holdConvertionCheckBox, &QCheckBox::clicked, [=](bool checked) {
		configObject->modifyConfig("holdingLineConvertion", checked);
		ui->convertionMenuCheckBox->setEnabled(ui->holdConvertionCheckBox->isChecked());
		if (not ui->holdConvertionCheckBox->isChecked())
			ui->convertionMenuCheckBox->setChecked(false);
		else
			ui->convertionMenuCheckBox->setChecked(configObject->getConfig("longHoldingConvertionMenu").toBool());
	});

	connect(ui->convertionMenuCheckBox, &QCheckBox::clicked, [=](bool checked) {
		configObject->modifyConfig("longHoldingConvertionMenu", checked);
	});
	ui->convertionMenuCheckBox->setEnabled(ui->holdConvertionCheckBox->isChecked());
	if (not ui->holdConvertionCheckBox->isChecked())
		ui->convertionMenuCheckBox->setChecked(false);
	else
		ui->convertionMenuCheckBox->setChecked(configObject->getConfig("longHoldingConvertionMenu").toBool());

	connect(ui->showCurrentFrameCheckBox, &QCheckBox::clicked, [=](bool checked) {
		configObject->modifyConfig("showCurrentFrameName", checked);
		emit switchCurrentFrameShowing(checked);
	});
	ui->showCurrentFrameCheckBox->setChecked(configObject->getConfig("showCurrentFrameName").toBool());

	connect(ui->autoSinkToolBarCheckBox, &QCheckBox::clicked, [=](bool checked) {
		configObject->modifyConfig("autoHidePartToolBar", checked);
	});
	ui->autoSinkToolBarCheckBox->setChecked(configObject->getConfig("autoHidePartToolBar").toBool());

	ui->ISOComboBox->setCurrentIndex(ISOValues.indexOf(configObject->getConfig("ISO")));
	connect(ui->ISOComboBox, &QComboBox::currentIndexChanged, [=](int index) {
		configObject->modifyConfig("ISO", ISOValues[index]);
		emit changeISO(ISOValues[index]);
	});

	connect(ui->debugCheckBox, &QCheckBox::clicked, [=](bool checked) {
		QObject* a = nullptr;
		a->objectName();
	});
}

void ConfigDialog::initCameraDeviceComboBox()
{
	for (int i = 1; i < ui->cameraDeviceComboBox->count(); i++) {
		ui->cameraDeviceComboBox->removeItem(1);
	}
	auto devices = cameraObject->getAvailableCameras();
	QString name = configObject->getConfig("preferredCameraDevice").toString();
	//ui->cameraDeviceComboBox->setCurrentIndex(0);
	for (auto device : devices) {
		ui->cameraDeviceComboBox->addItem(device.description());
	}
	for (int i = 0; i < devices.count(); i++) {
		if (name == devices[i].description()) {
			ui->cameraDeviceComboBox->setCurrentIndex(i + 1);
			return;
		}
	}
	if (name != "")
		ui->cameraDeviceComboBox->setCurrentIndex(1);
}

void ConfigDialog::initResolutionLabel()
{
	auto resolution = cameraObject->getMaxResolution();
	if (ui->cameraDeviceComboBox->currentIndex() == 0 or resolution.width() == 0 or resolution.height() == 0) {
		ui->resolutionValueLabel->setText("--");
	}
	else {
		ui->resolutionValueLabel->setText(QString("%0x%1").arg(resolution.width()).arg(resolution.height()));
	}
}

void ConfigDialog::initExposureComponsation()
{

}

void ConfigDialog::closeEvent(QCloseEvent* event)
{
	configObject->saveConfig();
	QDialog::closeEvent(event);
}
