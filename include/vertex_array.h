#pragma once
#include "vertex_buffer.h"
#include "vertex_buffer_layout.h"

class VertexArray {

public:
	VertexArray();
	~VertexArray();

	void addVertexBuffer(VertexBuffer& vb);
	void setLayout(VertexBufferLayout* layout);
	void bind();
	void unbind();

private:
	unsigned int vaoId;
	VertexBufferLayout* layout;
};