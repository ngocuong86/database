//
// Created by tainguyen on 4/23/20.
//

#include "Libraries/Log/LogPlus.hpp"
#include "common.hpp"
#include <utility>
#include "Exception2.hpp"

Exception2::Exception2() {
    errCode_ = ErrorCode::ErrUnknown;
    what_ = "";
    file_ = "";
    func_ = "";
    message_ = "";
    errCode_ = ErrorCode::ErrUnknown;
    threadID_ = -1;
    line_ = -1;
    userDataAvailable_ = FALSE;
    payload_ = "";
    payloadAvailable_ = FALSE;
}

Exception2::Exception2(
    int threadID,
    const char *file,
    const char *func,
    int line,
    int code
) noexcept :
    errCode_(code),
    threadID_(threadID),
    file_(file),
    func_(func),
    line_(line),
    message_(),
    //userData_(),
    userDataAvailable_(FALSE),
    payloadAvailable_(FALSE)
{
    char buffer[1024];
    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]",
                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);
    what_.assign(buffer, cx);
}

//Exception2::Exception2(
//    int threadID,
//    const char *file,
//    const char *func,
//    int line,
//    int code,
//    const JsonCommand &data
//) noexcept :
//    errCode_(code),
//    threadID_(threadID),
//    file_(file),
//    func_(func),
//    line_(line),
//    message_(),
//    userData_(data),
//    userDataAvailable_(TRUE),
//    payloadAvailable_(FALSE)
//{
//    char buffer[1024];
//    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]",
//                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);
//    what_.assign(buffer, cx);
//}

Exception2::Exception2(
    String payload,
    int threadID,
    const char *file,
    const char *func,
    int line,
    int code
) noexcept :
    errCode_(code),
    threadID_(threadID),
    file_(file),
    func_(func),
    line_(line),
    message_(),
    payload_(std::move(payload)),
    userDataAvailable_(FALSE),
    payloadAvailable_(TRUE)
{
    char buffer[1024];
    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]",
                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);
    what_.assign(buffer, cx);
}

//Exception2::Exception2(
//    int threadID,
//    const char *file,
//    const char *func,
//    int line,
//    int code,
//    const char *message,
//    ...
//) noexcept :
//    errCode_(code),
//    threadID_(threadID),
//    file_(file),
//    func_(func),
//    line_(line),
//    userData_(),
//    userDataAvailable_(FALSE),
//    payloadAvailable_(FALSE)
//{
//    char buffer[1024];
//    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]: ",
//                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);    what_.assign(buffer, cx);
//
//    va_list args;
//    va_start(args, message);
//    char linebuffer[1024] = { 0 };
//    vsnprintf(linebuffer, sizeof(linebuffer), message, args);
//    message_ = linebuffer;
//    va_end(args);
//
//    what_.append(message_);
//}

//Exception2::Exception2(
//    int threadID,
//    const char *file,
//    const char *func,
//    int line,
//    int code,
//    const JsonCommand &data,
//    const char *message,
//    ...
//) noexcept :
//    errCode_(code),
//    threadID_(threadID),
//    file_(file),
//    func_(func),
//    line_(line),
//    message_(),
//    userData_(data),
//    userDataAvailable_(TRUE),
//    payloadAvailable_(FALSE)
//{
//    char buffer[1024];
//    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]: ",
//                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);    what_.assign(buffer, cx);
//
//    va_list args;
//    va_start(args, message);
//    char linebuffer[1024] = { 0 };
//    vsnprintf(linebuffer, sizeof(linebuffer), message, args);
//    message_ = linebuffer;
//    va_end(args);
//
//    what_.append(message_);
//}

Exception2::Exception2(
    String payload,
    int threadID,
    const char *file,
    const char *func,
    int line,
    int code,
    const char *message,
    ...
) noexcept :
    errCode_(code),
    threadID_(threadID),
    file_(file),
    func_(func),
    line_(line),
    message_(),
    payload_(std::move(payload)),
    userDataAvailable_(FALSE),
    payloadAvailable_(TRUE)
{
    char buffer[1024];
    int cx = snprintf(buffer, 1024, "Error[%s] in thread[%d] file[%s] func[%s] line[%d]: ",
                      GetErrString(errCode_), threadID_, file_.c_str(), func_.c_str(), line_);    what_.assign(buffer, cx);

    va_list args;
    va_start(args, message);
    char linebuffer[1024] = { 0 };
    vsnprintf(linebuffer, sizeof(linebuffer), message, args);
    message_ = linebuffer;
    va_end(args);

    what_.append(message_);
}

//Exception2::Exception2(
//    const Exception2 &copied
//) noexcept :
//    what_(copied.what_),
//    errCode_(copied.errCode_),
//    threadID_(copied.threadID_),
//    file_(copied.file_),
//    func_(copied.func_),
//    line_(copied.line_),
//    message_(copied.message_),
//    userData_(copied.userData_),
//    userDataAvailable_(copied.userDataAvailable_),
//    payload_(copied.payload_),
//    payloadAvailable_(copied.payloadAvailable_)
//{
//}

//Exception2 &
//Exception2::operator=(
//    const Exception2 &copied
//) noexcept {
//    what_ = copied.what_;
//    errCode_ = copied.errCode_;
//    threadID_ = copied.threadID_;
//    file_ = copied.file_;
//    func_ = copied.func_;
//    line_ = copied.line_;
//    message_ = copied.message_;
//    userData_ = copied.userData_;
//    userDataAvailable_ = copied.userDataAvailable_;
//    payload_ = copied.payload_;
//    payloadAvailable_ = copied.payloadAvailable_;
//    return *this;
//}

Exception2::~Exception2() noexcept {
    what_.clear();
    file_.clear();
    func_.clear();
    message_.clear();
    errCode_ = ErrorCode::ErrUnknown;
    threadID_ = -1;
    line_ = -1;
    userDataAvailable_ = FALSE;
    payload_.clear();
    payloadAvailable_ = FALSE;
}

const char * Exception2::what() const noexcept {
    return what_.c_str();
}

int Exception2::errorCode() const noexcept {
    return errCode_;
}

int Exception2::threadID() const noexcept {
    return threadID_;
}

String Exception2::file() const noexcept {
    return file_;
}

String Exception2::func() const noexcept {
    return func_;
}

int Exception2::line() const noexcept {
    return line_;
}

String Exception2::message() const noexcept {
    return message_;
}

//void Exception2::setUserData(const JsonCommand& data) noexcept {
//    userData_ = data;
//    userDataAvailable_ = TRUE;
//}

//JsonCommand Exception2::getUserData() const noexcept {
//    return userData_;
//}

bool Exception2::isUserDataAvailable() const noexcept {
    return userDataAvailable_;
}

void Exception2::setPayload(const String &data) noexcept {
    payload_ = data;
    payloadAvailable_ = TRUE;
}

String Exception2::getPayload() const noexcept {
    return payload_;
}

bool Exception2::isPayloadAvailable() const noexcept {
    return payloadAvailable_;
}