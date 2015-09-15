struct SERVER
{
  1:string name,
  2:string addr,
  3:string userName,
  4:string password,
  5:bool isConnected,
}
struct DEVICE
{
  1:string name,
  2:bool isOnline,
  3:i64 id,
  4:i64 hwnd
}

struct TAGVAL
{
   1:string name,
   2:i64 timestamp,
   3:i64 state,
   4:i16 type,
   5:string value,
   6:i64 hwnd
}


struct TAGNODE
{
  1:string name,
  2:string desc,
  3:string engUnit,
  4:i64 id,
  5:i16 type,
  6:i16 IOState,
  7:double typicalVal,
  8:string value,
  9:string enumDesc,
  10:i64 timestamp,
  11:i64 state,
  12:string deviceName,
  13:string groupName,
  14:string sourceTag,
  15:double upperLimit,
  16:double lowerLimit,
  17:i16 pushreference,
  18:i16 ruleReference,
  19:i64 exceptionMin,
  20:i64 exceptionMax,
  21:double exceptionDev,
  22:i16 alarmType,
  23:i16 alarmState,
  24:double alarmHi,
  25:double alarmLo,
  26:double alarmHiHi,
  27:double alarmLolo,
  28:i16 hiPriority,
  29:i16 loPriority,
  30:i16 hihiPriority,
  31:i16 loloPriority,
  32:bool isArchived,
  33:bool isCompressed,
  34:bool interMethod,
  35:i64 hisIndex,
  36:i64 compressMin,
  37:i64 compressMax,
  38:double lastValue,
  39:i64 creationDate,
 
}


enum AGGREGATE
{
  AF_SUMMARY=0,
  AF_MINIMUM=1,
  AF_MAXIMUM=2,
  AF_AVERAGE=3,
  AF_COUNT=4,
  AF_SUMMARY_FOR_CONTINOUS=5,
  AF_AVERAGE_FOR_CONTINOUS=6,
}


service Agilor{
 void ping();
 bool EnumNodeInfo(1:i64 serverId, 2:SERVER info);
 i64  QuerySnapshots(1:string serverName,2:string tagNames,3:i32 tagCount);
 TAGVAL GetNextTagValue(1:i64 hRecordset,3:bool isRemoved);
 i64 SubscribeTags(1:string serverName,2:list<string> tagNames);
 bool GetSubTagValue(1:TAGVAL tagval);
 i64 UnSubscribeTags(1:string serverName,2:list<string> tagNames);
 i64 UnSubscribeAll(1:string serverName);
 i64 SetTagValue(1:string fullName,2:string value,3:bool isManual,4:string comment);
 i64 QueryTagHistory(1:string fullName,2:i64 startTime,3:i64 endTime,4:i64 step);
 i64 GetAggregateValue(1:i64 hRecordset,2:TAGVAL tagval,3:AGGREGATE ag,4:bool isRemoved);
 i64 QueryDeviceInfo(1:string serverName);
 DEVICE EnumDeviceInfo(1:i64 hRecordset);
 i64 QueryTagsbyDevice(1:string serverName,2:string deviceName);
 string EnumTagName(1:i64 hRecordset);
 bool GetTagNamebyId(1:i64 tagId,2:string tagName);
 i64 RemoveTag(1:string serverName,2:i64 tagId);
 TAGNODE GetTagInfo(1:string fullName);
 i64 AddNewTag(1:string serverName,2:TAGNODE node,3:bool isOverwrite);
}




