#include "string.h"

#include <iostream>
#include <stdexcept>
#include <utility>

namespace {
bool read_line(std::istream& in, String& out) {
    out.clear();
    char c;
    bool got_any = false;
    while (in.get(c)) {
        got_any = true;
        if (c == '\n') return true;
        if (c != '\r') out.push_back(c);
    }
    return got_any;
}

void interactive() {
    std::cout << "\nИнтерактивный режим RLE.\n"
                 "  c <строка>  — сжать      (c AAABCC  -> A3B1C2)\n"
                 "  d <строка>  — распаковать (d A3B1C2 -> AAABCC)\n"
                 "  q           — выход\n";
    String line;
    while (true) {
        std::cout << "> " << std::flush;
        if (!read_line(std::cin, line)) break;
        if (line.empty()) continue;

        char cmd = line[0];
        if (cmd == 'q') break;
        if ((cmd != 'c' && cmd != 'd') || (line.size() > 1 && line[1] != ' ')) {
            std::cout << "Неизвестная команда. Используйте c, d или q.\n";
            continue;
        }

        String arg;
        for (size_t i = 2; i < line.size(); ++i) arg.push_back(line[i]);

        try {
            if (cmd == 'c') {
                std::cout << arg.rle_compress() << '\n';
            } else {
                arg.rle_decompress();
                std::cout << arg << '\n';
            }
        } catch (const std::invalid_argument& e) {
            std::cout << "Ошибка: " << e.what() << '\n';
        }
    }
}

} 

int main() {
    interactive();
    return 0;
}