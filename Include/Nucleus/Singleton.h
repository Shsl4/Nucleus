#pragma once

#include <atomic>
#include <Nucleus/Shared.h>

#define SINGLETON_BODY(__class)                                                     \
                                                                                    \
private:                                                                            \
                                                                                    \
    friend Nucleus::Allocator<__class>;                                             \
                                                                                    \
    static inline Nucleus::Shared<__class> instance_ = nullptr;                     \
    static inline std::mutex mutex_ = {};                                           \
                                                                                    \
    __class() = default;                                                            \
                                                                                    \
public:                                                                             \
                                                                                    \
    __class(__class &other) = delete;                                               \
                                                                                    \
    void operator=(const __class&) = delete;                                        \
                                                                                    \
    static __class* instance(){                                                     \
                                                                                    \
        std::lock_guard<std::mutex> lock(mutex_);                                   \
                                                                                    \
        if (!instance_.valid()) { instance_ = Nucleus::Shared<__class>::make(); }   \
                                                                                    \
        return instance_.pointer();                                                 \
                                                                                    \
    }                                                                               \
                                                                                    \
    private: