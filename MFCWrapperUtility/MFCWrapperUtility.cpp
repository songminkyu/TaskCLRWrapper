// MFCWrapperUtility.cpp : DLL의 초기화 루틴을 정의합니다.
//

#include "pch.h"
#include "MFCWrapperUtility.h"
#ifdef _DEBUG
//#define new DEBUG_NEW
#endif 
#include <stlab/concurrency/default_executor.hpp>
#include <stlab/concurrency/future.hpp>
#include <stlab/concurrency/serial_queue.hpp>
#include <stlab/concurrency/utility.hpp>
#include <stlab/test/model.hpp>
//
//TODO: 이 DLL이 MFC DLL에 대해 동적으로 링크되어 있는 경우
//		MFC로 호출되는 이 DLL에서 내보내지는 모든 함수의
//		시작 부분에 AFX_MANAGE_STATE 매크로가
//		들어 있어야 합니다.
//
//		예:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 일반적인 함수 본문은 여기에 옵니다.
//		}
//
//		이 매크로는 MFC로 호출하기 전에
//		각 함수에 반드시 들어 있어야 합니다.
//		즉, 매크로는 함수의 첫 번째 문이어야 하며
//		개체 변수의 생성자가 MFC DLL로
//		호출할 수 있으므로 개체 변수가 선언되기 전에
//		나와야 합니다.
//
//		자세한 내용은
//		MFC Technical Note 33 및 58을 참조하십시오.
//

// MFCWrapperUtilityApp

BEGIN_MESSAGE_MAP(MFCWrapperUtilityApp, CWinApp)
END_MESSAGE_MAP()


// MFCWrapperUtilityApp 생성

MFCWrapperUtilityApp::MFCWrapperUtilityApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 MFCWrapperUtilityApp 개체입니다.

MFCWrapperUtilityApp theApp;


// MFCWrapperUtilityApp 초기화

BOOL MFCWrapperUtilityApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" __declspec(dllexport) void ShowMessageBox(LPCTSTR strText)
{
	AfxMessageBox(strText);
}

extern "C" __declspec(dllexport) int GetSum(int a, int b)
{
	return a + b;
}

extern "C" __declspec(dllexport) void GetNativeStructs(CppVectorInternal<NativeStruct>*out)
{    
    std::uniform_int_distribution<int> dist1{ 0, 10 };
    std::normal_distribution<double> dist2;
    std::normal_distribution<float> dist3;

    std::random_device device;
    std::default_random_engine engine{ device() };

    std::vector<NativeStruct> result;
    for (int i{}; i < 20; ++i) {
        result.push_back(NativeStruct{
            dist1(engine),
            dist2(engine),
            dist3(engine),
            });
    }

    *out = result;
}

void worker(std::promise<std::string>* p) {
    // 약속을 이행함. 해당 결과는 future에 들어가게 됨
    try {
        throw std::runtime_error("Some Error!");
    }
    catch (...) {
        // set_exception 에는 exception_ptr 를 전달해야 한다.
        p->set_exception(std::current_exception());
    }
}


extern "C" __declspec(dllexport) BSTR AsyncThreadTest()
{    
    std::promise<std::string> p;

    // 미래에 string 데이터를 돌려 주겠다는 약속.
    std::future<std::string> data = p.get_future();

    std::thread t(worker, &p);

    // 미래에 약속된 데이터를 받을 때 까지 기다린다.
    data.wait();
    std::string s;
    try {
        s = data.get().c_str();        
    }
    catch (const std::exception& e) {
        std::cout << "예외 : " << e.what() << std::endl;
    }
    t.join();

    std::wstring ws;
    ws.assign(s.begin(), s.end());
    return ::SysAllocString((const OLECHAR*)ws.c_str());
}


int sum(const std::vector<int>& v, int start, int end) {
     
    int total = 0;
    for (int i = start; i < end; ++i) {
        total += v[i];
    }
    return total;
}

int parallel_sum(const std::vector<int>& v) {
    // lower_half_future 는 1 ~ 500 까지 비동기적으로 더함
    // 참고로 람다 함수를 사용하면 좀 더 깔끔하게 표현할 수 도 있다.
    // --> std::async([&v]() { return sum(v, 0, v.size() / 2); });
    std::future<int> lower_half_future =
        std::async(std::launch::async, sum, cref(v), 0, v.size() / 2);

    // upper_half 는 501 부터 1000 까지 더함
    int upper_half = sum(v, v.size() / 2, v.size());
    
    return lower_half_future.get() + upper_half;
}

extern "C" __declspec(dllexport) unsigned int AsyncThreadTest_2()
{
    std::vector<int> v;
    v.reserve(100000);

    for (int i = 0; i < 100000; ++i) {
        v.push_back(i + 1);
    }

    return parallel_sum(v);
}



template <std::size_t no>
struct custom_scheduler {
    using result_type = void;

    void operator()(stlab::task<void()> f) const {
        ++counter();
        // The implementation on Windows or the mac uses a scheduler that allows many tasks in the
        // pool in parallel
#if defined(WIN32) || defined(__APPLE__)
        stlab::default_executor(std::move(f));
#else
        // The default scheduler under Linux allows only as many tasks as there are physical cores.
        // But this can lead to a dead lock in some of the tests
        std::thread(std::move(f)).detach();
#endif
    }

    static int usage_counter() { return counter().load(); }

    static void reset() {
        counter() = 0;
    }

    static std::atomic_int& counter() {
        static std::atomic_int counter;
        return counter;
    }

private:
    size_t _id = no; // only used for debugging purpose
};

template <std::size_t I>
stlab::executor_t make_executor() {
    return[_executor = custom_scheduler<I>{}](stlab::task<void()> f) mutable {
        _executor(std::move(f));
    };
}

void AwaitTest()
{
    using namespace std::chrono_literals;
    std::atomic<int> at = 1;
    stlab::serial_queue_t seriel_queue_1{ stlab::default_executor };
    stlab::serial_queue_t seriel_queue_2{ stlab::default_executor };
    stlab::serial_queue_t seriel_queue_3{ stlab::default_executor };

    std::vector<stlab::future<void>> test_futures;
    test_futures.emplace_back(
        stlab::async(seriel_queue_1.executor(), [&]
        {
            while (true)
            {
                if (at == 100 || at > 100) break;
                else
                {
                    std::this_thread::sleep_for(0.3s);
                    at++;
                }                
            }
        }
    ));

    test_futures.emplace_back(
        stlab::async(seriel_queue_2.executor(), [&]
        {
            while (true)
            {
                if (at == 100 || at > 100) break;
                else
                {
                    std::this_thread::sleep_for(0.3s);
                    at++;
                }
            }
        }
    ));
    test_futures.emplace_back(
        stlab::async(seriel_queue_3.executor(), [&]
        {
            while (true) 
            {
                if (at == 100 || at > 100) break;
                else
                {
                    std::this_thread::sleep_for(0.3s);
                    at++;
                }
            }
        }
    ));

    bool done{ false };
    auto done_future = stlab::when_all(stlab::default_executor, [&done] { done = true; },
        std::make_pair(test_futures.begin(), test_futures.end()));

    stlab::await(std::move(done_future));   
}
extern "C" __declspec(dllexport) void AsyncThreadTest_3()
{
    try
    { 
        AwaitTest();
        stlab::pre_exit();
    }
    catch (std::exception ex)
    {
        
    }    
}