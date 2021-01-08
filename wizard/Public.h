//
// Created by 何继胜 on 2017/5/3.
//

#ifndef OSLA_NEW_PUBLIC_H
#define OSLA_NEW_PUBLIC_H

enum ProcessState {
    NetworkConfigState,         /// 0
    TcpConnectingState,         /// 1
    TcpReceivedState,           /// 2
    SslConnectingState,         /// 3
    WaitingForInstrumentList,   /// 4
    WaitingForBindingAck,       /// 5
    WaitingForDataDownload,     /// 6
    WaitingForBindingConfirm    /// 7
};

enum SetupError {
    TransferTimeOut,
    InvalidFrameLength,
    AuthFailedError,
    CertChecksumError,
    CertFileOpenError,
    SslEncryptedError,
    BindingError,
    TimeOutError,
    UnknownError = -1,
};

enum PageId {
    Page_Language,
    Page_Netchoice,
    Page_Network,
    Page_Instrument,
    Page_Download,
    Page_Confirm
};


#endif //OSLA_NEW_PUBLIC_H
