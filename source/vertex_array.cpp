#include "vertex_array.h"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &vaoId);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &vaoId);
}

void VertexArray::bind() {
	glBindVertexArray(vaoId);
}

void VertexArray::unbind() {
	glBindVertexArray(0);
}

void VertexArray::setLayout(VertexBufferLayout* layout) {
	this->layout = layout;
}

void VertexArray::addVertexBuffer(VertexBuffer& vb) {
	bind();
	vb.bind();
	unsigned int offset = 0;
	auto elements = layout->getElements();
	for (int i = 0; i < elements.size(); i++) {
		VertexBufferElement element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout->getStride(), (void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}

}
