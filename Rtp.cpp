#include"RTP.h"
#include<string>
RtpPacket::RtpPacket() :
    mBuf((uint8_t*)malloc(4 + RTP_HEADER_SIZE + RTP_MAX_PKT_SIZE + 100)),mBuf4(mBuf + 4),mRtpHeader((RtpHeader*)mBuf4),mSize(0)
{

}
RtpPacket::~RtpPacket() {
    free(mBuf);
    mBuf = NULL;
}
void parseRtpHeader(uint8_t* buf, struct RtpHeader* rtpHeader)
{
    memset(rtpHeader, 0, sizeof(*rtpHeader));
    rtpHeader->version = (buf[0] & 0xC0) >> 6;
    rtpHeader->padding = (buf[0] & 0x20) >> 5;
    rtpHeader->extension = (buf[0] & 0x10) >> 4;
    rtpHeader->csrcLen = (buf[0] & 0x0F);
    rtpHeader->marker = (buf[1] & 0x80) >> 7;
    rtpHeader->payloadType = (buf[1] & 0x7F);
    rtpHeader->seq = ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF);
    rtpHeader->timestamp = ((buf[4] & 0xFF) << 24) | ((buf[5] & 0xFF) << 16)| ((buf[6] & 0xFF) << 8)| ((buf[7] & 0xFF));
    rtpHeader->ssrc = ((buf[8] & 0xFF) << 24) | ((buf[9] & 0xFF) << 16)| ((buf[10] & 0xFF) << 8) | ((buf[11] & 0xFF));
}

void parseRtcpHeader(uint8_t* buf, struct RtcpHeader* rtcpHeader) {

    memset(rtcpHeader, 0, sizeof(*rtcpHeader));
    rtcpHeader->version = (buf[0] & 0xC0) >> 6;
    rtcpHeader->padding = (buf[0] & 0x20) >> 5;
    rtcpHeader->rc = (buf[0] & 0x1F);
    rtcpHeader->packetType = (buf[1] & 0xFF);
    rtcpHeader->length = ((buf[2] & 0xFF) << 8) | (buf[3] & 0xFF);
}