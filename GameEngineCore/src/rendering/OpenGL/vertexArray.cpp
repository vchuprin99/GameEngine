#include "vertexArray.h"

#include <glad/glad.h>
#include <log.h>

GameEngine::VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_id);
}

GameEngine::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_id);
}

void GameEngine::VertexArray::addVertexBuffer(const VertexBuffer& vertexBuffer)
{
	bind();
	for (const BufferElement& currentElement : vertexBuffer.getLayout().getElements()) {
		glEnableVertexAttribArray(m_elementsCount);
		glVertexAttribPointer(
			m_elementsCount,
			static_cast<GLint>(currentElement.componentCount),
			currentElement.componentType,
			false,
			static_cast<GLsizei>(vertexBuffer.getLayout().getStride()),
			reinterpret_cast<const void*>(currentElement.offset)
		);
		++m_elementsCount;
	}
}

void GameEngine::VertexArray::setIndexBuffer(const IndexBuffer& indexBuffer)
{
	bind();
	indexBuffer.bind();
	m_indicesCount = indexBuffer.getCount();
}

void GameEngine::VertexArray::bind() const
{
	glBindVertexArray(m_id);
}

void GameEngine::VertexArray::unbind()
{
	glBindVertexArray(0);
}
