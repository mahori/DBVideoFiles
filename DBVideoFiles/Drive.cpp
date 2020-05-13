#include "pch.h"
#include "Drive.h"

using namespace std;
using namespace boost;

namespace
{

void findFiles(const CString& directory, const vector<wstring>& extensions, vector<wstring>& foundFiles)
{
    CFileFind fileFind;
    auto isWorking = fileFind.FindFile(directory + L"\\*.*");

    while (isWorking) {
        isWorking = fileFind.FindNextFile();

        if (fileFind.IsDots() || fileFind.IsSystem()) {
            continue;
        }

        auto fileName = fileFind.GetFileName();
        if (starts_with(fileName.GetString(), L".")) {
            continue;
        }

        auto filePath = fileFind.GetFilePath();

        if (fileFind.IsDirectory()) {
            ::findFiles(filePath, extensions, foundFiles);
            continue;
        }

        CPath path(filePath);
        wstring extension = path.GetExtension().GetString();
        if (find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
            foundFiles.push_back(filePath.GetString());
        }
    }

    fileFind.Close();
}

}

Drive::Drive(const wstring& driveLetter, const wstring& virtualLetter, const wstring& id, const wstring& serialNumber)
    : driveLetter_(driveLetter)
    , virtualLetter_(virtualLetter)
    , id_(id)
    , serialNumber_(serialNumber)
{
}

wstring Drive::driveLetter(void) const
{
    return driveLetter_;
}

wstring Drive::virtualLetter(void) const
{
    return virtualLetter_;
}

wstring Drive::id(void) const
{
    return id_;
}

wstring Drive::serialNumber(void) const
{
    return serialNumber_;
}

vector<wstring> Drive::findFiles(void) const
{
    vector<wstring> extensions;
    extensions.push_back(L".ts");
    extensions.push_back(L".mpg");
    extensions.push_back(L".m2p");

    auto directory = driveLetter_ + L":";
    vector<wstring> files;
    ::findFiles(directory.c_str(), extensions, files);

    return files;
}
