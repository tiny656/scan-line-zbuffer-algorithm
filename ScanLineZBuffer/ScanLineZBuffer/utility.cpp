#include "utility.h"

/**
* unsigned short 转换为 string
* @todo implement caching for better performance
* @param WORD(unsigned short) value, 传入要进行转换的值.
* @return string, 返回转换后的值.
*/
std::string uInt2String(unsigned short value) {
	std::stringstream ss;
	ss << value;
	std::string ret;
	ss >> ret;
	return ret;
}

/**
* 返回当前时间
* @todo
* @param, empty.
* @return string, 返回格式化当前时间.
*/
std::string logTime() {
	SYSTEMTIME systime;
	GetLocalTime(&systime);
	std::string ret = "";
	ret += uInt2String(systime.wYear) + "-";
	ret += uInt2String(systime.wMonth) + "-";
	ret += uInt2String(systime.wDay) + " ";
	ret += uInt2String(systime.wHour) + ":";
	ret += uInt2String(systime.wMinute) + ":";
	ret += uInt2String(systime.wSecond) + ".";
	ret += uInt2String(systime.wMilliseconds) + " ";
	return ret;
}

/**
* 对浮点数val四舍五入
* @todo
* @param double, 要进行四舍五入的值
* @return int, 返回四舍五入后结果
*/
int convertToInt(double val)
{
	return (int)(val + 0.5);
}
