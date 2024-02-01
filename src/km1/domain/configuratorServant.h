#pragma once

#include "kmTypes.h"

enum {
  ConfiguratorServantEvent_ConnectedByHost = 0x01,
  ConfiguratorServantEvent_ConnectionClosingByHost = 0x02,
  ConfiguratorServantEvent_KeyMemoryUpdateStarted = 0x10,
  ConfiguratorServantEvent_KeyMemoryUpdateDone = 0x11,
  ConfiguratorServantEvent_SimulatorModeEnabled = 0x20,
  ConfiguratorServantEvent_SimulatorModeDisabled = 0x21,
  ConfiguratorServantEvent_MuteModeEnabled = 0x22,
  ConfiguratorServantEvent_MuteModeDisabled = 0x23,
};

void configuratorServant_initialize(
    void (*stateNotificationCallback)(uint8_t event));
void configuratorServant_processUpdate();
void configuratorServant_emitRealtimeKeyEvent(uint8_t keyIndex, bool isDown);
void configuratorServant_emitRealtimeLayerEvent(uint16_t layerFlags);
