#pragma once

#include <siberix/core/time.h>
#include <siberix/device/device.h>

class ProcessorDevice : public Device {
public:
    ProcessorDevice(u32 _processorId);
    ~ProcessorDevice();

    void enable() override;
    void disable() override;

    u32 getProcessorId() { return m_processorId; }

private:
    u32 m_processorId;
};

class VisualOutputDevice : public Device {};

class TimerDevice : public Device {
public:
    TimerDevice(const char *name)
        : Device(name, DeviceBus::Software, DeviceType::SystemDevices) {}
    ~TimerDevice() {}

    virtual void     sleep(Duration duration) = 0;
    virtual void     sleep(u64 ms)            = 0;
    virtual Duration time()                   = 0;

    const char *getName() { return m_name; }
    u8          getId() { return m_id; }

private:
    u8 m_id;
};