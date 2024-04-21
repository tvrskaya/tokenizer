#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <variant>
#include <unordered_map>
#include <string.h>

// (1 + 2) * 3 / 4 + 5 * (6 - 7)

// 1. Токенизация
// 2. Парсер (построение дерева разбора выражения)

// +, -, *, /, %

//1. Добавить токены max, sqr - сомнительно но окэй
//2. Добавить все необходимые классы в определение Token - сомнительно но окэй
//3. реализовать ParseName - сомнительно но окэй
//4. Закончить Tokenize

class Tokenizer {
private:
    struct OpeningBracket {    };

    struct ClosingBracket {    };

    struct Number {
      int value;
    };

    struct UnknownToken {
      std::string value;
    };

    struct MinToken {};

    struct AbsToken {};

    struct Plus {};

    struct Minus {};

    struct Multiply {};

    struct Modulo {};

    struct Divide {};

    struct Comma {};
public:
    using Token = std::variant<OpeningBracket, ClosingBracket, Number, UnknownToken, MinToken, AbsToken, Plus, Minus, Multiply, Modulo, Divide, Comma>;
private:
    Number ParseNumber(const std::string& input, size_t& pos) {
      int value = 0;
      auto symbol = static_cast<unsigned char>(input[pos]);
      while (std::isdigit(symbol)) {
        value = value * 10 + ToDigit(symbol);
        if (pos == input.size() - 1) {
          break;
        }
        symbol = static_cast<unsigned char>(input[++pos]);
      }
      return Number{value};
    }

    std::string ParseName(const std::string& input, size_t& pos) {
        auto symbol = static_cast<const char*>(&input[pos]);
        for (auto it = kName2Token.begin(); it != kName2Token.end(); it++) {
            if (strncmp(symbol, it->first.c_str(), it->first.size()) == 0) {
                pos += it->first.size();
                return it->first;
            }
        }
        return "";//Нет такого токена
    }

    const std::unordered_map<char, Token> kSymbol2Token {
        {'(', OpeningBracket{}}, {')', ClosingBracket{}}, {'+', Plus{}}, {'-', Minus{}}, {'*', Multiply{}}, {'%', Modulo{}}, {'/', Divide{}}, {',', Comma{}}
    };
    const std::unordered_map<std::string, Token> kName2Token {
        {"min", MinToken{}}, {"abs", AbsToken{}}
    };

    int ToDigit(unsigned char symbol) {
      return symbol - '0';
    }
public:
    std::vector<Token> Tokenize(const std::string& input) {
      std::vector<Token> tokens;
      const size_t size = input.size();
      size_t pos = 0;
      while (pos < size) {
        const auto symbol = static_cast<unsigned char>(input[pos]);
        if (std::isspace(symbol)) {
          ++pos;
        } else if (std::isdigit(symbol)) {
          tokens.emplace_back(ParseNumber(input, pos));
        } else if (auto it = kSymbol2Token.find(symbol); it != kSymbol2Token.end()) {
          tokens.emplace_back(it->second);
          ++pos;
        } else if (auto it = kName2Token.find(ParseName(input, pos)); it != kName2Token.end()) {
            tokens.emplace_back(it->second);
        } else {
            tokens.emplace_back(UnknownToken{input});
            ++pos;
        }
      }
      return tokens;
    }

};

int main(void) {
    std::string expression = "(1+2) * abs(-5) - min(5, 3)f";
    Tokenizer tk;
    auto tokens = tk.Tokenize(expression);
    for (size_t i = 0; i < tokens.size(); i++) {
        //Получаем верные номера токенов(я проверил)
        std::cout << tokens[i].index() << " ";
    }
    return 0;
}