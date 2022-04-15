#include <catch2/catch_test_macros.hpp>

#include <ascript/lexer.hpp>
#include <ascript/scanner.hpp>

using namespace asc;

TEST_CASE("Empty lexer") {
    Scanner scanner("");
    Lexer lexer(scanner);
    REQUIRE(lexer.next().type() == TokenType::EndOfStream);
}

TEST_CASE("Lex strings") {
    Scanner scanner("id = \"Hello!\";");
    Lexer lexer(scanner);
    REQUIRE(lexer.next() == Token(TokenType::Identifier, "id"));
    REQUIRE(lexer.next() == Token(TokenType::Assign));
    REQUIRE(lexer.next() == Token(TokenType::StringLiteral, "Hello!"));
    REQUIRE(lexer.next() == Token(TokenType::Semicolon));
}

TEST_CASE("Lex expression") {
    Scanner scanner("1 + 2");
    Lexer lexer(scanner);
    REQUIRE(lexer.next() == Token(TokenType::Literal, "1"));
    REQUIRE(lexer.next() == Token(TokenType::Plus));
    REQUIRE(lexer.next() == Token(TokenType::Literal, "2"));
    REQUIRE(lexer.next() == Token(TokenType::EndOfStream));
}
