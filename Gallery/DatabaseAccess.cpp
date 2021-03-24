#include "DatabaseAccess.h"

std::list<Album> album_list_; // alright so, theoretically I could take the long way and not create a global variable, but it's too late to work hard on this project, I'm very sorry you had to witness this war crime.

bool DatabaseAccess::open()
{
	string dbFileName = "db.sqlite"; // have to switch to a new name every time
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

const std::list<Album> DatabaseAccess::getAlbums()
{
	const char* sqlStatement = "SELECT * FROM albums;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, albumsCallback, nullptr, &errMessage);
	std::list<Album> albums = album_list_;
	album_list_.clear();
	return albums;
}

const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	return std::list<Album>();
}

void DatabaseAccess::createAlbum(const Album& album)
{
	string complete_querie =
		"INSERT INTO albums (name, user_id, creation_date) VALUES (\"" + album.getName() + "\" ," + std::to_string(album.getOwnerId()) + " ,'" + album.getCreationDate() + "');";
	this->sendCommandToDB(complete_querie);
}

bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	return false;
}

Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	return Album();
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
	// does nothing
}

void DatabaseAccess::printAlbums()
{
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

void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{

}

void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	string complete_querie =
		"INSERT INTO tags(picture_id, user_id) VALUES ((SELECT id FROM pictures WHERE name LIKE " + pictureName + " and album_id = (SELECT id FROM albums WHERE albums.name LIKE " + albumName + ")), "
		+ std::to_string(userId) + "\");";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	string complete_querie = 
		"DELETE FROM tags WHERE user_id = " + std::to_string(userId) +
		" and picture_id = (SELECT id FROM pictures WHERE name LIKE \"" + pictureName + "\" and album_id = (SELECT id FROM albums WHERE albums.name LIKE \"" + albumName + "\"));";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::printUsers()
{
}

User DatabaseAccess::getUser(int userId)
{
	return User(0, "name");
}

void DatabaseAccess::createUser(User& user)
{
	string complete_querie = "INSERT INTO users(id, name) VALUES(" + std::to_string(user.getId()) + ",\"" + user.getName() + "\");";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::deleteUser(const User& user)
{
	string complete_querie = "DELETE FROM pictures WHERE id = \"" + std::to_string(user.getId()) + "\";";
	this->sendCommandToDB(complete_querie);
}

bool DatabaseAccess::doesUserExists(int userId)
{
	return false;
}

int DatabaseAccess::countAlbumsOwnedOfUser(const User& user)
{
	return 0;
}

int DatabaseAccess::countAlbumsTaggedOfUser(const User& user)
{
	return 0;
}

int DatabaseAccess::countTagsOfUser(const User& user)
{
	return 0;
}

float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
	return 0.0f;
}

User DatabaseAccess::getTopTaggedUser()
{
	return User(0, "name");
}

Picture DatabaseAccess::getTopTaggedPicture()
{
	return Picture(0, "name");
}

std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user)
{
	return std::list<Picture>();
}

int albumsCallback(void* data, int argc, char** argv, char** azColName)
{
	Album album(0, "");
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "ID") {
			album.setID(std::atoi(argv[i]));
		}
		else if (string(azColName[i]) == "NAME") {
			album.setName(argv[i]);
		}
		else if (string(azColName[i]) == "USER_ID") {
			album.setOwner(std::atoi(argv[i]));
		}
		else if (string(azColName[i]) == "CREATION_DATE") {
			album.setCreationDate(argv[i]);
		}
		cout << azColName[i] << endl;
	}
	album_list_.push_back(album);
	return 0;
}
