# Drone Firmware — Update Log

---

## Added Features

### PC UART
- Added UART6 for debug output

### M8N GPS
- Added UART4-based GPS packet reception

### FS-i6AB (IBUS Receiver)
- Added UART5-based IBUS packet reception

### Telemetry (ROHS)
- Added UART1-based AHRS data transmission

---

## Bug Fix

### SPI OVR Flag After DMA Callback

**Problem**

After `HAL_SPI_TxCpltCallback` was triggered, reusing SPI caused an OVR (Overrun) error,
which made subsequent SPI communication fail.

**Root Cause**

There were two causes:

The DMA TX complete callback fires when the DMA finishes transferring data to the SPI buffer,
but the SPI hardware may still be shifting out the last byte at that point.
Pulling CS low or starting the next transmission while the BSY flag is still set triggers an OVR error.


**Solution**

```c
while (__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_BSY))
{
    if (timeout-- == 0) break;
}
__HAL_SPI_CLEAR_OVRFLAG(&hspi1);
```

- Poll `SPI_FLAG_BSY` to wait until the SPI hardware is fully idle before pulling CS low or re-triggering DMA
- Added timeout to prevent infinite loop
- Clear OVR flag explicitly with `__HAL_SPI_CLEAR_OVRFLAG` to ensure the next transfer starts in a clean state