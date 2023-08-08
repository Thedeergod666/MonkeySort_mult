#include <iostream>
#include <thread>
#include <vector>

#undef ONE
//#include <mutex> //互斥量

int size = 1;

bool isSorted(int arr[], int size) {  // 是否排序正确
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i - 1]) {
            return false;
        }
    }
    return true;
}

void monkeySort(int arr[], int size, long long int& iterations) {
    std::thread::id threadID = std::this_thread::get_id();  
    std::hash<std::thread::id> hasher;
    unsigned int seed = hasher(threadID) ^ std::chrono::system_clock::now().time_since_epoch().count();
    std::srand(seed);  // 使用当前时间+线程id作为随机数生成器的种子

    while (!isSorted(arr, size)) {  //猴排循环
        // 生成两个随机索引
        int index1 = std::rand() % size;
        int index2 = std::rand() % size;

        // 交换两个索引处的元素
        std::swap(arr[index1], arr[index2]);

        iterations++;
    }
}

void myThreadFunction() {  // 线程的具体逻辑
    // int arr[] = { 
    // 50,49,48,47,46,45,44,43,42,41,
    // 40,39,38,37,36,35,34,33,32,31,
    // 30,29,28,27,26,25,24,23,22,21,
    // 20,19,18,17,16,15,14,13,12,11,
    // 10,9,8,7,6,5,4,3,2,1 };  // 待排序的数组

    int* arr = new int[size];
    for (int i = 0; i < size; ++i) {
        arr[i] = size - i;
    }

    //int size = sizeof(arr) / sizeof(arr[0]);

    long long int iterations = 0;  // 记录执行次数

    std::clock_t start = std::clock();  // 记录开始时间

    monkeySort(arr, size, iterations);

    std::clock_t end = std::clock();  // 记录结束时间
    double duration = static_cast<double>(end - start) / CLOCKS_PER_SEC;  // 计算耗时

    // std::lock_guard<std::mutex> lock(std::mutex mtx);  // 互斥量，保证输出不被打断  // 没用到（
    
    std::thread::id threadID = std::this_thread::get_id();  //线程ID
    std::cout << "线程ID：" << threadID << std::endl;

    std::cout << "排序后的数组：";
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "执行次数：" << iterations << std::endl;
    std::cout << "花费时间：" << duration << " 秒" << std::endl;

    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);
    struct std::tm* timeinfo = std::localtime(&timeStamp); 
    char buffer[80]; 
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo); 
    std::cout << "结束时间：" << buffer << std::endl << std::endl; 
}

int main() {
    char c = 'c';
    while(c == 'c' || c == 'C'){
        system("cls"); // 使用windows系统命令清屏

        unsigned int numThreads = std::thread::hardware_concurrency();

        std::cout << "伟大的猴排之旅即将开始，请输入您想排序的数组长度后回车（建议长度15~10）：";
        std::cin >> size;

        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t timeStamp = std::chrono::system_clock::to_time_t(now);
        struct std::tm* timeinfo = std::localtime(&timeStamp);  // 将时间戳转换为本地时间结构tm
        char buffer[80];  // 定义缓冲区
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);  // 格式化输出日期时间
        std::cout <<std::endl<< "猴排开始时间：" << buffer << std::endl;  // 输出日期时间

        std::cout<<"共检测到"<<numThreads<<"个线程，忍不了了，开猴！"<<std::endl<<std::endl;

        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(myThreadFunction));
        }

        // 等待所有线程完成
        for (auto& thread : threads) {
            thread.join();
        }
        
        std::cout<<numThreads<<"个线程全部完成工作~"<<std::endl<<std::endl<<"按C继续，其他键退出";
        std::cin>>c;
    }
    return 0;
}