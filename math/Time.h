#ifndef TIME_H
#define TIME_H

#include <cstdint>
#include <chrono>

class Time {

private:

	int64_t _ms;

	Time(const int64_t ms) : _ms(ms) {;}

public:

	/** empty ctor */
	Time() : _ms(0) {;}

	static Time fromSec(const float sec) {return Time(sec*1000);}

	static Time fromMSec(const int64_t msec) {return Time(msec);}

	/** time since 01.01.1970 */
	static Time current() {return Time(std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1));}

	/** time relative to app-start. needed for float to work */
	static Time runtime() {return current() - boot();}

	static Time boot() {
		static Time start = Time::current();
		return start;
	}


	/** convert to seconds */
	float seconds() const {return float(_ms) / 1000.0f;}

	/** conver to milliseconds */
	int64_t milliseconds() const {return _ms;}


	Time operator - (const Time other) const {
		return Time(this->_ms - other._ms);
	}

	Time operator + (const Time other) const {
		return Time(this->_ms + other._ms);
	}

	bool operator < (const Time other) const {
		return this->_ms < other._ms;
	}

	bool operator > (const Time other) const {
		return this->_ms > other._ms;
	}

};


#endif // TIME_H
