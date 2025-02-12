#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cctype>

bool had_error = false;
std::string read_file_contents(const std::string& filename);
void error(int line, const std::string& message);
void report(int line, const std::string& where, const std::string& message);

// Define the different kinds of tokens our language supports
enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals.
    IDENTIFIER, STRING, NUMBER,

    // Keywords.
    IF, ELSE, WHILE, FOR, RETURN, TRUE, FALSE,

    // End-of-file.
    END_OF_FILE
};

struct Token {
    TokenType     type;
    std::string   lexeme;
    std::string   literal;
    int           line;

    Token(TokenType type, const std::string& lexeme, const std::string& literal, int line)
        : type(type), lexeme(lexeme), literal(literal), line(line) {}
};

class Scanner {
public:
    /* C++ curiosity: whenever you write a constructor with one parameter,
    it can be used as a 'converting constructor', i.e. 
    if class Foo has a constructor of the form Foo(int x) {}, then
    if for some method bar(Foo foo), we can simply pass the int to bar
    and the compiler will make the implicit conversion
    (as long as there's only one)
    
    Using the keyword 'explicit' prevents the compiler from doing that,
    making the programme's behaviour a bit more predictable */
    explicit Scanner(const std::string& source) : source(source) {}

    std::vector<Token> scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanToken();
        }
        tokens.push_back(Token(TokenType::END_OF_FILE, "", "", line));
        return tokens;
    }

private:
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd() const {
        return current >= source.size();
    }

    void scanToken() {
        char c = advance();
        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;

            // Two slashes make a comment in Lox: the scanner advances until
            // it finds the line end
            case '/':
                if (match('/')) {
                    while (peek() != '\n' && !isAtEnd()) advance();
                }
                else {
                    addToken(TokenType::SLASH);
                }
                break;

            case ' ':
            case '\r':
            case '\t':
                // Skip whitespace
                break;
            case '\n':
                line++;
                break;

            case '"':
                string();
                break;

            default:
                if (isDigit(c)) {
                    number();
                }
                else if (isAlpha(c)) {
                    identifier();
                }
                else {
                    error(line, "Unexpected character");
                }
                break;
        }
    }

    // Returns the current character and advances the pointer
    // Programming nuance: current++ executes the statement involving
    // the incremented variable then increments
    // ++current increments THEN executes
    char advance() {
        return source[current++];
    }

    // Some (simple) tokens do not have literal values, e.g. braces, semicolons
    void addToken(TokenType type) {
        addToken(type, "");
    }

    // But some tokens do, e.g. strings, numerics, necessitating function overloading
    void addToken(TokenType type, const std::string& literal) {
        std::string text = source.substr(start, current - start);
        tokens.push_back(Token(type, text, literal, line));
    }

    // Conditionally consumes the next character if it matches `expected`
    bool match(char expected) {
        if (isAtEnd() || source[current] != expected) return false;
        current++;
        return true;
    }

    // Looks at the current character without consuming it
    char peek() const {
        if (isAtEnd()) return '\0';
        return source[current];
    }

    // Peeks at the next character
    char peekNext() const {
        if (current + 1 >= source.size()) return '\0';
        return source[current + 1];
    }

    // Processes a string literal
    void string() {
        while (peek() != '"' && !isAtEnd()) {
            if (peek() == '\n') line++;
            advance();
        }

        if (isAtEnd()) {
            std::cerr << "Unterminated string at line " << line << "\n";
            return;
        }

        // For the closing quote
        advance();

        // Extract the string value (without the quotes)
        std::string value = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, value);
    }

    // Process a number literal
    void number() {
        while (isDigit(peek())) advance();

        // Look for a fractional part
        if (peek() == '.' && isDigit(peekNext())) {
            advance();  // Consume the dot
            while (isDigit(peek())) advance();
        }

        std::string numberStr = source.substr(start, current - start);
        addToken(TokenType::NUMBER, numberStr);
    }

    // Process an identifier or keyword
    void identifier() {
        while (isAlphaNumeric(peek())) advance();

        std::string text = source.substr(start, current - start);
        TokenType type = identifierType(text);
        addToken(type);
    }

    // Determine if the identifier is a reserved keyword
    TokenType identifierType(const std::string& text) {
        if (text == "if") return TokenType::IF;
        if (text == "else") return TokenType::ELSE;
        if (text == "while") return TokenType::WHILE;
        if (text == "for") return TokenType::FOR;
        if (text == "return") return TokenType::RETURN;
        if (text == "true") return TokenType::TRUE;
        if (text == "false") return TokenType::FALSE;
        return TokenType::IDENTIFIER;
    }

    bool isDigit(char c) {
        return std::isdigit(static_cast<unsigned char>(c));
    }

    bool isAlpha(char c) {
        return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
    }

    bool isAlphaNumeric(char c) {
        return isAlpha(c) || isDigit(c);
    }
};

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
            Scanner scanner(file_contents);
            std::vector<Token> tokens = scanner.scanTokens();
            for (const auto& token : tokens) {
                std::cout << static_cast<int>(token.type) << " "
                    << token.lexeme << " " << token.literal << '\n';
            }
        }
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

void error(int line, const std::string& message) {
    report(line, "", message);
}

void report(int line, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << "] Error" << where << ": " << message << '\n';
    had_error = true;
}