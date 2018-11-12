// Copyright 2010-2013 Google
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef COMMON_FILE_RECORDIO_H_
#define COMMON_FILE_RECORDIO_H_

#include <string>
#include "file.h"
// This file defines some IO interfaces to compatible with Google
// IO specifications.
// This class appends a protocol buffer to a file in a binary format.
// The data written in the file follows the following format (sequentially):
// - MagicNumber (32 bits) to recognize this format.
// - Uncompressed data payload size (64 bits)
// - Compressed data payload size (64 bits), or 0 if the
//   data is not compressed.
// - Payload, possibly compressed. See RecordWriter::Compress()
//   and RecordReader::Uncompress
class RecordWriter
{
public:
    // Magic number when writing and reading protocol buffers.
    static const int kMagicNumber;

    explicit RecordWriter(File* const file);

    template <class P>
    bool WriteRecord(const P& proto)
    {
        std::string uncompressed_buffer;
        proto.SerializeToString(&uncompressed_buffer);
        const uint64_t uncompressed_size = uncompressed_buffer.size();

        return WriteRecord(uncompressed_buffer, uncompressed_size);
    }

    bool WriteRecord(const std::string &data, const uint64_t size)
    {
        const std::string compressed_buffer =
            use_compression_ ? Compress(data) : "";
        const uint64_t compressed_size = compressed_buffer.size();
        if (file_->Write(&kMagicNumber, sizeof(kMagicNumber)) != sizeof(kMagicNumber))
        {
            return false;
        }
        if (file_->Write(&size, sizeof(size)) != sizeof(size))
        {
            return false;
        }
        if (file_->Write(&compressed_size, sizeof(compressed_size)) !=
            sizeof(compressed_size))
        {
            return false;
        }
        if (use_compression_)
        {
            if (file_->Write(compressed_buffer.c_str(), compressed_size) !=
                compressed_size)
            {
                return false;
            }
        }
        else
        {
            if (file_->Write(data.c_str(), size) != size)
            {
                return false;
            }
        }
        return file_->Flush();
    }
    // Closes the underlying file.
    bool Close();

    void set_use_compression(bool use_compression);

private:
    std::string Compress(const std::string& input) const;
    File* const file_;
    bool use_compression_;
};

// This class reads a protocol buffer from a file.
// The format must be the one described in RecordWriter, above.
class RecordReader
{
public:
    explicit RecordReader(File* const file);

    template <class P>
    bool ReadRecord(P* const proto)
    {
        char *data;
        uint64_t usize = 0;
        if (ReadRecord(&data, &usize))
        {
            proto->ParseFromArray(data, usize);
            delete [] data;
            return true;
        }
        return false;
    }

    bool ReadRecord(std::string& data, uint64_t* size)
    {
        char* rdata = NULL;
        uint64_t usize;
        if (ReadRecord(&rdata, &usize))
        {
            data =  std::string(rdata, (size_t)usize);
            delete [] rdata;
            *size = usize;
            return true;
        }
        return false;
    }

    bool ReadRecord(char** data, uint64_t* size)
    {
        uint64_t usize = 0;
        uint64_t csize = 0;
        int magic_number = 0;
        if (file_->Read(&magic_number, sizeof(magic_number)) != sizeof(magic_number))
        {
            return false;
        }
        if (magic_number != RecordWriter::kMagicNumber)
        {
            return false;
        }
        if (file_->Read(&usize, sizeof(usize)) != sizeof(usize))
        {
            return false;
        }
        *size = usize;
        *data = new char[usize + 1];
        if (file_->Read(&csize, sizeof(csize)) != sizeof(csize))
        {
            delete [] *data;
            return false;
        }
        if (csize != 0) // The data is compressed.
        {
            char* compressed_buffer = new char[csize + 1];
            if (file_->Read(compressed_buffer, csize) != csize)
            {
                delete [] *data;
                delete [] compressed_buffer;
                return false;
            }
            compressed_buffer[csize] = '\0';
            Uncompress(compressed_buffer, csize, *data, usize);
            *(*data + usize) = '\0';
            delete [] compressed_buffer;
            // printf("uncompress data: %s size: %lld\r\n", *data, usize);
        }
        else
        {
            if (file_->Read(*data, usize) != usize)
            {
                delete [] *data;
                return false;
            }
        }
        return true;
    }

    // Closes the underlying file.
    bool Close();

private:
    void Uncompress(const char* const source,
                    uint64_t source_size,
                    char* output_buffer,
                    uint64_t output_size) const;

    File* const file_;
};

#endif  // COMMON_FILE_RECORDIO_H_
