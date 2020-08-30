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
	_groupList.push_back({ 2,"user", "ROLE_USER", 1 });

	_groupList.push_back({ 2,"user", "ROLE_USER", 1 });
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

	return pRet;
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
