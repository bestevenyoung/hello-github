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
#ifndef GODNUX_LOGGER_HPP
#define GODNUX_LOGGER_HPP

#include "../object.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <mutex>
#include <chrono>

namespace godnux {

class Logger : public Object {
    GDCLASS(Logger, Object);
    
public:
    enum LogLevel {
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_CRITICAL
    };
    
    struct LogEntry {
        LogLevel level;
        std::string message;
        std::string module;
        int process_id;
        uint64_t timestamp;
        std::string source_file;
        int source_line;
    };
    
private:
    std::vector<LogEntry> log_buffer;
    std::ofstream log_file;
    LogLevel current_level = LOG_INFO;
    size_t max_buffer_size = 1000;
    bool echo_to_console = true;
    std::mutex log_mutex;
    
    std::string get_level_string(LogLevel level) const;
    void write_to_file(const LogEntry& entry);
    void rotate_log_if_needed();
    
protected:
    static void _bind_methods();
    
public:
    Logger();
    ~Logger();
    
    // Logging methods
    void log(LogLevel level, const std::string& message, const std::string& module = "", 
             int process_id = 0, const std::string& source_file = "", int source_line = 0);
    
    void debug(const std::string& message, const std::string& module = "", 
               int process_id = 0, const std::string& source_file = "", int source_line = 0);
    void info(const std::string& message, const std::string& module = "", 
              int process_id = 0, const std::string& source_file = "", int source_line = 0);
    void warning(const std::string& message, const std::string& module = "", 
                 int process_id = 0, const std::string& source_file = "", int source_line = 0);
    void error(const std::string& message, const std::string& module = "", 
               int process_id = 0, const std::string& source_file = "", int source_line = 0);
    void critical(const std::string& message, const std::string& module = "", 
                  int process_id = 0, const std::string& source_file = "", int source_line = 0);
    
    // Configuration
    void set_log_level(LogLevel level) { current_level = level; }
    void set_echo_to_console(bool echo) { echo_to_console = echo; }
    void set_log_file(const std::string& file_path);
    
    // Log retrieval
    std::vector<LogEntry> get_recent_logs(size_t count = 100) const;
    std::vector<LogEntry> get_logs_by_level(LogLevel level, size_t count = 100) const;
    std::vector<LogEntry> get_logs_by_module(const std::string& module, size_t count = 100) const;
    std::vector<LogEntry> get_logs_by_process(int process_id, size_t count = 100) const;
    
    // System monitoring
    void log_system_stats();
    void log_process_creation(int pid, const std::string& name);
    void log_process_termination(int pid, const std::string& name);
    void log_module_load(const std::string& module_name);
    void log_module_unload(const std::string& module_name);
    
    // Resource reporting
    struct SystemStats {
        size_t memory_used;
        size_t memory_total;
        int process_count;
        int module_count;
        uint64_t uptime;
    };
    
    SystemStats get_current_stats() const;
};

} // namespace godnux

#endif // GODNUX_LOGGER_HPP