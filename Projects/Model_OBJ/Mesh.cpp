#include "Mesh.hpp"

Mesh::Mesh(std::vector<vertexData> * vd, std::vector<unsigned int> * id, std::vector<textureData> * td) {
	indices = *id;
	data = *vd;
	if(td) {
		textures = *td;
	}

	vbo = 0;
	ind = 0;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(vertexData), &data[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
	glBufferData(GL_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Mesh::~Mesh() {
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ind);
}

void Mesh::draw(unsigned int programId) {

	//attribute vec3 vertex
	int vertex = glGetAttribLocation(programId, "vertex");
	int normal = glGetAttribLocation(programId, "normal");
	int tangent = glGetAttribLocation(programId, "tangent");
	int color = glGetAttribLocation(programId, "color");
	int uv = glGetAttribLocation(programId, "uv");

	//textures
	std::string str = "texture";
	for(int i = 0 ; i < textures.size(); ++i) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
		std::string curStr = str + (char)(i + '0');
		glUniform1i(glGetUniformLocation(programId,  (char *)curStr.c_str()), i);
	}

	glBindTexture(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);

	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(vertex, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), 0);

	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(3 * sizeof(float)) );

	glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(6 * sizeof(float)) );

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(9 * sizeof(float)) );

	glEnableVertexAttribArray(vertex);
	glVertexAttribPointer(uv, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), (void*)(12 * sizeof(float)) );

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

	glDisableVertexAttribArray(vertex);
	glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(uv);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}