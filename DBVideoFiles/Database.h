#pragma once

class Database
{
public:
    Database(void);
    ~Database(void);

    bool registerFiles(const std::wstring& id, const std::vector<std::wstring>& files);

private:
    std::unique_ptr<CDatabase> pDatabase_;
};
