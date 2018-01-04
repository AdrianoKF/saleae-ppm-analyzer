#include "PPMAnalyzer.h"
#include "PPMConstants.h"
#include "PPMAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

PPMAnalyzer::PPMAnalyzer() :
	mSettings(new PPMAnalyzerSettings()),
	mSimulationInitialized(false)
{
	SetAnalyzerSettings(mSettings.get());
}

PPMAnalyzer::~PPMAnalyzer()
{
	KillThread();
}

void PPMAnalyzer::SetupResults()
{
	mResults.reset(new PPMAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());
	mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}

void PPMAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();
	mData = GetAnalyzerChannelData(mSettings->mInputChannel);

	int8_t channel_counter = -1;

	U16 ppm[MAX_PPM_CHANNELS];
	while (mData->DoMoreTransitionsExistInCurrentData()) {
		const U64 channel_start = mData->GetSampleNumber();

		// Attempt to find frame start by looking for a pulse longer than 2700us
		U16 width_usec = NextPulseWidthUsec();
		if (width_usec > MIN_FRAME_GAP)
		{
			// Long pulse marks end of a frame, re-initialize internal state and try again
			channel_counter = 0;
			mResults->AddMarker(mData->GetSampleNumber(), AnalyzerResults::MarkerType::DownArrow, mSettings->mInputChannel);
			mResults->CommitPacketAndStartNewPacket();
			mResults->CommitResults();
			continue;
		}

		if (channel_counter == -1)
		{
			// We are not yet synchronized
			continue;
		}

		if (width_usec > MIN_CHANNEL_TIME && width_usec < MAX_CHANNEL_TIME)
		{
			ppm[channel_counter] = width_usec;

			Frame frame;
			frame.mData1 = channel_counter;
			frame.mData2 = ppm[channel_counter];
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = channel_start;
			frame.mEndingSampleInclusive = mData->GetSampleNumber();

			mResults->AddFrame(frame);

			++channel_counter;
		}

		if (channel_counter >= MAX_PPM_CHANNELS)
		{
			// Mark as unsynchronized and wait for the next frame
			channel_counter = -1;
		}
	}
}

U16 PPMAnalyzer::NextPulseWidthUsec() {
	const U64 start = mData->GetSampleNumber();
	mData->AdvanceToNextEdge();
	mData->AdvanceToNextEdge();
	const U64 end = mData->GetSampleNumber();

	return static_cast<U16>(ceil(1e6 * (end - start) / mSampleRateHz));
}

bool PPMAnalyzer::NeedsRerun()
{
	return false;
}

U32 PPMAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels)
{
	if (!mSimulationInitialized)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitialized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 PPMAnalyzer::GetMinimumSampleRateHz()
{
	// Require at least microsecond resolution
	return 2'000'000;
}

const char* PPMAnalyzer::GetAnalyzerName() const
{
	return "Pulse Position Modulation";
}

const char* GetAnalyzerName()
{
	return "Pulse Position Modulation";
}

Analyzer* CreateAnalyzer()
{
	return new PPMAnalyzer();
}

void DestroyAnalyzer(Analyzer* analyzer)
{
	delete analyzer;
}
