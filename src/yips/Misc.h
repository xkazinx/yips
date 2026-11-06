#pragma once

#include <chrono>

// Milliseconds on a steady clock (timeGetTime on Windows; a browser has none)
inline u32 NowMs()
{
#ifdef _WIN32
	return timeGetTime();
#else
	return (u32)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#endif
}

class Misc
{
public:
	// The names of the files in a folder (no sub-folders), sorted
	static vector<string> FetchFileNamesFromDirectory(const Path & dir)
	{
		vector<string> file_names;
		std::error_code ec;

		for (const auto & entry : std::filesystem::directory_iterator(dir, ec))
		{
			if (entry.is_regular_file(ec))
				file_names += entry.path().filename().string();
		}

		if (ec)
			PrintDebug("FetchFileNamesFromDirectory: Could not read files from directory {}", dir.string());

		ranges::sort(file_names);
		return file_names;
	}
};
