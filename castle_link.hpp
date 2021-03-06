#pragma once
#include <stdint.h>
#include <stddef.h>
#include <array>


namespace CASTLELINK {
  static inline constexpr int16_t	PWM_DISABLE = -1;
}

class castleLinkRawData
{
public:
  castleLinkRawData();
  void  resetIndex(void);
  bool  push(const uint16_t val);
  void dbgTrace(void) const ;
private:
  static constexpr size_t raw_len = 11;
  volatile size_t  index;
  union {
    struct {
      uint16_t	calibration_1ms;
      uint16_t	bat_voltage;
      uint16_t	ripple_voltage;
      uint16_t	current;
      uint16_t	throttle;
      uint16_t	power;
      uint16_t	rpm;
      uint16_t	bec_voltage;
      uint16_t	bec_current;
      uint16_t	temp_linear_or_cal;
      uint16_t	temp_ntc_or_cal;
    };
    std::array <uint16_t, raw_len> raw;
  };
  uint8_t escIdx;

public:
  uint16_t	   get_calibration_1ms (void) const { return calibration_1ms; };
  uint16_t	   get_temp_linear_or_cal (void) const { return temp_linear_or_cal; };
  uint16_t	   get_temp_ntc_or_cal (void) const { return temp_ntc_or_cal; };
  const  std::array <uint16_t, raw_len> &
		   get_raw_ref	    (void) const { return raw; };
  size_t           get_raw_len	    (void) const { return raw_len;};
  void     setEscIdx(const  uint8_t _escIdx)  {escIdx = _escIdx;};
  uint8_t  getEscIdx(void)  const {return escIdx;};
};

typedef enum  : uint16_t {PWM_ORDER=0, CALIBRATE} IncomingMessageId;
typedef enum  : uint32_t {TELEMETRY=0}		  OutcommingMessageId;
typedef struct {
  IncomingMessageId msgId;
  int16_t  escIdx;
  int16_t  duty;
} TelemetryDownMsg;

class castleLinkData
{
  static constexpr std::array<float, 10> scale_coeffs {
      20,
      4,
      22,
      1,
      0.2502,
      20416.7,
      4,
      4,
      30,
      63.8125};

  
public:
  castleLinkData();
  castleLinkData(const castleLinkRawData* _raw);
  void  sendTelemetry(void);
  void populate(const castleLinkRawData* _raw);
  void dbgTrace(void) const ;
  
private:
  void convertValues(void);
  
private:

  struct {
    OutcommingMessageId msgId;
    union {
      struct {
	float	 bat_voltage;
	float	 ripple_voltage;
	float	 current;
	float	 throttle;
	float	 power;
	float	 rpm;
	float	 bec_voltage;
	float	 bec_current;
	float	 temperature;             
      };
      std::array<float, 9> datas;
    };
    uint32_t	escIdx;
  };
  const castleLinkRawData* raw;
};  

  
void castleLinkStart(void);
void castleLinkSetDuty(const uint8_t escIdx, const int16_t dutyPerTenThousand);
