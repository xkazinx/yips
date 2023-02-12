#pragma once
class Misc
{
public:
	static vector<string> FetchFileNamesFromDirectory(string dir)
	{
		vector<string> file_names;
		WIN32_FIND_DATA FindFileData;
		dir += "/*";
		HANDLE hFind = FindFirstFile(dir.c_str(), &FindFileData);
		if (hFind == INVALID_HANDLE_VALUE) 
		{
			PrintDebug("FetchFileNamesFromDirectory: Could not read files from directory {}", dir.c_str());
			return file_names;
		}

		do 
		{
			if(FindFileData.cFileName == "."s || FindFileData.cFileName == ".."s)
				continue;

			file_names.push_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
		return file_names;
	}
};

