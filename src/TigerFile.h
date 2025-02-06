/**
 * @file TigerFile.h
 * @brief contains the declaration for the TigerFile class
 * @author Evan Newman, evanmnewman1@gmail.com
 * @date February 2025
 */

#ifndef TIGERC_TIGERFILE_H_
#define TIGERC_TIGERFILE_H_

#include <iostream>
#include <fstream>
#include <sstream>

namespace tigerc {

class TigerFile {
public:
    enum Status {
        SUCCESS = 0,
        ERROR_FILE_NAME_LENGTH = 1,
        ERROR_FILE_EXTENSION = 2,
        ERROR_FILE_OPEN = 3,
        UNINITIALIZED = 4
    };

    TigerFile() : m_status(Status::UNINITIALIZED) {}

    TigerFile(std::string source_file_path) 
    : 
        m_file_path(source_file_path),
        m_status(Status::UNINITIALIZED)
    {
        if (m_file_path.size() <= sm_file_extension.size()) {
            m_status = Status::ERROR_FILE_NAME_LENGTH;
            return;
        } 
        
        else if (m_file_path.substr(m_file_path.size() - sm_file_extension.size(),
                                    sm_file_extension.size()) != sm_file_extension) {
            m_status = Status::ERROR_FILE_EXTENSION;
            return;
        }

        std::ifstream file(m_file_path);

        if (!file) {
            m_status = Status::ERROR_FILE_OPEN;
            return;
        }

        std::stringstream file_sstream;
        file_sstream << file.rdbuf();

        m_source = file_sstream.str();

        m_status = Status::SUCCESS;
    }

    Status get_status() { return m_status; }

    static std::string get_extension() { return sm_file_extension; }

    std::string get_path_wo_extension() {
        return m_file_path.substr(0, m_file_path.size() - sm_file_extension.size());
    }

    std::string get_path() { return m_file_path; }

    std::string get_source() { return m_source; }

private:
    inline static const std::string sm_file_extension = ".tiger";

    Status m_status;
    std::string m_file_path;
    std::string m_source;
};

} // namespace tigerc

#endif // include guard