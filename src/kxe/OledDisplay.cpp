#include "OledDisplay.h"

#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"
#include "km1/domain/keyboardMain.h"

static Adafruit_SSD1306 *display = nullptr;
static bool moduleActive = false;
static int pinSCL;
static int pinSDA;

//----------------------------------------------------------------------

static uint8_t getPressedKeyCode(const uint8_t *report) {
  for (int i = 2; i < 8; i++) {
    if (report[i] > 0) {
      return report[i];
    }
  }
  return 0;
}

static char strbuf[8];

#define pm(x) (x > 0 ? '+' : '-')

static void graphics_drawText(int y, int x, const char *text) {
  display->setCursor(x * 6, y * 8);
  display->println(text);
}

static void renderStatusView() {

  display->clearDisplay();
  display->setTextSize(1);

  graphics_drawText(0, 0, "Status");

  KeyboardMainExposedState *exposedState = &keyboardMain_exposedState;

  // hid key slots
  const uint8_t *b = exposedState->hidReportBuf;
  sprintf(strbuf, "%c%c%c%c%c%c", pm(b[2]), pm(b[3]), pm(b[4]), pm(b[5]),
          pm(b[6]), pm(b[7]));

  graphics_drawText(0, 15, strbuf);

  // key index
  uint8_t ki = exposedState->pressedKeyIndex;

  if (ki != KEYINDEX_NONE) {
    // keycode
    uint8_t kc = getPressedKeyCode(exposedState->hidReportBuf);
    // modifiers
    uint8_t m = exposedState->hidReportBuf[0];

    sprintf(strbuf, "KI:%d", ki);
    graphics_drawText(3, 0, strbuf);
    sprintf(strbuf, "KC:%d", kc);
    graphics_drawText(3, 6, strbuf);
    sprintf(strbuf, "M:%x", m);
    graphics_drawText(3, 13, strbuf);
  } else {
    sprintf(strbuf, "KI:");
    graphics_drawText(3, 0, strbuf);
    sprintf(strbuf, "KC:");
    graphics_drawText(3, 6, strbuf);
    sprintf(strbuf, "M:");
    graphics_drawText(3, 13, strbuf);
  }

  // layers
  uint8_t lsf = exposedState->layerStateFlags;
  sprintf(strbuf, "L:%x", lsf);
  graphics_drawText(3, 18, strbuf);

  display->display();
}

//----------------------------------------------------------------------

#define LOGO_HEIGHT 32
#define LOGO_WIDTH 128
static const uint8_t mainLogoData_kermiteDefault[] = {
    // https://i.imgur.com/5xe1T46.png
    // converted with https://javl.github.io/image2cpp/
    // 'kermite_logo, 128x32px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x0f, 0x01, 0xf0, 0xff, 0xfc, 0x7f, 0xe0, 0x3c,
    0x00, 0x78, 0x3c, 0x3f, 0xff, 0xc7, 0xff, 0xe0, 0x0f, 0x03, 0xf0, 0xff,
    0xfc, 0x7f, 0xf0, 0x3e, 0x00, 0xf8, 0x3c, 0x3f, 0xff, 0xc7, 0xff, 0xe0,
    0x0f, 0x07, 0xe0, 0xff, 0xfc, 0x7f, 0xf8, 0x3f, 0x01, 0xf8, 0x3c, 0x3f,
    0xff, 0xc7, 0xff, 0xe0, 0x0f, 0x0f, 0xc0, 0xff, 0xfc, 0x7f, 0xfc, 0x3f,
    0x83, 0xf8, 0x3c, 0x3f, 0xff, 0xc7, 0xff, 0xe0, 0x0f, 0x0f, 0x80, 0xf0,
    0x00, 0x78, 0x7e, 0x3f, 0xc7, 0xf8, 0x3c, 0x00, 0xf0, 0x07, 0x80, 0x00,
    0x0f, 0x1f, 0x80, 0xf0, 0x00, 0x78, 0x3e, 0x3f, 0xef, 0xf8, 0x3c, 0x00,
    0xf0, 0x07, 0x80, 0x00, 0x0f, 0x3f, 0x00, 0xf0, 0x00, 0x78, 0x1e, 0x3f,
    0xff, 0xf8, 0x3c, 0x00, 0xf0, 0x07, 0x80, 0x00, 0x0f, 0x7e, 0x00, 0xff,
    0xc0, 0x78, 0x1e, 0x3d, 0xff, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xfe, 0x00,
    0x0f, 0xfc, 0x00, 0xff, 0xc0, 0x78, 0x3e, 0x3c, 0xfe, 0x78, 0x3c, 0x00,
    0xf0, 0x07, 0xfe, 0x00, 0x0f, 0xfc, 0x00, 0xff, 0xc0, 0x78, 0x7e, 0x3c,
    0x7c, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xfe, 0x00, 0x0f, 0xfe, 0x00, 0xff,
    0xc0, 0x7f, 0xfc, 0x3c, 0x38, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xfe, 0x00,
    0x0f, 0xff, 0x00, 0xf0, 0x00, 0x7f, 0xf8, 0x3c, 0x10, 0x78, 0x3c, 0x00,
    0xf0, 0x07, 0x80, 0x00, 0x0f, 0xdf, 0x00, 0xf0, 0x00, 0x7f, 0xf0, 0x3c,
    0x00, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0x80, 0x00, 0x0f, 0xcf, 0x80, 0xf0,
    0x00, 0x7f, 0xe0, 0x3c, 0x00, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0x80, 0x00,
    0x0f, 0x87, 0xc0, 0xf0, 0x00, 0x7b, 0xe0, 0x3c, 0x00, 0x78, 0x3c, 0x00,
    0xf0, 0x07, 0x80, 0x00, 0x0f, 0x07, 0xe0, 0xff, 0xfc, 0x79, 0xf0, 0x3c,
    0x00, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xff, 0xe0, 0x0f, 0x03, 0xe0, 0xff,
    0xfc, 0x78, 0xf8, 0x3c, 0x00, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xff, 0xe0,
    0x0f, 0x01, 0xf0, 0xff, 0xfc, 0x78, 0xf8, 0x3c, 0x00, 0x78, 0x3c, 0x00,
    0xf0, 0x07, 0xff, 0xe0, 0x0f, 0x01, 0xf8, 0xff, 0xfc, 0x78, 0x7c, 0x3c,
    0x00, 0x78, 0x3c, 0x00, 0xf0, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

static const uint8_t *mainLogoDataPointer = mainLogoData_kermiteDefault;

static void renderMainLogo() {
  display->clearDisplay();
  display->drawBitmap(0, 0, mainLogoDataPointer, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display->display();
}

//----------------------------------------------------------------------

static void oledDisplay_setCustomLogo(const uint8_t *logoData) {
  mainLogoDataPointer = logoData;
}

static void oledDisplay_setupInstance(int _pinSDA, int _pinSCL) {
  if (_pinSDA == -1 || _pinSCL == -1) {
    moduleActive = false;
    return;
  }
  display = new Adafruit_SSD1306(128, 32, &Wire, -1);
  pinSDA = _pinSDA;
  pinSCL = _pinSCL;
  moduleActive = true;
}

static void oledDisplay_initialize() {
  if (!moduleActive) {
    return;
  }
  Wire.setSDA(pinSDA);
  Wire.setSCL(pinSCL);
  Wire.begin();
  moduleActive = display->begin(SSD1306_SWITCHCAPVCC, 0x3C);
  if (moduleActive) {
    display->setTextColor(SSD1306_WHITE);
  }
}

static void updateFrame(int frameTickMs) {
  bool bootComplete = frameTickMs > 3000;
  bool isMaster = true; //! keyboardMain_exposedState.isSplitSlave;
  if (bootComplete && isMaster) {
    renderStatusView();
  } else {
    renderMainLogo();
  }
}

static void oledDisplay_update() {
  if (!moduleActive) {
    return;
  }
  static uint32_t frameTickMs = 0;
  static uint32_t prevSystemMs = millis();
  uint32_t systemMs = millis();
  uint32_t elapsed = systemMs - prevSystemMs;
  if (elapsed > 40) {
    frameTickMs += elapsed;
    updateFrame(frameTickMs);
    prevSystemMs = systemMs;
  }
}

//----------------------------------------------------------------------

OledDisplay::OledDisplay(int pinSDA, int pinSCL) {
  oledDisplay_setupInstance(pinSDA, pinSCL);
}

void OledDisplay::setCustomLogo(const uint8_t *logoData) {
  oledDisplay_setCustomLogo(logoData);
}

void OledDisplay::initialize() { oledDisplay_initialize(); }

void OledDisplay::update() { oledDisplay_update(); }
