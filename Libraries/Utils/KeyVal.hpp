//
// Created by tainguyen on 5/8/20.
//

#ifndef HTTPS_SERVER_KEYVAL_HPP
#define HTTPS_SERVER_KEYVAL_HPP

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Log/LogPlus.hpp"

struct OneKeyVal {
    char* key;
    char* value;

    struct OneKeyVal* next;
};

class KeyVal {
public:
    KeyVal();
    ~KeyVal();

    int Add(const char* key, const char* value);
    int Add(const char* key, const char* value, size_t size);
    void Remove(const char* key);
    const char* Get(const char* key);
    void Clear();

    OneKeyVal* Head();
    OneKeyVal* Tail();
private:
    struct OneKeyVal* head;
    struct OneKeyVal* tail;
};

inline KeyVal::KeyVal() {
    head = nullptr;
    tail = nullptr;
}

inline KeyVal::~KeyVal() {
    Clear();
}

inline int KeyVal::Add(const char *key, const char *value, size_t size) {
    try {
        OneKeyVal* kv;
        const char* val;

        // Check for duplicate key names
        val = Get(key);
        if (val) {
            if (strcmp(val, value) == 0) { // Same value -> fine
                return 0;
            } else { // Different value -> bad
                return -1;
            }
        }

        kv = (OneKeyVal*)malloc(sizeof(OneKeyVal));
        if (!kv) {
            THROW_MESSAGE("Out of memory for new keyval");
        }

        kv->key = strdup(key);
        if (!kv->key) {
            free(kv);
            THROW_MESSAGE("Out of memory for new keyval name");
        }

        kv->value = (char*)malloc(size + 1);
        if (!kv->value) {
            free(kv);
            THROW_MESSAGE("Out of memory for new keyval value");
        }

        memcpy(kv->value, value, size);
        kv->value[size] = '\0';
        kv->next = nullptr;

        if (!head) {
            head = kv;
        }

        if (tail) {
            tail->next = kv;
        }

        tail = kv;

        return 0;
    } catch (std::exception &ex) {
        LOG_ERRO(ex.what());
        return -1;
    }
}

inline int KeyVal::Add(const char *key, const char *value) {
    return Add(key, value, strlen(value));
}

inline void KeyVal::Remove(const char *key) {
    OneKeyVal* kv;
    OneKeyVal* pkv;

    for (pkv = nullptr, kv = head; kv; pkv = kv, kv = kv->next) {
        if (strcasecmp(kv->key, key) == 0) {
            break;
        }
    }

    if (!kv) {
        return;
    }

    if (kv == head) {
        head = kv->next;
    }

    if (kv == tail) {
        tail = pkv;
    }

    if (pkv) {
        pkv->next = kv->next;
    }

    free(kv->key);
    free(kv->value);
    free(kv);
}

inline const char * KeyVal::Get(const char *key) {
    OneKeyVal* kv;

    for (kv = head; kv; kv = kv->next) {
        if (strcasecmp(kv->key, key) == 0) {
            return kv->value;
        }
    }

    return nullptr;
}

inline void KeyVal::Clear() {
    OneKeyVal* hkv;
    OneKeyVal* kv;

    hkv = head;
    for (kv = hkv; hkv; kv = hkv) {
        hkv = kv->next;
        free(kv->key);
        free(kv->value);
        free(kv);
    }

    head = nullptr;
    tail = nullptr;
}

inline OneKeyVal * KeyVal::Head() {
    return head;
}

inline OneKeyVal * KeyVal::Tail() {
    return tail;
}

#endif //HTTPS_SERVER_KEYVAL_HPP
