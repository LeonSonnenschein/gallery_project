#pragma once
#include "IDataAccess.h"

class DatabaseAccess : public IDataAccess
{
private:
	sqlite3* db;

public:
	bool open();
	void close();
	void clear();
	void closeAlbum(Album& pAlbum);
};

