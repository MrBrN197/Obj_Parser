#pragma once
#include <stdint.h>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using i32 = int32_t;

#ifdef _MSC_VER
	#ifdef _DEBUG
		#define ASSERT(x) if(!(x)) {printf("ASSERTION RAISED [%s:%d]\n", __FILE__, __LINE__); __debugbreak(); }
	#else
		#define ASSERT(x)
	#endif
#else
	#error "Other toolsets aren't currently supported"
#endif