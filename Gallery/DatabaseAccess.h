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
	const std::list<Album> getAlbums() override; // 3
	const std::list<Album> getAlbumsOfUser(const User& user) override; // 3
	void createAlbum(const Album& album) override; // 3
	bool doesAlbumExists(const std::string& albumName, int userId) override; // 3
	Album openAlbum(const std::string& albumName) override; // 3
	void closeAlbum(Album& pAlbum) override; // 1
	void printAlbums() override; // 3
	void deleteAlbum(const std::string& albumName, int userId) override; // 2 - done

	// Picture
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture) override; // 3
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName) override; // 3
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override; // 2 - done
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId) override; // 2 - done

	// User
	void printUsers() override; // 3
	User getUser(int userId) override; // 3
	void createUser(User& user) override; // 2 - done
	void deleteUser(const User& user) override; // 2 - done
	bool doesUserExists(int userId) override; // 3

	// Statistics
	int countAlbumsOwnedOfUser(const User& user) override; // 3
	int countAlbumsTaggedOfUser(const User& user) override; // 3
	int countTagsOfUser(const User& user) override; // 3
	float averageTagsPerAlbumOfUser(const User& user) override; // 3

	//	Queries
	User getTopTaggedUser() override; // 3
	Picture getTopTaggedPicture() override; // 3
	std::list<Picture> getTaggedPicturesOfUser(const User& user) override; // 3

	// Unrelated
	bool sendCommandToDB(string sqlStatement);

	friend int albumsCallback(void* data, int argc, char** argv, char** azColName);
};