#pragma once

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
