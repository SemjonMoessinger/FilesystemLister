#include "main.h"
#include <filesystem>

namespace fs = std::filesystem;

int main()
{
	std::cout << "Hello World." << std::endl;

	try {
        auto currentPath = fs::current_path();
        std::cout << currentPath << std::endl;

		//  C:/Daten/601 git/FilesystemLister
        std::cout << fs::file_size("../../../../TestFilesystem/TestFile.txt")
                  << std::endl;
	}
	catch (fs::filesystem_error& e) {
		std::cout << e.what() << '\n';
	}

	return 0;
}
