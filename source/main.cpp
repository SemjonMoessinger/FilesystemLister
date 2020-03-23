#include "main.h"
#include <set>
#include <filesystem>
#include <chrono>
#include <sstream>
#include <iomanip> // put_time
#include <string>  
#include <fstream>

namespace fs = std::filesystem;

// The following 
template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}
std::string createIsoTimeString(fs::file_time_type fileTime)
{
    std::time_t tt = to_time_t(fileTime);
    std::tm* gmt = std::gmtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt, "%Y-%M-%dT%H:%M:%S");
    return buffer.str();
}

////////////////////////////////////////////////////////////////////////////
// copy-paste from https://en.cppreference.com/w/cpp/string/char_traits
struct ci_char_traits : public std::char_traits<char> {
    static bool eq(char c1, char c2) { return toupper(c1) == toupper(c2); }
    static bool ne(char c1, char c2) { return toupper(c1) != toupper(c2); }
    static bool lt(char c1, char c2) { return toupper(c1) < toupper(c2); }
    static int compare(const char* s1, const char* s2, size_t n) {
        while (n-- != 0) {
            if (toupper(*s1) < toupper(*s2)) return -1;
            if (toupper(*s1) > toupper(*s2)) return 1;
            ++s1; ++s2;
        }
        return 0;
    }
    static const char* find(const char* s, int n, char a) {
        while (n-- > 0 && toupper(*s) != toupper(a)) {
            ++s;
        }
        return s;
    }
};

using ci_string = std::basic_string<char, ci_char_traits> ;

std::ostream& operator<<(std::ostream& os, const ci_string& str) {
    return os.write(str.data(), str.size());
}
////////////////////////////////////////////////////////////////////////////

using FileSet = std::set<ci_string>;

void addFilesInDir(const fs::path& dir, std::set<ci_string>& files);
FileSet getAllFilesRecursive(const fs::path& path);

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
        if (entry.is_directory()) {
            addFilesInDir(entry.path(), files);
        }
        else if (entry.is_regular_file()) 
        {
            std::stringstream buffer;
            buffer << "file: " << entry.path().string()
                << " | last write time: " << createIsoTimeString(entry.last_write_time())
                << " | size(Bytes): " << entry.file_size();

            files.insert(buffer.str().c_str());
        }
    }
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