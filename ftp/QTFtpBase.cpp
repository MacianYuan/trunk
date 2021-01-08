#include "ftp/QTFtpBase.h"

void ResetProgress(Progress& info) {
    info.consume_time = 0;

    info.path = "";
    info.fileName = "";
    info.fileSize = 0;
    memset(info.md5Result, 0x00, sizeof(info.md5Result));

    info.id = 0;
    info.userId = 0;
    info.instrumentId = 0;
	info.group_id = 0;
	info.actived_account = 0;
}