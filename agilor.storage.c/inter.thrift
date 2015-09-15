struct DEVICE
{
  1:string name,
  //2:bool isOnline,
  2:i64 id,
  //2:i64 hwnd
}

struct TAGVAL
{
   1:string name,
   2:i64 timestamp,
   3:i64 state,
   4:i16 type,
   5:string value,
   6:i64 hwnd,
   7:i64 id,
   8:string device
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
//连接相关
 void ping();
 



 //设备相关
 bool AddDevice(1:DEVICE device);
 bool DeleteDevice(1:string deviceName);
 bool ModifyDevice(1:DEVICE device);
 i64 QueryDeviceInfo();
 DEVICE EnumDeviceInfo(1:i64 hRecordset);
 list<DEVICE> GetAllDevices();
 i64 TagCountByDevice(1:string deviceName);
 
//点相关
 i64  QuerySnapshots(1:string tagNames,2:i32 tagCount); //获取点集合
 TAGVAL GetNextTagValue(1:i64 hRecordset,2:bool isRemoved);//根据结果集句柄获取点值
 i64 QueryTagHistory(1:string tagName,2:i64 startTime,3:i64 endTime,4:i64 step);//tagName实际为fullName,后台必须进行组合 
 TAGVAL GetAggregateValue(1:i64 hRecordset,2:AGGREGATE ag,3:bool isRemoved);
 i64 QueryTagsbyDevice(1:string deviceName);
 string EnumTagName(1:i64 hRecordset);
 string GetTagNamebyId(1:i64 tagId);
 //i64 SetTagValue(1:string tagName,2:string value,3:bool isManual,4:string comment);//tagName 实际为fullName,后台必须进行组合
 i64 SetValue(1:TAGVAL value);
 i64 SetValues(1:list<TAGVAL> values);
 
 i64 RemoveTag(1:i64 tagId);
 TAGNODE GetTagInfo(1:string tagName);//tagName 实际为fullName，后台必须进行组合
 i64 AddNewTag(1:TAGNODE node,2:bool isOverwrite);
 TAGNODE EnumNextTag(1:i64 hwnd);

 
 //订阅点相关
 i64 SubscribeTags(1:string tagNames);//订阅点集合
 TAGVAL GetSubTagValue();//获取订阅的点值
 i64 UnSubscribeTags(1:string tagNames);//取消点订阅
 i64 UnSubscribeAll();//取消所有点的订阅

 
 
 
 
 
 
 
}




