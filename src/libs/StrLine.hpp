#pragma once

struct StrLine {
	char dat[64 * 1024];
	size_t pos;

	StrLine() {
		pos = 0;
	}
};