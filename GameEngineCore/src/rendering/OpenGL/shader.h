#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace GameEngine {
	class Shader {
	public:
		Shader(const char* vertexShaderSource, const char* fragmentShaderSource);
		Shader() = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(Shader&&) = delete;
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		~Shader();

		void bind() const;
		static void unbind();
		
		void setFloat(const char* uniform, float value);
		void setMat4(const char* uniform, const glm::mat4& matrix);

		inline bool isCompiled() const { return m_isCompiled; }
	private:
		static bool createShader(const char* source, unsigned int type, unsigned int* id);

		unsigned int m_id = 0;
		bool m_isCompiled;
	};
}