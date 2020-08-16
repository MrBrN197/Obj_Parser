

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

using u8 = uint8_t;
using u32 = uint32_t;


int main() {

	FILE* fp = fopen("cube.obj", "r");
	if(!fp) {
		printf("Failed To Open File cube.obj File\n");
		return 1;
	}

	int failed;

	failed = fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	failed = fseek(fp, 0, SEEK_SET);
	if(failed) {
		printf("Failed To Seek File\n");
		return 2;
	}
	u8* buffer = new u8[size];
	fread(buffer, 1, size, fp);

	printf("File Size: %llu ", size);

	return 0;

}