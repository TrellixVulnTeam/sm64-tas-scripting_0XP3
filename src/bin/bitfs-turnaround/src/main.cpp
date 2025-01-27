
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>

#include <tasfw/Inputs.hpp>
#include <tasfw/Script.hpp>
#include <sm64/Camera.hpp>
#include <sm64/Trig.hpp>
#include <sm64/Types.hpp>

#include <tasfw/scripts/General.hpp>
#include <tasfw/scripts/BitFSPyramidOscillation.hpp>

#include "BitFsConfig.hpp"
#include <tasfw/scripts/BitFsScApproach.hpp>

using namespace std;

#pragma comment(lib, "Ws2_32.lib")

class MainScript : public TopLevelScript<LibSm64>
{
public:
	bool validation() { return true; }

	bool execution()
	{
		LongLoad(3277); //3536
		auto status = Modify<BitFsPyramidOscillation>(0.66f, 4);
		auto status2 = Modify<BitFsScApproach>(16384, 4, status);
		return true;
	}

	bool assertion()
	{
		// Save m64Diff to M64
		M64Diff diff = GetBaseDiff();
		for (auto& [frame, inputs]: diff.frames)
		{
			_m64->frames[frame] = inputs;
		}

		return true;
	}
};

int main(int argc, const char* argv[])
{
	namespace fs = std::filesystem;
	fs::path cfgPath =
		((argc >= 2) ? fs::path(argv[1]) : getPathToSelf().parent_path() / "config.json");

	BitFs_ConfigData cfg = BitFs_ConfigData::load(cfgPath);

	auto lib_path = cfg.libSM64;
	auto m64_path = cfg.m64File;

	M64 m64 = M64(m64_path);
	m64.load();

	auto status = MainScript::MainConfig<MainScript>(m64, lib_path);

	m64.save();

	return 0;
}
