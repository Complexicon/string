#ifndef CPP_APPROVED_BY_HALLMANN
#define CPP_APPROVED_BY_HALLMANN

//// String Class by cmplx ////

#ifndef STRNAME
#define STRNAME Str
#endif

#define str(x) (cmplx::String() + x).c_str()
#define eternalstr(x) (cmplx::String((cmplx::String() + x), true)).c_str()
#define strn(x) (cmplx::String() + x)

#ifdef _WIN64

typedef unsigned long long unsigned_t;
typedef long long integer_t;

#else

typedef unsigned long unsigned_t;
typedef long integer_t;

#endif

#define op_append(type) \
	void operator+=(type val) { operator+=(String(val)); }
#define op_concat(type) \
	String operator+(type val) { return operator+(String(val)); }

#define op_assign(type) \
	void operator=(type val) { operator=(String(val)); }

namespace cmplx {

inline char* strcpy(char* dest, const char* src) {
	while(*src != '\0') *dest++ = *src++;
	return &(*dest = '\0');
}

inline unsigned_t strlen(const char* str) {
	unsigned_t len = 0;
	while(*(str + len) != '\0') len++;
	return len;
}

// concats two strings
inline char* strcat(const char* src, const char* concat, bool freeSrc = false, bool freeConc = false) {
	char* concatted = new char[strlen(src) + strlen(concat) + 1];
	strcpy(strcpy(concatted, src), concat);
	if(freeSrc) delete[] src;
	if(freeConc) delete[] concat;
	return concatted;
}

// remember to delete[] !!!
inline char* itoa(integer_t value, unsigned_t minlen = 0, unsigned_t radix = 10, bool checkSign = true) {
	char* buf = new char[32];
	buf[31] = 0;
	int i = 30;
	unsigned_t abs = value < 0 && checkSign ? (unsigned_t)-value : (unsigned_t)value;
	for(; abs && i; abs /= radix) buf[i--] = "0123456789ABCDEF"[abs % radix];
	while(i > 30 - minlen) buf[i--] = '0';
	if(checkSign && value < 0) buf[i] = '-';
	if(value == 0) buf[i] = '0';
	char* destBuf = new char[strlen(&buf[i + (value > 0)]) + 1];
	strcpy(destBuf, &buf[i + (value > 0)]);
	delete[] buf;
	return destBuf;
}

// remember to delete[] !!!
inline char* ftoa(double num) {
	int x = (int)((num - int(num)) * 10000);
	char* joined = strcat(strcat(itoa((int)(x < 0 ? -num : num)), ".", true), itoa(x < 0 ? -x : x, 4), true, true);
	return x < 0 ? strcat("-", joined, false, true) : joined;
}

inline void strshift(char* toShift, char rangeTop, char rangeLower, char toAdd) {
	for(; *toShift != '\0'; toShift++) *toShift += toAdd * ((*toShift) <= rangeTop && (*toShift) >= rangeLower);
}

inline bool strcmp(const char* first, const char* second) {
	bool eq = true;
	for(; eq && *first != '\0' || *second != '\0'; first++, second++) eq = *first == *second;
	return eq;
}

class String {

  private:
	char* strPtr;

	void init(const char* str) {
		if(strPtr) delete[] strPtr;
		strPtr = new char[strlen(str) + 1];
		strcpy(strPtr, str);
	}

	void defVal(const char* in, bool dnd = false, bool freeIn = false) {
		strPtr = 0;
		doNotDestroy = dnd;
		init(in);
		if(freeIn) delete[] in;
	}

	static void updateStr(char** pointer, char* newPointer) {
		delete[] * pointer;
		*pointer = newPointer;
	}

  public:
	~String() {
		if(!doNotDestroy) delete[] strPtr;
	};

	operator const char*() { return strPtr; } // implicit
	operator char*() { return strPtr; }		  // implicit
	const char* c_str() { return strPtr; }
	unsigned_t len() { return strlen(strPtr); }
	void toLower() { strshift(strPtr, 'Z', 'A', 32); }
	void toUpper() { strshift(strPtr, 'z', 'a', -32); }
	void replace(char what, char replace) { strshift(strPtr, what, what, replace - what); };

	void substring(unsigned begin) {
		if(begin < len()) init(String(strPtr + begin).strPtr);
	};

	void substring(unsigned_t begin, unsigned_t end) {
		if(begin < len() && end < len()) {
			String tmp(strPtr + begin);
			tmp[end] = 0;
			init(tmp.strPtr);
		}
	};

	unsigned_t lastIndexOf(char c) {
		unsigned_t len = 0;
		unsigned_t lastIndex = 0;
		while(*(strPtr + len) != '\0') {
			if(*(strPtr + len) == c) lastIndex = len;
			len++;
		}
		return lastIndex;
	}

	bool contains(const char* key) {
		const char* temp = strPtr;
		const char* cpr = key;
		do {
			if(*cpr != 0) cpr = !(*temp == *cpr++) ? key : cpr;
			else
				return true;
		} while(*temp++ != 0);
		return false;
	}

	// reallocate to remove trailing null characters
	void truncate() { init(String(strPtr)); }

	// statics

	static String dump(const char* buffer, unsigned_t length) {
		String result = "[";
		for(unsigned_t i = 0; i < length; i++) result += strn((void*)((unsigned char)buffer[i]) + ", ");
		result[result.len() - 2] = ']';
		return result;
	}

	static String allocBuf(unsigned_t size, char fill = 0) {
		String buf = "";
		delete[] buf.strPtr;
		buf.strPtr = new char[size + 1];
		buf.strPtr[size] = 0; // nullterminate buffer
		for(unsigned_t i = 0; i < size; i++) buf.strPtr[i] = fill;
		return buf;
	}

	// dont free stringpointer on destructor if true
	bool doNotDestroy;

	// base constructors

	String() { defVal(""); }
	String(const String& str) { defVal(str.strPtr); };
	String(const String& str, bool dnd) { defVal(str.strPtr, dnd); };

	// extended constructor

	String(const char* str, bool freeIn = false) { defVal(str, false, freeIn); }
	String(bool keepalive, bool really) { defVal("", keepalive == really /* duh */); }

	// implementations for various types - base for all

	String(integer_t num) { defVal(itoa(num), false, true); };
	String(int num) { defVal(itoa(num), false, true); };					// for old compilers
	String(unsigned num) { defVal(itoa(num, 0, 10, false), false, true); }; // for old compilers
	String(double num) { defVal(ftoa(num), false, true); }
	String(void* hex) { defVal(strcat("0x", itoa((unsigned_t)hex, 0, 16, false), false, true), false, true); }

	//-----------------------------------
	// operator= - assign -- top are base
	//-----------------------------------

	void operator=(const char* str) { init(str); }
	void operator=(const String& str) { init(str.strPtr); }

	op_assign(integer_t);
	op_assign(int);		 // for old compilers
	op_assign(unsigned); // for old compilers
	op_assign(void*);
	op_assign(double);

	//-----------------------------------
	// operator== - compare
	//-----------------------------------

	bool operator==(const char* str) { return strcmp(strPtr, str); }
	bool operator==(const String& str) { return strcmp(strPtr, str.strPtr); }

	//-----------------------------------
	// operator+= - append -- top are base
	//-----------------------------------

	void operator+=(const char* concat) { updateStr(&strPtr, strcat(strPtr, concat)); }
	void operator+=(const String& concat) { operator+=(concat.strPtr); }

	op_append(integer_t);
	op_append(int);		 // for old compilers
	op_append(unsigned); // for old compilers
	op_append(void*);
	op_append(double);

	//-----------------------------------
	// operator+ - join two -- top are base
	//-----------------------------------

	String operator+(const char* concat) { return String(strcat(strPtr, concat), true); }
	String operator+(const String& concat) { return operator+(concat.strPtr); }

	op_concat(integer_t);
	op_concat(int);		 // for old compilers
	op_concat(unsigned); // for old compilers
	op_concat(void*);
	op_concat(double);

	String operator+(char ch) {
		char x[2] = {ch, 0};
		return operator+(x);
	}

	// index into string
	// returns nullpointer if overrange
	char& operator[](unsigned_t index) { return index >= len() ? *(char*)0 : strPtr[index]; }
};

} // namespace cmplx

// legacy typedef
typedef cmplx::String STRNAME;

#undef op_assign
#undef op_concat
#undef op_append

#endif