#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

namespace ebook {

enum class CommandType {
    READ,
    CHEER
};

struct Command {
    CommandType type;
    int32_t user_id = 0;
    int16_t current_page = 0;
};

std::istream& operator>>(std::istream& input, Command& command) {
    using namespace std::literals;
    
    std::string command_name;
    input >> command_name;
    
    if (command_name == "READ"s) {
        command.type = CommandType::READ;
        input >> command.user_id;
        input >> command.current_page;
    } else if (command_name == "CHEER"s) {
        command.type = CommandType::CHEER;
        input >> command.user_id;
    }
    return input;
}

void ApplyCommandREAD(const Command& command,
                      std::map<int32_t, int16_t>& readers,
                      std::vector<int32_t>& pages) {
    
    if (readers.count(command.user_id) != 0) {
        --pages[readers.at(command.user_id)];
    }
    readers[command.user_id] = command.current_page;
    ++pages[command.current_page];
}

void ApplyCommandCHEER(const Command& command,
                       std::map<int32_t, int16_t>& readers,
                       std::vector<int32_t>& pages,
                       std::ostream& output) {
    output << std::setprecision(6);
    
    if (readers.count(command.user_id) == 0) {
        output << 0 << std::endl;
    } else if (readers.size() == 1) {
        output << 1 << std::endl;
    } else {
        int16_t current_page = readers.at(command.user_id);
        double slower_readers = std::accumulate(pages.begin() + 1, pages.begin() + current_page, 0.0);
        output << slower_readers / (readers.size() - 1) << std::endl;
    }
}
    
void ProcessСommands(std::istream& input, std::ostream& output) {
    std::map<int32_t, int16_t> readers;
    std::vector<int32_t> pages(1001);
    
    int command_count;
    input >> command_count;
    
    for (int i = 0; i < command_count; ++i) {
        Command command;
        input >> command;
        
        switch (command.type) {
            case CommandType::READ: {
                ApplyCommandREAD(command, readers, pages);
                break;
            }
            case CommandType::CHEER: {
                ApplyCommandCHEER(command, readers, pages, output);
                break;
            }
        }
    }         
}

} // namespace ebook

int main() {
    ebook::ProcessСommands(std::cin, std::cout);
    return 0;
}