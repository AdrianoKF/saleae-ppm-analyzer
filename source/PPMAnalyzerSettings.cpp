#include "PPMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


PPMAnalyzerSettings::PPMAnalyzerSettings()
	: mInputChannel(UNDEFINED_CHANNEL)
{
	mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mInputChannelInterface->SetTitleAndTooltip("PPM", "Standard Pulse Position Modulation");
	mInputChannelInterface->SetChannel(mInputChannel);

	AddInterface(mInputChannelInterface.get());

	AddExportOption(0, "Export as text/csv file");
	AddExportExtension(0, "text", "txt");
	AddExportExtension(0, "csv", "csv");

	ClearChannels();
	AddChannel(mInputChannel, "PPM", false);
}

PPMAnalyzerSettings::~PPMAnalyzerSettings()
{
}

bool PPMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();

	ClearChannels();
	AddChannel(mInputChannel, "PPM", true);

	return true;
}

void PPMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel(mInputChannel);
}

void PPMAnalyzerSettings::LoadSettings(const char* settings)
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	text_archive >> mInputChannel;

	ClearChannels();
	AddChannel(mInputChannel, "PPM", true);

	UpdateInterfacesFromSettings();
}

const char* PPMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;

	return SetReturnString(text_archive.GetString());
}
