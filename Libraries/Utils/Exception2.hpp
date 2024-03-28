//
// Created by tainguyen on 4/23/20.
//

#ifndef HCG1_EXCEPTION2_HPP
#define HCG1_EXCEPTION2_HPP

#include <cstdarg>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
//#include "Controller/HcDefs.hpp"
//#include "Session/net/JsonCommand.hpp"

namespace libX {
namespace utils {

class Exception2 : public std::exception {
public:
    Exception2();
    Exception2(int threadID, const char* file, const char* func, int line, int code) noexcept ;
    //Exception2(int threadID, const char* file, const char* func, int line, int code, const JsonCommand& data) noexcept ;
    Exception2(String  payload, int threadID, const char* file, const char* func, int line, int code) noexcept ;
    Exception2(int threadID, const char* file, const char* func, int line, int code, const char* message, ...) noexcept ;
    //Exception2(int threadID, const char* file, const char* func, int line, int code, const JsonCommand& data, const char* message, ...) noexcept ;
    Exception2(String  payload, int threadID, const char* file, const char* func, int line, int code, const char* message, ...) noexcept ;
    Exception2(const Exception2& copied) noexcept ;
    Exception2& operator= (const Exception2& copied) noexcept ;
    ~Exception2() noexcept override;

private:
    String what_;
    int errCode_;
    int threadID_;
    String file_;
    String func_;
    int line_;
    String message_;
    //JsonCommand userData_;
    bool userDataAvailable_;
    String payload_;
    bool payloadAvailable_;

public:
    const char* what() const noexcept override;
    int errorCode() const noexcept;
    int threadID() const noexcept;
    String file() const noexcept;
    String func() const noexcept;
    int line() const noexcept;
    String message() const noexcept;
    //void setUserData(const JsonCommand& data) noexcept;
    //JsonCommand getUserData() const noexcept;
    bool isUserDataAvailable() const noexcept ;
    void setPayload(const String& data) noexcept;
    String getPayload() const noexcept;
    bool isPayloadAvailable() const noexcept ;
};

typedef Exception2* Exception2Ref;

}
}

#define ExceptionPure(e) Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e)
#define ExceptionMessage(e, x, args...) Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e, x, ##args)
#define ExceptionMessageWithData(e, d, x, args...) Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e, d, x, ##args)
#define ExceptionMessageWithPayload(e, d, x, args...) Exception2(d, pthread_self(), __FILENAME__, __func__, __LINE__, e, x, ##args)
#define ThrowExceptionPure(e) ST (throw Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e);)
#define ThrowExceptionMessage(e, x, args...) ST (throw Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e, x, ##args);)
#define ThrowExceptionMessageWithData(e, d, x, args...) ST (throw Exception2(pthread_self(), __FILENAME__, __func__, __LINE__, e, d, x, ##args);)
#define ThrowExceptionMessageWithPayload(e, d, x, args...) ST (throw Exception2(d, pthread_self(), __FILENAME__, __func__, __LINE__, e, x, ##args);)

#define require_action(X, E)            \
    do {                                \
        if (!(X)) {                     \
            ThrowExceptionPure(E);      \
        }                               \
    } while(0)

#define require_action_string(X, E, M)      \
    do {                                    \
        if (!(X)) {                         \
            ThrowExceptionMessage(E , M);   \
        }                                   \
    } while(0)

#define assert_action(X)                \
    do {                                \
        if (!(X)) return;               \
    } while(0)

#endif //HCG1_EXCEPTION2_HPP
