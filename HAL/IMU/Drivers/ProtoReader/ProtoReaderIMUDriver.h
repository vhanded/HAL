#pragma once

#include <HAL/IMU/IMUDriverInterface.h>

#include <PbMsgs/Reader.h>

namespace hal {

class ProtoReaderIMUDriver : public IMUDriverInterface
{
public:
    ProtoReaderIMUDriver(std::string filename);
    ~ProtoReaderIMUDriver();
    void RegisterIMUDataCallback(IMUDriverDataCallback callback);

  bool IsRunning() const override {
    return m_running;
  }

private:
    void _ThreadFunc();

private:
    pb::Reader&             m_reader;
    bool                    m_running;
    std::thread             m_callbackThread;
    IMUDriverDataCallback   m_callback;
};

} /* namespace */
