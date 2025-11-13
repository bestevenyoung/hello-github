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
#ifndef GODNUX_FILESYSTEM_HPP
#define GODNUX_FILESYSTEM_HPP

#include "../object.hpp"
#include "vfs.hpp"
#include <map>
#include <vector>
#include <unordered_set>

namespace godnux {

class FileSystem : public Object {
    GDCLASS(FileSystem, Object);
    
public:
    struct FileDescriptor {
        int fd;
        std::string path;
        File::Mode mode;
        size_t position;
        int owner_pid;
    };
    
    struct FilePermission {
        int owner_uid;
        int group_gid;
        int permissions;  // Unix-style 0777
    };
    
private:
    std::map<int, FileDescriptor> open_files;
    std::map<std::string, FilePermission> file_permissions;
    std::unordered_set<std::string> mounted_filesystems;
    int next_fd = 3;  // Start after stdin, stdout, stderr
    
    VFS virtual_filesystem;
    
protected:
    static void _bind_methods();
    
public:
    FileSystem();
    ~FileSystem();
    
    // File descriptor operations
    int open_file(const std::string& path, File::Mode mode, int process_id);
    bool close_file(int fd);
    std::string read_file(int fd, size_t bytes = 0);
    bool write_file(int fd, const std::string& data);
    size_t seek_file(int fd, size_t position);
    size_t get_file_size(int fd) const;
    
    // File permissions
    bool set_permissions(const std::string& path, int permissions, int owner_uid = 0);
    FilePermission get_permissions(const std::string& path) const;
    bool check_permission(const std::string& path, int required_permissions, int user_uid) const;
    
    // Filesystem operations
    bool mount(const std::string& source, const std::string& target, const std::string& fstype);
    bool unmount(const std::string& target);
    std::vector<std::string> get_mounts() const;
    
    // Integrity and provenance
    std::string calculate_file_hash(const std::string& path) const;
    bool verify_file_integrity(const std::string& path, const std::string& expected_hash) const;
    void audit_file_access(const std::string& path, int process_id, const std::string& operation);
    
    // Directory operations
    bool create_directory(const std::string& path);
    bool remove_directory(const std::string& path);
    std::vector<std::string> list_directory(const std::string& path);
};

} // namespace godnux

#endif // GODNUX_FILESYSTEM_HPP