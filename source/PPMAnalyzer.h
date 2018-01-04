#ifndef PPM_ANALYZER_H
#define PPM_ANALYZER_H

#include <Analyzer.h>
#include "PPMAnalyzerResults.h"
#include "PPMSimulationDataGenerator.h"

class PPMAnalyzerSettings;
class ANALYZER_EXPORT PPMAnalyzer : public Analyzer2
{
public:
	PPMAnalyzer();
	virtual ~PPMAnalyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData(U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels);
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr<PPMAnalyzerSettings> mSettings;
	std::auto_ptr<PPMAnalyzerResults> mResults;
	AnalyzerChannelData* mData;

	U16 NextPulseWidthUsec();

	PPMSimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitialized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer();
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer(Analyzer* analyzer);

#endif //PPM_ANALYZER_H
