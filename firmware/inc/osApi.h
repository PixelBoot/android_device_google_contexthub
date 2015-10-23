#ifndef _OS_API_H_
#define _OS_API_H_

#include <stdint.h>
#include <slab.h>


struct I2cEventData {
    void *cookie;
    uint32_t tx;
    uint32_t rx;
    int err;
};

union OsApiSlabItem { //main OS "things" slab must fit this
    struct I2cEventData i2cAppCbkEvt;
    struct {
        uint32_t toTid;
        void *cookie;
    } i2cAppCbkInfo;
};


//EXTERNAL API
//level 1 indices in the OS table
#define SYSCALL_OS_MAIN                   0
#define SYSCALL_OS_DRIVERS                1
#define SYSCALL_OS_LAST                   2 // always last. holes are allowed, but not immediately before this

//level 2 indices in the OS.drivers table
#define SYSCALL_OS_DRV_GPIO               0
#define SYSCALL_OS_DRV_I2C_MASTER         1
#define SYSCALL_OS_DRV_I2C_SLAVE          2
#define SYSCALL_OS_DRV_LAST               3 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.drivers.gpio table
/* more thing here eventually */
#define SYSCALL_OS_DRV_GPIO_REQ           0 // (uint32_t gpioNum) -> void* handle
#define SYSCALL_OS_DRV_GPIO_REL           1 // (handle) -> void
#define SYSCALL_OS_DRV_GPIO_CFG_IN        2 // (handle, i32 gpioSpeed, GpioPullMode pull) -> void
#define SYSCALL_OS_DRV_GPIO_CFG_OUT       3 // (handle, i32 gpioSpeed, GpioPullMode pull, GpioOpenDrainMode output, bool value) -> void
#define SYSCALL_OS_DRV_GPIO_CFG_ALT       4 // (handle, i32 gpioSpeed, GpioPullMode pull, GpioOpenDrainMode output, u32 altFunc) -> void
#define SYSCALL_OS_DRV_GPIO_GET           5 // (handle) -> bool isHigh
#define SYSCALL_OS_DRV_GPIO_SET           6 // (handle, bool value) -> void
#define SYSCALL_OS_DRV_GPIO_LAST          7 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.drivers.i2cM table
#define SYSCALL_OS_DRV_I2CM_REQ           0 // (I2cBus busId, I2cSpeed speed) -> int status
#define SYSCALL_OS_DRV_I2CM_REL           1 // (I2cBus busId) -> int status
#define SYSCALL_OS_DRV_I2CM_TXRX          2 // (I2cBus busId, I2cAddr addr, const void *txBuf, size_t txSize, void *rxBuf, size_t rxSize, uint32_t yourTidForEvts, void *cookie) -> int status
#define SYSCALL_OS_DRV_I2CM_LAST          3 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.drivers.i2cS table
#define SYSCALL_OS_DRV_I2CS_REQ           0 // (I2cBus busId, I2cAddr addr) -> int status
#define SYSCALL_OS_DRV_I2CS_REL           1 // (I2cBus busId) -> int status
#define SYSCALL_OS_DRV_I2CS_RX_EN         2 // (I2cBus busId, void *rxBuf, size_t rxSize, uint32_t yourTidForEvts, void *cookie) -> void
#define SYSCALL_OS_DRV_I2CS_TX_PRE        3 // (I2cBus busId, uint8_t byte, uint32_t yourTidForEvts, void *cookie) -> int status
#define SYSCALL_OS_DRV_I2CS_TX_PKT        4 // (I2cBus busId, const void *txBuf, size_t txSize, uint32_t yourTidForEvts, void *cookie) -> int status
#define SYSCALL_OS_DRV_I2CS_LAST          5 // always last. holes are allowed, but not immediately before this

//level 2 indices in the OS.main table
#define SYSCALL_OS_MAIN_EVENTQ            0
#define SYSCALL_OS_MAIN_LOGGING           1
#define SYSCALL_OS_MAIN_SENSOR            2
#define SYSCALL_OS_MAIN_LAST              3 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.main.event_queue table
#define SYSCALL_OS_MAIN_EVTQ_SUBCRIBE    0 // (uint32_t tid, uint32_t evtType) -> bool success
#define SYSCALL_OS_MAIN_EVTQ_UNSUBCRIBE  1 // ((uint32_t tid, uint32_t evtType) -> bool success
#define SYSCALL_OS_MAIN_EVTQ_ENQUEUE     2 // (uint32_t evtType, void *evtData, uint32_t tidForFreeEvt, bool external) -> bool success
#define SYSCALL_OS_MAIN_EVTQ_LAST        3 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.main.logging table
#define SYSCALL_OS_MAIN_LOG_LOGV         0 // (enum LogLevel level, const char *str, va_list *) -> void
#define SYSCALL_OS_MAIN_LOG_LAST         1 // always last. holes are allowed, but not immediately before this

//level 3 indices in the OS.main.sensors table
#define SYSCALL_OS_MAIN_SENSOR_SIGNAL    0 // (uint32_t handle, uint32_t intEvtNum, uint32_t value1, uint32_t value2_lo, uint32_t value2_hi) -> bool /* used by sensor-provding apps */
#define SYSCALL_OS_MAIN_SENSOR_REG       1 // (const struct SensorInfo *si, uint32_t tid) -> u32 handle
#define SYSCALL_OS_MAIN_SENSOR_UNREG     2 // (uint32_t handle) -> bool
#define SYSCALL_OS_MAIN_SENSOR_FIND      3 // (uint32_t sensorType, uint32_t idx, uint32_t *handleP) -> const struct SensorInfo* or NULL
#define SYSCALL_OS_MAIN_SENSOR_REQUEST   4 // (uint32_t clientId, uint32_t sensorHandle, uint32_t rate) -> bool success
#define SYSCALL_OS_MAIN_SENSOR_RATE_CHG  5 // (uint32_t clientId, uint32_t sensorHandle, uint32_t newRate) -> bool success
#define SYSCALL_OS_MAIN_SENSOR_RELEASE   5 // (uint32_t clientId, uint32_t sensorHandle) -> bool success
#define SYSCALL_OS_MAIN_SENSOR_TRIGGER   7 // (uint32_t clientId, uint32_t sensorHandle) -> bool success
#define SYSCALL_OS_MAIN_SENSOR_GET_RATE  8 // (uint32_t sensorHandle) -> uint32_t rate
#define SYSCALL_OS_MAIN_SENSOR_LAST      9 // always last. holes are allowed, but not immediately before this






//called by os entry point to export the api
void osApiExport(struct SlabAllocator *mainSlubAllocator);




#endif
