#pragma once

#include "StormLib.h"

struct Script {
	char* dat;
	size_t pos;
	DWORD bytes;

	Script(size_t sz) {
		dat = new char[sz];
		pos = 0;
		bytes = 1;
	}

	~Script() {
		delete [] dat;
	}

	void concat(const char* line) {
		size_t len = strlen(line);

		memcpy(dat + pos, line, len);
		pos += len;
		dat[pos] = 0;
	}
};