#include "CAD/language/program.hpp"

#include <array>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>

using namespace CAD;
using namespace language;

#define PRE_RETURN_SAVES   {if(compileInfo.infoPath != nullptr){                                                    \
                                std::filesystem::path path = compileInfo.parentPath / *compileInfo.infoPath;        \
                                if(saveStringVecToFile(res.info, path)){                                            \
                                    std::cout << "Saved info to " << path.string() << std::endl;                    \
                                }else{                                                                              \
                                    std::cout << "Failed to save info to " << path.string() << std::endl;           \
                                }                                                                                   \
                            }                                                                                       \
                            if(compileInfo.warningsPath != nullptr){                                                \
                                std::filesystem::path path = compileInfo.parentPath / *compileInfo.warningsPath;    \
                                if(saveStringVecToFile(res.warnings, path)){                                        \
                                    std::cout << "Saved warnings to " << path.string() << std::endl;                \
                                }else{                                                                              \
                                    std::cout << "Failed to save warnings to " << path.string() << std::endl;       \
                                }                                                                                   \
                            }                                                                                       \
                            if(compileInfo.errorsPath != nullptr){                                                  \
                                std::filesystem::path path = compileInfo.parentPath / *compileInfo.errorsPath;      \
                                if(saveStringVecToFile(res.errors, path)){                                          \
                                    std::cout << "Saved errors to " << path.string() << std::endl;                  \
                                }else{                                                                              \
                                    std::cout << "Failed to save errors to " << path.string() << std::endl;         \
                                }                                                                                   \
                            }}

#define PRE_RETURN_DELETES {if(deleteInfo)      {delete res.info;       }\
                            if(deleteWarnings)  {delete res.warnings;   }\
                            if(deleteErrors)    {delete res.errors;     }\
                            if(deleteTokens)    {delete res.tokens;     }}

struct token{
public:
    enum class TYPE : uint8_t {
        // Begins with a letter, ends before non-alphanumeric
        WORD = 0,
        // Starts and ends in "" or ''
        STRING = 1,
        // Starts with number, ends before non-number
        NUMBER = 2,
        // Any non-bracket (except angle), non-space, non-comma, and non-semicolon symbol (or combination of symbols)
        // `~!@#$%^&*-_=+\|:./?<>
        SYMBOL = 3,
        // Any left bracket ({[
        LBRACKET = 4,
        // Any right bracket )}]
        RBRACKET = 5,
        // Semicolon, comma
        BREAK = 6
    };
    static const std::array<std::string, 7> TOKEN_STRINGS;
    TYPE type;
    std::string value;
    std::size_t row;
};
const std::array<std::string, 7> token::TOKEN_STRINGS = {
    "WORD", "STRING", "NUMBER", "SYMBOL", "LBRACKET", "RBRACKET", "BREAK"
};
struct command{
    enum class CMD : uint8_t{
        // Create variable with undefined value
        DECLARE,
        // Create variable with undeclared value
        DECLARE_FORWARD,
        // Set value of variable
        ASSIGN,
        // if, else, switch, while, for, do-while
        KEYWORD_CONTROL,
        // break, return
        KEYWORD_ESCAPE,
        // Call built-in function
        FUNCTION_STD,
        // Call custom function
        FUNCTION_CUSTOM,
        // Push Scope
        PUSH_SCOPE,
        // Pop Scope
        POP_SCOPE
    };
    CMD cmd;
    std::vector<token> tokens;
    std::vector<std::unique_ptr<command>> children;
};
const std::unordered_map<char, const std::vector<std::string>> SYMBOLS = {
    // + (add), ++ (pre/post increment), += (add and assign)
    {'+',{"+", "="}},
    // - (sub), -- (pre/post decrement), -= (sub and assign), -> (NYI right arrow)
    {'-',{"-", "=", ">"}},
    // * (mult), ** (exponent), *= (mult and assign), **= (exponent and assign)
    {'*',{"*=", "*", "="}},
    // / (div), /= (div and assign)
    {'/',{"="}},
    // % (mod), %= (mod and assign)
    {'%',{"="}},
    // & (bitwise and, reference), && (boolean and), &= (bitwise and and assign)
    {'&',{"&", "="}},
    // | (bitwise or), || (boolean or, left if truthy else right), |= (bitwise or and assign), |. (conditional truthy access)
    {'|',{"|", "=", "."}},
    // ! (boolean not), != (value non-equality), !== (reference non-equality), !~= (fuzzy non-equality)
    {'!',{"~=","==","="}},
    // ~ (bitwise not), ~= (bitwise not and assign), ~== (fuzzy equality)
    {'~',{"==","="}},
    // ^ (bitwise xor), ^= (bitwise xor and assign)
    {'^',{"="}},
    // = (assign), == (value equality), === (reference equality), =? (assign if RHS value)
    {'=',{"==", "=", "?"}},
    // < (less than), <= (less than or equal), << (bitshift left), <- (NYI left arrow)
    {'<',{"=", "<", "-"}},
    // > (greater than), >= (greater than or equal), >> (bitshift right)
    {'>',{"=", ">"}},
    // ? (NYI ternary part 1), ?? (left it value else right), ?= (assign if LHS empty), ?. (conditional value access)
    {'?',{"?", "=", "."}},
    // : (NYI ternary part 2), (NYI linear range)
    {':',{}},
    // . (access), (NYI range)
    {'.',{}},
    // $ (import/export)
    {'$',{}},
    // @ (metadata)
    {'@',{}},
    // NYI preprocessor?
    {'#',{}}
};
const std::string LBRACKETS = "({[";
const std::string RBRACKETS = ")}]";
const std::vector<std::string> TYPES_STD = {
    // 64 bit integer
    "int",
    // 64 bit unsigned integer
    "uint",
    // 64 bit double precision floating point number
    "float",
    // String
    "string"
};

bool saveStringVecToFile(std::vector<std::string> *strs, std::filesystem::path path){
    std::ofstream fStream{path};
    if(!fStream.is_open()){
        return false;
    }
    for(const std::string s : *strs){
        fStream << s << '\n';
    }
    return true;
}

bool Program::CompileInfo::readFile(std::filesystem::path filePath){
    std::ifstream fstream{filePath};
    if(!fstream.is_open()){
        return false;
    }
    std::ostringstream buff;
    buff << fstream.rdbuf();
    this->file = buff.str();
    return true;
}
bool Program::compile(Program::CompileInfo compileInfo){
    // Create logs if required and not provided
    const std::string &file = compileInfo.file;
    struct Res{
        std::vector<std::string> *info, *warnings, *errors, *tokens;
    };
    Res res;
    res.info = compileInfo.info;
    res.warnings = compileInfo.warnings;
    res.errors = compileInfo.errors;
    res.tokens = compileInfo.tokens;

    bool deleteInfo = false;
    bool deleteWarnings = false;
    bool deleteErrors = false;
    bool deleteTokens = false;

    if(res.info == nullptr && compileInfo.infoPath != nullptr){
        res.info = new std::vector<std::string>();
        deleteInfo = true;
    }
    if(res.warnings == nullptr && compileInfo.warningsPath != nullptr){
        res.warnings = new std::vector<std::string>();
        deleteWarnings = true;
    }
    if(res.errors == nullptr && compileInfo.errorsPath != nullptr){
        res.errors = new std::vector<std::string>();
        deleteErrors = true;
    }
    if(res.tokens == nullptr && compileInfo.tokensPath != nullptr){
        res.tokens = new std::vector<std::string>();
        deleteTokens = true;
    }


    // Tokenize
    std::size_t index = 0;
    std::size_t row = 0;
    std::vector<token> tokens;
    std::vector<std::pair<uint32_t, char>> brackets;
    while(index < file.size()){
        char c = file.at(index);
        if(c == '/'){
            // Check for comments or other meanings of /
            if(index+1 == file.size()){
                // / cannot be the last character in the file
                if(res.errors != nullptr){
                    res.errors->push_back("Invalid use of \"/\"");
                }
                this->valid = false;
                PRE_RETURN_SAVES
                PRE_RETURN_DELETES
                return false;
            }else if(file.at(index+1) == '/'){
                // // means comment, ignore rest of line
                index += 1;
                while(++index < file.size() && file.at(index) != '\n'){}
                ++row;
            }else if(file.at(index+1) == '*'){
                // /* means multiline comment, ignore until */
                index += 1;
                while(++index+1 < file.size() && file.at(index) != '*' && file.at(index+1) != '/'){
                    if(file.at(index) == 'n'){
                        ++row;
                    }
                }
                ++index;
                if(!(file.at(index-1) == '*' && file.at(index) == '/')){
                    if(res.errors != nullptr){
                        res.errors->push_back("Unclosed multiline comment");
                    }
                    PRE_RETURN_SAVES
                    PRE_RETURN_DELETES
                    return false;
                }
                ++index;
            }else if(file.at(index+1) == '='){
                // divided by equals symbol
                tokens.push_back({token::TYPE::SYMBOL,"/=",row});
                index += 2;
            }else{
                // divided by symbol
                tokens.push_back({token::TYPE::SYMBOL,"/",row});
                index += 1;
            }
        }else if(c == '.' || (c >= '0' && c <= '9')){
            // Check if number, or find correct operator (.)
            char c2 = file.at(index+1);
            if(c == '0' && c2 == 'b'){
                // 0b means binary (0 and 1)
                std::string bin{'b'};
                ++index;
                char cn = file.at(++index);
                while(cn == '0' || cn == '1'){
                    bin += {cn};
                    cn = file.at(++index);
                }
                tokens.push_back({token::TYPE::NUMBER,bin,row});
            }else if(c == '0' && c2 == 'x'){
                // 0x means hexadecimal (0-9, A-F), preferring capital
                std::string hex{'x'};
                ++index;
                char cn = file.at(++index);
                while(('0' <= cn && cn <= '9') || ('a' <= cn && cn <= 'f') || ('A' <= cn && cn <= 'F')){
                    hex += {('a' <= cn && cn <= 'f') ? (char)(cn - 'a' + 'A') : cn};
                    cn = file.at(++index);
                }
                tokens.push_back({token::TYPE::NUMBER,hex,row});
            }if(c == '.' && c2 == '.'){
                // Consecutive dots may be implemented in the future
                std::size_t i=2;
                ++index;
                while(file.at(++index) == '.'){
                    ++i;
                }
                if(res.warnings != nullptr){
                    res.warnings->push_back("Multiple consecutive . not yet supported (found " + std::to_string(i) + ")");
                }
            }else if((c2 >= '0' && c2 <= '9') || (c2 == '.' && file.at(index+2) >= '0' && file.at(index+2) <= '9')){
                bool dotFound = c == '.' || c2 == '.';
                // If its a number, start collecting the number
                std::string num = {c};
                num += {c2};
                index += 2;
                char cn = file.at(index);
                while((cn >= '0' && cn <= '9') || (!dotFound && cn == '.')){
                    num += {cn};
                    cn = file.at(++index);
                }
                tokens.push_back({token::TYPE::NUMBER,num,row});
            }else{//if(c >= '0' && c <= '9'){
                // Handle single digit numbers
                tokens.push_back({token::TYPE::NUMBER,{c},row});
                index += 1;
            }
        }else{
            // Check if next character is symbol
            const auto symbolTable = SYMBOLS.find(c);
            if(symbolTable != SYMBOLS.end()){
                // If symbol found, check if compound symbol
                bool foundMatch = false;
                for(const auto &symbol : symbolTable->second){
                    bool match = true;
                    for(uint32_t i=0;i<symbol.size();++i){
                        if(symbol.at(i) != file.at(index+i+1)){
                            match = false;
                            break;
                        }
                    }
                    if(match){
                        tokens.push_back({token::TYPE::SYMBOL, std::string{c} + symbol, row});
                        index += 1 + symbol.size();
                        foundMatch = true;
                        break;
                    }
                }
                if(!foundMatch){
                    tokens.push_back({token::TYPE::SYMBOL,{c},row});
                    ++index;
                }
            }else if(LBRACKETS.find(c) != std::string::npos){
                // Left bracket
                tokens.push_back({token::TYPE::LBRACKET,{c},row});
                brackets.push_back({row, c});
                ++index;
            }else if(RBRACKETS.find(c) != std::string::npos){
                // Right bracket
                tokens.push_back({token::TYPE::RBRACKET,{c},row});
                if(brackets.empty()){
                    if(res.errors != nullptr){
                        res.errors->push_back("Unpaired closing bracket " + std::string{c} + " on line " + std::to_string(row));
                    }
                    PRE_RETURN_SAVES
                    PRE_RETURN_DELETES
                    return false;
                }
                std::pair<uint32_t, char> &prevBracket = brackets.back();
                char pbs = prevBracket.second;
                if((pbs=='('&&c==')') || (pbs=='['&&c==']') || (pbs=='{'&&c=='}')){
                    brackets.pop_back();
                }else{
                    if(res.errors != nullptr){
                        res.errors->push_back("Mismatched brackets " + std::string{prevBracket.second} + " on line " + std::to_string(prevBracket.first) + " and " + std::string{c} + " on line " + std::to_string(row));
                    }
                    PRE_RETURN_SAVES
                    PRE_RETURN_DELETES
                    return false;
                }
                ++index;
            }else if(c == '"' || c == '\''){
                // Collect string, and translate special characters
                std::string str;
                bool escape = false;
                char cn = file.at(++index);
                while(cn != c || escape){
                    if(escape){
                        switch(cn){
                            case '\\': str += "\\";break;
                            case 't': str += "\t";break;
                            case 'n': str += "\n";break;
                            case '0': str += "\0";break;
                            case '"': str += "\"";break;
                            case '\'': str += "'";break;
                            case '?': str += "\?";break;
                            case 'a': str += "\a";break;
                            case 'b': str += "\b";break;
                            case 'f': str += "\f";break;
                            case 'r': str += "\r";break;
                            case 'v': str += "\v";break;
                            default:{
                                if(res.errors != nullptr){
                                    res.errors->push_back("Unknown escaped character: "+std::string{cn});
                                }
                                PRE_RETURN_SAVES
                                PRE_RETURN_DELETES
                                return false;
                            }
                        }
                        escape = false;
                    }else{
                        if(cn == '\\'){
                            escape = true;
                        }else{
                            str += {cn};
                        }
                    }
                    cn = file.at(++index);
                }
                tokens.push_back({token::TYPE::STRING,str,row});
                ++index;
            }else if(c == ';' || c == ','){
                // break
                ++index;
                tokens.push_back({token::TYPE::BREAK,{c},row});
            }else if(c == ' ' || c == '\t' || c == '\n'){
                // Whitespace can be ignored
                char cn = file.at(index);
                while(cn == ' ' || cn == '\t' || cn == '\n'){
                    if(cn == '\n'){
                        ++row;
                    }
                    cn = file.at(++index);
                }
            }else{
                // word
                std::string word{c};
                char cn = file.at(++index);
                while(('a' <= cn && cn <= 'z') || ('A' <= cn && cn <= 'Z') || ('0' <= cn && cn <= '9') || cn == '_'){
                    word += {cn};
                    cn = file.at(++index);
                }
                tokens.push_back({token::TYPE::WORD,word,row});
            }
        }
    }
    // Check unclosed brackets
    if(!brackets.empty()){
        if(res.errors != nullptr){
            std::pair<uint32_t, char> &lastBracket = brackets.back();
            res.errors->push_back("Unpaired opening brackets (" + std::to_string(brackets.size()) + "), last one is " + std::string{lastBracket.second} + " on line " + std::to_string(lastBracket.first));
        }
        PRE_RETURN_SAVES
        PRE_RETURN_DELETES
        return false;
    }
    // Log all tokens if possible
    if(res.tokens != nullptr){
        for(const auto t : tokens){
            std::string tStr = token::TOKEN_STRINGS[(std::size_t)t.type] + " (" + std::to_string(t.row) + "): " + t.value;
            res.tokens->push_back(tStr);
        }
    }

    // Check if flag for skipping next steps is set
    if(!(compileInfo.flags &= FLAG_COMPILER_TOKENIZE_ONLY)){
        // Parse
        // Variable names will be replaced with a number to reduce runtime hashing overhead
        std::unordered_map<std::string, std::size_t> variableTable;
        std::vector<command> commands;
        for(std::size_t index=0;index<tokens.size();++index){

        }
    }

    // Save logs to file (commented lines are in macro, tokens doesn't exist until end so it is not included in macro)
    PRE_RETURN_SAVES
    // if(compileInfo.infoPath != nullptr){
    //     std::filesystem::path path = compileInfo.parentPath / *compileInfo.infoPath;
    //     if(saveStringVecToFile(res.info, path)){
    //         std::cout << "Saved info to " << path.string() << std::endl;
    //     }else{
    //         std::cout << "Failed to save info to " << path.string() << std::endl;
    //     }
    // }
    // if(compileInfo.warningsPath != nullptr){
    //     std::filesystem::path path = compileInfo.parentPath / *compileInfo.warningsPath;
    //     if(saveStringVecToFile(res.warnings, path)){
    //         std::cout << "Saved warnings to " << path.string() << std::endl;
    //     }else{
    //         std::cout << "Failed to save warnings to " << path.string() << std::endl;
    //     }
    // }
    // if(compileInfo.errorsPath != nullptr){
    //     std::filesystem::path path = compileInfo.parentPath / *compileInfo.errorsPath;
    //     if(saveStringVecToFile(res.errors, path)){
    //         std::cout << "Saved errors to " << path.string() << std::endl;
    //     }else{
    //         std::cout << "Failed to save errors to " << path.string() << std::endl;
    //     }
    // }
    if(compileInfo.tokensPath != nullptr){
        std::filesystem::path path = compileInfo.parentPath / *compileInfo.tokensPath;
        if(saveStringVecToFile(res.tokens, path)){
            std::cout << "Saved tokens to " << path.string() << std::endl;
        }else{
            std::cout << "Failed to save tokens to " << path.string() << std::endl;
        }
    }
    // Delete logs if owned locally (commented lines are in macro)
    PRE_RETURN_DELETES
    // if(deleteInfo){
    //     delete res.info;
    // }
    // if(deleteWarnings){
    //     delete res.warnings;
    // }
    // if(deleteErrors){
    //     delete res.errors;
    // }
    // if(deleteTokens){
    //     delete res.tokens;
    // }

    return true;
}