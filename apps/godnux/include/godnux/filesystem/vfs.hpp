/*
 * Godnux Kernel
 * Copyright (C) 2025 Barry
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef GODNUX_VFS_HPP
#define GODNUX_VFS_HPP

#include "../object.hpp"
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

namespace godnux {

class VFS : public Object {
    GDCLASS(VFS, Object);
    
public:
    struct MountPoint {
        std::string source;
        std::string target;
        std::string fstype;
        bool read_only;
    };
    
private:
    std::unordered_map<std::string, MountPoint> mount_points;
    std::string current_working_directory = "/";
    
protected:
    static void _bind_methods();
    
public:
    VFS();
    ~VFS();
    
    // Mount operations
    bool mount(const std::string& source, const std::string& target, const std::string& fstype, bool read_only = false);
    bool unmount(const std::string& target);
    std::vector<MountPoint> get_mount_points() const;
    
    // File operations
    bool file_exists(const std::string& path) const;
    bool is_directory(const std::string& path) const;
    std::vector<std::string> list_directory(const std::string& path) const;
    
    // Path operations
    std::string get_current_directory() const { return current_working_directory; }
    bool change_directory(const std::string& path);
    std::string get_absolute_path(const std::string& path) const;
};

class File : public Object {
    GDCLASS(File, Object);
    
public:
    enum Mode {
        READ,
        WRITE,
        APPEND,
        READ_WRITE
    };
    
private:
    std::string path;
    Mode mode;
    bool is_open = false;
    
protected:
    static void _bind_methods();
    
public:
    File();
    ~File();
    
    bool open(const std::string& path, Mode mode);
    void close();
    bool is_open() const { return is_open; }
    
    // Reading
    std::string read_line();
    std::string read_all();
    std::vector<uint8_t> read_bytes();
    
    // Writing
    bool write_string(const std::string& data);
    bool write_bytes(const std::vector<uint8_t>& data);
    
    // File info
    size_t get_size() const;
    bool exists(const std::string& path) const;
};

} // namespace godnux

#endif // GODNUX_VFS_HPP