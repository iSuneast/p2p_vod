#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

class Log {
public:
	~Log() {
		Exit();
	}

	static std::string GetCurrentTimeTag() {
		time_t t = time(nullptr);
		tm data;
		tm *now = &data;
		now = localtime(&t);
		char buf[30];
		sprintf(buf, "%04d.%02d.%02d_%02d.%02d.%02d",
			now->tm_year + 1900,
			now->tm_mon + 1,
			now->tm_mday,
			now->tm_hour,
			now->tm_min,
			now->tm_sec);
		return buf;
	}

	static void o(const char *format, ...) {
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		va_end(argptr);
	}

	static void dbg(bool show, const char *format, ...) {
		if (!show) {
			return;
		}
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
	}

	static void log(const char *format, ...) {
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stdout, format, argptr);
		vfprintf(log_file_, format, argptr);
		va_end(argptr);
	}

	static void f(const char *format, ...) {
		va_list argptr;
		va_start(argptr, format);
		vfprintf(log_file_, format, argptr);
		va_end(argptr);
	}

	static void err(const char *format, ...) {
		va_list argptr;
		va_start(argptr, format);
		vfprintf(stderr, format, argptr);
		va_end(argptr);
	}

	static void SetLog(const char *file_name, const char *mode = "a+") {
		Log::o("# log file: %s\n", file_name);
		log_file_ = fopen(file_name, mode);
		if (log_file_ == nullptr) {
			log_file_ = stderr;
			err("# Unable to open log file: %s\n", file_name);
		}
	}

	static FILE* log_file() {
		return log_file_;
	}

private:
	static void Exit() {
		fclose(log_file_);
	}

	static FILE *log_file_;
};

FILE *Log::log_file_ = stderr;
