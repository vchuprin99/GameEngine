#include "vertexBuffer.h"

#include <glad/glad.h>
#include <log.h>
namespace GameEngine {
	inline int GL_usage(VertexBuffer::Usage usage) {
		switch (usage) {
			case VertexBuffer::Usage::Static: {
				return GL_STATIC_DRAW;
				break;
			}
			case VertexBuffer::Usage::Stream: {
				return GL_STREAM_DRAW;
				break;
			}
			case VertexBuffer::Usage::Dynamic: {
				return GL_DYNAMIC_DRAW;
				break;
			}
			default: {
				LOG_CRIT("Unknown vertex buffer usage");
				return GL_STATIC_DRAW;
			}
		}
	}
	VertexBuffer::VertexBuffer(const void* data, const size_t size, Usage usage)
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_usage(usage));
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}
	void VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
	}
	void VertexBuffer::unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	void VertexBuffer::update(long long offset, size_t size, const void* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	}
}
