# STM32-based Drone Flight Controller

## 📌 Project Overview
This is a drone flight controller project built on the **STM32F405**. 

> 💡 **Note:** The initial structure was based on a public course(M-hive). Sections marked with **(mine)** represent components that I analyzed, redesigned, or independently implemented.

---

## 🛠 Hardware Architecture
* **MCU:** STM32F405
* **Sensor:** BNO080, ICM20602, LPS22HH
* **RC Receiver:** iBus protocol
* **Telemetry:** UART

---

## 💻 Software Architecture
* **OS:** Bare-metal (No RTOS)
* **Main Control Loop:** 1kHz PID attitude control
* **Sensor Read / Comm:** Full DMA Migration (SPI & UART)

---

## 🚀 Key Contributions (mine)

### 0. Architecture Refactoring
* Identified scalability bottlenecks in how the original tutorial organized sensor and communication logic, and successfully restructured the codebase.

### 1. Communication & Sensor Read Optimization: Migration to DMA (SPI & UART)
To optimize the 1kHz control loop, I eliminated CPU bottlenecks by migrating both sensor reads and telemetry communication from blocking/polling methods to **DMA (Direct Memory Access)**.

* **SPI DMA (Sensor Read):** Profiled and compared CPU occupancy using a logic analyzer.
  | Method | CPU Occupancy per Read | Performance Impact |
  | :--- | :--- | :--- |
  | **LL SPI Polling** | ~143µs | Baseline |
  | **HAL SPI DMA** | **~70µs** | **~50% CPU time reduction** |

* **UART DMA Rx:** Following the SPI optimization, I also migrated the UART telemetry communication to DMA. This successfully reduced polling delay, prevented data loss, and maximized CPU headroom for the main control loop.

---

## 📚 Attribution
* **Initial Reference:** [Course/Tutorial Name](Link here — *e.g., https://link-to-tutorial.com*)
