#pragma once
#ifndef UTIL_FILEMAP_HPP
#define UTIL_FILEMAP_HPP

#include <iostream>
#include <string>

#ifdef _WIN64
#include <windows.h>
#elif __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace util
{
#ifdef _WIN64

class FileMapReader
{
  public:
    FileMapReader(const std::string &filename)
    {
        m_hfile = CreateFileA(filename.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING,
                              FILE_ATTRIBUTE_NORMAL, NULL);
        if (m_hfile == INVALID_HANDLE_VALUE)
        {
            m_error_msg = "Failed to open file: " + filename;
            return;
        }
        LARGE_INTEGER fsize;
        if (!GetFileSizeEx(m_hfile, &fsize))
        {
            m_error_msg = "Failed to get file size: " + filename;
            return;
        }
        m_file_size = fsize.QuadPart;

        m_hmap = CreateFileMappingA(m_hfile, NULL, PAGE_READONLY, 0, 0, NULL);
        if (m_hmap == NULL)
        {
            m_error_msg = "Failed to create file mapping: " + filename;
            return;
        }
        m_data = MapViewOfFile(m_hmap, FILE_MAP_READ, 0, 0, 0);
        if (m_data == NULL)
        {
            m_error_msg = "Failed to map view of file: " + filename;
            return;
        }
    }
    ~FileMapReader()
    {
        if (m_data != NULL)
        {
            UnmapViewOfFile(m_data);
        }
        if (m_hmap != NULL)
        {
            CloseHandle(m_hmap);
        }
        if (m_hfile != INVALID_HANDLE_VALUE)
        {
            CloseHandle(m_hfile);
        }
    }

    bool good() const { return m_error_msg.empty(); }
    const std::string &error_msg() const { return m_error_msg; }
    const void *data() const { return m_data; }
    std::size_t file_size() const { return m_file_size; }

  private:
    FileMapReader(const FileMapReader &) = delete;
    FileMapReader &operator=(const FileMapReader &) = delete;
    FileMapReader(FileMapReader &&) = delete;
    FileMapReader &operator=(FileMapReader &&) = delete;

    HANDLE m_hfile, m_hmap;
    void *m_data;
    std::size_t m_file_size;
    std::string m_error_msg;
};
#elif __linux__

class FileMapReader
{
  public:
    FileMapReader(const std::string &filename)
    {
        m_fd = open(filename.c_str(), O_RDONLY);
        if (m_fd == -1)
        {
            m_error_msg = "Failed to open file: " + filename;
            return;
        }
        struct stat st;
        if (fstat(m_fd, &st) == -1)
        {
            m_error_msg = "Failed to get file size: " + filename;
            return;
        }
        m_file_size = st.st_size;
        if (!S_ISREG(st.st_mode))
        {
            m_error_msg = "File is not a regular file: " + filename;
            return;
        }

        m_data = mmap(NULL, m_file_size, PROT_READ, MAP_PRIVATE, m_fd, 0);
        if (m_data == MAP_FAILED)
        {
            m_error_msg = "Failed to map view of file: " + filename;
            return;
        }
    }
    ~FileMapReader()
    {
        if (m_data != MAP_FAILED)
        {
            munmap(m_data, m_file_size);
        }
        if (m_fd != -1)
        {
            close(m_fd);
        }
    }

    bool good() const { return m_error_msg.empty(); }
    const std::string &error_msg() const { return m_error_msg; }
    const void *data() const { return m_data; }
    std::size_t file_size() const { return m_file_size; }

  private:
    FileMapReader(const FileMapReader &) = delete;
    FileMapReader &operator=(const FileMapReader &) = delete;
    FileMapReader(FileMapReader &&) = delete;
    FileMapReader &operator=(FileMapReader &&) = delete;

    int m_fd;
    void *m_data;
    std::size_t m_file_size;
    std::string m_error_msg;
};

#endif

} // namespace util

#endif // UTIL_FILEMAP_HPP