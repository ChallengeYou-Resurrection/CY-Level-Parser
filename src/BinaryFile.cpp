#include "BinaryFile.h"

#include <iostream>

const char* BinaryFileBuffer::getBuffer() const {
    return m_buffer.data();
}

size_t BinaryFileBuffer::bufferSize() const{
    return m_buffer.size();
}

void BinaryFileBuffer::write(const char* buffer, size_t length) {
    std::copy(buffer, buffer + length, std::back_insert_iterator(m_buffer));
}