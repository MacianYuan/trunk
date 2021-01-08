//
// Created by 何继胜 on 2017/5/2.
//
#include <fstream>
#include <tools/common.h>
#include <tools/log.h>
#include <deviceinfo/version.h>
#include <tools/IniParse.h>

#include "controller/Context/SupportFeature.h"


bool SupportFeature::Parse() {
    ParseCmdLine();
    ParseConfig();
}

bool SupportFeature::ParseConfig() {
    IniParse ini(CLIENT_CONFIG);
    casEnabled = ini.GetBool("CAS.Enabled");
    return false;
}

bool SupportFeature::ParseCmdLine() {
    /// SN=ZYLABM-01610-00001-00001
    ///                 ----- 此5位是功能码，最大为FFFFF
    char line[1024] = { 0 };
    std::ifstream fin("/proc/cmdline");
    fin.getline(line, sizeof(line));
    fin.close();

    const char* ptr = strstr(line, "SN=");

    if (nullptr == ptr) {
        return false;
    }

    ptr += strlen("SN=ZYLABM-FFFFF-");

    long value = 0;
    for (int i = 0; i < 5; ++i) {
        int s = ConvertHexChar(ptr[i]);
        if (s < 0) {
            return false;
        }
        value += s << ((4 - i) * 4);
    }

    DEBUG_LOG("The Function Code is : " << value);
    ftp(value);
    return true;
}

bool SupportFeature::isCasEnabled() const {
    return casEnabled;
}

bool SupportFeature::isSupportFtp() const {
    //modify by li ,force to support ftp,2018-4-19
    return true;
    //return supportFtp;
}

void SupportFeature::ftp(int val) {
    DEBUG_LOG("val is " << val);
    supportFtp = false;
    DEBUG_LOG("");
    bool status =((val & 0x01) > 0 ? true : false);
    if (false == status) {
        return;
    }

    DEBUG_LOG("");
    char *able = getenv("FTP_UPLOAD_ABLE");
    DEBUG_LOG("FTF_UPLOAD_ABLE:" << (NULL == able ? "null" : able));
    if (NULL == able || 0 == atoi(able)) {
        return;
    }

    DEBUG_LOG("");
    IniParse ini(OSLA_CONFIG);
    status = ini.GetBool("Ftp.enable");
    if (false == status) {
        DEBUG_LOG("don't use ftp function, don't have config info");
        supportFtp = false;
        return;
    }
    DEBUG_LOG("");

    supportFtp = true;
    return;
}
