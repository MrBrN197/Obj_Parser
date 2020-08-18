

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <vector>
#include <regex>
#include <string>
#include <iostream>
#include <fstream>
#include <string>

using u8 = uint8_t;
using u32 = uint32_t;

#ifdef _MSC_VER
	#ifdef _DEBUG
		#define ASSERT(x) if(!(x)) {printf("ASSERTION RAISED [%s:%d]\n", __FILE__, __LINE__); __debugbreak(); }
	#else
		#define ASSERT(x)
	#endif
#else
	#error "Other toolsets aren't currently supported"
#endif

struct fvec3 {
	float x, y, z;
};

struct ivec3 {
	int x, y, z;
};

struct fvec2 {
	float x, y;
};

struct Vertex {
	ivec3 points;
	ivec3 texCoords;
	ivec3 normals;
	bool hasNormals;
	bool hasTexCoords;
};


int main() {

	std::vector<fvec3> points;
	std::vector<fvec2> texture_coords;
	std::vector<fvec3> normals;
	std::vector<Vertex> faces;

	std::ifstream file("cube.obj");
	if( file.is_open() ) {
		std::string str;
		while(file.good()){
			std::getline(file, str);
			std::smatch groups;
			std::regex reg("^[\\s\\t]*([a-z]+)");
			bool found = std::regex_search(str, groups, reg);
			if(found) {
				std::string type = groups.str(1);
				str = groups.suffix().str();
				if( type == "v") {
					reg = "(-?\\d+\\.\\d+) (-?\\d+\\.\\d+) (-?\\d+\\.\\d+)\\s?(\\d+\\.\\d+)?";
					std::smatch matches;
					found = std::regex_search(str, matches, reg);
					ASSERT(found)
					float x = (float)std::atof(matches.str(1).c_str());
					float y = (float)std::atof(matches.str(2).c_str());
					float z = (float)std::atof(matches.str(3).c_str());
					// std::cout << "Vertex X: " << x << " Y: " << y << " Z: " << z << std::endl;
					points.push_back({x, y, z});
				} else if(type == "vt") {
					reg = "(-?\\d+\\.\\d+) (-?\\d+\\.\\d+)";
					std::smatch matches;
					found = std::regex_search(str, matches, reg);
					ASSERT(found)
					float x = (float)std::atof(matches.str(1).c_str());
					float y = (float)std::atof(matches.str(2).c_str());
					float z = (float)std::atof(matches.str(3).c_str());
					// std::cout << "Tex Coords X: " << x << " Y: " << y << " Z: " << z << std::endl;
					texture_coords.push_back({x, y});
				} else if(type == "vn") {
					reg = "(-?\\d+\\.\\d+) (-?\\d+\\.\\d+) (-?\\d+\\.\\d+)";
					std::smatch matches;
					found = std::regex_search(str, matches, reg);
					ASSERT(found)
					float x = (float)std::atof(matches.str(1).c_str());
					float y = (float)std::atof(matches.str(2).c_str());
					float z = (float)std::atof(matches.str(3).c_str());
					// std::cout << "Normals X: " << x << " Y: " << y << std::endl;
					normals.push_back({x, y, z});
				} else if(type == "f") {
					reg = "(\\d+/?)+";
					std::vector<int> numbers;

					char c = str.front();
					str.erase(0, 1);
					while(!str.empty()) {
						while(!str.empty() && (c == '/' || c == ' ')) {
							c = str.front();
							str.erase(0, 1);
						}
						char number[10];
						u32 idx = 0;
						while(c >= '0' && c <= '9'){
							ASSERT(idx < 10)
							number[idx++] = c;
							if(str.empty())
								break;
							c = str.front();
							str.erase(0, 1);
						}
						number[idx] = '\0';
						numbers.push_back(std::atoi(number));
					}
					size_t size = numbers.size();
					Vertex vertex;
					if(size == 3){
						vertex.hasNormals = false;
						vertex.hasTexCoords = false;
						vertex.points = {numbers[0], numbers[1], numbers[2]};
						faces.push_back(vertex);
					}else if(size == 4) {
						vertex.hasNormals = false;
						vertex.hasTexCoords = false;
						vertex.points = {numbers[0], numbers[1], numbers[2]};
						vertex.points = {numbers[0], numbers[2], numbers[3]};
						faces.push_back(vertex);
					} else if(size == 12) {
						vertex.hasNormals = true;
						vertex.hasTexCoords = true;

						vertex.points = {numbers[0], numbers[3], numbers[6]};
						vertex.texCoords = {numbers[1], numbers[4], numbers[7]};
						vertex.normals = {numbers[2], numbers[5], numbers[8]};
						faces.push_back(vertex);

						vertex.points = {numbers[0], numbers[6], numbers[9]};
						vertex.texCoords = {numbers[1], numbers[7], numbers[10]};
						vertex.normals = {numbers[2], numbers[8], numbers[11]};
						faces.push_back(vertex);
					} else {
						ASSERT(false);
					}
				} else {
					continue;
					std::cout << "Element type '" << type << "' not supported\n";
					ASSERT(false)
				}
			}
		}
	}

	printf("Number Of Vertexs %llu\n", points.size());
	printf("Number Of TexCoords %llu\n", texture_coords.size());
	printf("Number Of Normals %llu\n", normals.size());
	printf("Number Of Faces %llu\n", faces.size());

	return 0;

}