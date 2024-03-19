#include <iostream>

#include "logger.h"


using namespace kuril::utility;
using namespace std;

int main() {
	Logger::instance()->open("./test.log");
	//Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "HELLO");
	//Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, "%s %s", "name", "jack");
	Logger::instance()->level(Logger::DEBUG);
	Logger::instance()->max(1000);//太小了似乎会导致日志翻滚模块出问题
	debug("%s %s", "n", "b");
	warn("%s %s", "n", "b");
	info("%s %s", "n", "b");
	fatal("%s %s", "n", "b");
	error("%s %s", "n", "b");

	return 0;
}