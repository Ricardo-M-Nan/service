#ifndef _MEDIASOURCE_H
#define _MEDIASOURCE_H
#include<queue>
#include<mutex>
#include<stdint.h>
#include"Log.h"
#include"UsageEnvironment.h"
#include"SelectPoller.h"

#define FRAME_MAX_SIZE (1024*200)
#define DEFAULT_FRAME_NUM 4

class MediaFrame
{

public:
    MediaFrame() :temp(new uint8_t[FRAME_MAX_SIZE]),mBuf(nullptr),mSize(0) 
    {

    }
    ~MediaFrame() 
    {
        delete[]temp;
    }
    uint8_t* temp;// ����
    uint8_t* mBuf;// ��������
    int mSize;
};

class MediaSource
{
public:
    explicit MediaSource(UsageEnvironment* env);
    virtual ~MediaSource();
    MediaFrame* getFrameFromOutputQueue();//��������л�ȡ֡
    void putFrameToInputQueue(MediaFrame* frame); // ��֡�����������
    int getFps() const
    {
        return mFps;
    }
    std::string getSourceName() 
    { return mSourceName; 
    }

private:
    static void taskCallback(void* arg);
protected:
    virtual void handleTask() = 0;
    void setFps(int fps) 
    { 
        mFps = fps;
    }

protected:
    UsageEnvironment* mEnv;
    MediaFrame mFrames[DEFAULT_FRAME_NUM];
    std::queue<MediaFrame*> mFrameInputQueue;
    std::queue<MediaFrame*> mFrameOutputQueue;
    std::mutex mMtx;
    ThreadPool::Task mTask;
    int mFps;
    std::string mSourceName;
};
#endif
