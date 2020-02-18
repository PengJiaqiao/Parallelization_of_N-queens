#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <queue>
#include <atomic>
#include <memory>
#include <mutex>
#include <chrono>
#include <thread>
#include <functional>
#include <vector>
#include <sys/time.h>

using namespace std;

atomic<long> sum;
long upperlim; // 1 <= n <= 32
int nThread = 4;
int maxDepth;

double now()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

// 定义任务接口
class task
{
public:
    // 稍后实现
    task(long row_, long ld_, long rd_) : row(row_), ld(ld_), rd(rd_) {}
    void run();
    void run(long row_, long ld_, long rd_);

private:
    long row;
    long ld;
    long rd;
};

//自旋锁类
class spinMutex
{
private:
    atomic_flag flag = ATOMIC_FLAG_INIT;

public:
    void lock()
    {
        while (flag.test_and_set(memory_order_acquire))
            ;
    }
    void unlock()
    {
        flag.clear(memory_order_release);
    }
};

//任务队列
class taskPool
{
private:
    int nThread;
    vector<thread> threads;
    mutable spinMutex mtx;
    queue<shared_ptr<task>> que;

    taskPool()
    {
    }

    bool pop(shared_ptr<task> &task)
    {
        lock_guard<spinMutex> lk(mtx);
        if (que.empty())
            return false;
        task = que.front();
        que.pop();
        return true;
    }

public:
    //实现单例模式
    static taskPool *instance()
    {
        static taskPool obj;
        return &obj;
    }

    //中止任务处理
    void stop()
    {
        nThread = 0;
    }

    //清空队列
    void clear()
    {
        lock_guard<spinMutex> lk(mtx);
        while (que.size() > 0)
            que.pop();
        threads.clear();
    }

    //判断队列是否为空
    bool empty() const
    {
        lock_guard<spinMutex> lk(mtx);
        return que.empty();
    }

    //获取队列长度
    size_t size() const
    {
        lock_guard<spinMutex> lk(mtx);
        return que.size();
    }

    //获取任务线程数
    int getThreads() const
    {
        return nThread;
    }

    //任务对象入队
    void push(const shared_ptr<task> &task)
    {
        lock_guard<spinMutex> lk(mtx);
        que.push(task);
    }

    //启动任务栈(启动处理线程)
    void start(int nThread)
    {
        this->nThread = nThread;

        for (int i = 0; i < nThread; ++i)
        {
            threads.push_back(thread(bind(&taskPool::run, this)));
        }
    }

    // 等待任务结束
    void join()
    {
        for (auto &thread : threads)
        {
            thread.join();
        }
        clear();
    }

    //这个方法里面处理具体任务
    void run()
    {
        shared_ptr<task> task;

        while (nThread > 0)
        {
            if (pop(task))
            {
                task->run();
            }
        }
    }
};

// 具体的任务
void task::run()
{
    if (row != upperlim)
    {
        long pos = upperlim & ~(row | ld | rd);
        if (taskPool::instance()->size() < maxDepth)
        {
            while (pos)
            {
                long p = pos & (~pos + 1);

                pos -= p;
                shared_ptr<task> nextTask(new task(row + p, (ld | p) << 1,
                                                   (rd | p) >> 1));
                taskPool::instance()->push(nextTask);
            }
        }
        else
        {
            while (pos)
            {
                long p = pos & (~pos + 1);

                pos -= p;
                task::run(row + p, (ld | p) << 1, (rd | p) >> 1);
            }
        }
    }
    else
        ++sum;
}

void task::run(long row, long ld, long rd)
{
    if (row != upperlim)
    {
        long pos = upperlim & ~(row | ld | rd);
        while (pos)
        {
            long p = pos & (~pos + 1);

            pos -= p;
            task::run(row + p, (ld | p) << 1, (rd | p) >> 1);
        }
    }
    else
        ++sum;
}

int main(int argc, char **argv)
{
    nThread = argc > 1 ? atoi(argv[1]) : 4;

    int n = 1;
    while (true)
    {
        double start = now();
        sum = 0;
        upperlim = 1;
        upperlim = (upperlim << n) - 1;
        maxDepth = nThread;
        shared_ptr<task> initTask(new task(0, 0, 0));
        taskPool::instance()->start(nThread);
        taskPool::instance()->push(initTask);
        while (1)
        {
            if (taskPool::instance()->size() > 0)
            {
                chrono::milliseconds dura(100);
                this_thread::sleep_for(dura);
            }
            else
            {
                chrono::milliseconds dura(100);
                this_thread::sleep_for(dura);
                if (taskPool::instance()->size() == 0)
                    break;
            }
        }
        taskPool::instance()->stop();
        taskPool::instance()->join();
        double end = now();
        double time = end - start;
        cout << "n = " << n << ", result = " << sum << ", Total time:" << time << "\n"
             << endl;
        ++n;
    }
    return 0;
}

/*
./main 8

n = 1, result = 1, Total time:0.146002

n = 2, result = 0, Total time:0.100318

n = 3, result = 0, Total time:0.100252

n = 4, result = 2, Total time:0.100326

n = 5, result = 10, Total time:0.100264

n = 6, result = 4, Total time:0.100293

n = 7, result = 40, Total time:0.110456

n = 8, result = 92, Total time:0.100304

n = 9, result = 352, Total time:0.100238

n = 10, result = 724, Total time:0.231307

n = 11, result = 2680, Total time:0.131987

n = 12, result = 14200, Total time:0.231982

n = 13, result = 73712, Total time:0.100265

n = 14, result = 365596, Total time:0.100266

n = 15, result = 2279184, Total time:0.600557

n = 16, result = 14772512, Total time:3.06581

n = 17, result = 95815104, Total time:21.2106

n = 18, result = 666090624, Total time:154.903

n = 19, result = 4968057848, Total time:1222.11

n = 20, result = 39029188884, Total time:9370.54
 */