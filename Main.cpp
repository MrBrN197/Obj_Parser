

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
using u16 = uint16_t;
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

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	std::ifstream file("plane.obj");
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


	if(!glfwInit()){
		printf("Failed to initialize GLFW\n");
		ASSERT(false);
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	auto status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if(!status){
		printf("Failed to initialize glad\n");
		glfwTerminate();
		ASSERT(false)
	}

	u32 vertexArray;
	glGenVertexArrays(1, (GLuint*)&vertexArray);
	glBindVertexArray(vertexArray);

	u32 pointsBuffer;
	u32 indicesBuffer;
	glGenBuffers(1, (GLuint*)&pointsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, (GLuint)pointsBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fvec3) * points.size(), points.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &indicesBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	u16* indices = new u16[faces.size() * 3];
	for(int i = 0; i < faces.size(); i++){
		Vertex vertex = faces[i];
		*(indices + i*3+0) = (u16)vertex.points.x;
		*(indices + i*3+1) = (u16)vertex.points.y;
		*(indices + i*3+2) = (u16)vertex.points.z;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * faces.size() * 3, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (const void*)0);


	glClearColor(0.4f, 0.2f, 0.8f, 1.f);
	while(!glfwWindowShouldClose(window)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		glDrawElements(GL_TRIANGLES, (int)(faces.size() * 3), GL_UNSIGNED_SHORT, nullptr);
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;

}