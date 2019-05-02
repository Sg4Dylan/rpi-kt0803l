#include <argp.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPiI2C.h>
#include "rpi-kt0803l.h"

int ktWiringPiFd;

uint8_t getBit(uint8_t bit, uint8_t word) {
    return (word & (1 << bit)) >> bit;
}

uint8_t setBit(uint8_t bit, uint8_t value, uint8_t word) {
    return (word ^ (getBit(bit, word) << bit)) | ((value == 0 ? 0 : 1) << bit);
}

uint8_t getBitField(uint8_t bitStart, uint8_t bitEnd, uint8_t word) {
    uint8_t out = 0;
    int inBit = bitEnd;
    while (inBit >= bitStart) {
        out = (out << 1) | getBit(inBit, word);
        inBit--;
    }
    return out;
}

uint8_t setBitField(uint8_t bitStart, uint8_t bitEnd, uint8_t word, uint8_t value) {
    uint8_t out = word;
    int inBit = 0;
    int outBit = bitStart;
    while (outBit <= bitEnd) {
        out = setBit(outBit, getBit(inBit, value), out);
        inBit++;
        outBit++;
    }
    return out;
}

void ktInit() {
    ktWiringPiFd = wiringPiI2CSetup(KT0803L_ADDRESS);
    if (ktWiringPiFd == -1 || ktWiringPiFd == -1) {
        fprintf(stderr, "Error initializing WiringPi\n");
        exit(1);
    }
}

uint8_t ktReadRegister(uint8_t reg) {
    return (uint8_t) wiringPiI2CReadReg8(ktWiringPiFd, reg);
}

void ktWriteRegister(uint8_t reg, uint8_t word) {
    wiringPiI2CWriteReg8(ktWiringPiFd, reg, (int) word);
}


uint8_t ktReadRegisterField(uint8_t bitStart, uint8_t bitEnd, uint8_t reg) {
    return getBitField(bitStart, bitEnd, ktReadRegister(reg));
}

void ktWriteRegisterField(uint8_t bitStart, uint8_t bitEnd, uint8_t reg, uint8_t value) {
    uint8_t in = ktReadRegister(reg);
    uint8_t ot = setBitField(bitStart, bitEnd, in, value);
    if (in != ot) {
        ktWriteRegister(reg, ot);
    }
}

uint32_t ktGetChannel() {
    uint32_t b0 = ktReadRegisterField(BIT_START_CHSEL_0, BIT_END_CHSEL_0, REG_CHSEL_0);
    uint32_t b8_1 = ktReadRegisterField(BIT_START_CHSEL_8_1, BIT_END_CHSEL_8_1, REG_CHSEL_8_1);
    uint32_t b11_9 = ktReadRegisterField(BIT_START_CHSEL_11_9, BIT_END_CHSEL_11_9, REG_CHSEL_11_9);
    // Multiply by 50 because the kt0803k takes the channel as a 12 bit value, in
    // increments of 50 KHz.
    return (b0 | (b8_1 << 1) | (b11_9 << 9)) * 50;
}

void ktSetChannel(uint32_t channel) {
    // Divide by 50 because the kt0803k takes the channel as a 12 bit value, in
    // increments of 50 KHz.
    uint32_t ktChannel = channel / 50;
    uint32_t b0 = ktChannel & 0x1;
    uint32_t b8_1 = (ktChannel >> 1) & 0xFF;
    uint32_t b11_9 = (ktChannel >> 9) & 0x07;
    ktWriteRegisterField(BIT_START_CHSEL_0, BIT_END_CHSEL_0, REG_CHSEL_0, (uint8_t) b0);
    ktWriteRegisterField(BIT_START_CHSEL_8_1, BIT_END_CHSEL_8_1, REG_CHSEL_8_1, (uint8_t) b8_1);
    ktWriteRegisterField(BIT_START_CHSEL_11_9, BIT_END_CHSEL_11_9, REG_CHSEL_11_9, (uint8_t) b11_9);
}

uint8_t ktGetRfGain() {
    uint8_t b1_0 = ktReadRegisterField(BIT_START_RFGAIN_1_0, BIT_END_RFGAIN_1_0, REG_RFGAIN_1_0);
    uint8_t b2 = ktReadRegisterField(BIT_START_RFGAIN_2, BIT_END_RFGAIN_2, REG_RFGAIN_2);
    uint8_t b3 = ktReadRegisterField(BIT_START_RFGAIN_3, BIT_END_RFGAIN_3, REG_RFGAIN_3);
    return b1_0 | (b2 << 2) | (b3 << 3);
}

void ktSetRfGain(uint8_t gain) {
    uint8_t b1_0 = gain & 0x3;
    uint8_t b2 = (gain >> 2) & 0x1;
    uint8_t b3 = (gain >> 3) & 0x1;
    ktWriteRegisterField(BIT_START_RFGAIN_1_0, BIT_END_RFGAIN_1_0, REG_RFGAIN_1_0, b1_0);
    ktWriteRegisterField(BIT_START_RFGAIN_2, BIT_END_RFGAIN_2, REG_RFGAIN_2, b2);
    ktWriteRegisterField(BIT_START_RFGAIN_3, BIT_END_RFGAIN_3, REG_RFGAIN_3, b3);
}

uint8_t ktGetStandby() {
    return ktReadRegisterField(BIT_START_STANDBY, BIT_END_STANDBY, REG_STANDBY);
}

void ktSetStandby(uint8_t standby) {
    ktWriteRegisterField(BIT_START_STANDBY, BIT_END_STANDBY, REG_STANDBY, standby);
}

uint8_t ktGetMute() {
    return ktReadRegisterField(BIT_START_MUTE, BIT_END_MUTE, REG_MUTE);
}

void ktSetMute(uint8_t mute) {
    ktWriteRegisterField(BIT_START_MUTE, BIT_END_MUTE, REG_MUTE, mute);
}

uint8_t ktGetMono() {
    return ktReadRegisterField(BIT_START_MONO, BIT_END_MONO, REG_MONO);
}

void ktSetMono(uint8_t mono) {
    ktWriteRegisterField(BIT_START_MONO, BIT_END_MONO, REG_MONO, mono);
}

uint8_t ktGetAlc() {
    return ktReadRegisterField(BIT_START_ALC_EN, BIT_END_ALC_EN, REG_ALC_EN);
}

void ktSetAlc(uint8_t alc) {
    ktWriteRegisterField(BIT_START_ALC_EN, BIT_END_ALC_EN, REG_ALC_EN, alc);
}

uint8_t ktGetPowerStatus() {
    return ktReadRegisterField(BIT_START_PW_OK, BIT_END_PW_OK, REG_PW_OK);
}

uint8_t ktGetSilenceDetected() {
    return ktReadRegisterField(BIT_START_SLNCID, BIT_END_SLNCID, REG_SLNCID);
}

int8_t ktGetPGA() {
    uint8_t hi = ktReadRegisterField(BIT_START_PGA_2_0, BIT_END_PGA_2_0, REG_PGA_2_0);
    uint8_t lo = ktReadRegisterField(BIT_START_PGA_LSB_1_0, BIT_END_PGA_LSB_1_0, REG_PGA_LSB_1_0);
    uint8_t x = (hi << 2) | lo;

    // pga is a signed 5 bit number, but if it's positive, we need to subtract
    // 3 and max with 0. Unlike most signed numbers, a set sign bit indicates a
    // positive value, rather than a negative one.
    if (getBit(4, x)) {
        int8_t out = ((int8_t) (x & 0xF)) - 3;
        if (out < 0) {
            out = 0;
        }
        return out;
    } else {
        return  ((int8_t) (x & 0xF)) * -1;
    }
}

void ktSetPGA(int8_t pga) {
    uint8_t x;

    // pga is a signed 5 bit number, but if it's positive, we need to add
    // 3. Unlike most signed numbers, a set sign bit indicates a positive
    // value, rather than a negative one.
    if (pga <= 0) {
        x = (uint8_t) ((pga * -1) & 0xF);
    } else {
        x = (uint8_t) (((pga + 3) & 0xF) | 0x10);
    }
    uint8_t hi = (x >> 2) & 0x7;
    uint8_t lo = x & 0x3;
    ktWriteRegisterField(BIT_START_PGA_2_0, BIT_END_PGA_2_0, REG_PGA_2_0, hi);
    ktWriteRegisterField(BIT_START_PGA_LSB_1_0, BIT_END_PGA_LSB_1_0, REG_PGA_LSB_1_0, lo);
}

uint8_t ktGetPilotToneAmplitudeAdjustment() {
    return ktReadRegisterField(BIT_START_PLTADJ, BIT_END_PLTADJ, REG_PLTADJ);
}

void ktSetPilotToneAmplitudeAdjustment(uint8_t pltadj) {
    ktWriteRegisterField(BIT_START_PLTADJ, BIT_END_PLTADJ, REG_PLTADJ, pltadj);
}

uint8_t ktGetPreEmphasisTimeConstant() {
    return ktReadRegisterField(BIT_START_PHTCNST, BIT_END_PHTCNST, REG_PHTCNST);
}

void ktSetPreEmphasisTimeConstant(uint8_t phtcnst) {
    ktWriteRegisterField(BIT_START_PHTCNST, BIT_END_PHTCNST, REG_PHTCNST, phtcnst);
}

uint8_t ktGetPreAutoPowerAmpDown() {
    return ktReadRegisterField(BIT_START_AUTO_PADN, BIT_END_AUTO_PADN, REG_AUTO_PADN);
}

void ktSetPreAutoPowerAmpDown(uint8_t atpadn) {
    ktWriteRegisterField(BIT_START_AUTO_PADN, BIT_END_AUTO_PADN, REG_AUTO_PADN, atpadn);
}

uint8_t ktGetPowerAmpPower() {
    return ktReadRegisterField(BIT_START_PDPA, BIT_END_PDPA, REG_PDPA);
}

void ktSetPowerAmpPower(uint8_t pdpa) {
    ktWriteRegisterField(BIT_START_PDPA, BIT_END_PDPA, REG_PDPA, pdpa);
}

uint8_t ktGetBass() {
    return ktReadRegisterField(BIT_START_BASS_1_0, BIT_END_BASS_1_0, REG_BASS_1_0);
}

void ktSetBass(uint8_t bass) {
    ktWriteRegisterField(BIT_START_BASS_1_0, BIT_END_BASS_1_0, REG_BASS_1_0, bass);
}

uint8_t ktGetAlcDecayTime() {
    return ktReadRegisterField(BIT_START_DECAY_TIME_3_0, BIT_END_DECAY_TIME_3_0, REG_ALC_DECAY_TIME_3_0);
}

void ktSetAlcDecayTime(uint8_t alcd) {
    ktWriteRegisterField(BIT_START_DECAY_TIME_3_0, BIT_END_DECAY_TIME_3_0, REG_ALC_DECAY_TIME_3_0, alcd);
}

uint8_t ktGetAlcAttackTime() {
    return ktReadRegisterField(BIT_START_ATTACK_TIME_3_0, BIT_END_ATTACK_TIME_3_0, REG_ALC_ATTACK_TIME_3_0);
}

void ktSetAlcAttackTime(uint8_t alca) {
    ktWriteRegisterField(BIT_START_ATTACK_TIME_3_0, BIT_END_ATTACK_TIME_3_0, REG_ALC_ATTACK_TIME_3_0, alca);
}

uint8_t ktGetAlcCompressedGain() {
    return ktReadRegisterField(BIT_START_ALCMPGAIN_2_0, BIT_END_ALCMPGAIN_2_0, REG_ALCMPGAIN_2_0);
}

void ktSetAlcCompressedGain(uint8_t alcg) {
    ktWriteRegisterField(BIT_START_ALCMPGAIN_2_0, BIT_END_ALCMPGAIN_2_0, REG_ALCMPGAIN_2_0, alcg);
}

uint8_t ktGetAlcHoldTime() {
    return ktReadRegisterField(BIT_START_ALCHOLD_2_0, BIT_END_ALCHOLD_2_0, REG_ALCHOLD_2_0);
}

void ktSetAlcHoldTime(uint8_t alchold) {
    ktWriteRegisterField(BIT_START_ALCHOLD_2_0, BIT_END_ALCHOLD_2_0, REG_ALCHOLD_2_0, alchold);
}

uint8_t ktGetAlcHighThreshold() {
    return ktReadRegisterField(BIT_START_ALCHIGHTH_2_0, BIT_END_ALCHIGHTH_2_0, REG_ALCHIGHTH_2_0);
}

void ktSetAlcHighThreshold(uint8_t alcthh) {
    ktWriteRegisterField(BIT_START_ALCHIGHTH_2_0, BIT_END_ALCHIGHTH_2_0, REG_ALCHIGHTH_2_0, alcthh);
}

uint8_t ktGetAlcLowThreshold() {
    return ktReadRegisterField(BIT_START_ALCLOWTH_3_0, BIT_END_ALCLOWTH_3_0, REG_ALCLOWTH_3_0);
}

void ktSetAlcLowThreshold(uint8_t alcthl) {
    ktWriteRegisterField(BIT_START_ALCLOWTH_3_0, BIT_END_ALCLOWTH_3_0, REG_ALCLOWTH_3_0, alcthl);
}

uint8_t ktGetSilenceDetect() {
    return ktReadRegisterField(BIT_START_SLNCDIS, BIT_END_SLNCDIS, REG_SLNCDIS);
}

void ktSetSilenceDetect(uint8_t slndt) {
    ktWriteRegisterField(BIT_START_SLNCDIS, BIT_END_SLNCDIS, REG_SLNCDIS, slndt);
}

uint8_t ktGetSilenceDetectionLowThreshold() {
    return ktReadRegisterField(BIT_START_SLNCTHL_2_0, BIT_END_SLNCTHL_2_0, REG_SLNCTHL_2_0);
}

void ktSetSilenceDetectionLowThreshold(uint8_t slnthl) {
    ktWriteRegisterField(BIT_START_SLNCTHL_2_0, BIT_END_SLNCTHL_2_0, REG_SLNCTHL_2_0, slnthl);
}

uint8_t ktGetSilenceDetectionHighThreshold() {
    return ktReadRegisterField(BIT_START_SLNCTHH_2_0, BIT_END_SLNCTHH_2_0, REG_SLNCTHH_2_0);
}

void ktSetSilenceDetectionHighThreshold(uint8_t slnthh) {
    ktWriteRegisterField(BIT_START_SLNCTHH_2_0, BIT_END_SLNCTHH_2_0, REG_SLNCTHH_2_0, slnthh);
}

uint8_t ktGetSilenceDetectionDurationTime() {
    uint8_t b2_0 = ktReadRegisterField(BIT_START_SLNCTIME_2_0, BIT_END_SLNCTIME_2_0, REG_SLNCTIME_2_0);
    uint8_t b3 = ktReadRegisterField(BIT_START_SLNCTIME_3, BIT_END_SLNCTIME_3, REG_SLNCTIME_3);
    return b2_0 | (b3 << 3);
}

void ktSetSilenceDetectionDurationTime(uint8_t gain) {
    uint8_t b2_0 = gain & 0x3;
    uint8_t b3 = (gain >> 3) & 0x1;
    ktWriteRegisterField(BIT_START_SLNCTIME_2_0, BIT_END_SLNCTIME_2_0, REG_SLNCTIME_2_0, b2_0);
    ktWriteRegisterField(BIT_START_SLNCTIME_3, BIT_END_SLNCTIME_3, REG_SLNCTIME_3, b3);
}

uint8_t ktGetSilenceDetectionCounterThreshold() {
    return ktReadRegisterField(BIT_START_SLNCCNTHIGH_2_0, BIT_END_SLNCCNTHIGH_2_0, REG_SLNCCNTHIGH_2_0);
}

void ktSetSilenceDetectionCounterThreshold(uint8_t slnthc) {
    ktWriteRegisterField(BIT_START_SLNCCNTHIGH_2_0, BIT_END_SLNCCNTHIGH_2_0, REG_SLNCCNTHIGH_2_0, slnthc);
}

uint8_t ktGetSilenceDetectionLowCounter() {
    return ktReadRegisterField(BIT_START_SLNCCNTLOW_2_0, BIT_END_SLNCCNTLOW_2_0, REG_SLNCCNTLOW_2_0);
}

void ktSetSilenceDetectionLowCounter(uint8_t slncntl) {
    ktWriteRegisterField(BIT_START_SLNCCNTLOW_2_0, BIT_END_SLNCCNTLOW_2_0, REG_SLNCCNTLOW_2_0, slncntl);
}

uint8_t ktGetSwitchingChannelMode() {
    return ktReadRegisterField(BIT_START_SW_MOD, BIT_END_SW_MOD, REG_SWMOD);
}

void ktSetSwitchingChannelMode(uint8_t swmod) {
    ktWriteRegisterField(BIT_START_SW_MOD, BIT_END_SW_MOD, REG_SWMOD, swmod);
}

uint8_t ktGetAudioFrequencyResponseEnhancement() {
    return ktReadRegisterField(BIT_START_AU_ENHANCE, BIT_END_AU_ENHANCE, REG_AU_ENHANCE);
}

void ktSetAudioFrequencyResponseEnhancement(uint8_t auenhance) {
    ktWriteRegisterField(BIT_START_AU_ENHANCE, BIT_END_AU_ENHANCE, REG_AU_ENHANCE, auenhance);
}

uint8_t ktGetFrequencyDeviation() {
    return ktReadRegisterField(BIT_START_FDEV, BIT_END_FDEV, REG_FDEV);
}

void ktSetFrequencyDeviation(uint8_t deviation) {
    ktWriteRegisterField(BIT_START_FDEV, BIT_END_FDEV, REG_FDEV, deviation);
}

uint8_t ktGetPaBias() {
    return ktReadRegisterField(BIT_START_PA_BIAS, BIT_END_PA_BIAS, REG_PA_BIAS);
}

void ktSetPaBias(uint8_t bias) {
    ktWriteRegisterField(BIT_START_PA_BIAS, BIT_END_PA_BIAS, REG_PA_BIAS, bias);
}

error_t ktParseBool(char *opt, char *arg, int *out) {
    if (strncasecmp("true", arg, 4) == 0) {
        *out = 1;
        return 0;
    } else if (strncasecmp("false", arg, 5) == 0) {
        *out = 0;
        return 0;
    } else {
        fprintf(stderr, "Error parsing boolean for option '%s'. Expected 'true' or 'false', got '%s'\n", opt, arg);
        return EINVAL;
    }
}

error_t ktParseInt(char *opt, char *arg, long int min, long int max, long int *out) {
    char *parseEnd = NULL;
    long int x = strtol(arg, &parseEnd, 10);
    if (*arg != '\0' && parseEnd != NULL && *parseEnd == '\0' && x >= min && x <= max) {
        *out = x;
        return 0;
    } else {
        fprintf(stderr, "Error parsing integer for option '%s'. Expected valid integer from %ld to %ld, got '%s'\n", opt, min, max, arg);
        return EINVAL;
    }
}

error_t ktHandleArgQuery() {
    printf("Channel              %d\n", ktGetChannel());
    printf("RF Gain              %d\n", ktGetRfGain());
    printf("Software Gain        %d\n", ktGetPGA());
    printf("Bass Gain            %d\n", ktGetBass());
    printf("Mono                 %s\n", ktGetMono() == 0 ? "false" : "true");
    printf("Mute                 %s\n", ktGetMute() == 0 ? "false" : "true");
    printf("AutoPA               %s\n", ktGetPreAutoPowerAmpDown() == 0 ? "false" : "true");
    printf("PA Bias              %s\n", ktGetPaBias() == 0 ? "false" : "true");
    printf("Standby              %s\n", ktGetStandby() == 0 ? "false" : "true");
    printf("Power Status         %s\n", ktGetPowerStatus() == 0 ? "false" : "true");
    printf("PowerAmpStatus       %s\n", ktGetPowerAmpPower() == 0 ? "false" : "true");
    printf("Silence Detected     %s\n", ktGetSilenceDetected() == 0 ? "false" : "true");
    printf("FDev                 %s\n", ktGetFrequencyDeviation() == 0 ? "false" : "true");
    printf("Switch Mode          %s\n", ktGetSwitchingChannelMode() == 0 ? "false" : "true");
    printf("Audio ENHANCE        %s\n", ktGetAudioFrequencyResponseEnhancement() == 0 ? "false" : "true");
    printf("Pilot Tone           %s\n", ktGetPilotToneAmplitudeAdjustment() == 0 ? "false" : "true");
    printf("PreEmp TimeConst     %s\n", ktGetPreEmphasisTimeConstant() == 0 ? "false" : "true");
    printf("ALC Enable           %s\n", ktGetAlc() == 0 ? "false" : "true");
    printf("ALC Compressed Gain  %d\n", ktGetAlcCompressedGain());
    printf("ALC Decay Time       %d\n", ktGetAlcDecayTime());
    printf("ALC Attack Time      %d\n", ktGetAlcAttackTime());
    printf("ALC Hold Time        %d\n", ktGetAlcHoldTime());
    printf("ALC HighThd          %d\n", ktGetAlcHighThreshold());
    printf("ALC LowThd           %d\n", ktGetAlcLowThreshold());
    printf("Silence Enable       %s\n", ktGetSilenceDetect() == 0 ? "false" : "true");
    printf("Silence LowThd       %d\n", ktGetSilenceDetectionLowThreshold());
    printf("Silence HighThd      %d\n", ktGetSilenceDetectionHighThreshold());
    printf("Silence Duration     %d\n", ktGetSilenceDetectionDurationTime());
    printf("Silence Counter Thd  %d\n", ktGetSilenceDetectionCounterThreshold());
    printf("SilenceLowCounter    %d\n", ktGetSilenceDetectionLowCounter());
    return 0;
}

error_t ktHandleArgStandby(char *arg) {
    int standby;
    error_t err = ktParseBool("standby", arg, &standby);
    if (err != 0) {
        return err;
    }
    ktSetStandby((uint8_t) standby);
    return 0;
}

error_t ktHandleArgMute(char *arg) {
    int mute;
    error_t err = ktParseBool("mute", arg, &mute);
    if (err != 0) {
        return err;
    }
    ktSetMute((uint8_t) mute);
    return 0;
}

error_t ktHandleArgMono(char *arg) {
    int mono;
    error_t err = ktParseBool("mono", arg, &mono);
    if (err != 0) {
        return err;
    }
    ktSetMono((uint8_t) mono);
    return 0;
}

error_t ktHandleArgAlc(char *arg) {
    int alc;
    error_t err = ktParseBool("alc", arg, &alc);
    if (err != 0) {
        return err;
    }
    ktSetMono((uint8_t) alc);
    return 0;
}

error_t ktHandleArgRfGain(char *arg) {
    long int rfgain;
    error_t err = ktParseInt("rfgain", arg, 0, 15, &rfgain);
    if (err != 0) {
        return err;
    }
    ktSetRfGain((uint8_t) rfgain);
    return 0;
}

error_t ktHandleArgSoftwareGain(char *arg) {
    long int softwareGain;
    error_t err = ktParseInt("softwaregain", arg, -15, 12, &softwareGain);
    if (err != 0) {
        return err;
    }
    ktSetPGA((int8_t) softwareGain);
    return 0;
}

error_t ktHandleArgPaBias(char *arg) {
    int pabias;
    error_t err = ktParseBool("pabias", arg, &pabias);
    if (err != 0) {
        return err;
    }
    ktSetPaBias((uint8_t) pabias);
    return 0;
}

error_t ktHandleArgPilotToneAmplitudeAdjustment(char *arg) {
    int pltadj;
    error_t err = ktParseBool("pltadj", arg, &pltadj);
    if (err != 0) {
        return err;
    }
    ktSetPilotToneAmplitudeAdjustment((uint8_t) pltadj);
    return 0;
}

error_t ktHandleArgPreEmphasisTimeConstant(char *arg) {
    int phtcnst;
    error_t err = ktParseBool("phtcnst", arg, &phtcnst);
    if (err != 0) {
        return err;
    }
    ktSetPreEmphasisTimeConstant((uint8_t) phtcnst);
    return 0;
}

error_t ktHandleArgAutoPowerAmpDown(char *arg) {
    int atpadn;
    error_t err = ktParseBool("atpadn", arg, &atpadn);
    if (err != 0) {
        return err;
    }
    ktSetPreAutoPowerAmpDown((uint8_t) atpadn);
    return 0;
}

error_t ktHandleArgPowerAmpPower(char *arg) {
    int pdpa;
    error_t err = ktParseBool("pdpa", arg, &pdpa);
    if (err != 0) {
        return err;
    }
    ktSetPowerAmpPower((uint8_t) pdpa);
    return 0;
}

error_t ktHandleArgFrequencyDeviation(char *arg) {
    int fdev;
    error_t err = ktParseBool("fdev", arg, &fdev);
    if (err != 0) {
        return err;
    }
    ktSetFrequencyDeviation((uint8_t) fdev);
    return 0;
}

error_t ktHandleArgBass(char *arg) {
    long int bass;
    error_t err = ktParseInt("bass", arg, 0, 3, &bass);
    if (err != 0) {
        return err;
    }
    ktSetBass((uint8_t) bass);
    return 0;
}

error_t ktHandleArgAlcDecayTime(char *arg) {
    long int alcd;
    error_t err = ktParseInt("alcd", arg, 0, 15, &alcd);
    if (err != 0) {
        return err;
    }
    ktSetAlcDecayTime((uint8_t) alcd);
    return 0;
}

error_t ktHandleArgAlcAttackTime(char *arg) {
    long int alca;
    error_t err = ktParseInt("alca", arg, 0, 15, &alca);
    if (err != 0) {
        return err;
    }
    ktSetAlcAttackTime((uint8_t) alca);
    return 0;
}

error_t ktHandleArgAlcCompressedGain(char *arg) {
    long int alcg;
    error_t err = ktParseInt("alcg", arg, 0, 7, &alcg);
    if (err != 0) {
        return err;
    }
    ktSetAlcCompressedGain((uint8_t) alcg);
    return 0;
}

error_t ktHandleArgAlcHoldTime(char *arg) {
    long int alchold;
    error_t err = ktParseInt("alchold", arg, 0, 7, &alchold);
    if (err != 0) {
        return err;
    }
    ktSetAlcHoldTime((uint8_t) alchold);
    return 0;
}

error_t ktHandleArgAlcHighThreshold(char *arg) {
    long int alcthh;
    error_t err = ktParseInt("alcthh", arg, 0, 7, &alcthh);
    if (err != 0) {
        return err;
    }
    ktSetAlcHighThreshold((uint8_t) alcthh);
    return 0;
}

error_t ktHandleArgAlcLowThreshold(char *arg) {
    long int alcthl;
    error_t err = ktParseInt("alcthl", arg, 0, 15, &alcthl);
    if (err != 0) {
        return err;
    }
    ktSetAlcLowThreshold((uint8_t) alcthl);
    return 0;
}

error_t ktHandleArgSilenceDetect(char *arg) {
    int slndt;
    error_t err = ktParseBool("slndt", arg, &slndt);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetect((uint8_t) slndt);
    return 0;
}

error_t ktHandleArgSilenceDetectionLowThreshold(char *arg) {
    long int slnthl;
    error_t err = ktParseInt("slnthl", arg, 0, 7, &slnthl);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetectionLowThreshold((uint8_t) slnthl);
    return 0;
}

error_t ktHandleArgSilenceDetectionHighThreshold(char *arg) {
    long int slnthh;
    error_t err = ktParseInt("slnthh", arg, 0, 7, &slnthh);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetectionHighThreshold((uint8_t) slnthh);
    return 0;
}

error_t ktHandleArgSilenceDetectionDurationTime(char *arg) {
    long int slntime;
    error_t err = ktParseInt("slntime", arg, 0, 15, &slntime);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetectionDurationTime((uint8_t) slntime);
    return 0;
}

error_t ktHandleArgSilenceDetectionCounterThreshold(char *arg) {
    long int slnthc;
    error_t err = ktParseInt("slnthc", arg, 0, 15, &slnthc);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetectionCounterThreshold((uint8_t) slnthc);
    return 0;
}

error_t ktHandleArgSilenceDetectionLowCounter(char *arg) {
    long int slncntl;
    error_t err = ktParseInt("slncntl", arg, 0, 7, &slncntl);
    if (err != 0) {
        return err;
    }
    ktSetSilenceDetectionLowCounter((uint8_t) slncntl);
    return 0;
}

error_t ktHandleArgAudioFrequencyResponseEnhancement(char *arg) {
    int auenhance;
    error_t err = ktParseBool("auenhance", arg, &auenhance);
    if (err != 0) {
        return err;
    }
    ktSetAudioFrequencyResponseEnhancement((uint8_t) auenhance);
    return 0;
}

error_t ktHandleArgSwitchingChannelMode(char *arg) {
    int swmod;
    error_t err = ktParseBool("swmod", arg, &swmod);
    if (err != 0) {
        return err;
    }
    ktSetSwitchingChannelMode((uint8_t) swmod);
    return 0;
}

error_t ktHandleArgChannel(char *arg) {
    long int channel;
    // 204750 is the maximum unsigned value that can be represented in 12 bits.
    error_t err = ktParseInt("channel", arg, 0, 204750, &channel);
    if (err != 0) {
        return err;
    }
    ktSetChannel((uint32_t) channel);
    return 0;
}

error_t ktHandleArg(int key, char *arg, struct argp_state *state) {
    switch (key) {
        case 'q':
            return ktHandleArgQuery();
        case 's':
            return ktHandleArgStandby(arg);
        case 'x':
            return ktHandleArgMute(arg);
        case 'm':
            return ktHandleArgMono(arg);
        case 'd':
            return ktHandleArgAutoPowerAmpDown(arg);
        case 'p':
            return ktHandleArgPowerAmpPower(arg);
        case 0x01:
            return ktHandleArgPaBias(arg);
        case 'g':
            return ktHandleArgRfGain(arg);
        case 'b':
            return ktHandleArgBass(arg);
        case 0x02:
            return ktHandleArgSoftwareGain(arg);
        case 'a':
            return ktHandleArgAlc(arg);
        case 0x03:
            return ktHandleArgAlcDecayTime(arg);
        case 0x04:
            return ktHandleArgAlcAttackTime(arg);
        case 0x05:
            return ktHandleArgAlcCompressedGain(arg);
        case 0x06:
            return ktHandleArgAlcHoldTime(arg);
        case 0x07:
            return ktHandleArgAlcHighThreshold(arg);
        case 0x08:
            return ktHandleArgAlcLowThreshold(arg);
        case 'n':
            return ktHandleArgSilenceDetect(arg);
        case 0x09:
            return ktHandleArgSilenceDetectionLowThreshold(arg);
        case 0x0A:
            return ktHandleArgSilenceDetectionHighThreshold(arg);
        case 0x0B:
            return ktHandleArgSilenceDetectionDurationTime(arg);
        case 0x0C:
            return ktHandleArgSilenceDetectionCounterThreshold(arg);
        case 0x0D:
            return ktHandleArgSilenceDetectionLowCounter(arg);
        case 'j':
            return ktHandleArgPilotToneAmplitudeAdjustment(arg);
        case 't':
            return ktHandleArgPreEmphasisTimeConstant(arg);
        case 'o':
            return ktHandleArgSwitchingChannelMode(arg);
        case 'e':
            return ktHandleArgAudioFrequencyResponseEnhancement(arg);
        case 'f':
            return ktHandleArgFrequencyDeviation(arg);
        case 'c':
            return ktHandleArgChannel(arg);
        default:
            return ARGP_ERR_UNKNOWN;
    }
}

int main(int argc, char **argv) {
    ktInit();
    const struct argp_option argOptions[] = {
        { .name = "query", .key = 'q', .doc = "Query FM radio state." },
        { .name = "standby", .key = 's', .arg = "true|false", .doc = "Set Chip Standby flag value. Unset by default." },
        { .name = "mute", .key = 'x', .arg = "true|false", .doc = "Set mute flag value. Unset by default." },
        { .name = "mono", .key = 'm', .arg = "true|false", .doc = "Set mono flag value. Unset by default." },
        { .name = "atpadn", .key = 'd', .arg = "true|false", .doc = "Set Automatic Power Down Power Amplifier flag value. Unset by default." },
        { .name = "pdpa", .key = 'p', .arg = "true|false", .doc = "Set Power Amplifier Power turnoff flag value. Unset by default." },
        { .name = "pabias", .key = 0x01, .arg = "true|false", .doc = "Set PA Bias flag. When this flag is set, signal strength is increased. This flag is set by default." },
        { .name = "rfgain", .key = 'g', .arg = "0-15", .doc = "Set radio RF gain. In other words, set the signal strength. Defaults to max RF gain." },
        { .name = "bass", .key = 'b', .arg = "0-3", .doc = "Set bass gain. 1 for 5dB, 2 for 11dB, 3 for 17dB. Defaults to disable gain." },
        { .name = "softwaregain", .key = 0x02, .arg = "-15-12", .doc = "Set radio internal software gain, in decibels. Defaults to 0. This gain is applied to the incoming audio signal before transmission. Note that this can cause clipping issues if the input signal is already at maximum loudness, so I generally recommend just leaving this at 0." },

        { .name = "alc", .key = 'a', .arg = "true|false", .doc = "Set Automatic Level Control flag value. Unset by default." },
        { .name = "alcd", .key = 0x03, .arg = "0-15", .doc = "Set ALC Decay Time. 25us as the default." },
        { .name = "alca", .key = 0x04, .arg = "0-15", .doc = "Set ALC Attack Time. 25us as the default." },
        { .name = "alcg", .key = 0x05, .arg = "0-7", .doc = "Set ALC Compressed Gain. 7/-3dB as the default." },
        { .name = "alchold", .key = 0x06, .arg = "0-7", .doc = "Set ALC Hold Time. 0/50ms as the default." },
        { .name = "alcthh", .key = 0x07, .arg = "0-7", .doc = "Set ALC High Threshold. 0/0.6 as the default." },
        { .name = "alcthl", .key = 0x08, .arg = "0-15", .doc = "Set ALC Low Threshold. 0/0.25 as the default." },

        { .name = "slndt", .key = 'n', .arg = "true|false", .doc = "Set Silence Detection flag value. Unset by default." },
        { .name = "slnthl", .key = 0x09, .arg = "0-7", .doc = "Set Silence Detection Low Threshold. 0/0.25mv as the default." },
        { .name = "slnthh", .key = 0x0A, .arg = "0-7", .doc = "Set Silence Detection High Threshold. 0/0.5mv as the default." },
        { .name = "slntime", .key = 0x0B, .arg = "0-15", .doc = "Set Silence Detection Duration Time. 0/50ms as the default." },
        { .name = "slnthc", .key = 0x0C, .arg = "0-7", .doc = "Set Silence Detection High Level Counter Threshold. 0/15 as the default." },
        { .name = "slncntl", .key = 0x0D, .arg = "0-7", .doc = "Set Silence Low Counter. 1 as the default." },

        { .name = "pltadj", .key = 'j', .arg = "true|false", .doc = "Set Pilot Tone Amplitude Adjustment flag value. Unset by default." },
        { .name = "phtcnst", .key = 't', .arg = "true|false", .doc = "Set Pre-emphasis Time-Constant flag value. Unset by default." },
        { .name = "swmod", .key = 'o', .arg = "true|false", .doc = "Set Switching Channel Mode flag value. Unset by default." },
        { .name = "auenhance", .key = 'e', .arg = "true|false", .doc = "Set Audio Frequency Response Enhancement flag value. Unset by default." },
        { .name = "fdev", .key = 'f', .arg = "true|false", .doc = "Set frequency deviation flag. 112.5KHz as the default." },
        { .name = "channel", .key = 'c', .arg = "0-204750", .doc = "Set channel frequency in KHz. Frequency will be rounded down to the nearest 50KHz. Valid frequency range is unknown, but anything within the standard FM radio broadcast spectrum should work." },
        { 0 }
    };
    const struct argp argParser = {
        .parser = ktHandleArg,
        .options = argOptions,
        .doc = "rpi-kt0803l - Configure the KT0803L FM radios over I^2C on the Raspberry Pi."
    };
    error_t parseError = argp_parse(&argParser, argc, argv, 0, NULL, NULL);
    if (parseError != 0) {
        if (parseError != EINVAL) {
            fprintf(stderr, "Error: %s\n", strerror(parseError));
        }
        exit(1);
    }
    return 0;
}
