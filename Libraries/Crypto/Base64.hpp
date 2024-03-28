/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Base64.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 19 Jul 2017 11:21:34
 *
 * Note:             https://en.wikipedia.org/wiki/Base64
 *
 ******************************************************************************/
#ifndef LIBX_CRYPTO_BASE64_HPP_
#define LIBX_CRYPTO_BASE64_HPP_

#include "Libraries/Utils/String.hpp"
#include "Libraries/Values/ValueArray.hpp"

namespace libX {

namespace crypto {

class Base64 {
private:
    static const String CODES;
public:
    /// @fn     Decode
    /// @brief  None
    /// @param  STRING input
    /// @return None
    static lstu8_t Decode(const String& input);

    /// @fn     Encode
    /// @brief  None
    /// @param  VALUEARRAY<BYTE>& input
    /// @return None
    static String Encode(lstu8_t& input);

    /// @fn     Encode
    /// @brief  None
    /// @param  STRING in
    /// @return None
    static String Encode(const String& input);
};

}
}

#endif /* !LIBX_CRYPTO_BASE64_HPP_ */
