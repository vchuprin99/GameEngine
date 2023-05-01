#pragma once

namespace GameEngine {
	class VertexBuffer {
	public:
		enum class Usage {
			Static, Stream, Dynamic
		};

		VertexBuffer(const void* data, const size_t size, Usage usage = Usage::Static);
		VertexBuffer() = delete;
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer(VertexBuffer&&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;
		VertexBuffer& operator=(VertexBuffer&&) = delete;

		void bind() const;
		static void unbind();
		
		void update(long long offset, size_t size, const void* data);
	private:
		unsigned int m_id = 0;
	};
}