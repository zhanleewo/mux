//
//  protocols.h
//  mux
//
//  Created by admin on 13-9-10.
//
//

#ifndef mux_protocols_h
#define mux_protocols_h

typedef enum _mux_server_status {
    MSSNormal = '1',
    MSSBusy = '2',
    MSSRefuse = '3',
} mux_server_status;

typedef enum _dest_host_connect_status {
    DSCSNormal = '1',
    DSCSDNSRvsError = '2',
    DSCSConnectLost = '3',
} dest_host_connect_status;
// C2S_APP_DATA_PKG APP_C2S_DATA_PKG DATA_C2S_DEDUP_DATA_PKG
// |_07_|_03_|_01_|

enum MuxDedupCommands {
    DEDUP_C2S_BITMAP_STATUS_REQ,
    DEDUP_C2S_BITMAP_MD5_REQ,
    DEDUP_C2S_BITMAP_RECOVER_REQ,
    DEDUP_C2S_BITMAP_SYNC_REQ,
    DEDUP_C2S_DATA_PKG,
    
    DEDUP_S2C_BITMAP_STATUS_RESP,
    DEDUP_S2C_BITMAP_MD5_RESP,
    DEDUP_S2C_BITMAP_RECOVER_RESP,
    DEDUP_S2C_BITMAP_SYNC_RESP,
    DEDUP_S2C_DATA_PKG,
};

enum MuxAppDataCommands {
    
    DATA_C2S_DEDUP_DATA_PKG = 0x01,
    DATA_C2S_BASE_DATA_PKG = 0x02,
    
    DATA_S2C_DEDUP_DATA_PKG = 0xf1,
    DATA_S2C_BASE_DATA_PKG = 0xf2,
};

enum MuxAppCommands {
    
    APP_C2S_CONNECT_REQ = 0x01,
    APP_C2S_CLOSE = 0x02,
    APP_C2S_RE_CONNECT = 0x03,
    APP_C2S_DATA_PKG = 0x04,
    
    APP_S2C_CONNECT_STATUS = 0xf1,
    APP_S2C_CLOSE_RESP = 0xf2,
    APP_S2C_DATA_PKG = 0xf3,
};

typedef enum MuxControlCommands{
    
    /**
     *	MuxClient Send To MuxServer
     */
    C2S_GET_SERVER_STATUS_REQ = 0x01,
    C2S_CONNECTION_REQ = 0x02,
    C2S_CLOSE_REQ = 0x03,
    C2S_KEEP_ALIVE_REQ = 0x04,
    C2S_CLOSE_STATUS_RESP = 0x05,
    C2S_GET_CONNECTIONS_REQ = 0x06,
    C2S_APP_DATA_PKG = 0x07,
    /**
     *	MuxServer Send To MuxClient
     */
    S2C_SERVER_STATUS_RESP = 0xf1,
    S2C_CLIENT_CONNECT_STATUS_RESP = 0xf2,
    S2C_LIENT_CLOSE_RESP = 0xf3,
    S2C_KEEP_ALIVE_RESP = 0xf4,
    S2C_CLOSE_CLIENT_REQ = 0xf5,
    S2C_CONNECTIONS_RESP = 0xf6,
    S2C_APP_DATA_PKG = 0xf7,
};

#endif
