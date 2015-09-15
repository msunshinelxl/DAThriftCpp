#ifndef REALAGILORHANDLER_H
#define REALAGILORHANDLER_H

#include <Windows.h>
#include "inter_types.h"
#include "Agilor.h"
#include "AgilorUtils.h"
#include <iostream>
#include <vector>
#include <mutex>
#include "ACI.h"

using namespace std;
using namespace boost;
using namespace aci;

class AgilorHandler :public virtual AgilorIf
{
private:
	static bool isConnected;
	static vector<string> _devices;
	static mutex _mDevice;
	
	static bool isExistDevice(string name);

	string serverName = "agilor";

public:
	AgilorHandler();
	~AgilorHandler();

	static void connAllDevices();

	void ping();

	//设备相关
	bool AddDevice(const DEVICE& device);
	bool DeleteDevice(const std::string& deviceName);
	bool ModifyDevice(const DEVICE& device);
	int64_t QueryDeviceInfo();
	void EnumDeviceInfo(DEVICE& _return, const int64_t hRecordset);
	void GetAllDevices(std::vector<DEVICE> & _return);
	int64_t TagCountByDevice(const std::string& deviceName);


	//点相关
	int64_t QuerySnapshots(const std::string& tagNames, const int32_t tagCount);
	void GetNextTagValue(TAGVAL& _return, const int64_t hRecordset, const bool isRemoved);

	int64_t QueryTagHistory(const std::string& tagName, const int64_t startTime, const int64_t endTime, const int64_t step);

	

	

	//int64_t SetTagValue(const std::string& fullName, const std::string& value, const bool isManual, const std::string& comment);

	

	void GetAggregateValue(TAGVAL& _return, const int64_t hRecordset, const AGGREGATE::type ag, const bool isRemoved);

	

	int64_t QueryTagsbyDevice(const std::string& deviceName);

	void EnumTagName(std::string& _return, const int64_t hRecordset);

	void GetTagNamebyId(std::string& _return, const int64_t tagId);


	int64_t SetValue(const TAGVAL& value);

	int64_t SetValues(const std::vector<TAGVAL> & values);

	int64_t RemoveTag(const int64_t tagId);

	void GetTagInfo(TAGNODE& _return, const std::string& tagName);

	int64_t AddNewTag(const TAGNODE& node, const bool isOverwrite);
	void EnumNextTag(TAGNODE& _return, const int64_t hwnd);


	//订阅点相关
	int64_t SubscribeTags(const std::string& tagNames);
	int64_t UnSubscribeTags(const std::string& tagNames);

	int64_t UnSubscribeAll();
	void GetSubTagValue(TAGVAL& _return);


};

#endif