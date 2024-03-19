#define _CRT_SECURE_NO_WARNINGS
#include "logger.h"

using namespace kuril::utility;

#include <time.h>//get time
#include <string.h>//memset
#include <stdexcept>//throw
#include <stdarg.h>//�ɱ���� vcsnprintf

//static��Ҫ�ȳ�ʼ��
const char* Logger::s_level[LEVEL_COUT] = {
	"DEBUG",
	"INFO",
	"WARN",
	"ERROR",
	"FATAL"
};

Logger* Logger::m_instance = NULL;
//�����͹��캯�����Գ�ʼ�������ٱ���
Logger::Logger()  : m_level(DEBUG), m_max(0), m_len(0)
{
}

Logger::~Logger()
{
	close();
}
//����ģʽ�Ĺؼ�
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
	//��ȡԭ�����ݵĳ���
	m_fout.seekp(0, ios::end);//��λ��ָ��
	m_len = m_fout.tellp();//��ȡָ��λ�û�ȡ����
}

void Logger::close()
{
	m_fout.close();
}

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
	if (m_level > level)//ö������enum��֧��ֱ�ӱȽϵ���//Խ��ǰԽ��
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

	const char* fmt = "%s %s %s: %d ";//����ɷ��Ż���fprintf����?
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
	//�ɱ������ʽ����û�����format���벻����
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

//��־����
void Logger::rotate() 
{
	//renameҪ�������ȹر�
	close();
	time_t ticks = time(NULL);
	struct tm* pmt = localtime(&ticks);
	char timestamp[32];
	memset(timestamp, 0, sizeof(timestamp));
	strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", pmt);
	string filename = m_filename + timestamp + ".log";//��Ҫ��׺������ᱨ��
	//��ȡ·��
	if (rename(m_filename.c_str(), filename.c_str()) != 0)
	{
		cout << "error" << endl;
	}
	open(m_filename);
}