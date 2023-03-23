#pragma once


namespace Apostle {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};
	
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return 4;
			case ShaderDataType::Float2:    return 4 * 2;
			case ShaderDataType::Float3:    return 4 * 3;
			case ShaderDataType::Float4:    return 4 * 4;
			case ShaderDataType::Mat3: 	    return 4 * 3 * 3;
			case ShaderDataType::Mat4: 	    return 4 * 4 * 4;
			case ShaderDataType::Int: 	    return 4;
			case ShaderDataType::Int2: 	    return 4 * 2;
			case ShaderDataType::Int3: 	    return 4 * 3;
			case ShaderDataType::Int4: 	    return 4 * 4;
			case ShaderDataType::Bool: 	    return 1;
		}

		AP_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	struct BufferElements
	{
		ShaderDataType Type;
		std::string Name;
		int Size;
		int Offset;
		bool isNormalised;

		BufferElements() {}

		BufferElements(ShaderDataType type, const std::string& name, bool isNormal = false)
			: Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), isNormalised(isNormal)
		{}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case ShaderDataType::Float:    return 1;
				case ShaderDataType::Float2:   return 2;
				case ShaderDataType::Float3:   return 3;
				case ShaderDataType::Float4:   return 4;
				case ShaderDataType::Mat3:     return 3 * 3;
				case ShaderDataType::Mat4:     return 4 * 4;
				case ShaderDataType::Int:      return 1;
				case ShaderDataType::Int2:     return 2;
				case ShaderDataType::Int3:     return 3;
				case ShaderDataType::Int4:     return 4;
				case ShaderDataType::Bool :    return 1;
			}
		}
	};
	
	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(const std::initializer_list<BufferElements>& elements)
			: m_Elements (elements) 
		{
			CalculateOffsetAndStride();
		}

		inline uint32_t GetStride() const { return m_Stride; }
		inline const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
		const std::vector<BufferElements>::const_iterator begin() const{ return m_Elements.begin(); }
		const std::vector<BufferElements>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElements> m_Elements;
		uint32_t m_Stride;
	};
	
	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		
		virtual const BufferLayout& GetLayout() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}