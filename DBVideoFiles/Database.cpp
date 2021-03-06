#include "pch.h"
#include "Database.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

Database::Database(void)
    : pDatabase_(make_unique<CDatabase>())
{
    auto connectString = ::AfxGetApp()->GetProfileStringW(L"Database", L"ConnectString", nullptr);

    try {
        pDatabase_->OpenEx(connectString);
    }
    catch (CDBException* pe) {
        pe->ReportError();
        pe->Delete();
    }
}

Database::~Database(void)
{
    if (pDatabase_->IsOpen()) {
        pDatabase_->Close();
    }
}

bool Database::registerFiles(const wstring& id, const vector<wstring>& files)
{
    if (!pDatabase_->IsOpen()) {
        return false;
    }

    if (!pDatabase_->BeginTrans()) {
        return false;
    }

    try {
        CString deleteSql;
        deleteSql.Format(L"DELETE FROM video_files WHERE drive = %s", id);
        pDatabase_->ExecuteSQL(deleteSql);

        for (const auto& file : files) {
            CPath path(file.c_str());
            int subPath = path.SkipRoot();

            CString entry(path);
            entry.Delete(0, subPath - 1);
            entry.Replace(L"\\", L"/");
            entry.Replace(L"'", L"''");

            CString insertSql;
            insertSql.Format(L"INSERT INTO video_files ( drive, path ) VALUES ( %s, '%s' )", id, entry);
            pDatabase_->ExecuteSQL(insertSql);
        }

        if (!pDatabase_->CommitTrans()) {
            return false;
        }
    }
    catch (CDBException* pe) {
        pDatabase_->Rollback();

        pe->ReportError();
        pe->Delete();

        return false;
    }

    return true;
}
