# BNO080 DMA Communication Bug

## Phenomenon
BNO080_Open();
BNO080_enableRotationVector();
delay(400);  // <- Adding this delay causes communication failure after a few cycles

## Prediction
Open() → enableRotationVector() → delay(400)
  → During 400ms, BNO080 keeps generating rotation vector data
  → Data accumulates in internal buffer
  → When ReadInfo() finally starts,
     BNO080 sends accumulated data as a large packet
  → Packet size exceeds 128 bytes (MAX_PACKET_SIZE)
  → Bug path triggered → State corruption

## Root Cause
DMA transfer starts
  ↓
receivePacket_DMA() returns false
  (when dataLength >= MAX_PACKET_SIZE, ret was never set to true)
  ↓
Caller executes CHIP_DESELECT + return false
  ↓
BNO080_Flag never transitions to Active (stays IDLE)
  ↓
DMA is still running, but CS is deasserted and Flag is IDLE
  ↓
Next loop attempts to start another DMA transfer → State corruption

## Fix
if (dataLength >= MAX_PACKET_SIZE)
{
    SPI_SendReceive_DMA(..., MAX_PACKET_SIZE);
    ret = true;  // Missing return value fix
}