#include "PPMAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "PPMAnalyzer.h"
#include "PPMAnalyzerSettings.h"
#include <iostream>
#include <fstream>

PPMAnalyzerResults::PPMAnalyzerResults(PPMAnalyzer* analyzer, PPMAnalyzerSettings* settings)
	: AnalyzerResults(),
	mSettings(settings),
	mAnalyzer(analyzer)
{
}

PPMAnalyzerResults::~PPMAnalyzerResults()
{
}

void PPMAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
	ClearResultStrings();
	Frame frame = GetFrame(frame_index);

	char number_str[24];
	snprintf(number_str, sizeof(number_str), "[Ch %llu] %llu usec", frame.mData1, frame.mData2);
	AddResultString(number_str);
}

void PPMAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
	std::ofstream file_stream(file, std::ios::out);

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for (U32 i = 0; i < num_frames; i++)
	{
		Frame frame = GetFrame(i);

		char time_str[128];
		AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

		file_stream << time_str << "," << frame.mData2 << std::endl;

		if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true)
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void PPMAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
	Frame frame = GetFrame(frame_index);
	ClearTabularText();

	char number_str[20];
	snprintf(number_str, sizeof(number_str), "[Ch %llu] %llu usec", frame.mData1, frame.mData2);
	AddTabularText(number_str);
}

void PPMAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
	ClearTabularText();
	char msg[32];
	snprintf(msg, sizeof(msg), "--- Frame #%llu", packet_id);
	AddTabularText(msg);
}

void PPMAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
	//not supported
}
