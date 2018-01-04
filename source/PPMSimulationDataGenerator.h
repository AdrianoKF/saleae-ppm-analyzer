#ifndef PPM_SIMULATION_DATA_GENERATOR
#define PPM_SIMULATION_DATA_GENERATOR
#include <SimulationChannelDescriptor.h>
#include <random>

class PPMAnalyzerSettings;

class PPMSimulationDataGenerator
{
public:
	PPMSimulationDataGenerator();
	~PPMSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, PPMAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	PPMAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

	std::default_random_engine generator;
	std::uniform_int_distribution<int> channelTimeDistribution;

protected:
	void CreatePPMFrame();

	SimulationChannelDescriptor mSimData;

};
#endif //PPM_SIMULATION_DATA_GENERATOR
