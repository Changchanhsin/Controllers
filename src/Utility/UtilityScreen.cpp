#include "UtilityScreen.h"

void UtilityScreen::writeDisplay(uint8_t b, uint8_t mode){
  Wire.beginTransmission(SSD1306_128x64_ADDRESS);
  Wire.write(mode == SSD1306_MODE_CMD ? 0X00 : 0X40);
  Wire.write(b);
  Wire.endTransmission();
}

void UtilityScreen::writeDisplay(uint8_t *b, uint8_t size, uint8_t mode){
  Wire.beginTransmission(SSD1306_128x64_ADDRESS);
  Wire.write(mode == SSD1306_MODE_CMD ? 0X00 : 0X40);
  for(uint8_t i=0; i<size;i++)
    Wire.write(b[i]);
  Wire.endTransmission();
}

uint8_t UtilityScreen::charWidth(uint8_t c) const {
  if (!m_font) {
    return 0;
  }
  uint8_t first = readFontByte(m_font + FONT_FIRST_CHAR);
  uint8_t count = readFontByte(m_font + FONT_CHAR_COUNT);
  if (c < first || c >= (first + count)) {
    return 0;
  }
  if (fontSize() > 1) {
    // Proportional font.
    return m_magFactor * readFontByte(m_font + FONT_WIDTH_TABLE + c - first);
  }
  // Fixed width font.
  return m_magFactor * readFontByte(m_font + FONT_WIDTH);
}

void UtilityScreen::clear() {
  clear(0, displayWidth() - 1, 0, displayRows() - 1);
}
//------------------------------------------------------------------------------
void UtilityScreen::clear(uint8_t colStart, uint8_t colEnd, uint8_t rowStart, uint8_t rowEnd) {
  // Cancel skip character pixels.
  m_skip = 0;

  // Insure only rows on display will be cleared.
  if (rowEnd >= displayRows()) rowEnd = displayRows() - 1;

  for (uint8_t r = rowStart; r <= rowEnd; r++) {
    setCursor(colStart, r);
    for (uint8_t c = colStart; c <= colEnd; c++) {
      // Insure clear() writes zero. result is (m_invertMask^m_invertMask).
      ssd1306WriteRamBuf(m_invertMask);
    }
  }
  setCursor(colStart, rowStart);
}
//------------------------------------------------------------------------------
void UtilityScreen::clearToEOL() {
  clear(m_col, displayWidth() - 1, m_row, m_row + fontRows() - 1);
}
//------------------------------------------------------------------------------
void UtilityScreen::displayRemap(bool mode) {
  ssd1306WriteCmd(mode ? SSD1306_SEGREMAP : SSD1306_SEGREMAP | 1);
  ssd1306WriteCmd(mode ? SSD1306_COMSCANINC : SSD1306_COMSCANDEC);
}
//------------------------------------------------------------------------------
size_t UtilityScreen::fieldWidth(uint8_t n) {
  return n * (fontWidth() + letterSpacing());
}
//------------------------------------------------------------------------------
uint8_t UtilityScreen::fontCharCount() const {
  return m_font ? readFontByte(m_font + FONT_CHAR_COUNT) : 0;
}
//------------------------------------------------------------------------------
char UtilityScreen::fontFirstChar() const {
  return m_font ? readFontByte(m_font + FONT_FIRST_CHAR) : 0;
}
//------------------------------------------------------------------------------
uint8_t UtilityScreen::fontHeight() const {
  return m_font ? m_magFactor * readFontByte(m_font + FONT_HEIGHT) : 0;
}
//------------------------------------------------------------------------------
uint8_t UtilityScreen::fontRows() const {
  return m_font ? m_magFactor * ((readFontByte(m_font + FONT_HEIGHT) + 7) / 8)
                : 0;
}
//------------------------------------------------------------------------------
uint16_t UtilityScreen::fontSize() const {
  return (readFontByte(m_font) << 8) | readFontByte(m_font + 1);
}
//------------------------------------------------------------------------------
uint8_t UtilityScreen::fontWidth() const {
  return m_font ? m_magFactor * readFontByte(m_font + FONT_WIDTH) : 0;
}
//------------------------------------------------------------------------------
void UtilityScreen::init(const DevType* dev) {
  m_col = 0;
  m_row = 0;
  const uint8_t* table = dev->initcmds;
  uint8_t size = readFontByte(&dev->initSize);
  m_displayWidth = readFontByte(&dev->lcdWidth);
  m_displayHeight = readFontByte(&dev->lcdHeight);
  m_colOffset = readFontByte(&dev->colOffset);
  for (uint8_t i = 0; i < size; i++) {
    ssd1306WriteCmd(readFontByte(table + i));
  }
  clear();
}
//------------------------------------------------------------------------------
void UtilityScreen::invertDisplay(bool invert) {
  ssd1306WriteCmd(invert ? SSD1306_INVERTDISPLAY : SSD1306_NORMALDISPLAY);
}
//------------------------------------------------------------------------------
void UtilityScreen::setCol(uint8_t col) {
  if (col < m_displayWidth) {
    m_col = col;
    col += m_colOffset;
    ssd1306WriteCmd(SSD1306_SETLOWCOLUMN | (col & 0XF));
    ssd1306WriteCmd(SSD1306_SETHIGHCOLUMN | (col >> 4));
  }
}

//------------------------------------------------------------------------------
void UtilityScreen::setCursor(uint8_t col, uint8_t row, int8_t off) {
  setCol(col);
  setRow(row, off);
}
//------------------------------------------------------------------------------
void UtilityScreen::setFont(const uint8_t* font) {
  m_font = font;
  if (font && fontSize() == 1) {
    m_letterSpacing = 0;
  } else {
    m_letterSpacing = 1;
  }
}
//------------------------------------------------------------------------------
void UtilityScreen::setRow(uint8_t row,int8_t offset=0) {
  if (row < displayRows()) {
    m_row = row;
    ssd1306WriteCmd(SSD1306_SETSTARTPAGE | m_row);
  }
}
//------------------------------------------------------------------------------
void UtilityScreen::ssd1306WriteRam(uint8_t c) {
  if (m_col < m_displayWidth) {
    writeDisplay(c ^ m_invertMask, SSD1306_MODE_RAM);
    m_col++;
  }
}
//------------------------------------------------------------------------------
void UtilityScreen::ssd1306WriteRamBuf(uint8_t c) {
  if (m_skip) {
    m_skip--;
  } else if (m_col < m_displayWidth) {
    writeDisplay(c ^ m_invertMask, SSD1306_MODE_RAM_BUF);
    m_col++;
  }
}
//------------------------------------------------------------------------------
PGMDATA(scaledNibble) = {0X00, 0X03, 0X0C, 0X0F, 0X30, 0X33, 0X3C, 0X3F,
                              0XC0, 0XC3, 0XCC, 0XCF, 0XF0, 0XF3, 0XFC, 0XFF};
//------------------------------------------------------------------------------
size_t UtilityScreen::strWidth(const char* str) const {
  size_t sw = 0;
  while (*str) {
    uint8_t cw = charWidth(*str++);
    if (cw == 0) {
      return 0;
    }
    sw += cw + letterSpacing();
  }
  return sw;
}
//------------------------------------------------------------------------------
void UtilityScreen::tickerInit(TickerState* state, const uint8_t* font,
                              uint8_t row, bool mag2X, uint8_t bgnCol,
                              uint8_t endCol) {
  state->font = font;
  state->row = row;
  state->mag2X = mag2X;
  state->bgnCol = bgnCol;
  state->endCol = endCol < m_displayWidth ? endCol : m_displayWidth - 1;
  state->nQueue = 0;
}
//------------------------------------------------------------------------------
bool UtilityScreen::tickerText(TickerState* state, const char* text) {
  if (!text) {
    state->nQueue = 0;
    return true;
  }
  if (state->nQueue >= TICKER_QUEUE_DIM) {
    return false;
  }
  if (state->nQueue == 0) {
    state->init = true;
  }
  state->queue[state->nQueue++] = text;
  return true;
}
//------------------------------------------------------------------------------
int8_t UtilityScreen::tickerTick(TickerState* state) {
  if (!state->font) {
    return -1;
  }
  if (!state->nQueue) {
    return 0;
  }
  setFont(state->font);
  m_magFactor = state->mag2X ? 2 : 1;
  if (state->init) {
    clear(state->bgnCol, state->endCol, state->row,
          state->row + fontRows() - 1);
    state->col = state->endCol;
    state->skip = 0;
    state->init = false;
  }
  // Adjust display width to truncate pixels after endCol.  Find better way?
  uint8_t save = m_displayWidth;
  m_displayWidth = state->endCol + 1;

  // Skip pixels before bgnCol.
  skipColumns(state->skip);
  setCursor(state->col, state->row);
  for (uint8_t i = 0; i < state->nQueue; i++) {
    const char* str = state->queue[i];
    while (*str && m_col <= state->endCol) {
      write(*str++);
    }
    if (m_col > state->endCol) {
      break;
    }
  }
  if (m_col <= state->endCol) {
    clear(m_col, m_col, state->row, state->row + fontRows() - 1);
  }
  // Restore display width.
  m_displayWidth = save;

  if (state->nQueue == 1 && *state->queue[0] == 0) {
    state->nQueue = 0;
    return 0;
  }
  if (state->col > state->bgnCol) {
    state->col--;
  } else {
    state->skip++;
    if (state->skip >= charSpacing(*state->queue[0])) {
      state->skip = 0;
      state->queue[0]++;
      if (*state->queue[0] == 0 && state->nQueue > 1) {
        state->nQueue--;
        for (uint8_t i = 0; i < state->nQueue; i++) {
          state->queue[i] = state->queue[i + 1];
        }
      }
    }
  }
  return state->nQueue;
}
//------------------------------------------------------------------------------
size_t UtilityScreen::write(uint8_t ch) {
  if (!m_font) {
    return 0;
  }
  uint8_t w = readFontByte(m_font + FONT_WIDTH);
  uint8_t h = readFontByte(m_font + FONT_HEIGHT);
  uint8_t nr = (h + 7) / 8;
  uint8_t first = readFontByte(m_font + FONT_FIRST_CHAR);
  uint8_t count = readFontByte(m_font + FONT_CHAR_COUNT);
  const uint8_t* base = m_font + FONT_WIDTH_TABLE;

  if (ch == '\r') {
    setCol(0);
    return 1;
  }
  if (ch == '\n') {
    setCol(0);
    uint8_t fr = m_magFactor * nr;
    setRow(m_row + fr);
    return 1;
  }
  bool nfSpace = false;
  if (first <= ch && ch < (first + count)) {
    ch -= first;
  } else if (ENABLE_NONFONT_SPACE && ch == ' ') {
    nfSpace = true;
  } else {
    // Error if not in font.
    return 0;
  }
  uint8_t s = letterSpacing();
  uint8_t thieleShift = 0;
  if (nfSpace) {
    // non-font space.
  } else if (fontSize() < 2) {
    // Fixed width font.
    base += nr * w * ch;
  } else {
    if (h & 7) {
      thieleShift = 8 - (h & 7);
    }
    uint16_t index = 0;
    for (uint8_t i = 0; i < ch; i++) {
      index += readFontByte(base + i);
    }
    w = readFontByte(base + ch);
    base += nr * index + count;
  }
  uint8_t scol = m_col;
  uint8_t srow = m_row;
  uint8_t skip = m_skip;
  for (uint8_t r = 0; r < nr; r++) {
    for (uint8_t m = 0; m < m_magFactor; m++) {
      skipColumns(skip);
      if (r || m) {
        setCursor(scol, m_row + 1);
      }
      for (uint8_t c = 0; c < w; c++) {
        uint8_t b = nfSpace ? 0 : readFontByte(base + c + r * w);
        if (thieleShift && (r + 1) == nr) {
          b >>= thieleShift;
        }
        if (m_magFactor == 2) {
          b = m ? b >> 4 : b & 0XF;
          b = readFontByte(scaledNibble + b);
          ssd1306WriteRamBuf(b);
        }
        ssd1306WriteRamBuf(b);
      }
      for (uint8_t i = 0; i < s; i++) {
        ssd1306WriteRamBuf(0);
      }
    }
  }
  setRow(srow);
  return 1;
}

void UtilityScreen::bitmap(uint8_t x, uint8_t y,uint8_t w, uint8_t h, uint8_t *bmp){
  for (uint8_t j = 0; j < h; j++) {
    setRow(y+j);
    setCol(x);
    for (uint8_t i = 0; i < w; i++) {
      ssd1306WriteRamBuf(readFontByte(bmp+i*h+j));
    }
  }
}
