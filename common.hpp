#ifndef DATABASE_COMMON_HPP
#define DATABASE_COMMON_HPP

#include <iostream>
#include <functional>
#include <memory>
#include <string>
#include <vector>

using namespace std;
#define DATABASE_DBUS_PATH_XML                           "/etc/database/dbus.xml"
#define DATABASE_PLATFORM_LEVELDB                        "leveldb"
#define DATABASE_PLATFORM_SQLITE                         "sqlite"
#define DATABASE_PLATFORM_ROCKDB                         "rockdb"
#define DATABASE_TYPE_CONNECT_MQTT                       "mqtt"
#define DATABASE_TYPE_CONNECT_DBUS                       "dbus"
#define DATABASE_DBUS_PATH                               "/tmp/database/"
#define DATABASE_DBUS_PATH_BACKUP                        "/etc/database/"
#define DATABASE_MQTT_HOST                               ("localhost")
#define DATABASE_MQTT_PORT                               (1883)
#define DATABASE_MQTT_USERNAME_DEFAULT                   "component"
#define DATABASE_MQTT_PASSWORD_DEFAULT                   "d0acc56610a760f1bb17a20472850d1e1a53052412fea7759c1dc96bde618358"
#define DATABASE_MQTT_TOPIC_REQ                          "component/database/"
#define DATABASE_MQTT_TOPIC_RES                          "component/database/#"
#define DATABASE_MQTT_SUBDONE                            "{\"database\":\"done\"}"
#define DATABASE_LOG_PATH                                "/tmp/database.txt"
//Dbus
#define DBUS_METHOD_ADD_RECORD                      "AddRecord"
#define DBUS_METHOD_ADD_MULTI_RECORDS               "AddMultiRecords"
#define DBUS_METHOD_DELETE_RECORD                   "DeleteRecord"
#define DBUS_METHOD_DELETE_MULTI_RECORDS            "DeleteMultiRecords"
#define DBUS_METHOD_DELETE_ALL_RECORDS              "DeleteAllRecord"
#define DBUS_METHOD_GET_RECORD                      "GetRecord"
#define DBUS_METHOD_GET_MULTI_RECORDS               "GetMultiRecords"
#define DBUS_METHOD_GET_ALL_RECORDS                 "GetAllRecords"
#define DBUS_METHOD_SEARCH_IN_VALUE                 "SearchInValue"
#define DBUS_METHOD_UPDATE_RECORD                   "UpdateRecord"
#define DBUS_METHOD_UPDATE_MULTI_RECORDS            "UpdateMultiRecords"
#define DBUS_METHOD_BACKUP_DATABASE                 "BackupDataBase"
#define DBUS_METHOD_RECOVER_FROM_BACKUP             "RecoverFromBackup"
#define DBUS_NAME                                   "lumi.database.server"
#define DBUS_INTERFACE                              "lumi.database.interface"

extern std::string database_platform;

enum ErrorCode {
    // Generic errors
    ErrUnknown = 6700, // Unknown error occurred
    ErrOption = 6701, // Option was not acceptable
    ErrSelector = 6702, // Selector passed in is invalid or unknown
    ErrExecutionState = 6703, // Call made in the wrong execution state (e.g. called at interrupt time)
    ErrPath = 6704, // Path is invalid
    ErrParam = 6705, // Parameter is incorrect, missing or not appropriate
    ErrUserRequiredErr = 6706, // User interaction is required
    ErrCommand = 6707, // Command invalid or not supported
    ErrID = 6708, // Unknown, invalid, or inappropriate identifier
    ErrState = 6709, // Not in appropriate state to perform operation
    ErrRange = 6710, // Index is out of range or not valid
    ErrRequest = 6711, // Request was improperly formed or not appropriate
    ErrResponse = 6712, // Response was incorrect or out of sequence
    ErrChecksum = 6713, // Checksum does not match the actual data
    ErrNotHandled = 6714, // Operation was not handled
    ErrVersion = 6715, // Version is not correct or not compatible
    ErrSignature = 6716, // Signature did not match the actual data
    ErrFormat = 6717, // Unknown, invalid or inappropriate file/data format
    ErrNotInitialized = 6718, // Action request before needed services were initialized
    ErrAlreadyInitialized = 6719, // Attempt made to initialize when already initialized
    ErrNotInUse = 6720, // Object is not in use (e.g. cannot abort if not already in use)
    ErrAlreadyInUse = 6721, // Object is in use (e.g. cannot reuse active param blocks)
    ErrTimeout = 6722, // Timeout occurred
    ErrCancel = 6723, // Operation canceled (successful cancel)
    ErrAlreadyCancel = 6724, // Operation has already been canceled
    ErrCannotCancel = 6725, // Operation could not be canceled (maybe already done or invalid)
    ErrDeleted = 6726, // Object has already been deleted
    ErrNotFound = 6727, // Something was not found
    ErrNoMemory = 6728, // Not enough memory was available to perform the operation
    ErrNoResource = 6729, // Resources unavailable to perform operation
    ErrDuplicate = 6730, // Duplicate found or something is a duplicate
    ErrImmutable = 6731, // Entity not changeable
    ErrUnsupportedData = 6732, // Data is unknown or not supported
    ErrIntegrity = 6733, // Data is corrupt
    ErrIncompatible = 6734, // Data is not compatible or it is an incompatible format
    ErrUnsupported = 6735, // Feature or option is not supported
    ErrUnexpected = 6736, // Feature or option is not expected
    ErrValue = 6737, // Value is not appropriate
    ErrNotReadable = 6738, // Could not read or reading is not allowed
    ErrNotWritable = 6739, // Could not write or writing is not allowed
    ErrBadReference = 6740, // An invalid or inappropriate reference was specified
    ErrFlag = 6741, // An invalid, inappropriate or unsupported flag was specified
    ErrMalformed = 6742, // Something was not formed correctly
    ErrSize = 6743, // Size was too big, too small or not appropriate
    ErrName = 6744, // Mane was not correct, allowed or appropriate
    ErrNotPrepared = 6745, // Device or service is not ready
    ErrRead = 6746, // Could not read
    ErrWrite = 6747, // Could not write
    ErrMismatch = 6748, // Something does not match
    ErrDate = 6749, // Date is invalid or out-of-range
    ErrUnderrun = 6750, // Less data than expected
    ErrOverrun = 6751, // More data than expected
    ErrEnding = 6752, // Connection, session or something is ending
    ErrConnection = 6753, // Connection failed or is not supported
    ErrAuthentication = 6754, // Authentication failed or is not supported
    ErrOpen = 6755, // Could not open file, pipe, device, etc.
    ErrType = 6756, // Incorrect or incompatible type (e.g. file, data, etc.)
    ErrSkip = 6757, // Items should be or was skipped
    ErrNoAck = 6758, // No acknowledge
    ErrCollision = 6759, // Collision occurred
    ErrBackoff = 6760, // Backoff in progress and operation intentionally failed
    ErrAddress = 6761, // Bad address or no acknowledge of address
    ErrInternal = 6762, // An error internal to the implementation occurred
    ErrNoSpace = 6763, // Not enough space to perform operation
    ErrCount = 6764, // Count is incorrect
    ErrEndOfData = 6765, // Reached the end of the data (e.g. recv returned 0)
    ErrWouldBlock = 6766, // Would need to block to continue (e.g. non-blocking read/write)
    ErrLook = 6767, // Special case that needs to be looked at (e.g. interleaved data)
    ErrSecurityRequired = 6768, // Security is required for the operation (e.g. must use encryption)
    ErrOrder = 6769, // Order is incorrect
    ErrUpgrade = 6770, // Must upgrade
    ErrAsync = 6771, // Async operation successfully started and is now in progress
    ErrDeprecated = 6772, // Operation or data is deprecated
    ErrPermission = 6773, // Permission denied
    ErrReadWouldBlock = 6774, // Needs more incoming data
    ErrWriteWouldBLock = 6775, // Needs more buffer space to write data
    ErrNullPtr = 6776, // Null pointer detected
    ErrDatabase = 6777, // Database error
    ErrStreamNotSupport = 6778,
    ErrStreamMaxCamera = 6779,

    // Smarthome system error
    ErrCurtainNotCalib = 600,
    ErrBatteryLow = 601,
    ErrBridgeReplaceFail = 696,
    ErrBridgeRestoreFail = 697,
    ErrBridgeBackupFail = 698,
    ErrTransmitFail = 699,
    ErrIrLearningTimeout = 700,
    ErrIrMemoryFull = 701,
    ErrIrLearnFail = 702,
    ErrIrActiveFail = 703
};

static const char* GetErrString(int err) {
    switch (err) {
        // Generic error
        case ErrorCode::ErrUnknown: return "ErrUnknown";
        case ErrorCode::ErrOption: return "ErrOption";
        case ErrorCode::ErrSelector: return "ErrSelector";
        case ErrorCode::ErrExecutionState: return "ErrExecutionState";
        case ErrorCode::ErrPath: return "ErrPath";
        case ErrorCode::ErrParam: return "ErrParam";
        case ErrorCode::ErrUserRequiredErr: return "ErrUserRequiredErr";
        case ErrorCode::ErrCommand: return "ErrCommand";
        case ErrorCode::ErrID: return "ErrID";
        case ErrorCode::ErrState: return "ErrState";
        case ErrorCode::ErrRange: return "ErrRange";
        case ErrorCode::ErrRequest: return "ErrRequest";
        case ErrorCode::ErrResponse: return "ErrResponse";
        case ErrorCode::ErrChecksum: return "ErrChecksum";
        case ErrorCode::ErrNotHandled: return "ErrNotHandled";
        case ErrorCode::ErrVersion: return "ErrVersion";
        case ErrorCode::ErrSignature: return "ErrSignature";
        case ErrorCode::ErrFormat: return "ErrFormat";
        case ErrorCode::ErrNotInitialized: return "ErrNotInitialized";
        case ErrorCode::ErrAlreadyInitialized: return "ErrAlreadyInitialized";
        case ErrorCode::ErrNotInUse: return "ErrNotInUse";
        case ErrorCode::ErrAlreadyInUse: return "ErrAlreadyInUse";
        case ErrorCode::ErrTimeout: return "ErrTimeout";
        case ErrorCode::ErrCancel: return "ErrCancel";
        case ErrorCode::ErrAlreadyCancel: return "ErrAlreadyCancel";
        case ErrorCode::ErrCannotCancel: return "ErrCannotCancel";
        case ErrorCode::ErrDeleted: return "ErrDeleted";
        case ErrorCode::ErrNotFound: return "ErrNotFound";
        case ErrorCode::ErrNoMemory: return "ErrNoMemory";
        case ErrorCode::ErrNoResource: return "ErrNoResource";
        case ErrorCode::ErrDuplicate: return "ErrDuplicate";
        case ErrorCode::ErrImmutable: return "ErrImmutable";
        case ErrorCode::ErrUnsupportedData: return "ErrUnsupportedData";
        case ErrorCode::ErrIntegrity: return "ErrIntegrity";
        case ErrorCode::ErrIncompatible: return "ErrIncompatible";
        case ErrorCode::ErrUnsupported: return "ErrUnsupported";
        case ErrorCode::ErrUnexpected: return "ErrUnexpected";
        case ErrorCode::ErrValue: return "ErrValue";
        case ErrorCode::ErrNotReadable: return "ErrNotReadable";
        case ErrorCode::ErrNotWritable: return "ErrNotWritable";
        case ErrorCode::ErrBadReference: return "ErrBadReference";
        case ErrorCode::ErrFlag: return "ErrFlag";
        case ErrorCode::ErrMalformed: return "ErrMalformed";
        case ErrorCode::ErrSize: return "ErrSize";
        case ErrorCode::ErrName: return "ErrName";
        case ErrorCode::ErrNotPrepared: return "ErrNotPrepared";
        case ErrorCode::ErrRead: return "ErrRead";
        case ErrorCode::ErrWrite: return "ErrWrite";
        case ErrorCode::ErrMismatch: return "ErrMismatch";
        case ErrorCode::ErrDate: return "ErrDate";
        case ErrorCode::ErrUnderrun: return "ErrUnderrun";
        case ErrorCode::ErrOverrun: return "ErrOverrun";
        case ErrorCode::ErrEnding: return "ErrEnding";
        case ErrorCode::ErrConnection: return "ErrConnection";
        case ErrorCode::ErrAuthentication: return "ErrAuthentication";
        case ErrorCode::ErrOpen: return "ErrOpen";
        case ErrorCode::ErrType: return "ErrType";
        case ErrorCode::ErrSkip: return "ErrSkip";
        case ErrorCode::ErrNoAck: return "ErrNoAck";
        case ErrorCode::ErrCollision: return "ErrCollision";
        case ErrorCode::ErrBackoff: return "ErrBackoff";
        case ErrorCode::ErrAddress: return "ErrAddress";
        case ErrorCode::ErrInternal: return "ErrInternal";
        case ErrorCode::ErrNoSpace: return "ErrNoSpace";
        case ErrorCode::ErrCount: return "ErrCount";
        case ErrorCode::ErrEndOfData: return "ErrEndOfData";
        case ErrorCode::ErrWouldBlock: return "ErrWouldBlock";
        case ErrorCode::ErrLook: return "ErrLook";
        case ErrorCode::ErrSecurityRequired: return "ErrSecurityRequired";
        case ErrorCode::ErrOrder: return "ErrOrder";
        case ErrorCode::ErrUpgrade: return "ErrUpgrade";
        case ErrorCode::ErrAsync: return "ErrAsync";
        case ErrorCode::ErrDeprecated: return "ErrDeprecated";
        case ErrorCode::ErrPermission: return "ErrPermission";
        case ErrorCode::ErrReadWouldBlock: return "ErrReadWouldBlock";
        case ErrorCode::ErrWriteWouldBLock: return "ErrWriteWouldBLock";
        case ErrorCode::ErrNullPtr: return "ErrNullPtr";
        case ErrorCode::ErrDatabase: return "ErrDatabase";
        case ErrorCode::ErrStreamNotSupport: return "ErrStreamNotSupport";
        case ErrorCode::ErrStreamMaxCamera: return "ErrStreamMaxCamera";
            // Smarthome System error
        case ErrorCode::ErrCurtainNotCalib: return "ErrCurtainNotCalib";
        case ErrorCode::ErrBatteryLow: return "ErrBatteryLow";
        case ErrorCode::ErrBridgeReplaceFail: return "ErrBridgeReplaceFail";
        case ErrorCode::ErrBridgeRestoreFail: return "ErrBridgeRestoreFail";
        case ErrorCode::ErrBridgeBackupFail: return "ErrBridgeBackupFail";
        case ErrorCode::ErrTransmitFail: return "ErrTransmitFail";
        case ErrorCode::ErrIrLearningTimeout: return "ErrIrLearningTimeout";
        case ErrorCode::ErrIrMemoryFull: return "ErrIrMemoryFull";
        case ErrorCode::ErrIrLearnFail: return "ErrIrLearnFail";
        case ErrorCode::ErrIrActiveFail: return "ErrIrActiveFail";

        default: return "ErrUnknown";
    }
}
#endif //DATABASE_COMMON_HPP
