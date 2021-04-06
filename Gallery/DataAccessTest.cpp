#include "DataAccessTest.h"

sqlite3* DataAccessTest::createGalleryDB()
{
	string dbFileName = "newGalleryDB.sqlite"; // have to switch to a new name every time
	int doesFileExist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &this->db);

	if (res != SQLITE_OK) {
		this->db = nullptr;
		cout << "Failed to open DB" << endl;

		return nullptr;
	}


	if (doesFileExist == 0) {
		cout << "database already exists..." << endl;
		return db;
	}
	sendCommandToDB(db, "CREATE TABLE USERS (ID INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , NAME TEXT NOT NULL)");
	sendCommandToDB(db, "CREATE TABLE ALBUMS (ID INTEGER PRIMARY KEY  AUTOINCREMENT  NOT NULL , NAME TEXT NOT NULL , USER_ID INTEGER NOT NULL , CREATION_DATE TEXT NOT NULL,  FOREIGN KEY(USER_ID) REFERENCES USERS (ID))");
	sendCommandToDB(db, "CREATE TABLE PICTURES ( ID  INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,  NAME TEXT  NOT NULL, LOCATION TEXT NOT NULL,CREATION_DATE TEXT NOT NULL, ALBUM_ID INTEGER NOT NULL,  FOREIGN KEY(ALBUM_ID ) REFERENCES ALBUMS (ID))");
	sendCommandToDB(db, "CREATE TABLE TAGS(PICTURE_ID INTEGER NOT NULL, USER_ID INTEGER NOT NULL, PRIMARY KEY(PICTURE_ID,USER_ID),  FOREIGN KEY(PICTURE_ID ) REFERENCES PICTURES (ID), FOREIGN KEY(USER_ID ) REFERENCES USERS (ID))");
    return this->db;
}

bool DataAccessTest::sendCommandToDB(sqlite3* db, string sqlStatement)
{
	int res = 0;
	char* errMessage = nullptr;

	res = sqlite3_exec(db, (const char*)&sqlStatement[0], nullptr, nullptr, &errMessage);
	if (res != SQLITE_OK)
	{
		cout << "Command faild.\nCommand sent: " << sqlStatement << endl;
		return false;
	}

	return true;
}