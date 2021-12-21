// Precise AVR pulse generation
// Copyright (C) 2021  Joshua Booth

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.

// You should have received a copy of the GNU Lesser Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef TIMER_EXT_PULSE_H_
#define TIMER_EXT_PULSE_H_

#include "extTimer.h"
#include "timerTypes.h"

const int minPulseChangeCycles = 256;

class PulseGen
{
public:
  typedef void (*stateChangeCallback_t)(PulseGen *pulse, void *data);

  PulseGen(volatile uint8_t *_pinReg, volatile uint8_t *_ddr,
    volatile uint16_t *_ocr, volatile uint8_t *_tccra, volatile uint8_t *_tccrc,
    uint8_t _pinBit, uint8_t _ddBit, uint8_t _com1, uint8_t _com0, uint8_t _foc,
    uint16_t prescalerDivisor, ExtTimer *_tcnt, stateChangeCallback_t _cb = nullptr, void *_cbData = nullptr);
  
  bool setStart(ticksExtraRange_t start);
  bool setEnd(ticksExtraRange_t end);

  const ticksExtraRange_t getStart();
  const ticksExtraRange_t getEnd();

  const uint8_t getPinState(); // HIGH or LOW

  bool cancel();

  void processCompareEvent();

  enum class PulseState : uint8_t {Idle, WaitingToScheduleHigh, ScheduledHigh, WaitingToScheduleLow, ScheduledLow};

  const PulseState getState();

  void setStateChangeCallback(stateChangeCallback_t _cb, const void *_cbData = nullptr);

private:
  // TODO: templatize these
  volatile uint8_t *pinReg; // Port Input Register
  volatile uint8_t *ddr; // Data Direction Register
  volatile uint16_t *ocr; // Output Compare Register
  volatile uint8_t *tccra;
  volatile uint8_t *tccrc;
  uint8_t pinBit; // Port Input Register bit
  uint8_t ddBit; // Data Direction Register bit
  uint8_t com1;
  uint8_t com0;
  uint8_t foc;
  ExtTimer *tcnt;

  // TODO: prefix with underscore
  ticks16_t minChangeTicks; // Min number of ticks to update a timer value or cancel the pulse
  volatile PulseState pulseState = PulseState::Idle;
  stateChangeCallback_t cb = nullptr;
  const void *cbData = nullptr;

  // TODO: prefix with underscore
  volatile ticksExtraRange_t start;
  volatile ticksExtraRange_t end;

  void scheduleHighState();
  void scheduleLowState();

  const bool ticksInScheduleRange(ticksExtraRange_t ticks);
  const bool hasTimeToUpdate(ticksExtraRange_t ticks);

  void updateState();
};

#ifdef HAVE_TCNT0

extern PulseGen PulseGen0A;
extern PulseGen PulseGen0B;

#endif // HAVE_TCNT0

#ifdef HAVE_TCNT1

extern PulseGen PulseGen1A;
extern PulseGen PulseGen1B;
#ifdef OCR1C
extern PulseGen PulseGen1C;
#endif // OCR1C

#endif // HAVE_TCNT1

#ifdef HAVE_TCNT2

extern PulseGen PulseGen2A;
extern PulseGen PulseGen2B;

#endif // HAVE_TCNT2

#ifdef HAVE_TCNT3

extern PulseGen PulseGen3A;
extern PulseGen PulseGen3B;
extern PulseGen PulseGen3C;

#endif // HAVE_TCNT3

#ifdef HAVE_TCNT4

extern PulseGen PulseGen4A;
extern PulseGen PulseGen4B;
extern PulseGen PulseGen4C;

#endif // HAVE_TCNT4

#ifdef HAVE_TCNT5

extern PulseGen PulseGen5A;
extern PulseGen PulseGen5B;
extern PulseGen PulseGen5C;

#endif // HAVE_TCNT5



#ifdef ARDUINO_AVR_UNO

#define PulseGenPin6 PulseGen0A
#define PulseGenPin5 PulseGen0B

#define PulseGenPin9 PulseGen1A
#define PulseGenPin10 PulseGen1B

#define PulseGenPin11 PulseGen2A
#define PulseGenPin3 PulseGen2B

#endif // ARDUINO_AVR_UNO



#ifdef ARDUINO_AVR_MEGA2560

#define PulseGenPin13Timer0 PulseGen0A
#define PulseGenPin4 PulseGen0B

#define PulseGenPin11 PulseGen1A
#define PulseGenPin12 PulseGen1B
#define PulseGenPin13Timer1 PulseGen1C

#define PulseGenPin10 PulseGen2A
#define PulseGenPin9 PulseGen2B

#define PulseGenPin5 PulseGen3A
#define PulseGenPin2 PulseGen3B
#define PulseGenPin3 PulseGen3C

#define PulseGenPin6 PulseGen4A
#define PulseGenPin7 PulseGen4B
#define PulseGenPin8 PulseGen4C

#define PulseGenPin46 PulseGen5A
#define PulseGenPin45 PulseGen5B
#define PulseGenPin44 PulseGen5C

#endif // ARDUINO_AVR_MEGA2560



#endif // TIMER_EXT_PULSE_H_
