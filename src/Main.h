#pragma once
#if WIN32
#define NOMINMAX
#include <windows.h>
#endif

#if defined(_MSC_VER)
    #define PRI64_X "I64X"
#else
    #include <cinttypes>
    #define PRI64_X PRIX64
#endif

#include <expected>
#include <string>
#include <sstream>
#include <format>
#include <vector>
#include <ranges>


#include "Version.h"
#include "Plugin.h"
#include "Utils.h"

// Signature types and structures
enum class SignatureType : uint32_t {
    IDA = 0,
    x64Dbg,
    Signature_Mask,
    SignatureByteArray_Bitmask
};

typedef struct {
    uint8_t value;
    bool isWildcard;
} SignatureByte;

using Signature = std::vector<SignatureByte>;
