#ifndef LINE_H
#define LINE_H

#include "Renderable.h"
#include "VBO.h"
#include "VAO.h"
#include "Shader.h"

class Line : public Renderable {

private:

	VAO vao;
	VBO<Vertex> vertices;
	Shader* shader;
	Mat4 mat;

public:

	Line() {
		vao.bind();
		vertices.bind();
		vao.setVertices(0, 0, 0);
		vao.unbind();
		mat = Mat4(1.0f);
	}

	void add(float x, float y, float z) {
		vertices.append(Vertex(x,y,z));
		vertices.upload();
	}

	void clear() {
		vertices.clear();
	}

	virtual void render() override {
		vao.bind();
		glDrawArrays(GL_LINES, 0, vertices.size());
		Error::assertOK();
		vao.unbind();
	}

	Shader* getShader() const override {
		return shader;
	}

	virtual const Mat4& getMatrix() const override {
		return mat;
	}


};

#endif // LINE_H
