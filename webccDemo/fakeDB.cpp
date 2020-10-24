#include "fakeDB.h"


#include <cJSON.h>

FakeDB& FakeDB::Inst()
{
	static FakeDB fakeDB;
	return fakeDB;
}

FakeDB::FakeDB()
{
	_userList.push_back({ 1,"admin","123",1 });
	_userList.push_back({ 2,"inuyasha","123",2 });
	_userList.push_back({ 3,"kagome","123",2 });
	_userList.push_back({ 4,"kikyo","123",2 });
	_userList.push_back({ 5,u8"せっしょうまる","123",2 });
	_userList.push_back({ 6,u8"かぐら","123",2 });
	_userList.push_back({ 7,u8"旺财","123",2 });
	_userList.push_back({ 8,u8"神楽","123",2 });

	_groupList.push_back({ 1,"admin", "ROLE_ADMIN", 1 });
	_groupList.push_back({ 2,u8"普通", "ROLE_USER", 1 });
	_groupList.push_back({ 3,u8"奥林匹斯神族", "ROLE_ADMIN,ROLE_USER", 1 });
}

std::shared_ptr<std::string> FakeDB::getUserListAll(bool isAdmin)
{
	std::lock_guard<std::mutex> lock(_lock);

	cJSON* root = cJSON_CreateObject();

	cJSON_AddNumberToObject(root, "code", 0);

	if (isAdmin)
	{
		cJSON_AddNumberToObject(root, "isEditable", 1);
	}
	else
	{
		cJSON_AddNumberToObject(root, "isEditable", 0);
	}

	cJSON* list = cJSON_CreateArray();
	for (std::list<UserInfo>::iterator iter = _userList.begin();
		iter != _userList.end(); iter++)
	{
		cJSON* record = cJSON_CreateObject();

		cJSON_AddNumberToObject(record, "id", iter->id);
		cJSON_AddStringToObject(record, "loginName", iter->loginname.c_str());
		cJSON_AddStringToObject(record, "groupName", getGroupNameByGroupID(iter->idGroupInfo));

		cJSON_AddItemToArray(list, record);
	}

	cJSON_AddItemToObject(root, "list", list);

	std::shared_ptr<std::string> pRet(new std::string(cJSON_Print(root)));
	cJSON_Delete(root);
	return pRet;
}

std::shared_ptr<std::string> FakeDB::getUserGroupList(bool isAdmin)
{
	std::lock_guard<std::mutex> lock(_lock);

	cJSON* root = cJSON_CreateObject();

	cJSON_AddNumberToObject(root, "code", 0);

	cJSON* list = cJSON_CreateArray();
	for (std::list<GroupInfo>::const_iterator iter = _groupList.begin();
		iter != _groupList.end(); iter++)
	{
		cJSON* record = cJSON_CreateObject();

		cJSON_AddNumberToObject(record, "idGroupInfo", iter->idGroupInfo);
		cJSON_AddStringToObject(record, "groupName", iter->groupName.c_str());
		cJSON_AddStringToObject(record, "roleType", iter->roleType.c_str());

		if (isAdmin)
		{
			cJSON_AddNumberToObject(record, "undeletable", 1);
		}
		else
		{
			cJSON_AddNumberToObject(record, "undeletable", 0);
		}

		cJSON_AddItemToArray(list, record);
	}


	cJSON_AddItemToObject(root, "list", list);

	std::shared_ptr<std::string> pRet(new std::string(cJSON_Print(root)));
	cJSON_Delete(root);
	return pRet;
}

bool FakeDB::isUserExist(std::string jsonLoginName) const
{
	using namespace std;

	cJSON* root = cJSON_ParseWithLength(jsonLoginName.c_str(), jsonLoginName.size());

	string loginName = cJSON_GetStringValue(cJSON_GetObjectItem(root, "loginName"));

	for (list<UserInfo>::const_iterator iter = _userList.begin(); iter != _userList.end(); iter++)
	{
		if (iter->loginname == loginName)
		{
			return true;
		}
	}

	cJSON_Delete(root);
	return false;
}

bool FakeDB::addUser(std::string jsonNewUserInfo)
{
	using namespace std;

	cJSON* root = cJSON_ParseWithLength(jsonNewUserInfo.c_str(), jsonNewUserInfo.size());

	string loginname = cJSON_GetStringValue(cJSON_GetObjectItem(root, "loginname"));
	string password = cJSON_GetStringValue(cJSON_GetObjectItem(root, "password"));
	unsigned int idGroupInfo = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "idGroupInfo"));
	cJSON_Delete(root);

	unsigned int iduserinfo = [](std::list<UserInfo>& _userList)->unsigned int {
		unsigned int i = 0;
		for (list<UserInfo>::const_iterator iter = _userList.begin(); iter != _userList.end(); iter++)
		{
			i += iter->id;
		}
		return i;
	}(_userList);
	_userList.push_back({ iduserinfo,loginname,password,idGroupInfo });

	return true;
}

bool FakeDB::updateUserInfo(std::string jsonNewUserInfo)
{
	using namespace std;

	cJSON* root = cJSON_ParseWithLength(jsonNewUserInfo.c_str(), jsonNewUserInfo.size());

	unsigned int iduserinfo = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "iduserinfo"));
	string password = cJSON_GetStringValue(cJSON_GetObjectItem(root, "password"));
	unsigned int idGroupInfo = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "iduserinfo"));

	for (list<UserInfo>::iterator iter = _userList.begin(); iter != _userList.end(); iter++)
	{
		if (iter->id == iduserinfo && password.empty() == false)
		{
			iter->password = password;
			iter->idGroupInfo = idGroupInfo;

			cJSON_Delete(root);
			return true;
		}
	}

	cJSON_Delete(root);
	return false;
}

bool FakeDB::delUserByID(std::string jsonUserID)
{
	using namespace std;

	cJSON* root = cJSON_ParseWithLength(jsonUserID.c_str(), jsonUserID.size());

	unsigned int iduserinfo = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "iduserinfo"));

	for (list<UserInfo>::iterator iter = _userList.begin(); iter != _userList.end(); iter++)
	{
		if (iter->id == iduserinfo)
		{
			cJSON_Delete(root);
			return true;
		}
	}

	cJSON_Delete(root);
	return false;
}


const char* FakeDB::getGroupNameByGroupID(unsigned int id) const
{
	for (std::list<GroupInfo>::const_iterator iter = _groupList.begin();
		iter != _groupList.end(); iter++)
	{
		if (iter->idGroupInfo == id)
		{
			return iter->groupName.c_str();
		}
	}
	return nullptr;
}

bool FakeDB::isLogin(std::string& username, std::string& password) const
{
	for (std::list<UserInfo>::const_iterator iter = _userList.begin();
		iter != _userList.end(); iter++)
	{
		if (username == iter->loginname && password == iter->password)
		{
			return true;
		}
	}

	return false;
}
