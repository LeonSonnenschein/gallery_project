#include "DatabaseAccess.h"

bool DatabaseAccess::open()
{
	string dbFileName = "newDB3.sqlite"; // have to switch to a new name every time
	int doesFileExist = _access(dbFileName.c_str(), 0);
	int res = sqlite3_open(dbFileName.c_str(), &this->db);

	if (res != SQLITE_OK) {
		this->db = nullptr;
		cout << "Failed to open DB" << endl;

		return false;
	}

	cout << "checking if DB exists..." << endl;

	if (doesFileExist == 0) {
		cout << "DB exists..." << endl;
		// init database
	}
	else
	{
		cout << "DB doesn't exists...\n created DB..." << endl;
	}

	return true;
}

void DatabaseAccess::close()
{
	cout << "Closing DB..." << endl;
	sqlite3_close(db);
	db = nullptr;
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
	// does nothing lol
}
