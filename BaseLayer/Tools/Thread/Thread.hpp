#ifndef _THREAD_HPP_
#define _THREAD_HPP_

// 
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <winbase.h>
#include <process.h>
typedef std::pair<HANDLE, UINT> thread_t;
#ifdef __DMC__
extern "C" WINBASEAPI BOOL WINAPI InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION,DWORD);
#endif
#if defined(_MSC_VER) && _MSC_VER < 1300
extern "C" WINBASEAPI BOOL WINAPI InitializeCriticalSectionAndSpinCount(LPCRITICAL_SECTION,DWORD);
extern "C" WINBASEAPI DWORD WINAPI SetCriticalSectionSpinCount(LPCRITICAL_SECTION, DWORD);
extern "C" WINBASEAPI BOOL WINAPI TryEnterCriticalSection(LPCRITICAL_SECTION);
#endif
#else
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
typedef std::pair<pthread_t, std::pair<bool, pthread_cond_t> > thread_t;
#endif

#include "../../Component/Interfaces/ICommand.h"

using namespace BaseLayer::Interfaces;

/**
 * Thread base PureMVC.
*/
class Thread
{
private:
    bool _auto_destroy;
    ICommand* _command;

    void* _thread_handler;
    std::size_t _stack_size;
    Thread& operator=(Thread const&);
public:
    /**
        * Default constructor.
        *
        * @param destroyable_command to run in thread. 
        * This command will be destroy in Thread's destructor.
        */
    explicit Thread(ICommand* destroyable_command)
        : _auto_destroy(true)
        , _command(destroyable_command)
#if defined(_WIN32) || defined(_WIN64)
        , _thread_handler(new thread_t((HANDLE)NULL, 0))
#else
        , _thread_handler(new thread_t())
#endif
        , _stack_size(0)
    {
        if (_command == NULL)
        {
            throw std::runtime_error("Command parameter is null.");
        }

#if !defined(_WIN32) && !defined(_WIN64)
        thread_t* thread = (thread_t*)_thread_handler;
        thread->second.first = false;
        ::pthread_cond_init(&thread->second.second, NULL);
#endif
}

    /**
        * Default constructor.
        *
        * @param command to run in thread.
        */
    explicit Thread(ICommand& command)
        : _auto_destroy(false)
        , _command(&command)
#if defined(_WIN32) || defined(_WIN64)
        , _thread_handler(new thread_t((HANDLE)NULL, 0))
#else
        , _thread_handler(new thread_t())
#endif
    {
#if !defined(_WIN32) && !defined(_WIN64)
        thread_t* thread = (thread_t*)_thread_handler;
        thread->second.first = false;
        ::pthread_cond_init(&thread->second.second, NULL);
#endif
    }

public:
    /**
        * Sets the thread's stack size in bytes.
        */
    void setStackSize(std::size_t size);

    /**
        * Returns the thread's stack size in bytes.
        *
        * @return the default stack size is used.
        */
    std::size_t getStackSize() const;

    /**
        * Start thread with notification.
        *
        * @param notification parameter.
        */
    void start(INotification const& notification)
    {
        if (isRunning())
        throw std::runtime_error("Thread is started!");

        struct ThreadEntry
        {
#if defined(_WIN32) || defined(_WIN64)
            static DWORD WINAPI action(LPVOID param_data)
            {
#else
            static void* action(void* param_data)
            {
                ::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
                void** data = (void**)param_data;
                ICommand* command = (ICommand*)data[0];
                INotification* notification = (INotification*)data[1];

                try
                {
                    command->execute(*notification);
                }
                catch(std::exception const* ex)
                {
            	    std::cerr << ex->what() << std::endl;
                    delete ex;
                }
                catch(std::exception const& ex)
                {
            	    std::cerr << ex.what() << std::endl;
                }
                catch(...)
                {
                    std::cerr << "Cannot handle exception from thread." << std::endl;
                }

#if defined(_WIN32) || defined(_WIN64)
                delete[] data;
                return S_OK;
#else
                std::pair<bool, pthread_cond_t>* run_cond = (std::pair<bool, pthread_cond_t>*) data[3];
                pthread_cond_signal(&run_cond->second);
                run_cond->first = false;
                delete[] data;
                return NULL;
#endif
            }
        };

        void** data = new void*[3];
        data[0] = _command;
        data[1] = const_cast<INotification*>(&notification);
   
        thread_t* thread = (thread_t*)_thread_handler;

#if defined(_WIN32) || defined(_WIN64)
        data[2] = NULL;
        thread->first = ::CreateThread(NULL,
            _stack_size, &ThreadEntry::action, data, 0, (DWORD*)&thread->second);
        if (!_thread_handler)
        {
            delete[] data;
            throw std::runtime_error("Cannot create thread!");
        }
#else
        data[2] = &thread->second;
        pthread_attr_t attributes;
        ::pthread_attr_init(&attributes);
        register int rc;
        if (_stack_size != 0 )
        {
            if ((rc = ::pthread_attr_setstacksize(&attributes, _stack_size)))
            {
                ::pthread_attr_init(&attributes);
                std::cerr << strerror(rc) << std::endl;
                delete[] data;
                throw std::runtime_error("Cannot set thread stack size!");
            }
        }
        thread->second.first = true;
        if ((rc = ::pthread_create(&thread->first, &attributes, &ThreadEntry::action, data)))
        {
            ::pthread_attr_init(&attributes);
            std::cerr << strerror(rc) << std::endl;
            thread->second.first = false;
            delete[] data;
            std::runtime_error("Cannot start thread!");
        }
        ::pthread_attr_init(&attributes);
#endif
    }

    /**
        * Start thread with notification. 
        *
        * @param destroyable_notification parameter will be destroy by Thread.
        */
    void start(INotification const* destroyable_notification)
    {
        if (destroyable_notification == NULL)
        {
            throw std::runtime_error("Notification parameter is null.");
        }

        if (isRunning())
        {
            delete destroyable_notification;
            throw std::runtime_error("Thread is started!");
        }

        struct ThreadEntry
        {
#if defined(_WIN32) || defined(_WIN64)
            static DWORD WINAPI action(LPVOID param_data)
            {
#else
            static void* action(void* param_data) {
                ::pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif
                void** data = (void**)param_data;
                ICommand* command = (ICommand*)data[0];
                INotification* notification = (INotification*)data[1];

                try
                {
                    command->execute(*notification);
                }
                catch(std::exception const* ex)
                {
                    std::cerr << ex->what() << std::endl;
                    delete ex;
                }
                catch (std::exception const& ex)
                {
                    std::cerr << ex.what() << std::endl;
                }
                catch(...)
                {
                    std::cerr << "Cannot handle exception from thread." << std::endl;
                }
                delete notification;
#if defined(_WIN32) || defined(_WIN64)
                delete[] data;
                return S_OK;
#else
                std::pair<bool, pthread_cond_t>* run_cond = (std::pair<bool, pthread_cond_t>*) data[3];
                pthread_cond_signal(&run_cond->second);
                run_cond->first = false;
                delete[] data;
                return NULL;
#endif
            }
        };

        void** data = new void*[3];
        data[0] = _command;
        data[1] = const_cast<INotification*>(destroyable_notification);
        thread_t* thread = (thread_t*)_thread_handler;

#if defined(_WIN32) || defined(_WIN64)
        data[2] = NULL;
        thread->first = ::CreateThread(NULL,
            _stack_size, &ThreadEntry::action, data, 0, (DWORD*)&thread->second);
        if (!_thread_handler)
        {
            delete destroyable_notification;
            delete[] data;
            throw std::runtime_error("Cannot create thread!");
        }
#else
        data[3] = &thread->second;
        pthread_attr_t attributes;
        ::pthread_attr_init(&attributes);
        register int rc;
        if (_stack_size != 0 )
        {
            if ((rc = ::pthread_attr_setstacksize(&attributes, _stack_size)))
            {
                ::pthread_attr_init(&attributes);
                std::cerr << strerror(rc) << std::endl;
                delete destroyable_notification;
                delete[] data;
                throw std::runtime_error("Cannot set thread stack size!");
            }
        }
        thread->second.first = true;
        if ((rc = ::pthread_create(&thread->first, &attributes, &ThreadEntry::action, data)))
        {
            ::pthread_attr_init(&attributes);
            std::cerr << strerror(rc) << std::endl;
            delete destroyable_notification;
            thread->second.first = false;
            delete[] data;
            std::runtime_error("Cannot start thread!");
        }
        ::pthread_attr_init(&attributes);
#endif
    }

    /**
        * Returns true if the thread is running.
        */
    bool isRunning(void) const
    {
        thread_t* thread = (thread_t*)_thread_handler;

#if defined(_WIN32) || defined(_WIN64)
    if (thread->first == NULL) return false;

    DWORD ec = 0;
    return ::GetExitCodeThread(thread->first, &ec) && ec == STILL_ACTIVE;
#else
    return thread->second.first;
#endif
    }

    /**
        * Waits until the thread completes execution.
        */
    void join(void)
    {
        if (!isRunning()) return;

        thread_t* thread = (thread_t*)_thread_handler;

#if defined(_WIN32) || defined(_WIN64)

        if (thread->first == NULL) return;

        switch (::WaitForSingleObject(thread->first, INFINITE))
        {
        case WAIT_OBJECT_0:
            if (::CloseHandle(thread->first))
                thread->first = NULL;
            else
                throw std::runtime_error("Cannot close thread handle!");
            return;
        default:
            throw std::runtime_error("Cannot join thread!");
        }
#else
        register int rc;
        if ((rc = ::pthread_join(thread->first, NULL)))
        {
            std::cerr << strerror(rc) << std::endl;
            throw std::runtime_error("Cannot join thread!");
        }
        thread->second.first = false;
#endif
    }

    /**
        * Waits for at most the given interval for the thread to complete.
        */
    void join(long milliseconds)
    {
        if (!tryJoin(milliseconds))
        {
            throw std::runtime_error("Join thread timeout!");
        }
    }

    /**
        * Waits for at most the given interval for the thread to complete.
        */
    bool tryJoin(long milliseconds)
    {
        if (!isRunning()) return true;


        thread_t* thread = (thread_t*)_thread_handler;

    #if defined(_WIN32) || defined(_WIN64)

        if (thread->first == NULL) return true;

        switch (::WaitForSingleObject(thread->first, milliseconds + 1))
        {
            case WAIT_TIMEOUT: return false;
            case WAIT_OBJECT_0:
                if (::CloseHandle(thread->first))
                    thread->first = NULL;
                else
                    throw std::runtime_error("Cannot close thread handle!");
                return true;
        default:
            throw std::runtime_error("Cannot join thread!");
        }
    #else
        register int rc;
        struct timespec   ts;
        struct timeval    tp;
        if ((rc = ::gettimeofday(&tp, NULL)))
        {
            std::cerr << strerror(rc) << std::endl;
            throw std::runtime_error("Cannot calculate time!");
        }

        ts.tv_sec  = tp.tv_sec + milliseconds / 1000;
        ts.tv_nsec = tp.tv_usec*1000 + (milliseconds % 1000)*1000000;
        if (ts.tv_nsec >= 1000000000)
        {
            ts.tv_nsec -= 1000000000;
            ts.tv_sec++;
        }

        /* If the wait timed out, in this example, the work is complete, and   */
        /* the thread will end.                                                */
        /* In reality, a timeout must be accompanied by some sort of checking  */
        /* to see if the work is REALLY all complete. In the simple example    */
        /* we will just go belly up when we time out.                          */
        if (rc == ETIMEDOUT)
            return false;
        return true;
    #endif
    #if defined(__DMC__)
        return false;
    #endif
    }

    /**
        * Suspends the current thread for the specified amount of time.
        */
    static void sleep(long milliseconds)
    {
#if defined(_WIN32) || defined(_WIN64)
        ::Sleep(DWORD(milliseconds));
#else
        ::usleep(milliseconds * 1000);
#endif
    }

    /**
        * Get current thread id.
        */
    static std::string getCurrentThreadId(void)
    {
        std::stringstream ss;
#if defined(_WIN32) || defined(_WIN64)
        ss << ::GetCurrentThreadId();
#else
        ss << ::pthread_self();
#endif
        return ss.str();
    }

    /**
        * Destructor.
        */
    ~Thread()
    {
        try
        {
            join();
        }
        catch (...)
        { }

        thread_t* thread = (thread_t*)_thread_handler;
#if !defined(_WIN32) && !defined(_WIN64)
        ::pthread_cond_destroy(&thread->second.second);
#endif
        delete thread;
        _thread_handler = NULL;

        if (_command != NULL && _auto_destroy)
            delete _command;
        _command = NULL;
    }
};

#endif