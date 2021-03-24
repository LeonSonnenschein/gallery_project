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

void DatabaseAccess::clear()
{
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
	// does nothing
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	string complete_querie = "DELETE FROM albums WHERE name like \"" + albumName + "%\" AND userID = " + std::to_string(userId) + ";";
	this->sendCommandToDB(complete_querie);
}

bool DatabaseAccess::sendCommandToDB(string sqlStatement)
{
	int res;
	char* errMessage = nullptr;

	res = sqlite3_exec(this->db, (const char*)&sqlStatement[0], nullptr, nullptr, &errMessage);

	if (res != SQLITE_OK)
	{
		cout << "Command faild.\nCommand sent: " << sqlStatement << endl;
		return false;
	}

	return true;
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	string complete_querie =
		"INSERT INTO tags(picture_id, user_id) VALUES ((SELECT id FROM pictures WHERE name LIKE " + pictureName + " and album_id = (SELECT id FROM albums WHERE albums.name LIKE " + albumName + ")), "
		+ std::to_string(userId) + "\");";
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	string complete_querie = 
		"DELETE FROM tags WHERE user_id = " + std::to_string(userId) +
		" and picture_id = (SELECT id FROM pictures WHERE name LIKE \"" + pictureName + "\" and album_id = (SELECT id FROM albums WHERE albums.name LIKE \"" + albumName + "\"));";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::createUser(User& user)
{
	string complete_querie = "INSERT INTO users(name) VALUES(" + user.getName() + ");";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::deleteUser(const User& user)
{
	
}


