#include <Windows.h>
#include <fstream>

void GetFirstFile(std::string input);
void GetSecondFile(std::string input, std::string compare, HANDLE hFind, HANDLE hFindName, WIN32_FIND_DATAA local, std::ofstream& OutputFile);


int main() {

	std::string Path = "C:\\Program Files (x86)\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo\\maps\\workshop";
	GetFirstFile(Path);
		
	
	return 0;

}




void GetFirstFile(std::string input) {

	std::ofstream OutputFile("mapcycle.txt");

	HANDLE hFind;
	HANDLE hFindName;
	WIN32_FIND_DATAA FileAttributes;
	WIN32_FIND_DATAA FileAttributesName;


	hFind = FindFirstFileA((LPCSTR)(input + "\\*").c_str(), &FileAttributes);

	while ((FileAttributes.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY || (*FileAttributes.cFileName == '.') || (*FileAttributes.cFileName == '..')) {
		FindNextFileA(hFind, &FileAttributes);
	}


		hFindName = FindFirstFileA((LPCSTR)(input + "\\" + FileAttributes.cFileName + "\\surf*").c_str(), &FileAttributesName);
		
		std::string compare = FileAttributesName.cFileName;
		

		if (compare.find("surf") != std::string::npos) {

		//we dont need .bsp ending
		compare.erase(compare.end() - 4, compare.end());

		OutputFile << "workshop\\" << FileAttributes.cFileName;
		OutputFile << "\\" << compare << std::endl;

	}
	//First file written above


	//Now the rest
	do{

		GetSecondFile(input, compare, hFind, hFindName, FileAttributes, OutputFile);

	} while (FindNextFileA(hFind, &FileAttributes));
	
	FindClose(hFind);

}



//Second Function exists because I'm too dumb to delete contents of FileAttributesName.cFileName after its being checked so we use a very shitty solution.
//Now cFileName is local -> always a fresh one without input
void GetSecondFile(std::string input, std::string compare, HANDLE hFind, HANDLE hFindName, WIN32_FIND_DATAA local, std::ofstream& OutputFile)
{
	WIN32_FIND_DATAA locName;

	if ((local.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY || (*local.cFileName == '.') || (*local.cFileName == '..')) {

		hFindName = FindFirstFileA((LPCSTR)(input + "\\" + local.cFileName + "\\surf*").c_str(), &locName);
		compare = locName.cFileName;
		if (compare.find("surf") != std::string::npos) {

			//we dont need .bsp ending
			compare.erase(compare.end() - 4, compare.end());

			OutputFile << "workshop\\" << local.cFileName;
			OutputFile << "\\" << compare << std::endl;

		}
		FindClose(hFindName);

	}
}
