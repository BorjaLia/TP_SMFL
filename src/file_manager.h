#include <string>
#include <fstream>

namespace file
{
	std::string SearchInFile(const char* filename, const char* search);
	bool SaveToFile(const char* filename, const char* filter, std::string info);
	bool SaveToFile(const char* filename, const char* filter, int info);
}