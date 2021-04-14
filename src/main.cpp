#include "windows.h"
#include <stdio.h>
#include <str.h>
inline void println(const char* msg) { printf("%s\n", msg); }

int main(int argc, char const* argv[]) {
	Str buf = Str::allocBuf(16);
	((char*)buf)[0] = 'a';
	println(Str::dump(buf, 16));
	buf.truncate();
	println(Str::dump(buf, 16));
	return 0;
}
