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
    srand(time(NULL));//ʱ���ʼ��

    Event_Scheduler* scheduler = Event_Scheduler::createNew(); //�¼�����
    ThreadPool* threadPool = ThreadPool::createNew(1);// �̳߳�ģ���ȡ��Դ ����ȡ�����ļ���һ��ģ�������߳��������
    MediaSessionManager* sessMgr = MediaSessionManager::createNew();//��Դ������
    UsageEnvironment* env = UsageEnvironment::createNew(scheduler, threadPool);//��ʵ����װ��env���棬����ʹ��

    Ipv4Address rtspAddr("127.0.0.1", 8554);//���װ����ֵ����
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

        session->startMulticast(); //�ಥ
        sessMgr->addSession(session);
    }
    LOGI("----------session init end------");


    rtspServer->start();

    env->getscheduler()->loop();
    return 0;
	return 0;
}