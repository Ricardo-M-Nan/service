#include "EventScheduler.h"
#include "ThreadPool.h"
#include "UsageEnvironment.h"
#include "MediaSessionManager.h"
#include "RtspServer.h"
#include "H264FileMediaSource.h"
#include "H264FileSink.h"
#include "AACFileMediaSource.h"
#include "AACFileSink.h"
#include "Log.h"
int main()
{
    srand(time(NULL));//时间初始化

    Event_Scheduler* scheduler = Event_Scheduler::createNew(); //事件调度
    ThreadPool* threadPool = ThreadPool::createNew(1);// 线程池模拟读取资源 将读取本地文件的一个模块在子线程里面完成
    MediaSessionManager* sessMgr = MediaSessionManager::createNew();//资源管理器
    UsageEnvironment* env = UsageEnvironment::createNew(scheduler, threadPool);//将实例封装到env里面，方便使用

    Ipv4Address rtspAddr("127.0.0.1", 8554);//类包装方便值传递
    RtspServer* rtspServer = RtspServer::createNew(env, sessMgr, rtspAddr);

    LOGI("----------session init start------");
    {
        std::cout << "ffplay -i -rtsp_transport tcp  rtsp://127.0.0.1:8554/test" << std::endl;
        MediaSession* session = MediaSession::createNew("test");
        MediaSource* source = H264FileMediaSource::createNew(env, "../data/test.h264");
        Sink* sink = H264FileSink::createNew(env, source);
        session->addSink(MediaSession::TrackId0, sink);

        source = AACFileMeidaSource::createNew(env, "../data/test.aac");
        sink = AACFileSink::createNew(env, source);
        session->addSink(MediaSession::TrackId1, sink);

        session->startMulticast(); //多播
        sessMgr->addSession(session);
    }
    LOGI("----------session init end------");


    rtspServer->start();

    env->getscheduler()->loop();
    return 0;
	return 0;
}