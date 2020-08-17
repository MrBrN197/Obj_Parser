

#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>
#include <vector>

using u8 = uint8_t;
using u32 = uint32_t;

struct Point {
	Point(float x, float y, float z) : x(x), y(y), z(z) {}
	float x, y, z;
};

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
	// u8* buffer = new u8[size];
	// fread(buffer, 1, size, fp);

	printf("File Size: %llu \n", size);

	char type;
	std::vector<Point> points;
	std::vector<Point> faces;
	while((type = fgetc(fp)) != EOF){
		if (type == '\n') {
			continue;
		}
		int status;
		float x_f, y_f, z_f;
		float unused_f; 
		u32 x_i, y_i, z_i;
		if (type == EOF) {
			printf("Failed To Get Character\n");
		}
		switch (type)
		{
		case 'v': {
			status = fscanf(fp, "%f %f %f %f", &x_f, &y_f, &z_f, &unused_f);
			if(status != 4) {
				printf("Failed to parse element type '%c' \n", type);
				return 1;
			}
			printf("Vertex %-4llu: X: %f, Y: %f, Z: %f \n", points.size(), x_f, y_f, z_f);
			points.emplace_back(x_f, y_f, z_f);
			break;
		}
		case 'f': {
			status = fscanf(fp, "%d %d %d", &x_i, &y_i, &z_i);
			if(status != 3) {
				printf("Failed to parse element type '%c' \n", type);
				return 1;
			}
			printf("Face %-4llu: [%4d %4d %4d]\n", faces.size(), x_i, y_i, z_i);
			faces.emplace_back((float)x_i, (float)y_i, (float)z_i);
			break;
		}
		default:
			printf("Element type '%c' not supported\n", type);
			return 1;
		}
		// skip newline
		type = fgetc(fp);
		if(type != '\n') {
			printf("Invalid Type\n");
			return 1;
		}
	}

	printf("Number Of Points %llu\n", points.size());
	printf("Number Of Faces %llu\n", faces.size());


	return 0;

}