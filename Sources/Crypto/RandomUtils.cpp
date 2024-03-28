//
// Created by TrungTQ on 19 Jul 2017 09:26:32
//

#include "Libraries/Crypto/Base64.hpp"
#include "Libraries/Utils/Random.hpp"
#include "Libraries/Utils/RandomUtils.hpp"

namespace libX {
namespace utils {

/// @fn     RandomNumber
/// @brief  None
/// @param  None
/// @return DWORD
u32_t
RandomUtils::RandomNumber(
) {
    Random_t random;
    random.Seed();
    return random.Next();
}

/// @fn     RandomString
/// @brief  None
/// @param  None
/// @return STRING
String
RandomUtils::RandomString(
    i32_t iSize
) {
    const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    String strRet;
    Random_t random;
    random.Seed();
    for (i32_t i = 0; i < iSize; i++) {
        strRet.push_back(alphanum[random.Next(sizeof(alphanum) - 1)]);
    }
    return strRet.substr(0, iSize);
}

/// @fn     RandomBinaryString
/// @brief  None
/// @param  IDWORD iSize
/// @param  BOOL   boDoBase64
/// @return STRING
String
RandomUtils::RandomBinaryString(
    i32_t   iSize,
    bool_t  boDoBase64
) {
    String strRet;
    Random_t random;
    random.Seed();
    for (i32_t i = 0; i < iSize; i++) {
        strRet.push_back(random.NextChar());
    }
    if (boDoBase64) {
        strRet = crypto::Base64::Encode(strRet);
    }
    return strRet;
}

/// @fn     NextBytes
/// @brief  None
/// @param  lstu8_t& rbyte
/// @return None
void
RandomUtils::NextBytes(
    lstu8_t& rbyte
) {
    Random_t random;
    random.Seed();
    for (u32_t i = 0; i < rbyte.Size(); i++) {
        rbyte[i] = random.NextChar();
    }
}

}
}
