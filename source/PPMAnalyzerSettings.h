#ifndef PPM_ANALYZER_SETTINGS
#define PPM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class PPMAnalyzerSettings : public AnalyzerSettings
{
public:
	PPMAnalyzerSettings();
	virtual ~PPMAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings(const char* settings);
	virtual const char* SaveSettings();

	Channel mInputChannel;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
};

#endif //PPM_ANALYZER_SETTINGS
