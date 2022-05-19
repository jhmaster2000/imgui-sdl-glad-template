#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>

std::string currentTime(std::chrono::time_point<std::chrono::system_clock> now) {
    // you need to get milliseconds explicitly
    std::chrono::duration<long long, std::milli> ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    // and that's a "normal" point of time with seconds
    __time64_t timeNow = std::chrono::system_clock::to_time_t(now);

    std::ostringstream currentTimeStream;
    currentTimeStream << std::put_time(localtime(&timeNow), "%d.%m.%Y %H:%M:%S")
                      << "." << std::setfill('0') << std::setw(3) << ms.count()
                      << " " << std::put_time(localtime(&timeNow), "%z");

    return currentTimeStream.str();
}

static auto vectorGetter = [](void* vec, int idx, const char** out_text) {
    std::vector<std::string>& vector = *static_cast<std::vector<std::string> *>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) return false;
    *out_text = vector.at(idx).c_str();
    return true;
};
