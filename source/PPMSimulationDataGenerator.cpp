#include "PPMSimulationDataGenerator.h"
#include "PPMAnalyzerSettings.h"
#include "PPMConstants.h"

#include <AnalyzerHelpers.h>

PPMSimulationDataGenerator::PPMSimulationDataGenerator()
	: channelTimeDistribution(MIN_CHANNEL_TIME, MAX_CHANNEL_TIME - PRE_PULSE_TIME)
{
}

PPMSimulationDataGenerator::~PPMSimulationDataGenerator()
{
}

void PPMSimulationDataGenerator::Initialize(U32 simulation_sample_rate, PPMAnalyzerSettings* settings)
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSimData.SetChannel(mSettings->mInputChannel);
	mSimData.SetSampleRate(simulation_sample_rate);
	mSimData.SetInitialBitState(BIT_HIGH);
}

U32 PPMSimulationDataGenerator::GenerateSimulationData(U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel)
{
	const U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample(largest_sample_requested, sample_rate, mSimulationSampleRateHz);

	while (mSimData.GetCurrentSampleNumber() < adjusted_largest_sample_requested)
	{
		CreatePPMFrame();
	}

	*simulation_channel = &mSimData;

	return 1;
}

void PPMSimulationDataGenerator::CreatePPMFrame()
{
	const int samples_per_usec = static_cast<int>(ceil(mSimData.GetSampleRate() / 1e6));

	// Frame separator
	mSimData.TransitionIfNeeded(BIT_HIGH);
	mSimData.Advance(FRAME_GAP_TIME * samples_per_usec);
	mSimData.Transition();
	mSimData.Advance(PRE_PULSE_TIME * samples_per_usec);
	mSimData.Transition();

	for (int channel = 0; channel < MAX_PPM_CHANNELS; ++channel)
	{
		const int pulse_width_usec = channelTimeDistribution(generator);
		const int pulse_length_samples = pulse_width_usec * samples_per_usec;

		mSimData.TransitionIfNeeded(BIT_HIGH);
		mSimData.Advance(pulse_length_samples);
		mSimData.Transition();
		mSimData.Advance(PRE_PULSE_TIME * samples_per_usec);
		mSimData.Transition();
	}
}
