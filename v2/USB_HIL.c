#include "USB_HIL.c"
#include "usbd_cdc.h"
#include "usbd_cdc_if.h"
#include "cmsis_os.h"

bool USBDeinit()
{
  if (USBD_Stop(&hUsbDeviceFS) != USBD_OK)
  {
    return false;
  }

  /* Deinitialize the USB device library */
  if (USBD_DeInit(&hUsbDeviceFS) != USBD_OK)
  {
    return false;
  }

  /* Disable USB voltage detector */
  HAL_PWREx_DisableUSBVoltageDetector();

  return true;
}

void MX_USB_DEVICE_Init_CDC(void)
{
  /* Init Device Library, add supported class and start the library. */
  if (USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS) != USBD_OK)
  {
    Error_Handler();
  }
  if (USBD_Start(&hUsbDeviceFS) != USBD_OK)
  {
    Error_Handler();
  }

  /* Enable USB voltage detector */
  HAL_PWREx_EnableUSBVoltageDetector();
}

void handleUSBHILData(void *argument)
{
  uint32_t notificationValue;

  // Infinite loop
  for (;;)
  {
    // Wait for a task notification
    xTaskNotifyWait(0, 0, &notificationValue, portMAX_DELAY);

    // Handle the USB HIL data

    // Process the received data
  }
}