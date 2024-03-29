#pragma once

#include <vector>

namespace GameEngine {
	enum class ShaderDataType {
		Float,
		Float2,
		Float3,
		Float4,

		Int,
		Int2,
		Int3,
		Int4,
	};

	struct BufferElement {
		ShaderDataType type;
		uint32_t componentType;
		size_t componentCount;
		size_t size;
		size_t offset;

		BufferElement(const ShaderDataType type);
	};

	class BufferLayout {
	public:
		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_elements(std::move(elements))
		{
			size_t offset = 0;
			m_stride = 0;
			for (auto& element : m_elements) {
				element.offset = offset;
				offset += element.size;
				m_stride += element.size;
			}
		}

		inline const std::vector<BufferElement>& getElements() const { return m_elements; }
		inline size_t getStride() const { return m_stride; }
	private:
		std::vector<BufferElement> m_elements;
		size_t m_stride = 0;
	};

	class VertexBuffer {
	public:
		enum class Usage {
			Static, Stream, Dynamic
		};

		VertexBuffer(const void* data, const size_t size, BufferLayout bufferLayout, const Usage usage = Usage::Static);
		VertexBuffer() = delete;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&&) = delete;

		inline unsigned int getHandle() const { return m_id; }
		inline const BufferLayout& getLayout() const { return m_bufferLayout; }
	private:
		unsigned int m_id = 0;
		BufferLayout m_bufferLayout;
	};
}