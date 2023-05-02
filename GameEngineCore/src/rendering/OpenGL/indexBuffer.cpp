#include "indexBuffer.h"

#include <glad/glad.h>
#include <log.h>

namespace GameEngine {
	inline constexpr int GLUsage(VertexBuffer::Usage usage);
	IndexBuffer::IndexBuffer(const void* data, const size_t count, const VertexBuffer::Usage usage)
		: m_count(count)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GLUsage(usage));
	}
	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}
	void IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	}
	void IndexBuffer::unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
