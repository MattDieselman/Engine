#include "Mesh.h"



bool Mesh::BufferModel(std::string objFile)
{
		//Declare texture Files

		//Declare vectors
		std::vector<glm::vec3> locs;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> norms;
		std::vector<VertInd> vertInds;
		//Try to open file
		std::ifstream inFile(objFile);
		if (inFile) {
			std::string line;
			//While the next line of the file is not empty (file still has good data)
			while (std::getline(inFile, line)) {
				std::istringstream objectData(line);
				std::string lineLable;
				objectData >> lineLable;

				//If line is for locations, save locations
				if (lineLable == "v") {
					float loc1; float loc2; float loc3;
					objectData >> loc1 >> loc2 >> loc3;
					locs.push_back(glm::vec3(loc1, loc2, loc3));
				}
				//If line is for uv data, save to uv vector
				else if (lineLable == "vt") {
					float uv1; float uv2;
					objectData >> uv1 >> uv2;
					uvs.push_back(glm::vec2(uv1, uv2));
				}
				//If line is for normals, save normal information
				else if (lineLable == "vn") {
					float norm1; float norm2; float norm3;
					objectData >> norm1 >> norm2 >> norm3;
					norms.push_back(glm::vec3(norm1, norm2, norm3));
				}
				//If line is for indices, save them
				else if (lineLable == "f") {
					for (int i = 0; i < 3; i++) {
						unsigned int ints[3];
						char chars[2];
						objectData >> ints[0] >> chars[0] >> ints[1] >> chars[1] >> ints[2];
						ints[0]--; ints[1]--; ints[2]--;
						vertInds.push_back(VertInd{ ints[0], ints[1], ints[2] });
					}
				}
			}
			inFile.close();
		}
		else {
			std::cout << "Error opening or reading file. File " << objFile << " was not found or was unable to be opened." << std::endl;
			return 0;
		}

		//Populate vertex buffer based on indices.
		vertCount = vertInds.size();

		std::vector<Vertex> vertBufData(vertCount);
		for (int i = 0; i < vertCount; i++) {
			vertBufData[i] = { locs[vertInds[i].locInd],
				uvs[vertInds[i].uvInd],
				norms[vertInds[i].normInd] };
		}

		//Create & Bind buffer
		GLuint vertBuf;
		glGenVertexArrays(1, &vertArr);
		glGenBuffers(1, &vertBuf);
		glBindVertexArray(vertArr);
		glBindBuffer(GL_ARRAY_BUFFER, vertBuf);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertCount, &vertBufData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);													//Step of vertex size, offset of 0
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));						//Step of vertex size, offset of size of 1 vec3 (location info)
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)((sizeof(glm::vec3)) + sizeof(glm::vec2)));	//Step of vertex size, offset of size of 1 vec3 and 1 vec 2 (location and uv info)
		glBindVertexArray(0);

		return true;
	
}

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}
