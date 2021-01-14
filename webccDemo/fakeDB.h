#pragma once
#ifndef _FAKEDB_H_
#define _FAKEDB_H_

#ifdef _WIN32
#pragma once
#endif

#include <string>
#include <list>
#include <memory>
#include <mutex>

struct UserInfo
{
	unsigned int id;
	std::string  loginname;
	std::string  password;

	//foreign key
	unsigned int idGroupInfo;
};

struct GroupInfo
{
	unsigned int idGroupInfo;
	std::string  groupName;
	std::string  roleType;

	//foreign key
	unsigned int idMapInfo;
};

struct MapInfo
{
	unsigned int idMapInfo;
	std::string  mapName;
};

struct RetMsgUserList
{
};

class FakeDB
{
public:
	static FakeDB& Inst();

	std::shared_ptr<std::string> getUserListAll(bool isAdmin);
	std::shared_ptr<std::string> getUserGroupList(bool isAdmin);

	bool updateUserInfo(std::string jsonNewUserInfo);
	bool addUser(std::string jsonNewUserInfo);
	bool delUserByID(std::string jsonUserID);
	bool isLogin(std::string& username, std::string& password) const;
	bool isUserExist(std::string jsonLoginName) const;
protected:
	std::list<UserInfo> _userList;
	std::list<GroupInfo> _groupList;
	std::list<MapInfo> _mapList;

	std::mutex _lock;

	const char* getGroupNameByGroupID(unsigned int id) const;
private:
	FakeDB(); // Private constructor
	FakeDB(const FakeDB&) {} // Prevent copy-construction
	FakeDB& operator=(const FakeDB&) {} // Prevent assignment
};


#endif // !_FAKEDB_H_
