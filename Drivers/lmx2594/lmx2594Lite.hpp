#pragma once

#include "stdint.h"

/*===========================================================================
 * Класс для работы с PLL LMX2594
 *===========================================================================*/

class LMX2594 {
 public:

  enum CPG_type {
    ma_0 = 0,
    ma_6 = 1,
    ma_12 = 3,
    ma_3 = 4,
    ma_9 = 5,
    ma_15 = 7,
    ma_auto
  };

  enum OUT_ISET_type {
    max_boost,
    boost2,
    boost1,
    noboost
  };

  enum SYSREF_DIV_PRE_type {
    divpre_1 = 1,
    divpre_2 = 2,
    divpre_4 = 4
  };

  enum FCAL_HPFD_ADJ_type {
    low_100,
    in_100_150,
    in_150_200,
    up_200
  };

  enum CAL_CLK_DIV_type {
    low_200,
    in_200_400,
    in_400_800,
    up_800
  };

  enum CHDIV_type {
    div_2,
    div_4,
    div_6,
    div_8,
    div_12,
    div_16,
    div_24,
    div_32,
    div_48,
    div_64,
    div_72,
    div_96,
    div_128,
    div_192,
    div_256,
    div_384,
    div_512,
    div_768
  };

  enum MASH_ORDER_type {
    integer,
    frac1,
    frac2,
    frac3,
    frac4,
    noChange
  };

  enum FCAL_LPFD_ADJ_type {
    up_10,
    in_5_10,
    in_2_5_5,
    low_2_5
  };

  enum MULT_type {
    bypass = 1,
    x3 = 3,
    x4,
    x5,
    x6,
    x7
  };

  enum VCO_SEL_type {
    vco_1 = 1,
    vco_2,
    vco_3,
    vco_4,
    vco_5,
    vco_6,
    vco_7  // у 2572 нет vco_7
  };

  enum OUTA_MUX_type {
    chdiva,
    vcoa,
    highza = 3
  };

  enum OUTB_MUX_type {
    chdivb,
    vcob,
    sysref,
    highzb
  };

  enum INPIN_FMT_type {
    inpin_cmos_cmos,
    inpin_lvds_cmos,
    inpin_cmos_lvds,
    inpin_lvds_lvds,
    inpin_lvdsf_cmos = 5,
    inpin_cmos_lvdsf,
    inpin_lvdsf_lvdsf
  };

  enum INPIN_LVL_type {
    inpin_vin2,
    inpin_vin4,
    inpin_vin
  };

  enum RAMP_TRIG_type {
    disabled,
    rclk_rise,
    rdir_rise,
    atrig = 4,
    rclk_fall = 9,
    rdir_fall = 10
  };

  enum RAMP_BURST_TRIG_type {
    ramp,
    trig_a,
    trig_b
  };

  static const uint32_t CHDIV_arr_i_max = 17;  //размер массива CHDIV_arr - 1

  CHDIV_type CHDIV_arr[CHDIV_arr_i_max + 1] = { div_2, div_4, div_6, div_8,
      div_12, div_16, div_24, div_32, div_48, div_64, div_72, div_96, div_128,
      div_192, div_256, div_384, div_512, div_768 };

  uint32_t CHDIV_val[CHDIV_arr_i_max + 1] = { 2, 4, 6, 8, 12, 16, 24, 32, 48,
      64, 72, 96, 128, 192, 256, 384, 512, 768 };

  // fvco_max определяется функцией
  static const uint64_t fvco_min = 7450000000;

  static const uint64_t fout_min = 10000000;
  static const uint64_t fout_max = 15000000000;

  static const uint32_t fosc_min = 5000000;  // fosc_max рассчитывается функцией

  // маскимальное значение мощности
  static const uint8_t PWR_Max = 63;
  // значение мощности при котором отключается канал
  static const uint8_t PWR_Off = PWR_Max + 1;

  // максимальный делитель
  uint32_t DEN_MAX = 0xFFFFFFFF;

  static const uint16_t pll_r_pre_max = 128;
  static const uint16_t pll_r_pre_min = 1;

 private:

  static const uint16_t pll_r_max = 255;
  static const uint16_t pll_r_min = 1;

  static const uint16_t pll_mult_max = 7;
  static const uint16_t pll_mult_min = 3;

  static const uint32_t N_max = 524287;  // N_min рассчитывается функцией

  static const uint32_t fin_mult_min = 30000000;
  static const uint32_t fin_mult_max = 70000000;

  static const uint32_t fout_mult_min = 180000000;
  static const uint32_t fout_mult_max = 250000000;

  static const uint32_t fin_osc_2x_max = 200000000;

  void find_num_den(double alfa, uint32_t &num, uint32_t &den, uint32_t &nint);

 public:

  // диапазон частот VCO (в МГц) для заполнения VCD
  // иницилизируется в конструкторах
  uint32_t Fvcd_min { }, Fvcd_max { };

  // число строк таблицы PWR иницилизируется в конструкторах
  uint32_t PWR_count { };

  // здесь в InitRegs() сохраняется начальное значение MASH_ORDER
  MASH_ORDER_type Init_MASH_ORDER { frac3 };

  // true - что бы записать все регситры
  bool first_init = true;

  // число регистров PLL на запись при смене частоты
  static constexpr uint8_t write_regs_count = 21;

  // номера записываемых регистров при перестройке частоты
  uint8_t Num_Write_regs[write_regs_count] = { 2, 78, 75, 46, 45, 44, 43, 42,
      39, 38, 37, 36, 34, 31, 20, 19, 17, 16, 14, 12, 0 };
  /* 78 - VCO_CAPCTRL_STRT
   * 75 - CHDIV
   * 46 - OUTB_MUX
   * 45 - OUTA_MUX
   * 44 - FRAC_ORDER
   * 43..42 - NUM
   * 39..38 - DEN
   * 37 - PFD_DLY_SEL
   * 36,34 - N
   * 31 - CHDIV_DIV2
   * 20 - VCO_SEL, VCO_SEL_FORCE
   * 19 - VCO_CAPCTRL
   * 17 - VCO_DACISET_STRT
   * 16 - VCO_DACISET
   * 14 - CPG
   * 12 - PLL_R_PRE
   * 11 - PLL_R - - не пишем!!!
   * 10 - MULT - не пишем!!!
   *  9 - OSC_2X - не пишем!!!
   *  8 - DAC_FORCE, CAP_FORCE - не пишем!!!
   *  2 - STM_ON - первым!!!
   *  1 - CAL_CLK_DIV - не пишем!!!
   *  0 - управление
   */

  // число регистров PLL
  static const uint8_t RegsFullCount = 113;
  // число регистров на запись
  static const uint8_t RegsCount = 107;
  // число регистров на запись если не используется Ramp
  static const uint8_t RegsCount_NoRamp = 79;

  union REGS {
    // инициализиуется константыми полями - не параметрами! Параметры в Init
    uint16_t Arr[RegsCount] { 0x2410, 0x0808, 0x0500, 0x0642, 0x0A43, 0x00C8,
        0xC802, 0x00B2, 0x2000, 0x0604, 0x1058, 0x0008, 0x5000, 0x4000, 0x1E00,
        0x064F, 0x0000, 0x0000, 0x0064, 0x2700, 0xC048, 0x0401, 0x0001, 0x007C,
        0x071A, 0x0C2B, 0x0DB0, 0x0002, 0x0488, 0x318C, 0x318C, 0x03EC, 0x0393,
        0x1E21, 0x0000, 0x0004, 0x0000, 0x0004, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0xC0C0, 0x07FC, 0x0300, 0x0300, 0x4180, 0x0000,
        0x0080, 0x0820, 0x0000, 0x0000, 0x0000, 0x0000, 0x0020, 0x0001, 0x0000,
        0x0000, 0x00A8, 0x0322, 0x0000, 0x1388, 0x0000, 0x01F4, 0x0000, 0x03E8,
        0x0000, 0xC350, 0x0001, 0x0000, 0x0000, 0x0000, 0x0800, 0x000C, 0x0000,
        0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0800, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
        0x0000, 0x0000 };  //, 0x0000, 0x0000, 0x0000, 0x0000,
                           // 0x0000, 0x0000 };

    struct {
      // R0
      uint16_t POWERDOWN :1, RESET :1, MUXOUT_LD_SEL :1, FCAL_EN :1, :1,
          FCAL_LPFD_ADJ :2, FCAL_HPFD_ADJ :2, OUT_MUTE :1, :4,
          VCO_PHASE_SYNC_EN :1, RAMP_EN :1;
      uint16_t CAL_CLK_DIV :3, :13;
      uint16_t :10, STM_ON :1, :5;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :14, OUT_FORCE :1, :1;
      uint16_t :11, VCO_CAPCTRL_FORCE :1, :2, VCO_DACISET_FORCE :1, :1;
      uint16_t :12, OSC_2X :1, :3;
      // R10
      uint16_t :7, MULT :5, :4;                        // MULT=1 (Bypass)
      uint16_t :4, PLL_R :8, :4;                        // PLL_R=2 (R)
      uint16_t PLL_R_PRE :12, :4;                        // PLL_R_PRE=1 (PreR)
      uint16_t :16;
      uint16_t :4, CPG :3, :9;                        // CPG=7 (15mA)
      uint16_t :16;
      uint16_t VCO_DACISET :9, :7;                        // VCO_DACISET=128
      uint16_t VCO_DACISET_STRT :9, :7;                  // VCO_DACISET_STRT=300
      uint16_t :16;
      uint16_t VCO_CAPCTRL :8, :8;                        // VCO_CAPCTRL=183
      // R20
      uint16_t :10, VCO_SEL_FORCE :1, VCO_SEL :3, :2;      // VCO_SEL=4
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      // R30
      uint16_t :16;
      uint16_t :14, CHDIV_DIV2 :1, :1;      // CHDIV_DIV2=1
      uint16_t :16;
      uint16_t :16;
      uint16_t PLL_N_18_16 :3, :13;
      uint16_t :16;
      uint16_t PLL_N_15_0 :16;      // PLL_N_15_0=124
      uint16_t :8, PFD_DLY_SEL :6, :1, MASH_SEED_EN :1;      // PFD_DLY_SEL=3
      uint16_t PLL_DEN_31_16 :16;
      uint16_t PLL_DEN_15_0 :16;      // PLL_DEN_15_0=4
      // R40
      uint16_t MASH_SEED_31_16 :16;
      uint16_t MASH_SEED_15_0 :16;
      uint16_t PLL_NUM_31_16 :16;
      uint16_t PLL_NUM_15_0 :16;
      uint16_t MASH_ORDER :3, :2, MASH_RESET_N :1, OUTA_PD :1, OUTB_PD :1,
          OUTA_PWR :6, :2;      // MASH_ORDER=3, MASH_RESET_N=1, OUTA_PWR=31
      uint16_t OUTB_PWR :6, :3, OUT_ISET :2, OUTA_MUX :2, :3;     // OUTB_PWR=31
      uint16_t OUTB_MUX :2, :14;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      // R50
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :9, INPIN_FMT :3, INPIN_LVL :2, INPIN_HYST :1, INPIN_IGNORE :1;  // INPIN_IGNORE=1
      uint16_t LD_TYPE :1, :15;      // LD_TYPE=1
      // R60
      uint16_t LD_DLY :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t MASH_RST_COUNT_31_16 :16;      // MASH_RST_COUNT_31_16=0
      // R70
      uint16_t MASH_RST_COUNT_15_0 :16;      // MASH_RST_COUNT_15_0=50000
      uint16_t :2, SYSREF_REPEAT :1, SYSREF_EN :1, SYSREF_PULSE :1,
          SYSREF_DIV_PRE :3, :8;      // SYSREF_DIV_PRE=4
      uint16_t SYSREF_DIV :11, :5;      // SYSREF_DIV=1
      uint16_t JESD_DAC1_CTRL :6, JESD_DAC2_CTRL :6, :4;    // JESD_DAC1_CTRL=63
      uint16_t JESD_DAC3_CTRL :6, JESD_DAC4_CTRL :6, SYSREF_PULSE_CNT :4;
      uint16_t :6, CHDIV :5, :5;      // CHDIV=3
      uint16_t :16;
      uint16_t :16;
      uint16_t :1, VCO_CAPCTRL_STRT :8, QUICK_RECAL_EN :1, :1,
          RAMP_THRESH_32 :1, :4;      // VCO_CAPCTRL_STRT=1
      uint16_t RAMP_THRESH_31_16 :16;      // RAMP_THRESH_31_16=26h
      // R80
      uint16_t RAMP_THRESH_15_0 :16;      // RAMP_THRESH_15_0=6666h (2'516'582)
      uint16_t RAMP_LIMIT_HIGH_32 :1, :15;
      uint16_t RAMP_LIMIT_HIGH_31_16 :16;      // RAMP_LIMIT_HIGH_31_16=1E00h
      uint16_t RAMP_LIMIT_HIGH_15_0 :16;      // RAMP_LIMIT_HIGH_15_0=0000h
                                              // (503'316'480)
      uint16_t RAMP_LIMIT_LOW_32 :1, :15;                               // 0001h
      uint16_t RAMP_LIMIT_LOW_31_16 :16;                                // D300h
      uint16_t RAMP_LIMIT_LOW_15_0 :16;                 // 0000h (7'834'959'872)
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      // R90
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :16;
      uint16_t :2, RAMP_BURST_COUNT :13, RAMP_BURST_EN :1;
      uint16_t RAMP_BURST_TRIG :2, :1, RAMP_TRIGA :4, RAMP_TRIGB :4, :4,
          RAMP0_RST :1;                            // RAMP_TRIGA=1, RAMP_TRIGB=1
      uint16_t RAMP0_DLY :1, :1, RAMP0_INC_29_16 :14;     // RAMP0_INC_29_16=80h
      uint16_t RAMP0_INC_15_0 :16;              // RAMP0_INC_15_0=0h (8'388'608)
      // R100
      uint16_t RAMP0_LEN :16;
      uint16_t RAMP0_NEXT_TRIG :2, :2, RAMP0_NEXT :1, RAMP1_RST :1,
          RAMP1_DLY :1, :9;      // RAMP0_NEXT_TRIG=1, RAMP0_NEXT=1
      uint16_t RAMP1_INC_29_16 :14, :2;      // RAMP1_INC_29_16=3F80h
      uint16_t RAMP1_INC_15_0 :16;      // RAMP1_INC_15_0=0000h (1'065'353'216)
      uint16_t RAMP1_LEN :16;
      uint16_t RAMP1_NEXT_TRIG :2, :2, RAMP1_NEXT :1, RAMP_MANUAL :1,
          RAMP_DLY_CNT :10;      // RAMP1_NEXT_TRIG=1
      uint16_t RAMP_SCALE_COUNT :3, :1, RAMP_TRIG_CAL :1, :11;
      /*
       uint16_t :16;
       uint16_t :16;
       uint16_t :16;
       // R110

       uint16_t :5, rb_VCO_SEL :3, :1, rb_LD_VTUNE :2, :5;
       uint16_t rb_VCO_CAPCTRL :8, :8;
       uint16_t rb_VCO_DACISET :9, :7; */
    } __attribute__((packed)) Bits;
  };

  // массив регистров
  REGS pll_reg;
  // храним последнюю частоту сравнения
  uint32_t fpd_last;

  LMX2594() {
    InitRegs();
  }

  void InitRegs();

  uint8_t WriteRegsCount();

  uint16_t* get_pll_reg_addr(uint32_t num = 0) {
    if (num < RegsCount)
      return &pll_reg.Arr[num];
    else
      return nullptr;
  }

  uint8_t* get_nwr_addr() {
    return Num_Write_regs;
  }

  FCAL_HPFD_ADJ_type set_fcal_hfpd(uint32_t fpd);

  FCAL_LPFD_ADJ_type set_fcal_lfpd(uint32_t fpd);

  CAL_CLK_DIV_type set_cal_clkdiv(uint32_t fosc);

  uint64_t fvco_max(uint32_t chdiv);

  uint32_t N_min(MASH_ORDER_type frac, uint64_t fvco);

  uint8_t pfd_dly_sel(MASH_ORDER_type frac, uint64_t fvco);

  uint32_t fpd_min(MASH_ORDER_type frac);

  uint32_t fpd_max(MASH_ORDER_type frac);

  uint32_t fosc_max(bool osc_2x);

  bool find_chdiv(uint64_t fout, uint32_t fpd, MASH_ORDER_type frac,
                  uint32_t *chdiv, uint32_t *chdiv_i);

  void setPartialAssist(int32_t ufvco);

  uint64_t get_fout_min() {
    return fout_min;
  }

  bool Set_F(uint64_t fout, uint32_t fosc);

  bool Set_PWR(uint8_t pwr);

  bool getChdiv_i(uint32_t chdiv, uint32_t *chdiv_i);

  uint8_t getNumRegs() {
    return RegsFullCount;
  }

  void SetCPG(uint8_t cpg);
  void stmOff();
  void WriteRegs();
  void MUXOUT(uint8_t LD_SEL);
  uint32_t WriteOneReg(uint8_t Num, uint16_t Reg);
  uint32_t ReadOneReg(uint8_t Num, uint16_t *Reg);
  uint32_t WriteItReg(uint8_t);
};
