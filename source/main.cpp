#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>  

#include "Helper.h"


namespace fs = std::filesystem;

using FileSet = std::set<ci_string>;

void addFilesInDir(const fs::path& dir, std::set<ci_string>& files);
FileSet getAllFilesRecursive(const fs::path& path);
ci_string convertFileEntryToString(const fs::directory_entry& entry);
void writeFilesInFile(const std::string& fileName, FileSet files);

int main(int argc, const char** argv)
{
    fs::path pathToListFiles;

    if (argc >= 2)
    {
        pathToListFiles = argv[1];
    }
    else
    {
        pathToListFiles = fs::current_path();
    }

    auto files = getAllFilesRecursive(pathToListFiles);

    writeFilesInFile("FileList.txt", files);

    return 0;
}

FileSet getAllFilesRecursive(const fs::path& path)
{
    FileSet files;
    try
    {
        addFilesInDir(path, files);
    }
    catch (fs::filesystem_error& ex)
    {
        std::cout << ex.what() << '\n';
    }

    return files;
}

void addFilesInDir(const fs::path& dir, std::set<ci_string>& files)
{
    for (const auto& entry : fs::directory_iterator(dir)) 
    {
        if (entry.is_directory()) 
        {
            // Watch out! The following line creates a (intended) recursion
            addFilesInDir(entry.path(), files);
        }
        else if (entry.is_regular_file()) 
        {
            files.insert(convertFileEntryToString(entry));
        }
    }
}

ci_string convertFileEntryToString(const fs::directory_entry& entry)
{
    std::stringstream buffer;
    buffer << "file: " << entry.path().string()
        << " | last write time: " << createIsoTimeString(entry.last_write_time())
        << " | size(Bytes): " << entry.file_size();
    return buffer.str().c_str();
}

void writeFilesInFile(const std::string& fileName, FileSet files)
{
    std::ofstream file(fileName);
    for (const ci_string& item : files)
    {
        file << item << std::endl;
    }
    file.close();
}