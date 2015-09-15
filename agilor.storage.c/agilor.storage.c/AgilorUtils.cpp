#include "AgilorUtils.h"

stringstream* AgilorUtils::str_stream = new stringstream();
AgilorUtils::AgilorUtils(){


}
AgilorUtils::~AgilorUtils(){

}
//ACI_SERVER_INFO AgilorUtils::parse(const SERVER& info)
//{
//	ACI_SERVER_INFO aci;
//	memcpy(aci.szServerAddr, info.addr.data(), sizeof(aci.szServerAddr));
//	memcpy(aci.szServerName, info.name.data(), sizeof(aci.szServerName));
//	memcpy(aci.szUserName, info.userName.data(), sizeof(aci.szUserName));
//	memcpy(aci.szPassword, info.password.data(), sizeof(aci.szPassword));
//	aci.bIsConnected = info.isConnected;
//	return aci;
//}

ACI_TAG_NODE AgilorUtils::node(const TAG_INFO& info)
{
	ACI_TAG_NODE node;
	node.alarmhihi = info.AlarmHiHi;
	node.alarmlolo = info.AlarmLoLo;
	memcpy(node.name, info.TagName, sizeof(node.name));
	memcpy(node.descriptor, info.TagDesc, sizeof(node.descriptor));
	memcpy(node.engunits, info.EngUnit, sizeof(node.engunits));
	node.pointid = info.TagID;
	node.pointtype = info.TagType;
	node.scan = info.IOState;
	node.reserved1 = info.Reserved1;

	node.typicalvalue = info.TypicalVal;

	switch (info.TagType)
	{
	case 'R':node.rval = info.rval; break;
	case 'S':memcpy(node.sval, info.sval, sizeof(node.sval)); break;
	case 'B':node.bval = info.bval; break;
	case 'L':node.lval = info.lval; break;
	default:
		break;
	}

	memcpy(node.enumdesc, info.EnumDesc, sizeof(node.enumdesc));
	node.timedate = info.Timestamp;
	node.istat = info.TagState;
	memcpy(node.sourceserver, info.DeviceName, sizeof(node.sourceserver));
	memcpy(node.sourcegroup, info.GroupName, sizeof(node.sourcegroup));
	memcpy(node.sourcetag, info.SourceTag, sizeof(node.sourcetag));

	node.upperlimit = info.UpperLimit;
	node.lowerlimit = info.LowerLimit;
	node.pushref1 = info.PushReference;
	node.ruleref1 = info.RuleReference;
	node.excmax = info.ExceptionMax;
	node.excmin = info.ExceptionMin;
	node.excdev = info.ExceptionDev;
	node.alarmtype = info.AlarmType;
	node.alarmstate = info.AlarmState;
	node.alarmmin = info.AlarmLo;
	node.alarmmax = info.AlarmHi;
	node.alarmhihi = info.AlarmHiHi;
	node.alarmlolo = info.AlarmLoLo;
	node.hipriority = info.HiPriority;
	node.lopriority = info.LoLoPriority;
	node.hihipriority = info.HiHiPriority;
	node.lolopriority = info.LoLoPriority;
	node.archiving = info.IsArchived;
	node.compressing = info.IsCompressed;
	node.step = info.InterMethod;
	node.reserved2 = info.Reserved2;
	node.hisidx = info.HisIndex;
	node.compmin = info.CompressMin;
	node.compmax = info.CompressMax;
	node.compdev = info.CompressDev;
	node.lasttime = info.LastTimestamp;
	node.lastval = info.LastValue;
	node.creationdate = info.CreationDate;


	return node;

}

ACI_TAGVAL AgilorUtils::parse(const TAGVAL& info)
{
	ACI_TAGVAL aci;
	memcpy(aci.TagName, info.name.data(), sizeof(aci.TagName));
	//if (info.type == 'R') aci.

	//此处为测试值，需重新转化
	aci.rval = 110.543;
	aci.TagState = info.state;
	aci.TagType = info.type;
	aci.Timestamp = info.timestamp;
	return aci;
}

TAG_VALUE_LOCAL AgilorUtils::parse_local(const TAGVAL& info)
{
	TAG_VALUE_LOCAL re;
	strcpy_s(re.szTagSource, sizeof(re.szTagSource), info.name.data());

	//aci::parseValue()

	//此处为测试值，需重新转化
	aci::parseValue(re, info.value, info.type);

	re.nTagState = info.state;
	re.cTagType = info.type;
	re.lTimeStamp = info.timestamp;
	return re;
}

ACI_TAG_NODE AgilorUtils::parse(const TAGNODE& info)
{



	ACI_TAG_NODE node;
	aci::init(node);

	node.alarmhihi = info.alarmHiHi;
	node.alarmlolo = info.alarmLolo;
	node.alarmstate = info.alarmState;
	node.alarmtype = info.alarmType;
	node.compmax = info.compressMax;
	node.compmin = info.compressMin;
	node.creationdate = info.creationDate;
	memcpy(node.descriptor, info.desc.data(), boost::extent<BOOST_TYPEOF(node.descriptor)>::value);
	memcpy(node.engunits, info.engUnit.data(), boost::extent<BOOST_TYPEOF(node.engunits)>::value);
	memcpy(node.enumdesc, info.enumDesc.data(), boost::extent<BOOST_TYPEOF(node.enumdesc)>::value);
	node.excdev = info.exceptionDev;
	node.excmax = info.exceptionMax;
	node.excmin = info.exceptionMin;
	node.hihipriority = info.hihiPriority;
	node.hipriority = info.hiPriority;
	node.hisidx = info.hisIndex;
	node.istat = info.state;
	node.pointtype = info.type;
	node.scan = info.IOState;

	switch (info.type)
	{
	case 'R':node.rval = boost::lexical_cast<float>(info.value); break;
	case 'S':memcpy(node.sval, info.value.data(), sizeof(node.sval)); break;
	case 'B':node.bval = boost::lexical_cast<bool>(info.value); break;
	case 'L':node.lval = boost::lexical_cast<long>(info.value); break;
	default:
		break;
	}
	memcpy(node.name, info.name.data(), boost::extent<BOOST_TYPEOF(node.name)>::value);
	memcpy(node.sourceserver, info.deviceName.data(), boost::extent<BOOST_TYPEOF(node.sourceserver)>::value);
	memcpy(node.sourcegroup, info.groupName.data(), boost::extent<BOOST_TYPEOF(node.sourcegroup)>::value);
	memcpy(node.sourcetag, info.sourceTag.data(), boost::extent<BOOST_TYPEOF(node.sourcetag)>::value);
	return node;
}


void AgilorUtils::parse(const DEV_INFO&aci, DEVICE& info){

	info.name = aci.szDeviceName;

}


void AgilorUtils::parse(const  ACI_TAGVAL&aci, TAGVAL& info)
{
	info.name = aci.TagName;
	info.state = aci.TagState;
	info.timestamp = aci.Timestamp;
	info.type = aci.TagType;
	switch (aci.TagType)
	{
	case 'R':info.value = to_string(aci.rval); break;
	case 'B':info.value = to_string(aci.bval); break;
	case 'S':info.value = aci.sval; break;
	default:
		break;
	}
}





void AgilorUtils::parse(const TAG_INFO& aci, TAGNODE& info)
{
	info.alarmHi = aci.AlarmHi;
	info.alarmHiHi = aci.AlarmHiHi;
	info.alarmLo = aci.AlarmLo;
	info.alarmLolo = aci.AlarmLoLo;
	info.alarmState = aci.AlarmState;
	info.alarmType = aci.AlarmType;
	info.compressMin = aci.CompressMin;
	info.compressMax = aci.CompressMax;
	info.creationDate = aci.CreationDate;
	info.desc = aci.TagDesc;
	info.deviceName = aci.DeviceName;
	info.engUnit = aci.EngUnit;
	info.enumDesc = aci.EnumDesc;
	info.exceptionDev = aci.ExceptionDev;
	info.exceptionMax = aci.ExceptionMax;
	info.exceptionMin = aci.ExceptionMin;
	info.groupName = aci.GroupName;
	info.hihiPriority = aci.HiHiPriority;
	info.hiPriority = aci.HiPriority;
	info.hisIndex = aci.HisIndex;
	info.id = aci.TagID;
	info.interMethod = aci.InterMethod;
	info.IOState = aci.IOState;
	info.isArchived = aci.IsArchived;
	info.isCompressed = aci.IsCompressed;
	info.lastValue = aci.LastValue;
	info.loloPriority = aci.LoLoPriority;
	info.loPriority = aci.LoPriority;
	info.lowerLimit = aci.LowerLimit;
	info.name = aci.TagName;
	info.pushreference = aci.PushReference;
	info.ruleReference = aci.RuleReference;
	info.sourceTag = aci.SourceTag;
	info.state = aci.TagState;
	info.timestamp = aci.Timestamp;
	info.type = aci.TagType;
	info.typicalVal = aci.TypicalVal;
	info.upperLimit = aci.UpperLimit;
	switch (aci.TagType)
	{
	case 'R':info.value = to_string(aci.rval); break;
	case 'B':info.value = to_string(aci.bval); break;
	case 'S':info.value = aci.sval; break;
	default:
		break;
	}




}


float AgilorUtils::toFloat(string value)
{
	str_stream->clear();
	float result;
	(*str_stream) << value;
	(*str_stream) >> result;

	return result;

}

bool AgilorUtils::toBool(string value)
{
	str_stream->clear();
	bool result;

	(*str_stream) << value;
	(*str_stream) >> result;

	return result;
}