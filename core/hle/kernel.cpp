// Copyright 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>
#include "common/archives.h"
#include "common/serialization/atomic.h"
#include "client_port.h"
#include "config_mem.h"
#include "handle_table.h"
#include "recorder.h"
#include "kernel.h"
#include "memory.h"
#include "process.h"
#include "resource_limit.h"
#include "shared_page.h"
#include "thread.h"
#include "timer.h"

namespace Kernel {

/// Initialize the kernel
KernelSystem::KernelSystem(Memory::MemorySystem& memory, Core::Timing& timing,
                           std::function<void()> prepare_reschedule_callback, u32 system_mode,
                           u32 num_cores, u8 n3ds_mode)
    : memory(memory), timing(timing),
      prepare_reschedule_callback(std::move(prepare_reschedule_callback)) {
    std::generate(memory_regions.begin(), memory_regions.end(),
                  [] { return std::make_shared<MemoryRegionInfo>(); });
    MemoryInit(system_mode, n3ds_mode);

    resource_limits = std::make_unique<ResourceLimitList>(*this);
    for (u32 core_id = 0; core_id < num_cores; ++core_id) {
        thread_managers.push_back(std::make_unique<ThreadManager>(*this, core_id));
    }
    timer_manager = std::make_unique<TimerManager>(timing);
    ipc_recorder = std::make_unique<IPCDebugger::Recorder>();
    stored_processes.assign(num_cores, nullptr);

    next_thread_id = 1;
}

/// Shutdown the kernel
KernelSystem::~KernelSystem() {
    ResetThreadIDs();
};

ResourceLimitList& KernelSystem::ResourceLimit() {
    return *resource_limits;
}

const ResourceLimitList& KernelSystem::ResourceLimit() const {
    return *resource_limits;
}

u32 KernelSystem::GenerateObjectID() {
    return next_object_id++;
}

std::shared_ptr<Process> KernelSystem::GetCurrentProcess() const {
    return current_process;
}

void KernelSystem::SetCurrentProcess(std::shared_ptr<Process> process) {
    current_process = process;
    SetCurrentMemoryPageTable(process->vm_manager.page_table);
}

void KernelSystem::SetCurrentProcessForCPU(std::shared_ptr<Process> process, u32 core_id) {
    if (current_cpu->GetID() == core_id) {
        current_process = process;
        SetCurrentMemoryPageTable(process->vm_manager.page_table);
    } else {
        stored_processes[core_id] = process;
        thread_managers[core_id]->cpu->SetPageTable(process->vm_manager.page_table);
    }
}

void KernelSystem::SetCurrentMemoryPageTable(std::shared_ptr<Memory::PageTable> page_table) {
    memory.SetCurrentPageTable(page_table);
    if (current_cpu != nullptr) {
        current_cpu->SetPageTable(page_table);
    }
}

void KernelSystem::SetCPUs(std::vector<std::shared_ptr<ARM_Interface>> cpus) {
    ASSERT(cpus.size() == thread_managers.size());
    u32 i = 0;
    for (const auto& cpu : cpus) {
        thread_managers[i++]->SetCPU(*cpu);
    }
}

void KernelSystem::SetRunningCPU(ARM_Interface* cpu) {
    if (current_process) {
        stored_processes[current_cpu->GetID()] = current_process;
    }
    current_cpu = cpu;
    timing.SetCurrentTimer(cpu->GetID());
    if (stored_processes[current_cpu->GetID()]) {
        SetCurrentProcess(stored_processes[current_cpu->GetID()]);
    }
}

ThreadManager& KernelSystem::GetThreadManager(u32 core_id) {
    return *thread_managers[core_id];
}

const ThreadManager& KernelSystem::GetThreadManager(u32 core_id) const {
    return *thread_managers[core_id];
}

ThreadManager& KernelSystem::GetCurrentThreadManager() {
    return *thread_managers[current_cpu->GetID()];
}

const ThreadManager& KernelSystem::GetCurrentThreadManager() const {
    return *thread_managers[current_cpu->GetID()];
}

TimerManager& KernelSystem::GetTimerManager() {
    return *timer_manager;
}

const TimerManager& KernelSystem::GetTimerManager() const {
    return *timer_manager;
}

SharedPage::Handler& KernelSystem::GetSharedPageHandler() {
    return *shared_page_handler;
}

const SharedPage::Handler& KernelSystem::GetSharedPageHandler() const {
    return *shared_page_handler;
}

IPCDebugger::Recorder& KernelSystem::GetIPCRecorder() {
    return *ipc_recorder;
}

const IPCDebugger::Recorder& KernelSystem::GetIPCRecorder() const {
    return *ipc_recorder;
}

void KernelSystem::AddNamedPort(std::string name, std::shared_ptr<ClientPort> port) {
    named_ports.emplace(std::move(name), std::move(port));
}

u32 KernelSystem::NewThreadId() {
    return next_thread_id++;
}

void KernelSystem::ResetThreadIDs() {
    next_thread_id = 0;
}

template <class Archive>
void KernelSystem::serialize(Archive& ar, const unsigned int file_version) {
    ar& memory_regions;
    ar& named_ports;
    // current_cpu set externally
    // NB: subsystem references and prepare_reschedule_callback are constant
    ar&* resource_limits.get();
    ar& next_object_id;
    ar&* timer_manager.get();
    ar& next_process_id;
    ar& process_list;
    ar& current_process;
    // NB: core count checked in 'core'
    for (auto& thread_manager : thread_managers) {
        ar&* thread_manager.get();
    }
    ar& config_mem_handler;
    ar& shared_page_handler;
    ar& stored_processes;
    ar& next_thread_id;
    // Deliberately don't include debugger info to allow debugging through loads

    if (Archive::is_loading::value) {
        for (auto& memory_region : memory_regions) {
            memory_region->Unlock();
        }
        for (auto& process : process_list) {
            process->vm_manager.Unlock();
        }
    }
}

SERIALIZE_IMPL(KernelSystem)

} // namespace Kernel
