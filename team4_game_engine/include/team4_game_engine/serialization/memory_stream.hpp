#pragma once
#include <cassert>
#include <memory>
#include <type_traits>
#include <team4_game_engine/serialization/endianess.hpp>

namespace team4_game_engine::serialization {
	class MemoryStream {
	public:
		MemoryStream() {
			m_buffer = static_cast<char*>(std::malloc(m_capacity));
		}

		explicit MemoryStream(size_t initialCapacity) {
			m_buffer = static_cast<char*>(std::malloc(initialCapacity));
			m_capacity = initialCapacity;
		}

		MemoryStream(const char* buffer, size_t size) {
			m_buffer = static_cast<char*>(std::malloc(size));
			std::memcpy(m_buffer, buffer, size);
			m_capacity = size;
		}

		MemoryStream(char* buffer, size_t size) {
			m_buffer = buffer;
			buffer = nullptr;
			m_capacity = size;
		}

		MemoryStream(const MemoryStream& stream) {
			m_buffer = static_cast<char*>(std::malloc(stream.m_capacity));
			std::memcpy(m_buffer, stream.m_buffer, stream.m_capacity);
			m_capacity = stream.m_capacity;
			m_size = stream.m_size;

			m_head = stream.m_head;
		}

		MemoryStream(MemoryStream&& stream) {
			m_buffer = stream.m_buffer;
			m_capacity = stream.m_capacity;
			m_size = stream.m_size;
			m_head = stream.m_head;

			stream.m_buffer = nullptr;
			stream.m_capacity = 0;
			stream.m_size = 0;
			stream.m_head = 0;
		}

		~MemoryStream() {
			std::free(m_buffer);
		}
		size_t Size() const { return m_size; }
		void Size(size_t size) { m_size = size; }
		const char* Data() const { return m_buffer; }
		template<typename T>
		void Write(T data) {
			static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic write only support primitive data types");
			if(DetectEndianess() == PlateformEndianess::BigEndiean) {
				auto dataLittleEndian = SwapEndian(data);
				Write(reinterpret_cast<char*>(&dataLittleEndian), sizeof(dataLittleEndian));
			}
			else {
				Write(reinterpret_cast<char*>(&data), sizeof(data));
			}
		}

		void Write(const char* data, size_t dataSize) {
			if (m_size + dataSize > m_capacity) {
				Resize(2 * m_capacity);
			}
			std::memcpy(m_buffer + m_size, data, dataSize);
			m_size += dataSize;
		}

		template<typename T>
		T Read() {
			static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "Generic read only support primitive data types");

			T value;
			Read(reinterpret_cast<char*>(&value), sizeof(T));

			if(DetectEndianess() == PlateformEndianess::BigEndiean)
				value = SwapEndian(value);

			return value;
		}

		void Read(char* data, size_t size){
			assert(size + m_head <= m_size);
			std::memcpy(data, m_buffer + m_head, size);
			m_head += size;
		}

	private:
		void Resize(size_t capacity) {
			m_buffer = static_cast<char*>(std::realloc(m_buffer, capacity));
			m_capacity = capacity;
		}
		char* m_buffer;
		size_t m_capacity = 32;
		size_t m_size = 0;

		size_t m_head = 0;
	};
}