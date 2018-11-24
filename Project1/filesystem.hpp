/* --------------------------------- UAA SOFTWARE CONFIDENTIAL  -----------------------------------
 *
 * (c) Copyright 2016 UAA Software. All Rights Reserved.
 *
 * NOTICE:  All information contained herein is, and remains the property of UAA Software and its
 * suppliers, if any.  The intellectual and technical concepts contained herein are proprietary to
 * UAA software and its suppliers and may be covered by U.S. and Foreign Patents, patents in
 * process, and are protected by trade secret or copyright law.  Dissemination of this information
 * or reproduction of this material is strictly forbidden unless prior written permission is
 * obtained from UAA Software.
 */

#ifndef _VE_FILESYSTEM_
#define _VE_FILESYSTEM_

#include <cstdio>
#include <string>
#include "../common.hpp"

#include <json/json.h>
#include <json/reader.h>

namespace ve {

/**
 * @brief File data RAII class.
 *
 * Wraps a block of file data, used for both reading and writing.
 * May or may not actually own the data. If user doesn't want data to be freed
 * When this goes out of scope, then set dataOwned to false.
 *
 */
struct FileData {
    uint8_t* data = nullptr;
    uint64_t size = 0;
    uint64_t count = 1;
    
    //! Set this to false if you don't want data to go out of scope with
    //  the object.
    bool dataOwned = false;

    FileData()
    {}

    FileData(uint64_t size_)
        : data(new uint8_t[size_]), size(sizeof(uint8_t)), count(size_), dataOwned(true)
    {}

    ~FileData()
    {
        if (dataOwned) delete data;
        data = nullptr;
    }
};

template <typename T>
T& FileDataRead(FileData& data, off_t* offset)
{
    auto ptr = reinterpret_cast<T*>(data.data + *offset);
    *offset += sizeof(T);
    return *ptr;
}

template <typename T>
uint32_t FileDataRead(FileData& data, off_t* offset, std::vector<T>& vec)
{
    auto count = FileDataRead<uint32_t>(data, offset);
    vec.clear();
    vec.resize(count);
    for (int i = 0; i < (int) count; i++) {
        vec[i] = FileDataRead<T>(data, offset);
    }
    return count;
}

inline void FileDataReadStr(FileData& data, off_t* offset, std::string& str)
{
    // TODO: should probably make this faster.
    auto count = FileDataRead<uint32_t>(data, offset);
    str = "";
    for (int i = 0; i < (int) count; i++) {
        str += FileDataRead<char>(data, offset);
    }
}

// ------------------------------------------ File ---------------------------------------------

/**
 * @brief File operation abstraction.
 *
 * Wraps file operations. The file may come from difference sources
 * (eg. from a .zip pack or from an actual file).
 *
 */
class File {
    DISALLOW_COPY(File)
    FILE* m_fp = nullptr;

public:
    File();
    File(std::string fileName, std::string mode = "r");
    File(const char* fileName, const char* mode = "r");
    virtual ~File();

    //! Open a file.
    void open(const char* fileName, const char* mode);

    //! Read data from file.
    bool read(FileData& dst);

    //! Write data to file.
    bool write(const FileData& src);

    //! Get the current offset.
    size_t offset(void);

    //! Flush file to disk.
    void flush(void);

    //! Seek the file offset.
    int64_t seek(int64_t offset, int origin);

    //! Close the file.
    void release(void);

    // ------------------------------- Getter functions -------------------------------------

    inline FILE* getFP(void) {
        // No ownership transfer.
        return m_fp;
    }

    inline bool valid(void) {
        return m_fp != nullptr;
    }
    
    uint64_t getSize(void);
};

// ------------------------------------------ File System ---------------------------------------------

class FileSystem {
public:
    static bool exists(const std::string& fileName);
    static std::string fileName(const std::string& path_);
    static std::string parentPath(const std::string& fileName);
    static std::string assemble(const std::string& parentPath, const std::string& filePath);
    static std::string setFileExtension(const std::string& path_, const std::string& ext);
    static std::vector<std::string> listDirectory(const std::string& path_);
};

// ------------------------------------------ JsonFileInput ---------------------------------------------

/**
 * @brief JSON file reader.
 *
 * Helper class for reading JSON files.
 *
 */
class JsonFileInput {
    DISALLOW_COPY(JsonFileInput)

    Json::Value m_dataRoot;

public:
    JsonFileInput(const std::string fileName);
    virtual ~JsonFileInput();

    //! Get the read JSON data.
    inline Json::Value& data(void)
    {
        return this->m_dataRoot;
    }

    //! Get the read JSON data.
    operator Json::Value& ()
    {
        return this->data();
    }
};

} // namespace ve
#endif // _VE_FILESYSTEM_
