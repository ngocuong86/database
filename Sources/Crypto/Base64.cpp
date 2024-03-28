//
// Created by TrungTQ on 19 Jul 2017 11:21:55
//

#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Crypto/Base64.hpp"

namespace libX {
namespace crypto {

const String Base64::CODES = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

/// @fn     Decode
/// @brief  None
/// @param  STRING input
/// @return None
lstu8_t
Base64::Decode(
    const String& input
) {
    if (input.length() %4 == 0) {
        lstu8_t deCoded(input.length() * 3 / 4 - (input.find("=") > 0 ? (input.length() - input.find("=")) : 0));
        i32_t j = 0;
        i32_p pb = new i32_t[4];
        for (u32_t i = 0; i < input.length(); i += 4) {
            pb[0] = CODES.find(input[i]);
            pb[1] = CODES.find(input[i + 1]);
            pb[2] = CODES.find(input[i + 2]);
            pb[3] = CODES.find(input[i + 3]);
            deCoded[j++] = (u8_t) (pb[0] << 2 | pb[1] >> 4);
            if (pb[2] < 64) {
                deCoded[j++] = (u8_t) (pb[1] << 4 | pb[2] >> 2);
                if (pb[3] < 64) {
                    deCoded[j++] = (u8_t) (pb[2] << 6 | pb[3]);
                }
            }
        }
        return deCoded;
    }
    THROW_MESSAGE("Invalid base64 input");
}

/// @fn     Encode
/// @brief  None
/// @param  VALUEARRAY<BYTE>& in
/// @return None
String
Base64::Encode(
    lstu8_t& input
) {
    String strOut;
    strOut.reserve((input.Size() * 4) / 3);
    u32_t b = 0;
    for (u32_t i = 0; i < input.Size(); i+= 3) {
        b = (input[i] & 0xFC) >> 2;
        strOut.append(1, CODES[b]);
        b = (input[i] & 0x03) << 4;
        if (i + 1 < input.Size()) {
            b |= (input[i + 1] & 0xF0) >> 4;
            strOut.append(1, CODES[b]);
            b = (input[i + 1] & 0x0F) << 2;
            if (i + 2 < input.Size()) {
                b |= (input[i + 2] & 0xC0) >> 6;
                strOut.append(1, CODES[b]);
                b = input[i + 2] & 0x3F;
                strOut.append(1, CODES[b]);
            } else {
                strOut.append(1, CODES[b]);
                strOut.append("=");
            }
        } else {
            strOut.append(1, CODES[b]);
            strOut.append("==");
        }
    }
    return strOut;
}

/// @fn     Encode
/// @brief  None
/// @param  STRING in
/// @return None
String
Base64::Encode(
    const String& input
) {
    ValueArray<u8_t> ainput(input.length());
    for (u32_t i = 0; i < ainput.Size(); i++) {
        ainput[i] = input[i];
    }
    return Encode(ainput);
}

}
}
