#define _CRT_SECURE_NO_WARNINGS
#include "logger.h"

using namespace kuril::utility;

#include <time.h>//get time
#include <string.h>//memset
#include <stdexcept>//throw
#include <stdarg.h>//可变参数 vcsnprintf

//static都要先初始化
const char* Logger::s_level[LEVEL_COUT] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

Logger* Logger::m_instance = NULL;
//析构和构造函数可以初始化和销毁变量
Logger::Logger()  : m_level(DEBUG), m_max(0), m_len(0)
{
}

Logger::~Logger()
{
	close();
}
//单例模式的关键
Logger* Logger::instance()
{
	if (m_instance == NULL)
	{
		m_instance = new Logger();
	}
	return m_instance;
}

void Logger::open(const string& filename)
{
	m_filename = filename;
	m_fout.open(filename, ios::app);
	if (m_fout.fail())
	{
		throw logic_error("open file failed" + filename);
	}
	//获取原本内容的长度
	m_fout.seekp(0, ios::end);//定位读指针
	m_len = m_fout.tellp();//读取指针位置获取长度
}

void Logger::close()
{
	m_fout.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
	if (m_level > level)//枚举类型enum还支持直接比较的吗？//越靠前越大？
	{
		return;
	}

	if (m_fout.fail())
	{
		throw logic_error("open file failed" + m_filename);
	}

	time_t ticks = time(NULL);
	struct tm* ptm = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptm);

	const char* fmt = "%s %s %s: %d ";//这里可否优化成fprintf（）?
	int size = snprintf(NULL, 0, fmt, timestamp, s_level[level], file, line);
	if (size > 0)
	{
		char* buffer = new char[size + 1];
		memset(buffer, 0, size + 1);
		snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
		buffer[size] = 0;
		cout << buffer << endl;
		m_fout << buffer;
		m_len += size;
		delete buffer;
	}
	//可变参数格式化，没有这个format输入不进来
	va_list arg_ptr;
	va_start(arg_ptr, format);
	size = vsnprintf(NULL, 0, format, arg_ptr); 
	va_end(arg_ptr);
	if (size > 0)
	{
		char* content = new char[size + 1];
		va_start(arg_ptr, format);
		vsnprintf(content, size + 1, format, arg_ptr);
		va_end(arg_ptr);
		cout << content << endl;
		m_fout << content;
		m_len += size;
		delete content;
	}
	m_fout << '\n';
	m_fout.flush();

	if (m_len >= m_max && m_max > 0)
	{
		rotate();
	}
}

//日志翻滚
void Logger::rotate() 
{
	//rename要用所以先关闭
	close();
	time_t ticks = time(NULL);
	struct tm* pmt = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", pmt);
	string filename = m_filename + timestamp + ".log";//需要后缀名否则会报错？
	//获取路径
	if (rename(m_filename.c_str(), filename.c_str()) != 0)
	{
		cout << "error" << endl;
	}
	open(m_filename);
}