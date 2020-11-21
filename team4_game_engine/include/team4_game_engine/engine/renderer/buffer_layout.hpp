#pragma once
#include <string>
#include <vector>
#include <glad/glad.h>
#include <team4_game_engine/components/components.hpp>

// Cette class permet de définir la structure des données envoyé au GPU et comment utilisé les données qui lui sont transmisse
// ex: https://developer.apple.com/library/archive/documentation/3DDrawing/Conceptual/OpenGLES_ProgrammingGuide/Art/interleaved_vertex_data_1_2x.png

namespace team4_game_engine::renderer {
	enum class ShaderDataType {

		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		Bool
	};

	// ShaderDataType to byte size
	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:  return 4;
		case ShaderDataType::Float2: return 4 * 2;
		case ShaderDataType::Float3: return 4 * 3;
		case ShaderDataType::Float4: return 4 * 4;
		case ShaderDataType::Mat3:   return 4 * 3 * 3;
		case ShaderDataType::Mat4:   return 4 * 4 * 4;
		case ShaderDataType::Int:    return 4;
		case ShaderDataType::Int2:   return 4 * 2;
		case ShaderDataType::Int3:   return 4 * 3;
		case ShaderDataType::Int4:   return 4 * 4;
		case ShaderDataType::Bool:   return 1;
		default: return 0;
		}
	}

	// ShaderDataType to base type
	static GLenum ShaderDataTypeBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:  return GL_FLOAT;
		case ShaderDataType::Float2: return GL_FLOAT;
		case ShaderDataType::Float3: return GL_FLOAT;
		case ShaderDataType::Float4: return GL_FLOAT;
		case ShaderDataType::Mat3:   return GL_FLOAT;
		case ShaderDataType::Mat4:   return GL_FLOAT;
		case ShaderDataType::Int:    return GL_INT;
		case ShaderDataType::Int2:   return GL_INT;
		case ShaderDataType::Int3:   return GL_INT;
		case ShaderDataType::Int4:   return GL_INT;
		case ShaderDataType::Bool:   return GL_BOOL;
		default: return 0;
		}
	}

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		bool Normalize;
		uint32_t Size;
		uint32_t Offset;
		components::ComponentType componentType;
		BufferElement(ShaderDataType type, const std::string& name, components::ComponentType _componentType, bool normalize = false) : Name(name), Type(type), componentType(_componentType), Size(ShaderDataTypeSize(type)), Offset(0), Normalize(normalize) { }

		uint32_t GetComponentCount() const {
			switch (Type)
			{
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3:   return 3;
			case ShaderDataType::Mat4:   return 4;
			case ShaderDataType::Int:    return 1;
			case ShaderDataType::Int2:   return 2;
			case ShaderDataType::Int3:   return 3;
			case ShaderDataType::Int4:   return 4;
			case ShaderDataType::Bool:   return 1;
			default: return 0;
			}
		};
	};

	class BufferLayout
	{
	public:
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements) {
			CalculateOffsetsAndStride();
		}
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator  begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator  end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator  begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator  end() const { return m_Elements.end(); }
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
		void CalculateOffsetsAndStride() {
			m_Stride = 0;
			for (BufferElement& element : m_Elements)
			{
				element.Offset = m_Stride;
				m_Stride += element.Size;
			}
		}
	};
}