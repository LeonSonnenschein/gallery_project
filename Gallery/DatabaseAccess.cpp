#include "DatabaseAccess.h"
#include "DatabaseAccess.h"

std::list<Album> global_album_list; // alright so, theoretically I could take the long way and not create a global variable, but it's too late to work hard on this project, I'm very sorry you had to witness this war crime.
int global_count = 0; // again, really sorry, its harder for me than it is for anyone that looks at it
User* global_user; // this is getting worse every time
int global_available_id = 0; // the pain, it hurts so much
std::list<Picture> global_picture_list; // the pain is gone, I feel nothing now, there is nothing left in me
sqlite3* global_db; // and... the pain is back
std::set<int> global_users_tags; // cool, more global, I would say that I'm dead inside but to be honest I died around the third global

bool DatabaseAccess::open()
{
	string dbFileName = "db.sqlite";
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

	global_db = this->db; // don't look, the look of it can burn your eyes, you will be able to see again, you just won't want to

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
	this->sendCommandToDB("DELETE FROM tags;");
	this->sendCommandToDB("DELETE FROM albums;");
	this->sendCommandToDB("DELETE FROM pictures;");
	this->sendCommandToDB("DELETE FROM users;");
}

const std::list<Album> DatabaseAccess::getAlbums()
{
	const char* sqlStatement = "SELECT * FROM albums;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, albumsCallback, nullptr, &errMessage);
	std::list<Album> albums = global_album_list;
	global_album_list.clear();

	return albums;
}

const std::list<Album> DatabaseAccess::getAlbumsOfUser(const User& user)
{
	string temp = "SELECT * FROM albums WHERE user_id = " + std::to_string(user.getId()) + ";";
	const char* sqlStatement = temp.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, albumsCallback, nullptr, &errMessage);
	std::list<Album> albums = global_album_list;
	global_album_list.clear();

	return albums;
}

void DatabaseAccess::createAlbum(const Album& album)
{
	string complete_querie =
		"INSERT INTO albums (name, user_id, creation_date) VALUES (\"" + album.getName() + "\" ," + std::to_string(album.getOwnerId()) + " ,'" + album.getCreationDate() + "');";
	this->sendCommandToDB(complete_querie);
}

bool DatabaseAccess::doesAlbumExists(const std::string& albumName, int userId)
{
	bool ans = false;
	string string_statement = "SELECT * FROM albums WHERE user_id = " + std::to_string(userId) + " AND name LIKE \"" + albumName + "\";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, countCallback, nullptr, &errMessage);
	if (global_count != 0)
	{
		ans = true;
		global_count = 0;
	}

	return ans;
}

Album DatabaseAccess::openAlbum(const std::string& albumName)
{
	string temp = "SELECT * FROM albums WHERE name LIKE \"" + albumName + "\";";
	const char* sqlStatement = temp.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, albumsCallback, nullptr, &errMessage);
	Album album(global_album_list.front());
	global_album_list.clear();

	return album;
}

void DatabaseAccess::closeAlbum(Album& pAlbum)
{
	// does nothing
}

void DatabaseAccess::printAlbums()
{
	const char* sqlStatement = "SELECT * FROM albums;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, printCallback, nullptr, &errMessage);
}

void DatabaseAccess::deleteAlbum(const std::string& albumName, int userId)
{
	string complete_querie = "DELETE FROM albums WHERE name LIKE \"" + albumName + "\" AND user_id = " + std::to_string(userId) + ";";
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

int DatabaseAccess::valid_id(string table_name)
{
	sqlite3_stmt* stmt;
	int ans = 0;

	int rc = sqlite3_prepare_v2(this->db, "SELECT id FROM users WHERE id = -1", -1, &stmt, NULL);
	if (rc != SQLITE_OK) {
		return -1;
	}

	ans = sqlite3_column_int(stmt, 0);

	sqlite3_finalize(stmt);

	return ans + 1;
}

int albumsPrintCallback(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "ID") {
			cout << argv[i] << "|";
		}
		else if (string(azColName[i]) == "NAME") {
			cout << argv[i] << "|";
		}
		else if (string(azColName[i]) == "USER_ID") {
			cout << argv[i] << "|";
		}
		else if (string(azColName[i]) == "CREATION_DATE") {
			cout << argv[i];
		}
		//cout << argv[i] << endl;
	}
	cout << "\n";
	return 0;
}

int printCallback(void* data, int argc, char** argv, char** azColName)
{
	int i = 0;
	for (i = 0; i < argc - 1; i++) {
		cout << argv[i] << "|";
	}
	cout << argv[i] << endl;
	return 0;
}

int countCallback(void* data, int argc, char** argv, char** azColName)
{
	global_count++;

	return 0;
}

int getUserCallback(void* data, int argc, char** argv, char** azColName)
{
	string name = "";
	int user_id = -1;

	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "ID") {
			user_id = atoi(argv[i]);
		}
		else if (string(azColName[i]) == "NAME") {
			name = argv[i];
		}
	}
	global_user = new User(user_id, name);
	return 0;
}

void DatabaseAccess::addPictureToAlbumByName(const std::string& albumName, const Picture& picture)
{
	string complete_querie = "INSERT INTO pictures(id, name, location, creation_date, album_id) VALUES(" + std::to_string(picture.getId()) + ", \"" + picture.getName() + "\", \"" + picture.getPath()
		+ "\", '" + picture.getCreationDate() + "',(SELECT id FROM albums WHERE name like \"" + albumName + "\"));";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName)
{
	string complete_querie = "DELETE FROM pictures WHERE album_id = " + (string)"(SELECT id FROM albums WHERE name LIKE \"" + albumName + "\") AND name LIKE \"" + pictureName + "\";";
	this->sendCommandToDB(complete_querie);
}

void DatabaseAccess::tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId)
{
	string complete_querie =
		"INSERT INTO tags(picture_id, user_id) VALUES ((SELECT id FROM pictures WHERE name LIKE \"" + pictureName + "\" and album_id = (SELECT id FROM albums WHERE albums.name LIKE \"" + albumName + "\")), "
		+ std::to_string(userId) + ");";
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
	const char* sqlStatement = "SELECT * FROM users;";
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, printCallback, nullptr, &errMessage);
	//int res = sqlite3_exec(db, sqlStatement, printCallback, nullptr, &errMessage);
}

User DatabaseAccess::getUser(int userId)
{
	string string_statement = "SELECT * FROM users WHERE id = " + std::to_string(userId) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, getUserCallback, nullptr, &errMessage);

	User user(global_user->getId(), global_user->getName());
	delete global_user;

	return user;
}

void DatabaseAccess::createUser(User& user)
{
	string complete_querie = "INSERT INTO users(id, name) VALUES(" + std::to_string(user.getId()) + ",\"" + user.getName() + "\");";
	this->sendCommandToDB(complete_querie);
	
}

void DatabaseAccess::deleteUser(const User& user)
{
	string complete_querie = "DELETE FROM users WHERE id = \"" + std::to_string(user.getId()) + "\";";
	this->sendCommandToDB(complete_querie);
}

bool DatabaseAccess::doesUserExists(int userId)
{
	bool ans = false;
	string string_statement = "SELECT * FROM users WHERE id = " + std::to_string(userId) + ";";
	const char* sqlStatement =  string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, countCallback, nullptr, &errMessage);
	if (global_count != 0)
	{
		ans = true;
		global_count = 0;
	}

	return ans;
}

int DatabaseAccess::countAlbumsOwnedOfUser(const User& user) // SELECT * FROM albums WHERE user_id = 2;
{
	int ans = 0;
	string string_statement = "SELECT user_id FROM albums WHERE user_id = " + std::to_string(user.getId()) + ";"; // I could use COUNT function but that will require a good implementation of sqlite 
																												  // in c++ or adding another callback function, and nither of those things are going to happen
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, countCallback, nullptr, &errMessage);

	ans = global_count;
	global_count = 0;

	return ans;
}

int DatabaseAccess::countAlbumsTaggedOfUser(const User& user)
{
	int ans = 0;
	string string_statement = "SELECT DISTINCT albums.id FROM tags JOIN pictures ON tags.picture_id = pictures.id JOIN albums ON albums.id = pictures.album_id WHERE albums.user_id = " + std::to_string(user.getId()) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, countCallback, nullptr, &errMessage);

	ans = global_count;
	global_count = 0;

	return ans;
}

int DatabaseAccess::countTagsOfUser(const User& user)
{
	int ans = 0;
	string string_statement = "SELECT * FROM tags WHERE user_id = " + std::to_string(user.getId()) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, countCallback, nullptr, &errMessage);

	ans = global_count;
	global_count = 0;

	return ans;
}

float DatabaseAccess::averageTagsPerAlbumOfUser(const User& user)
{
	return (float)this->countAlbumsOwnedOfUser(user) / (float)this->countAlbumsTaggedOfUser(user); // this one was really funny
}

User DatabaseAccess::getTopTaggedUser()
{
	const char* sqlStatement = "SELECT * FROM users WHERE id = (SELECT user_id FROM(SELECT user_id, count(*) FROM tags GROUP BY user_id ORDER BY count(*) DESC) LIMIT 1);";
	char* errMessage = nullptr;
	int res = sqlite3_exec(db, sqlStatement, getUserCallback, nullptr, &errMessage);

	User user(global_user->getId(), global_user->getName());
	delete global_user;

	return user;
}

Picture DatabaseAccess::getTopTaggedPicture()
{
	string string_statement = "SELECT * FROM pictures WHERE id = (SELECT picture_id FROM(SELECT picture_id, count(*) FROM tags GROUP BY user_id ORDER BY count(*) DESC) LIMIT 1);";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;

	int res = sqlite3_exec(global_db, sqlStatement, pictureCallback, nullptr, &errMessage);

	Picture picture(global_picture_list.front());
	global_picture_list.clear();

	return picture;
}

std::list<Picture> DatabaseAccess::getTaggedPicturesOfUser(const User& user)
{
	string string_statement = "SELECT DISTINCT pictures.id, pictures.name, pictures.location, pictures.creation_date, pictures.album_id FROM tags JOIN pictures ON tags.picture_id = pictures.id WHERE tags.user_id = "
		+ std::to_string(user.getId()) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;

	int res = sqlite3_exec(global_db, sqlStatement, pictureCallback, nullptr, &errMessage);

	std::list<Picture> picture_list(global_picture_list);
	global_picture_list.clear();

	return picture_list;
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
	}

	string string_statement = "SELECT * FROM pictures WHERE album_id = " + std::to_string(album.getID()) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;

	int res = sqlite3_exec(global_db, sqlStatement, pictureCallback, nullptr, &errMessage);

	std::list<Picture> picture_list(global_picture_list);

	album.setPictureList(global_picture_list);
	global_picture_list.clear();
	global_album_list.clear();

	global_album_list.push_back(album);
	return 0;
}

int pictureCallback(void* data, int argc, char** argv, char** azColName)
{
	Picture picture(0, "");
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "ID") {
			picture.setId(std::atoi(argv[i]));
		}
		else if (string(azColName[i]) == "NAME") {
			picture.setName(argv[i]);
		}
		else if (string(azColName[i]) == "LOCATION") {
			picture.setPath(argv[i]);
		}
		else if (string(azColName[i]) == "CREATION_DATE") {
			picture.setCreationDate(argv[i]);
		}
	}
	string string_statement = "SELECT * FROM tags WHERE picture_id = " + std::to_string(picture.getId()) + ";";
	const char* sqlStatement = string_statement.c_str();
	char* errMessage = nullptr;

	int res = sqlite3_exec(global_db, sqlStatement, tagsCallback, nullptr, &errMessage);

	picture.setTagedUsers(global_users_tags);
	global_users_tags.clear();

	global_picture_list.push_back(picture);
	return 0;
}

int tagsCallback(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "USER_ID") {
			global_users_tags.insert(std::atoi(argv[i]));
		}
	}
	return 0;
}
