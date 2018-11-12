// Copyright 2011, Tencent Inc.

#ifndef COMMON_SYSTEM_CPU_CPU_USAGE_H
#define COMMON_SYSTEM_CPU_CPU_USAGE_H

#include <stdint.h>
#include <string>

unsigned int GetLogicalCpuNumber();
unsigned long long GetPhysicalMemorySize();
bool GetOsKernelInfo(std::string* kernerl_info);
unsigned long GetHertz();
int GetLinuxVersion();

/// @brief Get cpu usage of a specified process.
/// This function is implemented simply on windows platform.
/// On windows os, it's not reenterable.
/// @param pid process id
/// @param cpu cpu usage
/// @retval true if function runs succesfully
bool GetCpuUsage(int32_t pid, double* cpu);

/// @brief Get cpu usage of a specified process.
/// This function is implemented simply on windows platform.
/// On windows os, it's not reenterable.
/// @param pid process id
/// @param cpu cpu usage
/// @retval true if function runs succesfully
bool GetCpuUsageSinceLastCall(int32_t pid, double* cpu);

/// @brief Get cpu usage of a specified process.
/// This function is implemented simply on windows platform.
/// On windows os, it's not reenterable.
/// @param pid process id
/// @param sample_internal sample internal time (ms)
/// @param cpu cpu usage
/// @retval true if function runs succesfully
bool GetProcessCpuUsage(int32_t pid, uint64_t sample_period, double* cpu);

/// @brief Get cpu usage of a specified thread.
/// This function is implemented simply on linux platform, not support windows,
/// @param pid process id
/// @param tid thread id
/// @param sample_internal sample internal time (ms)
/// @param cpu cpu usage
/// @retval true if function runs succesfully
bool GetThreadCpuUsage(int32_t pid, int tid, uint64_t sample_period, double* cpu);

/// @brief Get cpu time of system.
/// This function is implemented simply on linux platform, not support windows,
/// @param total_cpu_time total cpu time
/// @retval true if function runs succesfully
bool GetTotalCpuTime(uint64_t* total_cpu_time);

/// @brief Get cpu time of a specified process.
/// This function is implemented simply on linux platform, not support windows,
/// @param pid process id
/// @param process_cpu_time process cpu time
/// @retval true if function runs succesfully
bool GetProcessCpuTime(int32_t pid, uint64_t* process_cpu_time);

/// @brief Get cpu time of a specified thread.
/// This function is implemented simply on linux platform, not support windows,
/// @param pid process id
/// @param tid thread id
/// @param thread_cpu_time thread cpu time
/// @retval true if function runs succesfully
bool GetThreadCpuTime(int32_t pid, int tid, uint64_t* thread_cpu_time);

bool getMemoryUsedKiloBytes(int32_t pid, uint64_t* vm_size, uint64_t* mem_size);

bool getMemoryUsedBytes(int32_t pid, uint64_t* vm_size, uint64_t* mem_size);

#endif // COMMON_SYSTEM_CPU_CPU_USAGE_H
