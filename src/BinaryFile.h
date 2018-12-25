#pragma once

#include <vector>
#include <cstdint>
#include <string>

using u8 = uint8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;

class BinaryFileBuffer {
    public:
        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, u8 n) {
            auto value = reinterpret_cast<const char*>(&n);
            buff.write(value, sizeof(u8));
            return buff;
        }

        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, i16 n) {
            auto value = reinterpret_cast<const char*>(&n);
            buff.write(value, sizeof(i16));
            return buff;
        }
        
        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, u16 n) {
            auto value = reinterpret_cast<const char*>(&n);
            buff.write(value, sizeof(u16));
            return buff;
        }

        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, u32 n) {
            auto value = reinterpret_cast<const char*>(&n);
            buff.write(value, sizeof(u32));
            return buff;
        }

        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, const std::string& s) {
            buff << (uint32_t)s.size();
            buff.write(s.c_str(), s.size());
            return buff;
        }

        template<typename T>
        friend BinaryFileBuffer& operator <<(BinaryFileBuffer& buff, const T& object) {
            object.serialize(buff);
            return buff;
        }

        const char* getBuffer() const;
        size_t      bufferSize() const;

    private:
        void write(const char* buffer, size_t length);
        std::vector<char> m_buffer;
};