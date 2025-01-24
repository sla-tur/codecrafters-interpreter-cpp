#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

//enum class TokenType;
//class Token;

std::string read_file_contents(const std::string& filename);
bool is_at_end();
char advance();
//inline std::string token_type_to_string(TokenType type);
//inline std::string anyToString(const std::any& value);

/*class Token {
    const TokenType     type;
    const std::string   lexeme;
    const std::any      literal;
    const int           line;

    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type(type),
          lexeme(lexeme),
          literal(literal),
          line(line) {}
    
    std::string toString() const {
        return tokenTypeToString(type) + " "
        + lexeme + " " + anyToString(literal);
    }
};

/* class Scanner {
    private:
        
} */

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    bool had_error = false;
    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            for (std::size_t index = 0; index < file_contents.size(); ++index) {
                switch(file_contents[index]) {
                    case '(':
                        std::cout << "LEFT_PAREN ( null" << std::endl;
                        break;
                    case ')':
                        std::cout << "RIGHT_PAREN ) null" << std::endl;
                        break;
                    case '{':
                        std::cout << "LEFT_BRACE { null" << std::endl;
                        break;
                    case '}':
                        std::cout << "RIGHT_BRACE } null" << std::endl;
                        break;
                    case ',':
                        std::cout << "COMMA , null" << std::endl;
                        break;
                    case '.':
                        std::cout << "DOT . null" << std::endl;
                        break;
                    case '+':
                        std::cout << "PLUS + null" << std::endl;
                        break;
                    case '-':
                        std::cout << "MINUS - null" << std::endl;
                        break;
                    case ';':
                        std::cout << "SEMICOLON ; null" << std::endl;
                        break;
                    case '*':
                        std::cout << "STAR * null" << std::endl;
                        break;
                    case '=':
                        if (index + 1 < file_contents.size() 
                        && file_contents[index+1] == '=') {
                            std::cout << "EQUAL_EQUAL == null" << std::endl;
                            ++index;
                            break;
                        } else {
                            std::cout << "EQUAL = null" << std::endl;
                            break;
                        }
                    case '!':
                        if (index + 1 < file_contents.size() 
                        && file_contents[index+1] == '=') {
                            std::cout << "BANG_EQUAL != null" << std::endl;
                            ++index;
                            break;
                        } else {
                            std::cout << "BANG ! null" << std::endl;
                            break;
                        }
                    case '>':
                        if (index + 1 < file_contents.size() 
                        && file_contents[index+1] == '=') {
                            std::cout << "GREATER_EQUAL >= null" << std::endl;
                            ++index;
                            break;
                        } else {
                            std::cout << "GREATER > null" << std::endl;
                            break;
                        }
                    case '<':
                        if (index + 1 < file_contents.size() 
                        && file_contents[index+1] == '=') {
                            std::cout << "LESS_EQUAL <= null" << std::endl;
                            ++index;
                            break;
                        } else {
                            std::cout << "LESS < null" << std::endl;
                            break;
                        }
                    default:
                        std::cerr << "[line 1] Error: Unexpected character: "
                            << file_contents[index] << std::endl;
                        had_error = true;
                        break;
                }
            }
        }
        std::cout << "EOF  null" << std::endl;
        if (had_error) {
            std::exit(65);
        }

        
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

    return 0;
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }   

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}