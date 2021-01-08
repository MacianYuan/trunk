//
// Created by 何继胜 on 2017/5/2.
//

#ifndef OSLA_NEW_SUPPORTFEATURE_H
#define OSLA_NEW_SUPPORTFEATURE_H


class SupportFeature {
public:
    bool Parse();
    bool isSupportFtp() const;
    bool isCasEnabled() const;

private:
    bool ParseCmdLine();
    bool ParseConfig();
    void ftp(int val);

private:
    bool casEnabled{false}; ///  cas功能
    bool supportFtp{ false };   /// 是否启用ftp功能
};


#endif //OSLA_NEW_SUPPORTFEATURE_H
