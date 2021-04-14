#include "windows.h"
#include <stdio.h>
#include <str.h>
#define println(x) printf("%s\n", x)

int main(int argc, char const* argv[]) {
	char* test = new char[40];
	println(str("float:" + -0.99 + " " + Str::dump(test, 40)));
	return 0;
}
