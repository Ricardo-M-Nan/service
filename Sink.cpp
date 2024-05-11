#include "Sink.h"
#include "Socketsapi.h"
#include "Log.h"


Sink::Sink(UsageEnvironment* env, MediaSource* mediaSource, int payloadType) :
    mMediaSource(mediaSource),
    mEnv(env),
    mCsrcLen(0),
    mExtension(0),
    mPadding(0),
    mVersion(RTP_VESION),
    mPayloadType(payloadType),
    mMarker(0),
    mSeq(0),
    mSSRC(rand()),
    mTimestamp(0),
    mTimerId(0),
    mSessionSendPacket(NULL),
    mArg1(NULL),
    mArg2(NULL)
{

    LOGI("Sink()");
    mTimerEvent = Timer_Event::createNew(this);
    mTimerEvent->setTimeoutCallback(cbTimeout);


}

Sink::~Sink() {
    LOGI("~Sink()");
    delete mTimerEvent;
    delete mMediaSource;
}
void Sink::stopTimerEvent() {

    mTimerEvent->stop();

}
void Sink::setSessionCb(SessionSendPacketCallback cb, void* arg1, void* arg2) {
    mSessionSendPacket = cb;
    mArg1 = arg1;
    mArg2 = arg2;
}

void Sink::sendRtpPacket(RtpPacket* packet) {
    RtpHeader* rtpHeader = packet->mRtpHeader;
    rtpHeader->csrcLen = mCsrcLen;
    rtpHeader->extension = mExtension;
    rtpHeader->padding = mPadding;
    rtpHeader->version = mVersion;
    rtpHeader->payloadType = mPayloadType;
    rtpHeader->marker = mMarker;
    rtpHeader->seq = htons(mSeq);
    rtpHeader->timestamp = htonl(mTimestamp);
    rtpHeader->ssrc = htonl(mSSRC);

    if (mSessionSendPacket) {
        //arg1 mediaSession ����ָ��
        //arg2 mediaSession ���ص�track����ָ��
        mSessionSendPacket(mArg1, mArg2, packet, PacketType::RTPPACKET);
    }

}

void Sink::cbTimeout(void* arg) {
    Sink* sink = (Sink*)arg;
    sink->handleTimeout();
}
void Sink::handleTimeout() {
    MediaFrame* frame = mMediaSource->getFrameFromOutputQueue();
    if (!frame) {
        return;
    }
    this->sendFrame(frame);// �ɾ�������ʵ�ַ����߼�

    mMediaSource->putFrameToInputQueue(frame);//��ʹ�ù���frame����������У�������������Ժ󣬼���һ�����߳�task�����ļ��ж�ȡ�����ٴν�����д�뵽frame
}

void Sink::runEvery(int interval) {
    mTimerId = mEnv->getscheduler()->addTimedEventRunEvery(mTimerEvent, interval);
}
