#pragma once

#include <iostream>
#include <fstream>

using namespace std;

namespace kuril {
	namespace utility {
#define debug(format, ...) \
	Logger::instance()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define info(format, ...) \
	Logger::instance()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define warn(format, ...) \
	Logger::instance()->log(Logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define error(format, ...) \
	Logger::instance()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define fatal(format, ...) \
	Logger::instance()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

		class Logger
		{
		public:
			enum Level
			{
				DEBUG = 0,
				INFO,
				WARN,
				ERROR,
				FATAL,
				LEVEL_COUT
			};
			static Logger* instance();//单例模式的关键
			void open(const string& filename);
			void close();
			void log(Level level, const char* file, int line, const char* format, ...);
			void level(Level level)
			{
				m_level = level;
			};
			void max(int bytes)
			{
				m_max = bytes;
			}

		private:
			Logger();
			~Logger();
			void rotate();
		private:
			//私有成员存在的意义？
			string m_filename;
			ofstream m_fout;
			Level m_level;
			static const char* s_level[LEVEL_COUT];//有什么用? 为了更方便地记录
			static Logger* m_instance; //创建那么对应要存储
			int m_max;
			int m_len;
		};


	}
}