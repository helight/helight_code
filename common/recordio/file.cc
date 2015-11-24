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
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <boost/scoped_array.hpp>

#include "file.h"

File::File(FILE* const f_des, const std::string& name)
    : f_(f_des), name_(name) {}

bool File::Delete(const char* const name)
{
    return remove(name) == 0;
}

bool File::Exists(const char* const name)
{
    return access(name, F_OK) == 0;
}

size_t File::Size()
{
    struct stat f_stat;
    stat(name_.c_str(), &f_stat);
    return f_stat.st_size;
}

bool File::Flush()
{
    return fflush(f_) == 0;
}

bool File::Close()
{
    if (fclose(f_) == 0)
    {
        f_ = NULL;
        return true;
    }
    else
    {
        return false;
    }
}

void File::ReadOrDie(void* const buf, size_t size)
{
    //CHECK_EQ(fread(buf, 1, size, f_), size);
    assert(fread(buf, 1, size, f_) == size);
    return;
}

size_t File::Read(void* const buf, size_t size)
{
    return fread(buf, 1, size, f_);
}

void File::WriteOrDie(const void* const buf, size_t size)
{
    // CHECK_EQ(fwrite(buf, 1, size, f_), size);
    assert(fwrite(buf, 1, size, f_) == size);
    return;
}

size_t File::Write(const void* const buf, size_t size)
{
    return fwrite(buf, 1, size, f_);
}

File* File::OpenOrDie(const char* const name, const char* const flag)
{
    FILE* const f_des = fopen(name, flag);
    if (f_des == NULL)
    {
        std::cerr << "Cannot open " << name;
        exit(1);
    }
    File* const f = new File(f_des, name);
    return f;
}

File* File::Open(const char* const name, const char* const flag)
{
    FILE* const f_des = fopen(name, flag);
    if (f_des == NULL) return NULL;
    File* const f = new File(f_des, name);
    return f;
}

char* File::ReadLine(char* const output, uint64_t max_length)
{
    return fgets(output, max_length, f_);
}

int64_t File::ReadToString(std::string* const output, uint64_t max_length)
{
    // CHECK_NOTNULL(output);
    assert(output != NULL);
    output->clear();

    if (max_length == 0) return 0;
    if (max_length < 0) return -1;

    int64_t needed = max_length;
    int32_t bufsize = (needed < (2 << 20) ? needed : (2 << 20));

    boost::scoped_array<char> buf(new char[bufsize]);

    int64_t nread = 0;
    while (needed > 0)
    {
        nread = Read(buf.get(), (bufsize < needed ? bufsize : needed));
        if (nread > 0)
        {
            output->append(buf.get(), nread);
            needed -= nread;
        }
        else
        {
            break;
        }
    }
    return (nread >= 0 ? static_cast<int64_t>(output->size()) : -1);
}

size_t File::WriteString(const std::string& line)
{
    return Write(line.c_str(), line.size());
}

bool File::WriteLine(const std::string& line)
{
    if (Write(line.c_str(), line.size()) != line.size()) return false;
    return Write("\n", 1) == 1;
}

std::string File::filename() const
{
    return name_;
}

bool File::Open() const
{
    return f_ != NULL;
}

void File::Init() {}

namespace file
{
    Status SetContents(const std::string& filename, const std::string& contents,
                       int32_t flags)
    {
        if (flags != Defaults())
        {
            // LOG(DFATAL) << "file::SetContents() with unsupported flags=" << flags;
            return Status(false);
        }
        File* file = File::Open(filename, "w");
        if (file == NULL) return Status(false);
        return Status(file->WriteString(contents) == contents.size());
    }

    Status GetContents(const std::string& filename, std::string* output,
                       int32_t flags)
    {
        if (flags != Defaults())
        {
            // LOG(DFATAL) << "file::GetContents() with unsupported flags=" << flags;
            return Status(false);
        }
        File* file = File::Open(filename, "r");
        if (file == NULL) return Status(false);
        int64_t size = file->Size();
        return Status(size == file->ReadToString(output, size));
    }

}  // namespace file
