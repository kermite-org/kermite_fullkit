#pragma once

#define Kermite_ConfigStorageFormatRevision 6
#define Kermite_RawHidMessageProtocolRevision 5
#define Kermite_ProfileBinaryFormatRevision 5
#define Kermite_ConfigParametersRevision 5

#ifndef EXTR_KERMITE_VARIATION_NAME
#define EXTR_KERMITE_VARIATION_NAME "dev"
#endif

#ifndef EXTR_KERMITE_PROJECT_RELEASE_BUILD_REVISION
#define EXTR_KERMITE_PROJECT_RELEASE_BUILD_REVISION 0
#endif

#ifndef EXTR_KERMITE_IS_RESOURCE_ORIGIN_ONLINE
#define EXTR_KERMITE_IS_RESOURCE_ORIGIN_ONLINE 0
#endif

#define Kermite_Project_VariationName EXTR_KERMITE_VARIATION_NAME
#define Kermite_Project_ReleaseBuildRevision EXTR_KERMITE_PROJECT_RELEASE_BUILD_REVISION
#define Kermite_Project_IsResourceOriginOnline EXTR_KERMITE_IS_RESOURCE_ORIGIN_ONLINE

#define Kermite_CommonSerialNumberPrefix "A152FD2C"

#ifdef ARDUINO_ARCH_RP2040
#define KERMITE_TARGET_MCU_RP2040
#endif

#if defined KERMITE_TARGET_MCU_RP2040
#define Kermite_Project_McuCode "M02"
#else
// #error KERMITE_TARGET_MCU_* is not defined
#define Kermite_Project_McuCode "M00"
#endif
