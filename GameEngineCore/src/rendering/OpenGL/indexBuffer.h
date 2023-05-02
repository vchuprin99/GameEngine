#pragma once
#include "VertexBuffer.h"

namespace GameEngine {
	class IndexBuffer {
	public:
		IndexBuffer(const void* data, const size_t count, const VertexBuffer::Usage usage = VertexBuffer::Usage::Static);
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer(IndexBuffer&&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;
		IndexBuffer& operator=(IndexBuffer&&) = delete;

		void bind() const;
		static void unbind();

		inline size_t getCount() const { return m_count; }
	private:
		unsigned int m_id;
		size_t m_count;
	};
}