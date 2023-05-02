#pragma once

#include "VertexBuffer.h"
#include "indexBuffer.h"

namespace GameEngine {
	class VertexArray {
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&&) = delete;

		void addVertexBuffer(const VertexBuffer& vertexBuffer);
		void setIndexBuffer(const IndexBuffer& indexBuffer);

		void bind() const;
		static void unbind();

		size_t getIndicesCount() const { return m_indicesCount; }
	private:
		unsigned int m_id = 0;
		unsigned int m_elementsCount = 0;
		size_t m_indicesCount = 0;
	};
}