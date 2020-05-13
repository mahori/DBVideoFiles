#pragma once

class Drive
{
public:
    Drive(const std::wstring& driveLetter, const std::wstring& virtualLetter, const std::wstring& id, const std::wstring& serialNumber);

    std::wstring driveLetter(void) const;
    std::wstring virtualLetter(void) const;
    std::wstring id(void) const;
    std::wstring serialNumber(void) const;
    std::vector<std::wstring> findFiles(void) const;

private:
    std::wstring driveLetter_;
    std::wstring virtualLetter_;
    std::wstring id_;
    std::wstring serialNumber_;
};
