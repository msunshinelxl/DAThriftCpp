#include "AgilorHandler.h"

bool AgilorHandler::isConnected = false;
vector<string> AgilorHandler::_devices;
mutex   AgilorHandler::_mDevice;

AgilorHandler::AgilorHandler()
{
	if (!isConnected)
	{
		//assert(Agcn_Startup()==-1);
		aci::startup();
		aci::createConnNode(this->serverName);
		assert(!aci::connect(this->serverName, LOCALHOST, this->serverName, this->serverName));

		long hwnd = aci::queryDeviceInfo(this->serverName);
		DEV_INFO dev;
		_devices.clear();
		while (aci::enumDeviceInfo(hwnd, NULL, &dev))
		{
			_mDevice.lock();
			_devices.push_back(string(dev.szDeviceName));
			if (!dev.bIsOnline)
			{
				aci::registerDevice(LOCALHOST, RTDB_PORT, _devices.back());
			}
			_mDevice.unlock();
			
		}
		isConnected = true;
	}
}
AgilorHandler::~AgilorHandler()
{
	if (isConnected)
	{
		Agcn_Disconnect(this->serverName.data());
		Agcn_RemoveNode(this->serverName.data());
		Agcn_Cleanup();
		isConnected = false;
	}

	for (vector<string>::iterator it = _devices.begin(); it != _devices.end(); it++)
	{
		aci::md_UnregisterDevice(*it, NULL);
		it->erase();
	}
}

void AgilorHandler::ping() {
	// Your implementation goes here
	printf("real agilor ping\n");
}


void AgilorHandler::connAllDevices()
{
}


bool AgilorHandler::isExistDevice(string name)
{

	_mDevice.lock();
	bool r = find(_devices.begin(), _devices.end(), name) != _devices.end();
	_mDevice.unlock();
	return r;
}


bool  AgilorHandler::AddDevice(const DEVICE& device)
{
	cout << "trigger: insert " << device.name << endl;
	if (isExistDevice(device.name)) return false;

	ACI_TAG_NODE tag;
	init(tag);
	strcpy(tag.sval, "DEFAULT");
	strcpy(tag.sourceserver, device.name.data());
	strcpy(tag.sourcegroup, device.name.data());
	strcpy(tag.sourcetag, device.name.data());
	string name = "%%%%" + device.name + "%%%%";
	strcpy(tag.name, name.data());
	
	if (!aci::addNewTag(this->serverName, tag, false))
	{
		long r = 0;
		if (!(r = DRTDB_RegisterDevice(LOCALHOST, RTDB_PORT, const_cast<LPSTR>(device.name.data()))))

		//if (!(r=aci::registerDevice(LOCALHOST, RTDB_PORT, device.name)))
		{
			cout << "trigger: insert  " << device.name<< " registerDevice result:"<<r << endl;
			_mDevice.lock();
			_devices.push_back(device.name);
			_mDevice.unlock();
			
			cout << "trigger: insert " << device.name <<" end"<< endl;
			return true;
		}
		cout << "trigger: insert  " <<device.name<< " registerDevice result:" <<r<< endl;
	}
	cout << "trigger: insert " << device.name << " end" << endl;
	return false;
}

bool AgilorHandler::DeleteDevice(const std::string& deviceName)
{
	_mDevice.lock();
	long hwnd = Agpt_QueryTagsbyDevice(this->serverName.data(), deviceName.data());

	long tagId = 0;
	_TCHAR tagName[C_TAGNAME_LEN];

	while (Agpt_EnumTagName(hwnd, &tagId, tagName))
	{
		TAG_INFO info;
		string name(tagName);
		Agpt_GetTagInfo((this->serverName + "." + name).data(), &info);

		ACI_TAG_NODE node = AgilorUtils::node(info);

		node.scan = 0;
		//Agpt_AddNewTag(this->serverName.data(), &node, true);
		aci::removeTag(this->serverName, info.TagID);
	}

	for (vector<string>::iterator it = _devices.begin(); it != _devices.end(); it++)
	{
		if (*it == deviceName)
			_devices.erase(it++);
	}

	_mDevice.unlock();

	return true;
}

int64_t AgilorHandler::QueryDeviceInfo() {
	return aci::queryDeviceInfo(this->serverName);
}

void AgilorHandler::EnumDeviceInfo(DEVICE& _return, const int64_t hRecordset) {


	DEV_INFO info;
	long id;
	if (Agpt_EnumDeviceInfo((long)hRecordset, &id, &info))
	{
		AgilorUtils::parse(info, _return);
		_return.id = 1;
	}
	else
	{
		_return.id = -1;
	}
}

bool AgilorHandler::ModifyDevice(const DEVICE& device)
{
	return true;
}



void AgilorHandler::GetAllDevices(std::vector<DEVICE> & _return)
{
	long hwnd = QueryDeviceInfo();
	if (hwnd > 0)
	{
		DEVICE data;
		do
		{
			EnumDeviceInfo(data, hwnd);
			if (data.id < 0) break;
		} while (true);
	}
}

int64_t AgilorHandler::TagCountByDevice(const std::string& deviceName)
{
	long hwnd = aci::queryTagsbyDevice(this->serverName, deviceName);
	if (hwnd < 0) return 0;//此处应该抛出异常
	string name;
	long count = 0;
	while (aci::enumTagName(hwnd, NULL, name))
		count++;
	return count>0 ? count - 2 : 0;
}

int64_t AgilorHandler::QuerySnapshots(const std::string & tagNames, const int32_t tagCount) {
	// Your implementation goes here

	return (int64_t)Agda_QuerySnapshots(serverName.data(), tagNames.data(), tagCount);
}

void AgilorHandler::GetNextTagValue(TAGVAL& _return, const int64_t hRecordset, const bool isRemoved) {
	// Your implementation goes here
	ACI_TAGVAL info;
	BOOL r = Agda_GetNextTagValue(hRecordset, &info, isRemoved);
	AgilorUtils::parse(info, _return);

	if (r)_return.id = 1;
	else _return.id = 0;
}

int64_t AgilorHandler::SubscribeTags( const string & tagNames) {
	// Your implementation goes here

	return Agda_SubscribeTags(serverName.data(), tagNames.data(), tagNames.size());
}

void AgilorHandler::GetSubTagValue(TAGVAL& _return) {
	// Your implementation goes here
	
	ACI_TAGVAL val;
	Agda_GetSubTagValue(&val);
	AgilorUtils::parse(val, _return);
	
}

int64_t AgilorHandler::UnSubscribeTags(const std::string& tagNames) {
	// Your implementation goes here

	return Agda_UnSubscribeTags(serverName.data(), tagNames.data(), tagNames.size());

}

int64_t AgilorHandler::UnSubscribeAll() {
	// Your implementation goes here
	return Agda_UnSubscribeAll(serverName.data());
}

//int64_t AgilorHandler::SetTagValue(const std::string& fullName, const std::string& value, const bool isManual, const std::string& comment) {
//	// Your implementation goes here
//
//	return 0;
//}

int64_t AgilorHandler::QueryTagHistory(const std::string& tagName, const int64_t startTime, const int64_t endTime, const int64_t step) {
	// Your implementation goes here

	
	return aci::queryTagHistory(serverName, tagName, startTime, endTime, 0);

}

void AgilorHandler::GetAggregateValue(TAGVAL& _return, const int64_t hRecordset, const AGGREGATE::type ag, const bool isRemoved) {
	// Your implementation goes here

	ACI_TAGVAL val;

	Agda_GetAggregateValue((long)hRecordset, &val, (int)ag, isRemoved);
	AgilorUtils::parse(val, _return);
}





int64_t AgilorHandler::QueryTagsbyDevice(const std::string& deviceName) {
	// Your implementation goes here
	return Agpt_QueryTagsbyDevice(this->serverName.data(), deviceName.data());
}

void AgilorHandler::EnumTagName(std::string& _return, const int64_t hRecordset) {
	// Your implementation goes here
	//return Agpt_EnumTagName()

	long id = 0;
	char tagName[C_TAGNAME_LEN];
	Agpt_EnumTagName(hRecordset, &id, tagName);
	_return.append(tagName);
}

void AgilorHandler::GetTagNamebyId(std::string& _return, const int64_t tagId) {
	// Your implementation goes here
	char name[C_TAGNAME_LEN];
	Agpt_GetTagNamebyID(const_cast<LPSTR>(this->serverName.data()), tagId, name);
	_return.append(name);
}

int64_t AgilorHandler::SetValue(const TAGVAL& value)
{

	TAG_VALUE_LOCAL tag;
	aci::parseValue(tag, value.value, value.type);
	tag.lTagID = value.id;
	tag.cTagType = value.type;
	tag.lTimeStamp = time(NULL);
	tag.nTagState = value.state;
	strcpy_s(tag.szTagSource, sizeof(tag.szTagSource), value.name.data());
	//memcpy(tag.szTagSource, value.name.data(), sizeof(tag.szTagSource));
	//long r = DRTDB_MD_SendNewValue((LPSTR)value.device.data(), tag);
	long r = aci::md_sendNewValue(value.device, tag);
	

	return r;
	//DRTDB_SendNewValue
}

int64_t AgilorHandler::RemoveTag(const int64_t tagId) {
	return Agpt_RemoveTag(this->serverName.data(), tagId);
	// Your implementation goes here
}

void AgilorHandler::GetTagInfo(TAGNODE& _return, const std::string& tagName){
	// Your implementation goes here
	TAG_INFO info;
		Agpt_GetTagInfo((this->serverName + "." + tagName).data(), &info);

		AgilorUtils::parse(info, _return);
}

int64_t AgilorHandler::AddNewTag(const TAGNODE& node, const bool isOverwrite) {
	// Your implementation goes here
	printf("AddNewTag\n");

	ACI_TAG_NODE n = AgilorUtils::parse(node);

	int i = Agpt_AddNewTag(serverName.data(), &n, isOverwrite);
	if (!i)
	{
		TAG_VALUE_LOCAL value;
		aci::parseValue(value, node.value, node.type);
		value.lTimeStamp = time(NULL);
		value.cTagType = node.type;
		//value.nTagState = 8208;
		memcpy(value.szTagSource, node.sourceTag.data(), sizeof(value.szTagSource));
		int w = aci::md_sendNewValue(node.deviceName, value, true);
	}
	return i;

}


void AgilorHandler::EnumNextTag(TAGNODE& _return, const int64_t hwnd)
{
	string name;
	long id;

	_return.id = -1;

	while (aci::enumTagName(hwnd, &id, name))
	{
		TAG_INFO info;
		//aci::getTagInfo(this->serverName, name, &info);
		Agpt_GetTagInfo((this->serverName + "." + name).data(), &info);

		if (info.IOState != 16 && info.IOState != 0)
		{
			AgilorUtils::parse(info, _return);
			_return.id = id;
			break;
		}
		name.clear();
	}
}

int64_t AgilorHandler::SetValues(const std::vector<TAGVAL>& values)
{
	if (values.empty()) return 0;
	LPTAGVALUELOCAL locals = new TAG_VALUE_LOCAL[values.size()];
	int i = 0;
	for (std::vector<TAGVAL>::const_iterator it = values.begin(); it != values.end(); it++)
	{
		locals[i] = AgilorUtils::parse_local(*it);
	}
	return aci::md_sendNewValues(values.at(0).device, locals, values.size());
	
}
