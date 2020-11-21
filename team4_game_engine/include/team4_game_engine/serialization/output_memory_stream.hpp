#pragma once
#include <team4_game_engine/serialization/memory_stream.hpp>

namespace team4_game_engine::serialization {
	class OutputMemoryStream {
	public:
		OutputMemoryStream() { }

		explicit OutputMemoryStream(size_t initialCapacity) :m_stream(initialCapacity) { }

		size_t Size() const { return m_stream.Size(); }
		const char* Data() const { return m_stream.Data(); }

		template<typename T>
		void Write(T data) {
			m_stream.Write(data);
		}

		void Write(const char* data, size_t dataSize) {
			m_stream.Write(data, dataSize);
		}
		MemoryStream& RawStream() { return m_stream; }
	private:
		MemoryStream m_stream;
	};
}