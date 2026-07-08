# 🛸 STM32-based Drone Flight Controller (Success Flying)

STM32F405 MCU 기반의 드론 비행 제어기(Flight Controller, FC) 개발 프로젝트입니다.

> 💡 **안내:** 전체적인 초기 구조는 M-hive의 공개 강의를 바탕으로 구축되었으나, **(mine)** 표시가 된 세부 항목은 기존 코드의 확장성 및 성능 한계를 직접 분석하여 **설계 변경, 리팩토링 및 독자적인 최적화를 수행한 핵심 기여 내용**입니다.

---

## 🛠 Hardware Architecture
* **MCU:** STM32F405
* **Sensor:** BNO080, ICM20602, LPS22HH
* **RC Receiver:** iBus Protocol
* **Telemetry:** UART

---

## 💻 Software Architecture
* **OS:** Bare-metal (No RTOS)
* **Main Control Loop:** 1kHz PID Attitude & Rate Control
* **Sensor Read / Comm:** Full DMA Migration (SPI & UART)

---

## 🚀 Key Contributions (mine)

### 1. Architecture Refactoring
* **문제 정의:** 기존 교육용 소스코드의 센서 데이터 획득 및 통신 로직 구조가 기능 추가 시 상호 의존성을 높이고 확장성을 저해하는 Bottleneck임을 식별.
* **해결 방안:** 드라이버 레이어와 제어 루프 레이어를 명확히 분리하고 모듈화를 진행하여, 새로운 센서나 통신 프로토콜을 쉽게 탑재할 수 있도록 구조적 결합도를 낮춤.

### 2. Communication & Sensor Read Optimization: Migration to DMA (SPI & UART)
안정적인 1kHz 제어 루프(1ms 이내에 센서 데이터 획득, PID 연산, 모터 PWM 출력을 모두 완료해야 함)를 확보하기 위해, 기존의 Blocking/Polling 방식을 탈피하고 **DMA(Direct Memory Access)** 기반으로 전면 마이그레이션을 진행하여 CPU 오버헤드를 극적으로 줄였습니다.

#### 📊 SPI DMA (Sensor Read)
로직 분석기(Logic Analyzer)를 통해 센서 리드 시의 CPU 점유 시간을 직접 프로파일링하고 비교 분석했습니다.

| Method | CPU Occupancy per Read | Performance Impact |
| :--- | :--- | :--- |
| **LL SPI Polling (기존)** | ~143µs | Baseline |
| **HAL SPI DMA (개선) (mine)** | **~70µs** | **~50% CPU 자원 소모 절감** |

* **성과:** 센서 데이터를 읽어오는 동안 CPU가 자유로워져 제어 루프 내 컴퓨팅 마진을 대폭 확보하였으며, 추후 칼만 필터나 고도화된 제어 알고리즘을 추가할 수 있는 기반을 마련했습니다.

#### 📡 UART DMA Rx
SPI 최적화 성공 모델을 기반으로, RC 조종기 신호(iBus) 및 Telemetry 데이터를 수신하는 UART 통신 채널 역시 DMA 방식으로 마이그레이션했습니다.
* **성과:** 다른 연산 중에도 수신 데이터가 메모리 버퍼에 자동으로 적재되도록 설계하여, Polling 지연을 제거하고 고속 비행 환경에서도 데이터 누락(Data Loss)을 완벽하게 방지했습니다.

---

## 📚 Attribution
* **Initial Reference:** M-hive STM32 드론 제어 계측 강의 (https://link-to-tutorial.com)
