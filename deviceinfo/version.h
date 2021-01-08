#ifndef VERSION_H
#define VERSION_H

#define APP_NAME            "OSLA"
#define APP_VERSION_STR     "2.15"
#define APP_VERSION_CODE    215
#define APP_TYPE            241
#define APP_AUTHOR          "zdzhang@gmail.com"
#define ORG_NAME            "BIOTECH"
#define ORG_DOMAIN          "biotech.ustc.edu.cn"

#define OSLA_CONFIG         "/storage/conf/OSLA.conf"
#define CLIENT_CONFIG	    "/storage/conf/controller.conf"
#define SSL_CERT            "/storage/conf/sslcert.pem"
#define NET_CONFIG          "/storage/conf/net.conf"
#define NETTYPE_CONFIG      "/storage/conf/nettype.conf"
#define WLAN_CONFIG         "/storage/conf/wlan.conf"
#define WLAN_SCAN_FILE       "/storage/conf/wlan_sacn.conf"

#define DATABASE_USER       ""
#define DATABASE_PASS       ""
#define DATABASE_NAME       "/storage/conf/sql.db"
#define DATABASE_HOST       ""
#define DATABASE_DRIVER     "QSQLITE"

//升级部分数据定义
//下载后的文件名
#define UPGRADE_CONFIG              "/storage/conf/upgrade.conf"
#define UPGRADE_FILE_NAME           "/storage/sd0/upgrade/upgrade.tar.gz"
#define UPGRADE_UNCOMPRESS_PATH     "/storage/sd0/upgrade/dest"
#define UPDATE_SH                  "/storage/sd0/upgrade/dest/upgrade.sh"

//#define OFFLINE             1

#endif // VERSION_H
