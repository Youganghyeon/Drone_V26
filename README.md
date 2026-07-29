# 🛸 STM32-based Drone Flight Controller (Success Flying)

STM32F405 MCU 기반의 드론 비행 제어기(Flight Controller, FC) 개발 프로젝트입니다.

> 💡 전체적인 초기 구조는 M-hive의 공개 강의를 바탕으로 구축되었으며, **(mine)** 표시가 된 항목은 기존 코드의 구조적 한계를 직접 분석하여 리팩토링, 최적화, 또는 신규 설계한 내용입니다.

---

## 🛠 Hardware Architecture

* **MCU:** STM32F405
* **Sensor:** BNO080(IMU), ICM20602(IMU), LPS22HH(기압)
* **RC Receiver:** FS-iA6B (iBus Protocol)
* **Telemetry:** UART

---

## 💻 Software Architecture

* **OS:** Bare-metal (No RTOS)
* **Main Control Loop:** 1kHz Cascade PID (Rate Loop + Attitude Loop)
* **Sensor Read / Comm:** DMA Migration (SPI & UART)
* **Safety:** Battery Low-Voltage Fail-Safe, Communication Link Monitoring

---

## 🚀 Key Contributions (mine)

### 1. Architecture Refactoring

확장성을 위해 드라이버 레이어와 제어 루프 레이어를 명확히 분리하고 기능 단위로 모듈화하여, 새로운 센서나 통신 프로토콜을 쉽게 추가할 수 있는 구조로 리팩토링했습니다.

### 2. Communication & Sensor Read Optimization: Migration to DMA (SPI & UART)

안정적인 1kHz 제어 루프(1ms 이내에 센서 데이터 획득, PID 연산, 모터 PWM 출력을 모두 완료해야 함)를 확보하기 위해, 기존의 Blocking/Polling 방식을 탈피하고 **DMA(Direct Memory Access)** 기반으로 적용하여 CPU 오버헤드를 줄였습니다.

#### 📊 SPI DMA (Sensor Read)

로직 분석기(Logic Analyzer)를 통해 센서 비교 분석했습니다.

| Method | CPU Occupancy per Read | Performance Impact |
| :--- | :--- | :--- |
| **LL SPI Polling (기존)** | ~143µs | Baseline |
| **HAL SPI DMA (개선) (mine)** | **~70µs** | **~50% CPU 자원 소모 절감** |


### 3. Custom UART/USB Bootloader Design (mine)

Header/Length/Data/CRC로 구성된 자체 통신 프로토콜을 FSM(Finite State Machine) 기반으로 설계하고, Flash 1024KB 중 64KB를 Bootloader 영역, 나머지 960KB를 Application 영역으로 파티셔닝했습니다. NRST 4회 입력 또는 iBus 프로토콜로 전달되는 RC 송신기 스위치 감지 시 업데이트 모드로 진입하도록 구현했고, 그 외에는 즉시 Application으로 Jump하도록 설계했습니다. 

---

## 📚 Attribution

* **Initial Reference:** M-hive STM32 드론 제어 계측 강의 (https://link-to-tutorial.com)
