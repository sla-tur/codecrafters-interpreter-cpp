#include <cstring>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

//enum class TokenType;
//class Token;

std::string read_file_contents(const std::string& filename);
//inline std::string token_type_to_string(TokenType type);
//inline std::string anyToString(const std::any& value);

/* enum class TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens.
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

    EOFILE
}; */

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

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        
        if (!file_contents.empty()) {
            for (auto c : file_contents) {
                switch(c) {
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
                }
            }
        }
        std::cout << "EOF  null" << std::endl;
        
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


