#include "file_manager.h"

namespace file
{
	std::string SearchInFile(const char* filename, const char* search)
	{
		std::ifstream file(filename);
		std::string line;
		std::string filter = "";
		std::string result = "";

		bool onInfo = true;
		bool shouldSkip = false;

		while (std::getline(file, line))
		{
			onInfo = true;
			filter = "";
			result = "";
			int i = 0;
			int size = static_cast<int>(line.size());
			while (i < size)
			{
				char c = line[i];
				char nc = line[i + 1];
				if (line[0] == '#')
				{
					shouldSkip = true;
				}
				if (onInfo && !shouldSkip)
				{
					filter.push_back(c);
				}
				else if (!shouldSkip)
				{
					result.push_back(c);
				}
				shouldSkip = false;
				if (nc == '=')
				{
					onInfo = false;
					shouldSkip = true;
				}
				else if (nc == ' ')
				{
					shouldSkip = true;
				}
				if (nc == '|')
				{
					i = size;
				}
				i++;
			}
			if (filter == search)
			{
				return result;
			}
		}
		return "-1";
	}

	bool SaveToFile(const char* filename, const char* filter, std::string info)
	{
		std::ifstream file(filename);
		std::string line;
		std::string settings = "";

		bool saved = false;

		while (std::getline(file, line))
		{
			std::string newLine = line;

			if (!line.empty() && line[0] != '#')
			{
				size_t eqPos = line.find('=');
				if (eqPos != std::string::npos)
				{
					std::string key = line.substr(0, eqPos);
					while (!key.empty() && key.back() == ' ')
					{
						key.pop_back();
					}
					if (key == filter)
					{
						size_t commentPos = line.find('|', eqPos);
						if (commentPos != std::string::npos)
						{
							newLine = key + " = " + info + " " + line.substr(commentPos);
						}
						else
						{
							newLine = key + " = " + info;
						}
						saved = true;
					}
				}
			}
			settings.append(newLine + "\n");
		}
		file.close();

		std::ofstream outFile(filename, std::ios::trunc);

		outFile << settings;
		outFile.close();

		return saved;
	}

	bool SaveToFile(const char* filename, const char* filter, int info)
	{
		return SaveToFile(filename, filter, std::to_string(info));
	}
}