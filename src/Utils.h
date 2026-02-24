#pragma once
#if WIN32
#include <windows.h>
#endif

// To fix regex_error(error_stack) for longer signatures
#define _REGEX_MAX_STACK_COUNT 20000

#include <cstdint>
#include <vector>
#include <regex>
#include <string_view>

// Generic utility functions

bool SetClipboardText( std::string_view text );
bool GetRegexMatches( std::string string, std::regex regex, std::vector<std::string>& matches );
constexpr auto BIT( uint8_t index ) {
	return 1LLU << index;
}
constexpr auto SET_BIT( auto x, uint8_t index ) {
	return x | BIT( index );
}
constexpr bool GET_BIT( auto x, uint8_t index ) {
	return x & BIT( index );
}
