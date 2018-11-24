#include "filesystem.hpp"
#include <filesystem>

using namespace std;

namespace ve {

static size_t VEfwrite(void const* buf, size_t size, size_t count, FILE* stream)
{
    // fwrite doesn't want to seem to work for sizes above this,
    assert(size < 4000);

    uint8_t* buf_ = (uint8_t*) buf;
    size_t written = 0;
    size_t sizeLeft = size;

    while (written < count) {
        written += fwrite(buf_, sizeLeft, count, stream);
        buf_ += written;
        sizeLeft -= written;
    }

    return size * count;
}

// ------------------------------------------ File ---------------------------------------------

File::File()
{}

File::File(std::string fileName, std::string mode)
{
    this->open(fileName.c_str(), mode.c_str());
}

File::File(const char* fileName, const char* mode)
{
    this->open(fileName, mode);
}

File::~File()
{
    this->release();
}

void File::open(const char* fileName, const char* mode)
{
    if (m_fp) fclose(m_fp);
    m_fp = nullptr;
    fopen_s(&m_fp, fileName, mode);
}

bool File::read(FileData& dst)
{
    if (!m_fp) {
        derr("Invalid file.");
        return false;
    }

    if (dst.size <= 0 || dst.count <= 0) {
        dst.size = 1;
        dst.count = this->getSize();
    }
    if (!dst.data) {
        dst.data = new uint8_t[ dst.size * dst.count ];
        if (!dst.data) {
            return false;
        }
        dst.dataOwned = true;
    }

    size_t nr = fread(dst.data, dst.size, dst.count, m_fp);
    if (nr != dst.count) {
        derr_fatal("Could not read enough bytes (%d vs %d).", nr, dst.count);
        dst.count = nr;
        return true;
    }

    return true;
}

bool File::write(const FileData& src)
{
    if (!m_fp) {
        derr("Invalid file.");
        return false;
    }
    if (!src.data || !src.size) {
        return true;
    }

    size_t nw = VEfwrite(src.data, src.size, src.count, m_fp);
    if (nw != src.count) {
        derr("Could not write to file.");
        return false;
    }

    return true;
}

size_t File::offset(void)
{
    if (!m_fp) return 0;
    return (size_t) ftell(m_fp);
}

void File::flush(void)
{
    if (!m_fp) return;
    fflush(m_fp);
}

int64_t File::seek(int64_t offset, int origin)
{
    if (!m_fp) return 0;
    return (int64_t) fseek(m_fp, (long int) offset, origin);
}

void File::release(void)
{
    if (!m_fp) return;
    fclose(m_fp);
    m_fp = NULL;
}

uint64_t File::getSize(void)
{
    // TODO: https://www.securecoding.cert.org/confluence/display/c/FIO19-C.+Do+not+use+fseek%28%29+and+ftell%28%29+to+compute+the+size+of+a+regular+file

    off_t prev = ftell(m_fp);
    if (prev == -1) {
        derr("Could not tell current FP offset.\n");
        goto error1;
    }

    if (fseek(m_fp, 0 , SEEK_END) != 0) {
        derr("Could not seek to end of file. Not regular file?\n");
        goto error1;
    }

    off_t sz = ftell(m_fp);
    if (sz == -1) {
        derr("Could not tell end FP offset.\n");
        goto error1;
    }

    fseek(m_fp, prev , SEEK_SET);
    return (uint64_t) sz;

    // Exit stack.
error1:
    if (prev != -1) {
        fseek(m_fp, prev , SEEK_SET);
    }
    return 0;
}


// ------------------------------------------ File System ---------------------------------------------

bool FileSystem::exists(const std::string& fileName)
{
    using namespace std::experimental::filesystem;
    path p(fileName);
    return std::experimental::filesystem::exists(p) && is_regular_file(p);
}

std::string FileSystem::fileName(const std::string& path_)
{
    using namespace std::experimental::filesystem;
    path p(path_);
    return p.filename().string();
}

std::string FileSystem::parentPath(const std::string& fileName)
{
    using namespace std::experimental::filesystem;
	path p(fileName);
	if (!p.has_parent_path()) {
		return "";
	}
	return p.parent_path().generic_string();
}

std::string FileSystem::assemble(const std::string& parentPath, const std::string& filePath)
{
    using namespace std::experimental::filesystem;
	path p(parentPath);
	return (p / filePath).generic_string();
}

std::string FileSystem::setFileExtension(const std::string& path_, const std::string& ext)
{
    using namespace std::experimental::filesystem;
	path p(path_);
    auto p_ = p.replace_extension(ext.c_str());
    return p_.generic_string();
}

std::vector<std::string>
FileSystem::listDirectory(const std::string& path_)
{
    using namespace std::experimental::filesystem;

    std::vector<std::string> listing;
	path p(path_);

    try {
        if (!std::experimental::filesystem::exists(p)) {
            derr("Filesystem error: Directory %s does not exist!\n", path_.c_str());
            return listing;
        }
        if (!is_directory(p)) {
            derr("Filesystem error: %s is not a directory!\n", path_.c_str());
            return listing;
        }
        vector<path> vlisting;
        copy(
            directory_iterator(p),
            directory_iterator(),
            back_inserter(vlisting)
        );
        std::sort(vlisting.begin(), vlisting.end());
        for (vector<path>::const_iterator it(vlisting.begin());
                it != vlisting.end(); ++it) {
            listing.push_back(it->generic_string());
        }
        return listing;
    } catch (const filesystem_error& ex) {
        derr("Filesystem error: %s", ex.what());
    }

    return listing;
}

// ------------------------------------------ JsonFileInput ---------------------------------------------

JsonFileInput::JsonFileInput(const std::string fileName)
{
    auto file = make_unique<File>(fileName, "rb");

    // Bail out if file failed to open.
    if (!file.get() || !file->valid()) {
        derr("Could not open json file %s\n", fileName.c_str());
        derr_fatal("Could not open JSON file.");
    }

    // Read in the file.
    FileData data;
    if (!file->read(data)) {
        derr("Could not load json file %s\n", fileName.c_str());
        return;
    }
    file.reset(nullptr);

    Json::Features features;
    Json::Reader reader(features);

    // Parse JSON content.
    bool ret = reader.parse((const char* ) data.data, (const char* ) data.data + (data.count * data.size), m_dataRoot);
    if (!ret) {
        derr("Failed to parse json file:\n\n%s\n", reader.getFormattedErrorMessages().c_str());
        return;
    }
}

JsonFileInput::~JsonFileInput()
{}

} // namespave ve
