#ifndef _AACFILESINK_H
#define _AACFILESINK_H

#include "UsageEnvironment.h"
#include "Sink.h"
#include "MediaSource.h"

class AACFileSink : public Sink
{
public:
    static AACFileSink* createNew(UsageEnvironment* env, MediaSource* mediaSource);;

    AACFileSink(UsageEnvironment* env, MediaSource* mediaSource, int payloadType);
    virtual ~AACFileSink();

    virtual std::string getMediaDescription(uint16_t port);
    virtual std::string getAttribute();

protected:
    virtual void sendFrame(MediaFrame* frame);

private:
    RtpPacket mRtpPacket;
    uint32_t mSampleRate;   // ����Ƶ��
    uint32_t mChannels;         // ͨ����
    int mFps;
};
#endif