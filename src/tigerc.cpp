#include <iostream>
#include <sstream>
#include <fstream>

#include <antlr4-runtime.h>

#include "TigerLexer.h"
#include "TigerParser.h"

#include "TigerFile.h"

#include "PrinterFunctions.h"

#include "Common.h"

using namespace tigerc;

bool tigerc::enable_logging = false;

int compile(TigerFile&& source_file, bool write_tokens, bool write_parse_tree) {

    antlr4::ANTLRInputStream input(source_file.get_source());
    TigerLexer lexer(&input);

    antlr4::CommonTokenStream tokens(&lexer);
    tokens.fill();

    if (lexer.getNumberOfSyntaxErrors() != 0) {
        ErrorLogger(source_file.get_path() << ": Lexer encountered " << lexer.getNumberOfSyntaxErrors() << " syntax errors");
        return Status::SCANNER_ERROR;
    }

    if (write_tokens) {
        std::string token_file_path = source_file.get_path_wo_extension() + ".tokens";
        std::ofstream token_file(token_file_path, std::ios::trunc);

        if (!token_file) {
            ErrorLogger("Failed to open file " << token_file_path << " for writing token stream");
            return Status::GENERAL_ERROR;
        }

        DebugLogger("Writing token stream from " << source_file.get_path() << " to file " << token_file_path);
        token_file << token_stream_to_string(tokens, lexer);

        token_file.flush();
    }
    
    TigerParser parser(&tokens);
    antlr4::tree::ParseTree* tree = parser.tiger_program();

    if (parser.getNumberOfSyntaxErrors() != 0) {
        ErrorLogger(source_file.get_path() << ": Parser encountered " << parser.getNumberOfSyntaxErrors() << " syntax errors");
        return Status::PARSER_ERROR;
    }

    if (parser.getCurrentToken()->getType() != parser.EOF) {
        ErrorLogger(source_file.get_path() << ": Parser finished before end of file. ");
        return Status::PARSER_ERROR;
    }    

    if (write_parse_tree) {
        std::string parse_tree_file_path = source_file.get_path_wo_extension() + ".tree.gv";
        std::ofstream parse_tree_file(parse_tree_file_path, std::ios::trunc);

        if (!parse_tree_file){
            ErrorLogger("Failed to open file " << parse_tree_file_path << "for writing parse tree graph");
            return Status::GENERAL_ERROR;
        }

        DebugLogger("Writing parse tree from " << source_file.get_path() << " to file " << parse_tree_file_path);
        parse_tree_file << parse_tree_to_graph_viz(tree, parser);

        parse_tree_file.flush();
    }

    return Status::SUCCESS;
}


int main(int argc, char** argv) {
    std::stringstream correct_usage;
    correct_usage << "Usage: tigerc [ Options ] -f file_path.tiger"            << std::endl;
    correct_usage << "Options:"                                                << std::endl;
    correct_usage << "    -h    Print this usage menu"                         << std::endl;
    correct_usage << "    -s    Write the token stream to a file"              << std::endl;
    correct_usage << "    -p    Write the parse tree to a GraphViz DOT format" << std::endl;
    correct_usage << "    -v    Enable debug logging statements to std out"    << std::endl;

    TigerFile tiger_source;
    bool write_tokens = false;
    bool write_parse_tree = false;

    for (int i = 1; i < argc; i++) {
        std::string argval(argv[i]);

        if (argval.size() < 2 || argval.at(0) != '-') {
            std::cerr << "Error invalid program argument: unknown argument " << argval << std::endl;
            std::cerr << correct_usage.str();
            return Status::GENERAL_ERROR;
        }

        switch (argval.at(1)) {
            case 'f':
                if (i + 1 < argc) {
                    tiger_source = TigerFile(argv[i + 1]);
                    i++;
                } 
                break;

            case 's':
                write_tokens = true;
                break;

            case 'p':
                write_parse_tree = true;
                break;

            case 'h':
                std::cout << correct_usage.str();
                return Status::SUCCESS;

            case 'v':
                enable_logging = true;
                break;

            default:
                std::cerr << "Error invalid program argument: unknown flag -" << argval << std::endl;
                std::cerr << correct_usage.str();
                return Status::GENERAL_ERROR;
        }
    }

    switch (tiger_source.get_status()) {
        case TigerFile::Status::ERROR_FILE_NAME_LENGTH:
            std::cerr << "Error invalid program argument: tiger source file " << tiger_source.get_path()
                        << " specified after -f flag has invalid length" << std::endl;
            std::cerr << correct_usage.str();
            return Status::GENERAL_ERROR;

        case TigerFile::Status::ERROR_FILE_EXTENSION:
            std::cerr << "Error invalid program argument: tiger source file " << tiger_source.get_path()
                        << " specified after -f flag has invalid extension, format must be <source file>.tiger" << std::endl;
            std::cerr << correct_usage.str();
            return Status::GENERAL_ERROR;

        case TigerFile::Status::ERROR_FILE_OPEN:
            std::cerr << "Error invalid program argument: tiger source file " << tiger_source.get_path()
                        << " specified after -f flag could not be opened" << std::endl;
            std::cerr << correct_usage.str();
            return Status::GENERAL_ERROR;

        case TigerFile::Status::UNINITIALIZED:
            std::cerr << "Error invalid program argument: no -f flag was found " << tiger_source.get_path() << std::endl;
            std::cerr << correct_usage.str();
            return Status::GENERAL_ERROR;
    }

    DebugLogger("Parsing file " << tiger_source.get_path()
                << ", with write_tokens " << (write_tokens ? "enabled" : "disabled")
                << ", and write_parse_tree " << (write_parse_tree ? "enabled" : "disabled"));

    return compile(std::move(tiger_source), write_tokens, write_parse_tree);
}