/*! @file radio_config.h
 * @brief This file contains the automatically generated
 * configurations.
 *
 * @n WDS GUI Version: 3.2.11.0
 * @n Device: Si4463 Rev.: B1                                 
 *
 * @b COPYRIGHT
 * @n Silicon Laboratories Confidential
 * @n Copyright 2017 Silicon Laboratories, Inc.
 * @n http://www.silabs.com
 */

#ifndef M350k1_H_
#define M350k1_H_

// USER DEFINED PARAMETERS
// Define your own parameters here

// INPUT DATA
/*
// Crys_freq(Hz): 26000000    Crys_tol(ppm): 20    IF_mode: 2    High_peRF_M350k1_Ch_Fil: 1    OSRtune: 0    Ch_Fil_Bw_AFC: 0    ANT_DIV: 0    PM_pattern: 0    
// MOD_type: 2    Rsymb(sps): 1000    Fdev(Hz): 1000    RXBW(Hz): 150000    Manchester: 0    AFC_en: 0    Rsymb_error: 0.0    Chip-Version: 2    
// RF Freq.(MHz): 350    API_TC: 29    fhst: 250000    inputBW: 0    BERT: 0    RAW_dout: 0    D_source: 0    Hi_pfm_div: 1    
// 
// # RX IF frequency is  -406250 Hz
// # WB filter 1 (BW =  33.07 kHz);  NB-filter 1 (BW = 33.07 kHz)
// 
// Modulation index: 2
*/


// CONFIGURATION PARAMETERS



// CONFIGURATION COMMANDS

/*
// Command:                  RF_M350k1_POWER_UP
// Description:              Command to power-up the device and select the operational mode and functionality.
*/
#define RF_M350k1_POWER_UP 0x02, 0x01, 0x00, 0x01, 0x8C, 0xBA, 0x80

/*
// Command:                  RF_M350k1_GPIO_PIN_CFG
// Description:              Configures the GPIO pins.
*/
#define RF_M350k1_GPIO_PIN_CFG 0x13, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties:           RF_M350k1_GLOBAL_XO_TUNE_2
// Number of properties:     2
// Group ID:                 0x00
// Start ID:                 0x00
// Default values:           0x40, 0x00, 
// Descriptions:
//   GLOBAL_XO_TUNE - Configure the internal capacitor frequency tuning bank for the crystal oscillator.
//   GLOBAL_CLK_CFG - Clock configuration options.
*/
#define RF_M350k1_GLOBAL_XO_TUNE_2 0x11, 0x00, 0x02, 0x00, 0x36, 0x00

/*
// Set properties:           RF_M350k1_GLOBAL_CONFIG_1
// Number of properties:     1
// Group ID:                 0x00
// Start ID:                 0x03
// Default values:           0x20, 
// Descriptions:
//   GLOBAL_CONFIG - Global configuration settings.
*/
#define RF_M350k1_GLOBAL_CONFIG_1 0x11, 0x00, 0x01, 0x03, 0x60

/*
// Set properties:           RF_M350k1_INT_CTL_ENABLE_2
// Number of properties:     2
// Group ID:                 0x01
// Start ID:                 0x00
// Default values:           0x04, 0x00, 
// Descriptions:
//   INT_CTL_ENABLE - This property provides for global enabling of the three interrupt groups (Chip, Modem and Packet Handler) in order to generate HW interrupts at the NIRQ pin.
//   INT_CTL_PH_ENABLE - Enable individual interrupt sources within the Packet Handler Interrupt Group to generate a HW interrupt on the NIRQ output pin.
*/
#define RF_M350k1_INT_CTL_ENABLE_2 0x11, 0x01, 0x02, 0x00, 0x01, 0xD8

/*
// Set properties:           RF_M350k1_FRR_CTL_A_MODE_4
// Number of properties:     4
// Group ID:                 0x02
// Start ID:                 0x00
// Default values:           0x01, 0x02, 0x09, 0x00, 
// Descriptions:
//   FRR_CTL_A_MODE - Fast Response Register A Configuration.
//   FRR_CTL_B_MODE - Fast Response Register B Configuration.
//   FRR_CTL_C_MODE - Fast Response Register C Configuration.
//   FRR_CTL_D_MODE - Fast Response Register D Configuration.
*/
#define RF_M350k1_FRR_CTL_A_MODE_4 0x11, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00

/*
// Set properties:           RF_M350k1_PREAMBLE_CONFIG_1
// Number of properties:     1
// Group ID:                 0x10
// Start ID:                 0x04
// Default values:           0x21, 
// Descriptions:
//   PREAMBLE_CONFIG - General configuration bits for the Preamble field.
*/
#define RF_M350k1_PREAMBLE_CONFIG_1 0x11, 0x10, 0x01, 0x04, 0x31

/*
// Set properties:           RF_M350k1_SYNC_CONFIG_3
// Number of properties:     3
// Group ID:                 0x11
// Start ID:                 0x00
// Default values:           0x01, 0x2D, 0xD4, 
// Descriptions:
//   SYNC_CONFIG - Sync Word configuration bits.
//   SYNC_BITS_31_24 - Sync word.
//   SYNC_BITS_23_16 - Sync word.
*/
#define RF_M350k1_SYNC_CONFIG_3 0x11, 0x11, 0x03, 0x00, 0x01, 0xB4, 0x2B

/*
// Set properties:           RF_M350k1_PKT_CONFIG1_1
// Number of properties:     1
// Group ID:                 0x12
// Start ID:                 0x06
// Default values:           0x00, 
// Descriptions:
//   PKT_CONFIG1 - General configuration bits for transmission or reception of a packet.
*/
#define RF_M350k1_PKT_CONFIG1_1 0x11, 0x12, 0x01, 0x06, 0x02

/*
// Set properties:           RF_M350k1_PKT_FIELD_1_CONFIG_1
// Number of properties:     1
// Group ID:                 0x12
// Start ID:                 0x0F
// Default values:           0x00, 
// Descriptions:
//   PKT_FIELD_1_CONFIG - General data processing and packet configuration bits for Field 1.
*/
#define RF_M350k1_PKT_FIELD_1_CONFIG_1 0x11, 0x12, 0x01, 0x0F, 0x04

/*
// Set properties:           RF_M350k1_MODEM_MOD_TYPE_12
// Number of properties:     12
// Group ID:                 0x20
// Start ID:                 0x00
// Default values:           0x02, 0x80, 0x07, 0x0F, 0x42, 0x40, 0x01, 0xC9, 0xC3, 0x80, 0x00, 0x06, 
// Descriptions:
//   MODEM_MOD_TYPE - Selects the type of modulation. In TX mode, additionally selects the source of the modulation.
//   MODEM_MAP_CONTROL - Controls polarity and mapping of transmit and receive bits.
//   MODEM_DSM_CTRL - Miscellaneous control bits for the Delta-Sigma Modulator (DSM) in the PLL Synthesizer.
//   MODEM_DATA_RATE_2 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_DATA_RATE_1 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_DATA_RATE_0 - Unsigned 24-bit value used to determine the TX data rate
//   MODEM_TX_NCO_MODE_3 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_2 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_1 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_TX_NCO_MODE_0 - TX Gaussian filter oversampling ratio and Byte 3 of unsigned 26-bit TX Numerically Controlled Oscillator (NCO) modulus.
//   MODEM_FREQ_DEV_2 - 17-bit unsigned TX frequency deviation word.
//   MODEM_FREQ_DEV_1 - 17-bit unsigned TX frequency deviation word.
*/
#define RF_M350k1_MODEM_MOD_TYPE_12 0x11, 0x20, 0x0C, 0x00, 0x02, 0x00, 0x07, 0x00, 0x27, 0x10, 0x01, 0x8C, 0xBA, 0x80, 0x00, 0x00

/*
// Set properties:           RF_M350k1_MODEM_FREQ_DEV_0_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x0C
// Default values:           0xD3, 
// Descriptions:
//   MODEM_FREQ_DEV_0 - 17-bit unsigned TX frequency deviation word.
*/
#define RF_M350k1_MODEM_FREQ_DEV_0_1 0x11, 0x20, 0x01, 0x0C, 0x79

/*
// Set properties:           RF_M350k1_MODEM_TX_RAMP_DELAY_8
// Number of properties:     8
// Group ID:                 0x20
// Start ID:                 0x18
// Default values:           0x01, 0x00, 0x08, 0x03, 0xC0, 0x00, 0x10, 0x20, 
// Descriptions:
//   MODEM_TX_RAMP_DELAY - TX ramp-down delay setting.
//   MODEM_MDM_CTRL - MDM control.
//   MODEM_IF_CONTROL - Selects Fixed-IF, Scaled-IF, or Zero-IF mode of RX Modem operation.
//   MODEM_IF_FREQ_2 - the IF frequency setting (an 18-bit signed number).
//   MODEM_IF_FREQ_1 - the IF frequency setting (an 18-bit signed number).
//   MODEM_IF_FREQ_0 - the IF frequency setting (an 18-bit signed number).
//   MODEM_DECIMATION_CFG1 - Specifies three decimator ratios for the Cascaded Integrator Comb (CIC) filter.
//   MODEM_DECIMATION_CFG0 - Specifies miscellaneous parameters and decimator ratios for the Cascaded Integrator Comb (CIC) filter.
*/
#define RF_M350k1_MODEM_TX_RAMP_DELAY_8 0x11, 0x20, 0x08, 0x18, 0x01, 0x80, 0x08, 0x03, 0x40, 0x00, 0x30, 0x10

/*
// Set properties:           RF_M350k1_MODEM_BCR_OSR_1_9
// Number of properties:     9
// Group ID:                 0x20
// Start ID:                 0x22
// Default values:           0x00, 0x4B, 0x06, 0xD3, 0xA0, 0x06, 0xD3, 0x02, 0xC0, 
// Descriptions:
//   MODEM_BCR_OSR_1 - RX BCR/Slicer oversampling rate (12-bit unsigned number).
//   MODEM_BCR_OSR_0 - RX BCR/Slicer oversampling rate (12-bit unsigned number).
//   MODEM_BCR_NCO_OFFSET_2 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_NCO_OFFSET_1 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_NCO_OFFSET_0 - RX BCR NCO offset value (an unsigned 22-bit number).
//   MODEM_BCR_GAIN_1 - The unsigned 11-bit RX BCR loop gain value.
//   MODEM_BCR_GAIN_0 - The unsigned 11-bit RX BCR loop gain value.
//   MODEM_BCR_GEAR - RX BCR loop gear control.
//   MODEM_BCR_MISC1 - Miscellaneous control bits for the RX BCR loop.
*/
#define RF_M350k1_MODEM_BCR_OSR_1_9 0x11, 0x20, 0x09, 0x22, 0x04, 0x3B, 0x00, 0x78, 0xFD, 0x00, 0x3D, 0x02, 0xC2

/*
// Set properties:           RF_M350k1_MODEM_AFC_GEAR_7
// Number of properties:     7
// Group ID:                 0x20
// Start ID:                 0x2C
// Default values:           0x00, 0x23, 0x83, 0x69, 0x00, 0x40, 0xA0, 
// Descriptions:
//   MODEM_AFC_GEAR - RX AFC loop gear control.
//   MODEM_AFC_WAIT - RX AFC loop wait time control.
//   MODEM_AFC_GAIN_1 - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality.
//   MODEM_AFC_GAIN_0 - Sets the gain of the PLL-based AFC acquisition loop, and provides miscellaneous control bits for AFC functionality.
//   MODEM_AFC_LIMITER_1 - Set the AFC limiter value.
//   MODEM_AFC_LIMITER_0 - Set the AFC limiter value.
//   MODEM_AFC_MISC - Specifies miscellaneous AFC control bits.
*/
#define RF_M350k1_MODEM_AFC_GEAR_7 0x11, 0x20, 0x07, 0x2C, 0x04, 0x36, 0x80, 0x05, 0x48, 0x1A, 0x80

/*
// Set properties:           RF_M350k1_MODEM_AGC_CONTROL_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x35
// Default values:           0xE0, 
// Descriptions:
//   MODEM_AGC_CONTROL - Miscellaneous control bits for the Automatic Gain Control (AGC) function in the RX Chain.
*/
#define RF_M350k1_MODEM_AGC_CONTROL_1 0x11, 0x20, 0x01, 0x35, 0xE2

/*
// Set properties:           RF_M350k1_MODEM_AGC_WINDOW_SIZE_9
// Number of properties:     9
// Group ID:                 0x20
// Start ID:                 0x38
// Default values:           0x11, 0x10, 0x10, 0x0B, 0x1C, 0x40, 0x00, 0x00, 0x2B, 
// Descriptions:
//   MODEM_AGC_WINDOW_SIZE - Specifies the size of the measurement and settling windows for the AGC algorithm.
//   MODEM_AGC_RFPD_DECAY - Sets the decay time of the RF peak detectors.
//   MODEM_AGC_IFPD_DECAY - Sets the decay time of the IF peak detectors.
//   MODEM_FSK4_GAIN1 - Specifies the gain factor of the secondary branch in 4(G)FSK ISI-suppression.
//   MODEM_FSK4_GAIN0 - Specifies the gain factor of the primary branch in 4(G)FSK ISI-suppression.
//   MODEM_FSK4_TH1 - 16 bit 4(G)FSK slicer threshold.
//   MODEM_FSK4_TH0 - 16 bit 4(G)FSK slicer threshold.
//   MODEM_FSK4_MAP - 4(G)FSK symbol mapping code.
//   MODEM_OOK_PDTC - Configures the attack and decay times of the OOK Peak Detector.
*/
#define RF_M350k1_MODEM_AGC_WINDOW_SIZE_9 0x11, 0x20, 0x09, 0x38, 0x11, 0xED, 0xED, 0x00, 0x02, 0x80, 0x00, 0x00, 0x2B

/*
// Set properties:           RF_M350k1_MODEM_OOK_CNT1_9
// Number of properties:     9
// Group ID:                 0x20
// Start ID:                 0x42
// Default values:           0xA4, 0x03, 0x56, 0x02, 0x00, 0xA3, 0x02, 0x80, 0xFF, 
// Descriptions:
//   MODEM_OOK_CNT1 - OOK control.
//   MODEM_OOK_MISC - Selects the detector(s) used for demodulation of an OOK signal, or for demodulation of a (G)FSK signal when using the asynchronous demodulator.
//   MODEM_RAW_SEARCH - Defines and controls the search period length for the Moving Average and Min-Max detectors.
//   MODEM_RAW_CONTROL - Defines gain and enable controls for raw / nonstandard mode.
//   MODEM_RAW_EYE_1 - 11 bit eye-open detector threshold.
//   MODEM_RAW_EYE_0 - 11 bit eye-open detector threshold.
//   MODEM_ANT_DIV_MODE - Antenna diversity mode settings.
//   MODEM_ANT_DIV_CONTROL - Specifies controls for the Antenna Diversity algorithm.
//   MODEM_RSSI_THRESH - Configures the RSSI threshold.
*/
#define RF_M350k1_MODEM_OOK_CNT1_9 0x11, 0x20, 0x09, 0x42, 0xA4, 0x02, 0xD6, 0x83, 0x00, 0x32, 0x01, 0x80, 0xFF

/*
// Set properties:           RF_M350k1_MODEM_RSSI_CONTROL_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x4C
// Default values:           0x01, 
// Descriptions:
//   MODEM_RSSI_CONTROL - Control of the averaging modes and latching time for reporting RSSI value(s).
*/
#define RF_M350k1_MODEM_RSSI_CONTROL_1 0x11, 0x20, 0x01, 0x4C, 0x10

/*
// Set properties:           RF_M350k1_MODEM_RSSI_COMP_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x4E
// Default values:           0x32, 
// Descriptions:
//   MODEM_RSSI_COMP - RSSI compensation value.
*/
#define RF_M350k1_MODEM_RSSI_COMP_1 0x11, 0x20, 0x01, 0x4E, 0x40

/*
// Set properties:           RF_M350k1_MODEM_CLKGEN_BAND_1
// Number of properties:     1
// Group ID:                 0x20
// Start ID:                 0x51
// Default values:           0x08, 
// Descriptions:
//   MODEM_CLKGEN_BAND - Select PLL Synthesizer output divider ratio as a function of frequency band.
*/
#define RF_M350k1_MODEM_CLKGEN_BAND_1 0x11, 0x20, 0x01, 0x51, 0x0B

/*
// Set properties:           RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x00
// Default values:           0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE13_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE12_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE11_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE10_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE9_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE8_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE7_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE6_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE5_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE4_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE3_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE2_7_0 - Filter coefficients for the first set of RX filter coefficients.
*/
#define RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12 0x11, 0x21, 0x0C, 0x00, 0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01

/*
// Set properties:           RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x0C
// Default values:           0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xC4, 0x30, 0x7F, 0xF5, 0xB5, 
// Descriptions:
//   MODEM_CHFLT_RX1_CHFLT_COE1_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COE0_7_0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM0 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM1 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM2 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX1_CHFLT_COEM3 - Filter coefficients for the first set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE13_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE12_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE11_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE10_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE9_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE8_7_0 - Filter coefficients for the second set of RX filter coefficients.
*/
#define RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12 0x11, 0x21, 0x0C, 0x0C, 0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F, 0xFF, 0xBA, 0x0F, 0x51, 0xCF, 0xA9

/*
// Set properties:           RF_M350k1_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12
// Number of properties:     12
// Group ID:                 0x21
// Start ID:                 0x18
// Default values:           0xB8, 0xDE, 0x05, 0x17, 0x16, 0x0C, 0x03, 0x00, 0x15, 0xFF, 0x00, 0x00, 
// Descriptions:
//   MODEM_CHFLT_RX2_CHFLT_COE7_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE6_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE5_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE4_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE3_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE2_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE1_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COE0_7_0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM0 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM1 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM2 - Filter coefficients for the second set of RX filter coefficients.
//   MODEM_CHFLT_RX2_CHFLT_COEM3 - Filter coefficients for the second set of RX filter coefficients.
*/
#define RF_M350k1_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12 0x11, 0x21, 0x0C, 0x18, 0xC9, 0xFC, 0x1B, 0x1E, 0x0F, 0x01, 0xFC, 0xFD, 0x15, 0xFF, 0x00, 0x0F

/*
// Set properties:           RF_M350k1_PA_TC_1
// Number of properties:     1
// Group ID:                 0x22
// Start ID:                 0x03
// Default values:           0x5D, 
// Descriptions:
//   PA_TC - Configuration of PA ramping parameters.
*/
#define RF_M350k1_PA_TC_1 0x11, 0x22, 0x01, 0x03, 0x3D

/*
// Set properties:           RF_M350k1_SYNTH_PFDCP_CPFF_7
// Number of properties:     7
// Group ID:                 0x23
// Start ID:                 0x00
// Default values:           0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03, 
// Descriptions:
//   SYNTH_PFDCP_CPFF - Feed forward charge pump current selection.
//   SYNTH_PFDCP_CPINT - Integration charge pump current selection.
//   SYNTH_VCO_KV - Gain scaling factors (Kv) for the VCO tuning varactors on both the integrated-path and feed forward path.
//   SYNTH_LPFILT3 - Value of resistor R2 in feed-forward path of loop filter.
//   SYNTH_LPFILT2 - Value of capacitor C2 in feed-forward path of loop filter.
//   SYNTH_LPFILT1 - Value of capacitors C1 and C3 in feed-forward path of loop filter.
//   SYNTH_LPFILT0 - Bias current of the active amplifier in the feed-forward loop filter.
*/
#define RF_M350k1_SYNTH_PFDCP_CPFF_7 0x11, 0x23, 0x07, 0x00, 0x2C, 0x0E, 0x0B, 0x04, 0x0C, 0x73, 0x03

/*
// Set properties:           RF_M350k1_FREQ_CONTROL_INTE_8
// Number of properties:     8
// Group ID:                 0x40
// Start ID:                 0x00
// Default values:           0x3C, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0xFF, 
// Descriptions:
//   FREQ_CONTROL_INTE - Frac-N PLL Synthesizer integer divide number.
//   FREQ_CONTROL_FRAC_2 - Frac-N PLL fraction number.
//   FREQ_CONTROL_FRAC_1 - Frac-N PLL fraction number.
//   FREQ_CONTROL_FRAC_0 - Frac-N PLL fraction number.
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_1 - EZ Frequency Programming channel step size.
//   FREQ_CONTROL_CHANNEL_STEP_SIZE_0 - EZ Frequency Programming channel step size.
//   FREQ_CONTROL_W_SIZE - Set window gating period (in number of crystal reference clock cycles) for counting VCO frequency during calibration.
//   FREQ_CONTROL_VCOCNT_RX_ADJ - Adjust target count for VCO calibration in RX mode.
*/
#define RF_M350k1_FREQ_CONTROL_INTE_8 0x11, 0x40, 0x08, 0x00, 0x4F, 0x0E, 0x27, 0x62, 0x76, 0x27, 0x20, 0xFD


// AUTOMATICALLY GENERATED CODE! 
// DO NOT EDIT/MODIFY BELOW THIS LINE!
// --------------------------------------------


#define RADIO_M350k1_CONFIGURATION_DATA_ARRAY { \
        0x07, RF_M350k1_POWER_UP, \
        0x08, RF_M350k1_GPIO_PIN_CFG, \
        0x06, RF_M350k1_GLOBAL_XO_TUNE_2, \
        0x05, RF_M350k1_GLOBAL_CONFIG_1, \
        0x06, RF_M350k1_INT_CTL_ENABLE_2, \
        0x08, RF_M350k1_FRR_CTL_A_MODE_4, \
        0x05, RF_M350k1_PREAMBLE_CONFIG_1, \
        0x07, RF_M350k1_SYNC_CONFIG_3, \
        0x05, RF_M350k1_PKT_CONFIG1_1, \
        0x05, RF_M350k1_PKT_FIELD_1_CONFIG_1, \
        0x10, RF_M350k1_MODEM_MOD_TYPE_12, \
        0x05, RF_M350k1_MODEM_FREQ_DEV_0_1, \
        0x0C, RF_M350k1_MODEM_TX_RAMP_DELAY_8, \
        0x0D, RF_M350k1_MODEM_BCR_OSR_1_9, \
        0x0B, RF_M350k1_MODEM_AFC_GEAR_7, \
        0x05, RF_M350k1_MODEM_AGC_CONTROL_1, \
        0x0D, RF_M350k1_MODEM_AGC_WINDOW_SIZE_9, \
        0x0D, RF_M350k1_MODEM_OOK_CNT1_9, \
        0x05, RF_M350k1_MODEM_RSSI_CONTROL_1, \
        0x05, RF_M350k1_MODEM_RSSI_COMP_1, \
        0x05, RF_M350k1_MODEM_CLKGEN_BAND_1, \
        0x10, RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12, \
        0x10, RF_M350k1_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12, \
        0x10, RF_M350k1_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12, \
        0x05, RF_M350k1_PA_TC_1, \
        0x0B, RF_M350k1_SYNTH_PFDCP_CPFF_7, \
        0x0C, RF_M350k1_FREQ_CONTROL_INTE_8, \
        0x00 \
 }


#endif /* RADIO_CONFIG_H_ */