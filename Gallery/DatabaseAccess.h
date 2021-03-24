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
	const std::list<Album> getAlbums(); // 3
	const std::list<Album> getAlbumsOfUser(const User& user); // 3
	void createAlbum(const Album& album); // 3
	bool doesAlbumExists(const std::string& albumName, int userId); // 3
	Album openAlbum(const std::string& albumName); // 3
	void closeAlbum(Album& pAlbum); // 1
	void printAlbums(); // 3
	void deleteAlbum(const std::string& albumName, int userId); // 2 - done

	// Picture
	void addPictureToAlbumByName(const std::string& albumName, const Picture& picture); // 3
	void removePictureFromAlbumByName(const std::string& albumName, const std::string& pictureName); // 3
	void tagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId); // 2 - done
	void untagUserInPicture(const std::string& albumName, const std::string& pictureName, int userId); // 2 - done

	// User
	void printUsers(); // 3
	User getUser(int userId); // 3
	void createUser(User& user); // 2 - done
	void deleteUser(const User& user); // 2 - done
	bool doesUserExists(int userId); // 3

	// Statistics
	int countAlbumsOwnedOfUser(const User& user); // 3
	int countAlbumsTaggedOfUser(const User& user); // 3
	int countTagsOfUser(const User& user); // 3
	float averageTagsPerAlbumOfUser(const User& user); // 3

	//	Queries
	User getTopTaggedUser(); // 3
	Picture getTopTaggedPicture(); // 3
	std::list<Picture> getTaggedPicturesOfUser(const User& user); // 3

	// Unrelated
	bool sendCommandToDB(string sqlStatement);
};

