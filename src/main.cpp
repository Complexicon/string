#include "windows.h"
#include <stdio.h>
#include <str.h>
#define println(x) printf("%s\n", x)

int main(int argc, char const* argv[]) {
	println(str(7.6));
	return 0;
}
