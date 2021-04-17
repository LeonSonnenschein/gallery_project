#pragma once
#include "IDataAccess.h"

class DatabaseAccess : public IDataAccess
{
private:
	sqlite3* db;

public:

	bool open(); // 1 - done
	void close(); // 1 - done 
	void clear(); // 1 - done 

	// Album
	const std::list<Album> getAlbums() override; // 3 - done
	const std::list<Album> getAlbumsOfUser(const User& user) override; // 3 - done
	void createAlbum(const Album& album) override; // 3 - done
	bool doesAlbumExists(const std::string& albumName, int userId) override; // 3 - done
	Album openAlbum(const std::string& albumName) override; // 3 - done
	void closeAlbum(Album& pAlbum) override; // 1 - done
	void printAlbums() override; // 3 - done
	void deleteAlbum(const std::string& albumName, int userId) override; // 2 - done

	// Picture
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) override; // 3 - done
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) override; // 3 - done
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override; // 2 - done
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override; // 2 - done

	// User
	void printUsers() override; // 3 - done
	User getUser(int userId) override; // 3 - done
	void createUser(User& user) override; // 2 - done
	void deleteUser(const User& user) override; // 2 - done
	bool doesUserExists(int userId) override; // 3 - done

	// Statistics
	int countAlbumsOwnedOfUser(const User& user) override; // 3 - done
	int countAlbumsTaggedOfUser(const User& user) override; // 3 - done
	int countTagsOfUser(const User& user) override; // 3 - done
	float averageTagsPerAlbumOfUser(const User& user) override; // 3 - done

	//	Queries
	User getTopTaggedUser() override; // 3 - done
	Picture getTopTaggedPicture() override; // 3 - done
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override; // 3

	// Unrelated
	bool sendCommandToDB(string sqlStatement);

	friend int albumsCallback(void* data, int argc, char** argv, char** azColName); // usefull

	friend int pictureCallback(void* data, int argc, char** argv, char** azColName); // usefull

	friend int tagsCallback(void* data, int argc, char** argv, char** azColName); // usefull

	friend int printCallback(void* data, int argc, char** argv, char** azColName); // usefull

	friend int countCallback(void* data, int argc, char** argv, char** azColName); // usefull

	friend int getUserCallback(void* data, int argc, char** argv, char** azColName); // usefull

	int valid_id(string table_name);
};