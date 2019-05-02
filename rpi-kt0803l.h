#ifndef kt0803l_h
#define kt0803l_h

// 0x7C and 0x7D are write/read, but WiringPi implicitly assumes that the least
// significant bit is used to distinguish between reading and writing, so it
// takes the 7 most significant bits as the address instead. 0x7C >> 1 = 0x3E.
#define KT0803L_ADDRESS 0x3E

// == BEGIN REGISTERS ==
#define REG_CHSEL_8_1 0x00
#define BIT_START_CHSEL_8_1 0x00
#define BIT_END_CHSEL_8_1 0x07

#define REG_RFGAIN_1_0 0x01
#define BIT_START_RFGAIN_1_0 0x06
#define BIT_END_RFGAIN_1_0 0x07

#define REG_PGA_2_0 0x01
#define BIT_START_PGA_2_0 0x03
#define BIT_END_PGA_2_0 0x05

#define REG_CHSEL_11_9 0x01
#define BIT_START_CHSEL_11_9 0x00
#define BIT_END_CHSEL_11_9 0x02

#define REG_CHSEL_0 0x02
#define BIT_START_CHSEL_0 0x07
#define BIT_END_CHSEL_0 0x07

#define REG_RFGAIN_3 0x02
#define BIT_START_RFGAIN_3 0x06
#define BIT_END_RFGAIN_3 0x06

#define REG_MUTE 0x02
#define BIT_START_MUTE 0x03
#define BIT_END_MUTE 0x03

#define REG_PLTADJ 0x02
#define BIT_START_PLTADJ 0x02
#define BIT_END_PLTADJ 0x02

#define REG_PHTCNST 0x02
#define BIT_START_PHTCNST 0x00
#define BIT_END_PHTCNST 0x00

#define REG_ALC_EN 0x04
#define BIT_START_ALC_EN 0x07
#define BIT_END_ALC_EN 0x07

#define REG_MONO 0x04
#define BIT_START_MONO 0x06
#define BIT_END_MONO 0x06

#define REG_PGA_LSB_1_0 0x04
#define BIT_START_PGA_LSB_1_0 0x04
#define BIT_END_PGA_LSB_1_0 0x05

#define REG_BASS_1_0 0x04
#define BIT_START_BASS_1_0 0x00
#define BIT_END_BASS_1_0 0x01

#define REG_STANDBY 0x0B
#define BIT_START_STANDBY 0x07
#define BIT_END_STANDBY 0x07

#define REG_PDPA 0x0B
#define BIT_START_PDPA 0x05
#define BIT_END_PDPA 0x05

#define REG_AUTO_PADN 0x0B
#define BIT_START_AUTO_PADN 0x02
#define BIT_END_AUTO_PADN 0x02

#define REG_ALC_DECAY_TIME_3_0 0x0C
#define BIT_START_DECAY_TIME_3_0 0x04
#define BIT_END_DECAY_TIME_3_0 0x07

#define REG_ALC_ATTACK_TIME_3_0 0x0C
#define BIT_START_ATTACK_TIME_3_0 0x00
#define BIT_END_ATTACK_TIME_3_0 0x03

#define REG_PA_BIAS 0x0E
#define BIT_START_PA_BIAS 0x01
#define BIT_END_PA_BIAS 0x01

#define REG_PW_OK 0x0F
#define BIT_START_PW_OK 0x04
#define BIT_END_PW_OK 0x04

#define REG_SLNCID 0x0F
#define BIT_START_SLNCID 0x02
#define BIT_END_SLNCID 0x02

#define REG_PGAMOD 0x10
#define BIT_START_PGAMOD 0x10
#define BIT_END_PGAMOD 0x10

#define REG_SLNCDIS 0x12
#define BIT_START_SLNCDIS 0x07
#define BIT_END_SLNCDIS 0x07

#define REG_SLNCTHL_2_0 0x12
#define BIT_START_SLNCTHL_2_0 0x04
#define BIT_END_SLNCTHL_2_0 0x06

#define REG_SLNCTHH_2_0 0x12
#define BIT_START_SLNCTHH_2_0 0x01
#define BIT_END_SLNCTHH_2_0 0x03

#define REG_SWMOD 0x12
#define BIT_START_SW_MOD 0x00
#define BIT_END_SW_MOD 0x00

#define REG_RFGAIN_2 0x13
#define BIT_START_RFGAIN_2 0x07
#define BIT_END_RFGAIN_2 0x07

#define REG_PA_CTRL 0x13
#define BIT_START_PA_CTLR 0x02
#define BIT_END_PA_CTLR 0x02

#define REG_SLNCTIME_2_0 0x14
#define BIT_START_SLNCTIME_2_0 0x05
#define BIT_END_SLNCTIME_2_0 0x07

#define REG_SLNCCNTHIGH_2_0 0x14
#define BIT_START_SLNCCNTHIGH_2_0 0x02
#define BIT_END_SLNCCNTHIGH_2_0 0x04

#define REG_SLNCTIME_3 0x14
#define BIT_START_SLNCTIME_3 0x00
#define BIT_END_SLNCTIME_3 0x00

#define REG_ALCMPGAIN_2_0 0x15
#define BIT_START_ALCMPGAIN_2_0 0x05
#define BIT_END_ALCMPGAIN_2_0 0x07

#define REG_SLNCCNTLOW_2_0 0x16
#define BIT_START_SLNCCNTLOW_2_0 0x00
#define BIT_END_SLNCCNTLOW_2_0 0x02

#define REG_FDEV 0x17
#define BIT_START_FDEV 0x06
#define BIT_END_FDEV 0x06

#define REG_AU_ENHANCE 0x17
#define BIT_START_AU_ENHANCE 0x05
#define BIT_END_AU_ENHANCE 0x05

#define REG_XTAL_SEL 0x17
#define BIT_START_XTAL_SEL 0x03
#define BIT_END_XTAL_SEL 0x03

#define REG_DCLK 0x1E
#define BIT_START_DCLK 0x06
#define BIT_END_DCLK 0x06

#define REG_XTALD 0x1E
#define BIT_START_XTALD 0x05
#define BIT_END_XTALD 0x05

#define REG_REF_CLK_3_0 0x1E
#define BIT_START_REF_CLK_3_0 0x00
#define BIT_END_REF_CLK_3_0 0x03

#define REG_ALCHOLD_2_0 0x26
#define BIT_START_ALCHOLD_2_0 0x05
#define BIT_END_ALCHOLD_2_0 0x07

#define REG_ALCHIGHTH_2_0 0x26
#define BIT_START_ALCHIGHTH_2_0 0x01
#define BIT_END_ALCHIGHTH_2_0 0x03

#define REG_ALCLOWTH_3_0 0x27
#define BIT_START_ALCLOWTH_3_0 0x00
#define BIT_END_ALCLOWTH_3_0 0x03
// == END REGISTERS ==

uint8_t getBit(uint8_t bit, uint8_t word);

uint8_t setBit(uint8_t bit, uint8_t value, uint8_t word);

uint8_t getBitField(uint8_t bitStart, uint8_t bitEnd, uint8_t word);

uint8_t setBitField(uint8_t bitStart, uint8_t bitEnd, uint8_t value, uint8_t word);

void ktInit();

uint8_t ktReadRegister(uint8_t reg);

void ktWriteRegister(uint8_t reg, uint8_t word);

uint8_t ktReadRegisterField(uint8_t bitStart, uint8_t bitEnd, uint8_t reg);

void ktWriteRegisterField(uint8_t bitStart, uint8_t bitEnd, uint8_t reg, uint8_t value);

uint32_t ktGetChannel();

void ktSetChannel(uint32_t channel);

uint8_t ktGetRfGain();

void ktSetRfGain(uint8_t gain);

uint8_t ktGetStandby();

void ktSetStandby(uint8_t standby);

uint8_t ktGetMute();

void ktSetMute(uint8_t mute);

uint8_t ktGetMono();

void ktSetMono(uint8_t mono);

uint8_t ktGetAlc();

void ktSetAlc(uint8_t alc);

uint8_t ktGetPowerStatus();

uint8_t ktGetSilenceDetected();

int8_t ktGetPGA();

void ktSetPGA(int8_t pga);

uint8_t ktGetPilotToneAmplitudeAdjustment();

void ktSetPilotToneAmplitudeAdjustment(uint8_t pltadj);

uint8_t ktGetPreEmphasisTimeConstant();

void ktSetPreEmphasisTimeConstant(uint8_t phtcnst);

uint8_t ktGetPreAutoPowerAmpDown();

void ktSetPreAutoPowerAmpDown(uint8_t atpadn);

uint8_t ktGetPowerAmpPower();

void ktSetPowerAmpPower(uint8_t pdpa);

uint8_t ktGetBass();

void ktSetBass(uint8_t bass);

uint8_t ktGetAlcDecayTime();

void ktSetAlcDecayTime(uint8_t alcd);

uint8_t ktGetAlcAttackTime();

void ktSetAlcAttackTime(uint8_t alca);

uint8_t ktGetAlcCompressedGain();

void ktSetAlcCompressedGain(uint8_t alcg);

uint8_t ktGetAlcHoldTime();

void ktSetAlcHoldTime(uint8_t alchold);

uint8_t ktGetAlcHighThreshold();

void ktSetAlcHighThreshold(uint8_t alcthh);

uint8_t ktGetAlcLowThreshold();

void ktSetAlcLowThreshold(uint8_t alcthl);

uint8_t ktGetSilenceDetect();

void ktSetSilenceDetect(uint8_t slndt);

uint8_t ktGetSilenceDetectionLowThreshold();

void ktSetSilenceDetectionLowThreshold(uint8_t slnthl);

uint8_t ktGetSilenceDetectionHighThreshold();

void ktSetSilenceDetectionHighThreshold(uint8_t slnthh);

uint8_t ktGetSilenceDetectionDurationTime();

void ktSetSilenceDetectionDurationTime(uint8_t gain);

uint8_t ktGetSilenceDetectionCounterThreshold();

void ktSetSilenceDetectionCounterThreshold(uint8_t slnthc);

uint8_t ktGetSilenceDetectionLowCounter();

void ktSetSilenceDetectionLowCounter(uint8_t slncntl);

uint8_t ktGetSwitchingChannelMode();

void ktSetSwitchingChannelMode(uint8_t swmod);

uint8_t ktGetAudioFrequencyResponseEnhancement();

void ktSetAudioFrequencyResponseEnhancement(uint8_t auenhance);

uint8_t ktGetFrequencyDeviation();

void ktSetFrequencyDeviation(uint8_t deviation);

uint8_t ktGetPaBias();

void ktSetPaBias(uint8_t bias);

/** 
 * Parse a boolean from a NULL-terminated string, storing the result in *out.
 * Returns EINVAL and prints an error message to stderr in the event of a parse
 * error, leaving *out unchaged.
 */
error_t ktParseBool(char *opt, char *arg, int *out);

/** 
 * Parse an integer from a NULL-terminated string, storing the result in *out.
 * Returns EINVAL and prints an error message to stderr in the event of a parse
 * error, leaving *out unchaged.
 */
error_t ktParseInt(char *opt, char *arg, long int min, long int max, long int *out);

error_t ktHandleArgQuery();

error_t ktHandleArgStandby(char *arg);

error_t ktHandleArgMute(char *arg);

error_t ktHandleArgMono(char *arg);

error_t ktHandleArgAlc(char *arg);

error_t ktHandleArgRfGain(char *arg);

error_t ktHandleArgSoftwareGain(char *arg);

error_t ktHandleArgPaBias(char *arg);

error_t ktHandleArgPilotToneAmplitudeAdjustment(char *arg);

error_t ktHandleArgPreEmphasisTimeConstant(char *arg);

error_t ktHandleArgAutoPowerAmpDown(char *arg);

error_t ktHandleArgPowerAmpPower(char *arg);

error_t ktHandleArgFrequencyDeviation(char *arg);

error_t ktHandleArgBass(char *arg);

error_t ktHandleArgAlcDecayTime(char *arg);

error_t ktHandleArgAlcAttackTime(char *arg);

error_t ktHandleArgAlcCompressedGain(char *arg);

error_t ktHandleArgAlcHoldTime(char *arg);

error_t ktHandleArgAlcHighThreshold(char *arg);

error_t ktHandleArgAlcLowThreshold(char *arg);

error_t ktHandleArgSilenceDetect(char *arg);

error_t ktHandleArgSilenceDetectionLowThreshold(char *arg);

error_t ktHandleArgSilenceDetectionHighThreshold(char *arg);

error_t ktHandleArgSilenceDetectionDurationTime(char *arg);

error_t ktHandleArgSilenceDetectionCounterThreshold(char *arg);

error_t ktHandleArgSilenceDetectionLowCounter(char *arg);

error_t ktHandleArgAudioFrequencyResponseEnhancement(char *arg);

error_t ktHandleArgSwitchingChannelMode(char *arg);

error_t ktHandleArgChannel(char *arg);

error_t ktHandleArg(int key, char *arg, struct argp_state *state);

#endif
