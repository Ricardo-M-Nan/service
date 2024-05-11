#ifndef _RTP_H
#define _RTP_H
#include<iostream>
#include<stdint.h>
#include<stdlib.h>
#define RTP_VESION              2
#define RTP_PAYLOAD_TYPE_H264   96
#define RTP_PAYLOAD_TYPE_AAC    97
#define RTP_HEADER_SIZE         12
#define RTP_MAX_PKT_SIZE        1400
struct RtpHeader
{
    uint8_t csrcLen : 4;//CSRC��������ռ4λ��ָʾCSRC ��ʶ���ĸ�����
    uint8_t extension : 1;//ռ1λ�����X=1������RTP��ͷ�����һ����չ��ͷ��
    uint8_t padding : 1;//����־��ռ1λ�����P=1�����ڸñ��ĵ�β�����һ����������İ�λ�飬���ǲ�����Ч�غɵ�һ���֡�
    uint8_t version : 2;//RTPЭ��İ汾�ţ�ռ2λ����ǰЭ��汾��Ϊ2��
    uint8_t payloadType : 7;//��Ч�غ����ͣ�ռ7λ������˵��RTP��������Ч�غɵ����ͣ���GSM��Ƶ��JPEMͼ��ȡ�
    uint8_t marker : 1;//��ǣ�ռ1λ����ͬ����Ч�غ��в�ͬ�ĺ��壬������Ƶ�����һ֡�Ľ�����������Ƶ����ǻỰ�Ŀ�ʼ��
    uint16_t seq;//ռ16λ�����ڱ�ʶ�����������͵�RTP���ĵ����кţ�ÿ����һ�����ģ����к���1��������ͨ�����к�����ⱨ�Ķ�ʧ��������������ģ��ָ����ݡ�
    uint32_t timestamp;//ռ32λ��ʱ����ӳ�˸�RTP���ĵĵ�һ����λ��Ĳ���ʱ�̡�������ʹ��ʱ���������ӳٺ��ӳٶ�����������ͬ�����ơ�
    uint32_t ssrc;//ռ32λ�����ڱ�ʶͬ����Դ���ñ�ʶ�������ѡ��ģ��μ�ͬһ��Ƶ���������ͬ����Դ��������ͬ��SSRC��

    uint8_t payload[0];
};
struct RtcpHeader
{
    // byte 0
    uint8_t rc : 5;// reception report count
    uint8_t padding : 1;
    uint8_t version : 2;
    // byte 1
    uint8_t packetType;

    // bytes 2,3
    uint16_t length;
};
class RtpPacket {
public:
    RtpPacket();
    ~RtpPacket();
public:
    uint8_t* mBuf; // 4+rtpHeader+rtpBody
    uint8_t* mBuf4;// rtpHeader+rtpBody
    RtpHeader* const mRtpHeader;
    int mSize;// rtpHeader+rtpBody
};
void parseRtpHeader(uint8_t* buf, struct RtpHeader* rtpHeader);
void parseRtcpHeader(uint8_t* buf, struct RtcpHeader* rtcpHeader);
#endif // !_RTP_H

