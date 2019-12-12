#ifndef THREAD_SAFE_DATA_H
#define THREAD_SAFE_DATA_H

#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>
#include "Controlling.h"
namespace RAT {
class Controlling;
class ThreadSafeData {
public:
    ThreadSafeData() = default;

    // Multiple threads/readers can read the counter's value at the same time.
    void get() const {
        std::shared_lock lock(mutex_);
    }

    // Only one thread/writer can increment/write the counter's value.
    void write() {
        std::unique_lock lock(mutex_);
    }

    void setMovingValue(int v) {
        std::unique_lock lock(mutex_);
        mControlling->setMovingValue(v);
    }

    void setStreeringValue(int v) {
        std::unique_lock lock(mutex_);
        mControlling->setSteeringValue(v);
    }

    void setVehicleMovingValue() {
        std::unique_lock lock(mutex_);
        mControlling->setVehicleMovingValue();
    }

    void setVehicleSteeringValue() {
        std::unique_lock lock(mutex_);
        mControlling->setVehicleSteeringValue();
    }

    void setVehicleMovingNull() {
        std::unique_lock lock(mutex_);
        mControlling->setVehicleMovingNull();
    }

    void setVehicleSteeringNull() {
        std::unique_lock lock(mutex_);
        mControlling->setVehicleSteeringNull();
    }

    void setControlling(Controlling* ptrCnt) {
        mControlling = ptrCnt;
    }

private:
    mutable std::shared_mutex mutex_;
    Controlling* mControlling;
};
}
#endif
