#pragma once

class Drive;

class Drives
{
public:
    Drives(void);

    std::size_t count(void) const;
    std::tuple<std::wstring, std::wstring, std::wstring, std::wstring> at(std::size_t index) const;
    std::vector<std::wstring> findFiles(std::size_t index) const;

private:
    std::vector<std::shared_ptr<Drive>> drives_;
};
