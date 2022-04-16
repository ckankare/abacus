#include <catch2/catch_test_macros.hpp>

#include <ascript/lexer.hpp>
#include <ascript/scanner.hpp>

using namespace asc;

TEST_CASE("Empty lexer") {
    Scanner scanner("");
    Lexer lexer(scanner);
    CHECK(lexer.next().type() == TokenType::EndOfStream);
}

TEST_CASE("Lex strings") {
    Scanner scanner("id = \"Hello!\";");
    Lexer lexer(scanner);
    CHECK(lexer.next() == Token(TokenType::Identifier, "id"));
    CHECK(lexer.next() == Token(TokenType::Assign));
    CHECK(lexer.next() == Token(TokenType::StringLiteral, "Hello!"));
    CHECK(lexer.next() == Token(TokenType::Semicolon));
}

TEST_CASE("Lex expression") {
    Scanner scanner("1 + 2");
    Lexer lexer(scanner);
    CHECK(lexer.next() == Token(TokenType::Literal, "1"));
    CHECK(lexer.next() == Token(TokenType::Plus));
    CHECK(lexer.next() == Token(TokenType::Literal, "2"));
    CHECK(lexer.next() == Token(TokenType::EndOfStream));
}

TEST_CASE("Lexer look_ahead") {
    Scanner scanner("foo = bar - 42");
    Lexer lexer(scanner);
    {
        auto look_ahead = lexer.look_ahead<5>();
        CHECK(look_ahead[0] == Token(TokenType::Identifier, "foo"));
        CHECK(look_ahead[1] == Token(TokenType::Assign));
        CHECK(look_ahead[2] == Token(TokenType::Identifier, "bar"));
        CHECK(look_ahead[3] == Token(TokenType::Minus));
        CHECK(look_ahead[4] == Token(TokenType::Literal, "42"));
    }

    CHECK(lexer.next() == Token(TokenType::Identifier, "foo"));
    CHECK(lexer.next() == Token(TokenType::Assign));
    CHECK(lexer.next() == Token(TokenType::Identifier, "bar"));
    {
        auto look_ahead = lexer.look_ahead<4>();
        CHECK(look_ahead[0] == Token(TokenType::Minus));
        CHECK(look_ahead[1] == Token(TokenType::Literal, "42"));
        CHECK(look_ahead[2] == Token(TokenType::EndOfStream));
        CHECK(look_ahead[3] == Token(TokenType::EndOfStream));
    }
    CHECK(lexer.next() == Token(TokenType::Minus));
    CHECK(lexer.next() == Token(TokenType::Literal, "42"));
    CHECK(lexer.next() == Token(TokenType::EndOfStream));
}

TEST_CASE("Lexer next_if") {
    Scanner scanner("true == 0 || 1");
    Lexer lexer(scanner);
    {
        auto next_if = lexer.next_if(TokenType::Identifier, TokenType::Equals);
        REQUIRE(next_if.has_value());
        CHECK(next_if.value()[0] == Token(TokenType::Identifier, "true"));
        CHECK(next_if.value()[1] == Token(TokenType::Equals));
    }
    {
        auto next_if = lexer.next_if(TokenType::Identifier, TokenType::Equals);
        REQUIRE(!next_if.has_value());
    }
}
