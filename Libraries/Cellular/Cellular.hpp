//
// Created by tainguyen on 7/22/20.
//

#ifndef HCG1_CELLULAR_HPP
#define HCG1_CELLULAR_HPP

#include <cstdlib>
#include <cstdio>
#include <cstring>

class Cellular {
public:
    static int config(const char* option);
    static const char* strerror(int errcode);
};

inline int Cellular::config(const char *option) {
    int temp;
    char config[50] = "/home/root/config_usb3g/";
    char pass_work[50] = "/etc/ppp/ev-connect-chat";

    strcat(config,option);

    LOG_WARN("Cellular config: %s", config);

    if(option == nullptr) {
        temp = -1;
        return temp;
    }

    FILE *s,*t;
    int c;
    s = fopen(config, "r");
    t = fopen(pass_work, "w");
    if(s == nullptr) {
        temp = -2;
        return temp;
    }
    if(t == nullptr) {
        temp = -3;
        return temp;
    }
    while((c = fgetc(s)) != EOF) {
        fputc(c,t);
    }
    fclose(s);
    fclose(t);

    LOG_WARN("Cellular config: Copy file done");

    temp = system("/home/root/usb_modeswitch.sh");
    LOG_WARN("Cellular config: Restart service done");
    if(temp != 0) {
        temp = -4;
        return temp;
    }
    return temp;
}

inline const char* Cellular::strerror(int errcode) {
    switch (errcode) {
        case -1: return "Invalid input APN";
        case -2: return "Could not open config file at /home/root/config_usb3g";
        case -3: return "Could not open file /etc/ppp/ev-connect-chat";
        case -4: return "Could not restart cellular service";
        default: return "Unknown error";
    }
}

#endif //HCG1_CELLULAR_HPP
