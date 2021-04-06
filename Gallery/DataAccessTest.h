#pragma once
#include <list>
#include <iostream>
#include "sqlite3.h"
#include <io.h>

using std::string;
using std::cout;
using std::endl;


class DataAccessTest
{
private:
	sqlite3* db;
public:
	sqlite3* createGalleryDB();
	bool sendCommandToDB(sqlite3* db, string sqlStatement);
};

