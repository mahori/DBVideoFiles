#include "pch.h"
#include "Drives.h"
#include "Drive.h"

#pragma comment(lib, "mpr.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace boost;

namespace
{

vector<wstring> getRootPathNames(void)
{
    auto bufferLength = ::GetLogicalDriveStringsW(0, nullptr);
    vector<WCHAR> buffer(bufferLength);
    ::GetLogicalDriveStringsW(bufferLength, buffer.data());

    vector<wstring> rootPathNames;
    split(rootPathNames, buffer, [](WCHAR c) { return (c == L'\0'); }, token_compress_on);

    return rootPathNames;
}

wstring getVolumeName(const wstring& rootPathName)
{
    array<WCHAR, MAX_PATH + 1> volumeNameBuffer;
    auto volumeNameSize = MAX_PATH + 1;
    ::GetVolumeInformationW(rootPathName.c_str(), volumeNameBuffer.data(), volumeNameSize, nullptr, nullptr, nullptr, nullptr, 0);

    auto it = find(volumeNameBuffer.begin(), volumeNameBuffer.end(), L'\0');
    wstring volumeName(volumeNameBuffer.begin(), it);

    if (volumeName == L"Shared Folders") {
        array<WCHAR, 1024> buffer;
        DWORD bufferSize = 1024;
        ::WNetGetUniversalNameW(rootPathName.c_str(), UNIVERSAL_NAME_INFO_LEVEL, buffer.data(), &bufferSize);

        LPUNIVERSAL_NAME_INFO pUNI = reinterpret_cast<LPUNIVERSAL_NAME_INFO>(buffer.data());
        wstring universalName = pUNI->lpUniversalName;

        wregex re(L"\\\\\\\\\\w+\\\\(.+)\\\\");
        wsmatch m;
        if (regex_match(universalName, m, re)) {
            volumeName = m[1];
        }
    }

    return volumeName;
}

tuple<bool, wstring, wstring, wstring> parseVolumeName(const wstring& volumeName)
{
    bool matched = false;
    wstring virtualLetter;
    wstring id;
    wstring serialNumber;

    wregex re1(L"([[:alpha:]]) (\\d+) (\\S+)");
    wregex re2(L"(\\d+) (\\S+)");
    wregex re3(L"(\\d+)");
    wsmatch m;

    if (regex_match(volumeName, m, re1)) {
        matched = true;
        virtualLetter = m[1];
        id = m[2];
        serialNumber = m[3];
    } else if (regex_match(volumeName, m, re2)) {
        matched = true;
        id = m[1];
        serialNumber = m[2];
    } else if (regex_match(volumeName, m, re3)) {
        matched = true;
        id = m[1];
    }

    return make_tuple(matched, virtualLetter, id, serialNumber);
}

wstring getDriveLetter(const wstring& rootPathName)
{
    wstring driveLetter;

    wregex re(L"([[:alpha:]]):\\\\");
    wsmatch m;

    if (regex_match(rootPathName, m, re)) {
        driveLetter = m[1];
    }

    return driveLetter;
}

}

Drives::Drives(void)
{
    auto rootPathNames = ::getRootPathNames();

    for (const auto& rootPathName : rootPathNames) {
        auto volumeName = ::getVolumeName(rootPathName);

        auto [matched, virtualLetter, id, serialNumber] = ::parseVolumeName(volumeName);
        if (matched) {
            auto driveLetter = ::getDriveLetter(rootPathName);

            drives_.push_back(make_shared<Drive>(driveLetter, virtualLetter, id, serialNumber));
        }
    }
}

size_t Drives::count(void) const
{
    return drives_.size();
}

tuple<wstring, wstring, wstring, wstring> Drives::at(size_t index) const
{
    auto drive = drives_.at(index);

    return make_tuple(drive->driveLetter(), drive->virtualLetter(), drive->id(), drive->serialNumber());
}

vector<wstring> Drives::findFiles(size_t index) const
{
    auto drive = drives_.at(index);

    return drive->findFiles();
}
