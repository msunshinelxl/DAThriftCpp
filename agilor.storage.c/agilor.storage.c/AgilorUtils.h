#ifndef AGILORUTILS_H
#define AGILORUTILS_H

#include <Windows.h>
#include "ACI_H.H"
#include "inter_types.h"
#include <boost\type_traits.hpp>
#include <boost\typeof\typeof.hpp>
#include <boost\lexical_cast.hpp>
#include <sstream>
#include "ACI.h"

using namespace std;
using namespace boost;


class AgilorUtils
{
private :
	static stringstream* str_stream;

public:
	AgilorUtils();
	~AgilorUtils();
	static ACI_TAG_NODE node(const TAG_INFO& info);
	static ACI_TAGVAL parse(const TAGVAL& info);
	static ACI_TAG_NODE parse(const TAGNODE& info);
	static TAG_VALUE_LOCAL parse_local(const TAGVAL& info);
	
	static DEVICE parse(const DEV_INFO& info);
	static void parse(const TAG_INFO &aci, TAGNODE & info);
	static void parse(const DEV_INFO&aci, DEVICE& info);
	static void parse(const  ACI_TAGVAL&aci, TAGVAL& info);
	static float toFloat(string value);
	static bool toBool(string value);


	
};


#endif