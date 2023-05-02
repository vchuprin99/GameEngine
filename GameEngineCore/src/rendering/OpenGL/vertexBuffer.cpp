#include "vertexBuffer.h"

#include <glad/glad.h>
#include <log.h>
namespace GameEngine {
	inline constexpr unsigned int shaderDataToComponentsCount(const ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Int:
			return 1;
		case ShaderDataType::Float2:
		case ShaderDataType::Int2:
			return 2;
		case ShaderDataType::Float3:
		case ShaderDataType::Int3:
			return 3;
		case ShaderDataType::Float4:
		case ShaderDataType::Int4:
			return 4;
		}
	}
	inline constexpr size_t shaderDataTypeSize(const ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return sizeof(GLfloat) * shaderDataToComponentsCount(type);

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return sizeof(GLint) * shaderDataToComponentsCount(type);
		}
	}
	inline constexpr size_t shaderDataTypeToComponentType(const ShaderDataType type) {
		switch (type)
		{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
				return GL_FLOAT;

			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
				return GL_INT;
		}
	}

	inline constexpr int GLUsage(VertexBuffer::Usage usage) {
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
	VertexBuffer::VertexBuffer(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage)
		: m_bufferLayout(std::move(bufferLayout))
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ARRAY_BUFFER, m_id);
		glBufferData(GL_ARRAY_BUFFER, size, data, GLUsage(usage));
	}
	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_id);
	}
	BufferElement::BufferElement(const ShaderDataType type) :
		type(type),
		componentType(shaderDataTypeToComponentType(type)),
		componentCount(shaderDataToComponentsCount(type)),
		size(shaderDataTypeSize(type)),
		offset(0)
	{}
}
