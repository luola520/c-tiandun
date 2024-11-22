#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include <iphlpapi.h>
#include <curl/curl.h>
#include <nlohmann/json.hpp> // 使用 JSON 解析库 https://github.com/nlohmann/json

#pragma comment(lib, "iphlpapi.lib") // 链接 IPHelper 库


// 硬编码的参数
const std::string SOFTWARE_NAME = "test";
const std::string SOFTWARE_VERSION = "1";

// 获取MAC地址的函数
std::string getMacAddress() {
    IP_ADAPTER_INFO AdapterInfo[16];       // 预分配16个适配器的缓冲区
    DWORD BufferLength = sizeof(AdapterInfo);

    if (GetAdaptersInfo(AdapterInfo, &BufferLength) != ERROR_SUCCESS) {
        return "00:00:00:00:00:00"; // 获取失败返回默认值
    }

    PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
    char macAddr[18];
    sprintf_s(macAddr, "%02X:%02X:%02X:%02X:%02X:%02X",
              pAdapterInfo->Address[0],
              pAdapterInfo->Address[1],
              pAdapterInfo->Address[2],
              pAdapterInfo->Address[3],
              pAdapterInfo->Address[4],
              pAdapterInfo->Address[5]);
    return std::string(macAddr);
}

// 发送HTTP GET请求并返回响应
std::string httpGet(const std::string& url) {
    CURL* curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, +[](void* contents, size_t size, size_t nmemb, void* userp) -> size_t {
            ((std::string*)userp)->append((char*)contents, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return response;
}

// 验证卡密
bool verifyCard(const std::string& card) {
    std::string mac = getMacAddress();
    std::string url = "http://154.12.35.129:19001/AzPIwdmeddCUr8pSd4W7i/?card=" + card +
                      "&mac=" + mac +
                      "&soft=" + SOFTWARE_NAME +
                      "&var=" + SOFTWARE_VERSION;
    std::string response = httpGet(url);

    // 解析JSON
    auto jsonResponse = nlohmann::json::parse(response);
    int status = jsonResponse["status"];
    if (status == 2000) {
        std::cout << "验证成功！" << std::endl;
        return true;
    } else {
        std::cout << "验证失败: " << jsonResponse["msg"] << std::endl;
        return false;
    }
}

// 心跳验证
bool heartbeat(const std::string& card) {
    std::string mac = getMacAddress();
    std::string url = "http://154.12.35.129:19001/uxItb4abOmlCG6O8BH0LH/?card=" + card +
                      "&mac=" + mac +
                      "&soft=" + SOFTWARE_NAME +
                      "&var=" + SOFTWARE_VERSION;
    std::string response = httpGet(url);

    // 解析JSON
    auto jsonResponse = nlohmann::json::parse(response);
    int status = jsonResponse["status"];
    if (status == 2000) {
        return true;
    } else {
        return false;
    }
}

// 主程序
int main() {
    std::string card;

    std::cout << "请输入卡密: ";
    std::cin >> card;

    // 验证卡密
    if (!verifyCard(card)) {
        std::cout << "卡密验证失败，程序即将退出！" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(3)); // 停留几秒显示失败信息
        return 1; // 验证失败退出
    }

    // 验证成功
    std::cout << "欢迎使用系统！程序正在运行中..." << std::endl;
    std::cout << "--------------------------------------------" << std::endl;

    // 开始心跳验证线程
    bool running = true;
    std::thread heartbeatThread([&]() {
        while (running) {
            if (!heartbeat(card)) {
                running = false;
                std::cout << "\n[错误] 心跳验证失败！程序即将退出..." << std::endl;
                break;
            } else {
                std::cout << "[心跳] 验证成功。" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒一次心跳
        }
    });

    // 主线程继续运行，并等待用户手动结束
    std::cout << "[提示] 按 Enter 键退出程序..." << std::endl;
    std::cin.ignore(); // 忽略之前的换行符
    std::cin.get();    // 等待用户按下回车键

    running = false;  // 通知心跳线程退出
    heartbeatThread.join(); // 等待心跳线程结束

    std::cout << "程序已退出。" << std::endl;
    return 0;
}
