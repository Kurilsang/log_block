#include <iostream>

#include "logger.h"


using namespace kuril::utility;
using namespace std;

int main() {
	Logger::instance()->open("./test.log");
	//Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "HELLO");
	//Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "%s %s", "name", "jack");
	Logger::instance()->level(Logger::DEBUG);
	Logger::instance()->max(1000);//̫С���ƺ��ᵼ����־����ģ�������
	debug("%s %s", "n", "b");
	warn("%s %s", "n", "b");
	info("%s %s", "n", "b");
	fatal("%s %s", "n", "b");
	error("%s %s", "n", "b");

	return 0;
}