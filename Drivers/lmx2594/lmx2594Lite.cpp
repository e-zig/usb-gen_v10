#include "lmx2594Lite.hpp"
#include "stm32f0xx_hal.h"
#include "pllsrv.h"

//==================================================================================
// Инициализация регистров LMX2594
//==================================================================================
void LMX2594::InitRegs() {

  pll_reg.Bits.POWERDOWN = 0;
  pll_reg.Bits.RESET = 0;
  pll_reg.Bits.MUXOUT_LD_SEL = 1;  // всегда 1 !!!
  pll_reg.Bits.FCAL_EN = 1;
  pll_reg.Bits.FCAL_LPFD_ADJ = up_10;
  pll_reg.Bits.FCAL_HPFD_ADJ = low_100;
  pll_reg.Bits.OUT_MUTE = 1;
  pll_reg.Bits.VCO_PHASE_SYNC_EN = 0;
  pll_reg.Bits.RAMP_EN = 0;

  pll_reg.Bits.CAL_CLK_DIV = low_200;  //

  pll_reg.Bits.STM_ON = 1;

  pll_reg.Bits.OUT_FORCE = ~pll_reg.Bits.OUT_MUTE;

  pll_reg.Bits.VCO_CAPCTRL_FORCE = 0;
  pll_reg.Bits.VCO_DACISET_FORCE = 0;

  pll_reg.Bits.OSC_2X = 0;

  pll_reg.Bits.MULT = bypass;
  pll_reg.Bits.PLL_R = 1;  // !!!
  pll_reg.Bits.PLL_R_PRE = 1;

  pll_reg.Bits.CPG = ma_15;

  pll_reg.Bits.VCO_DACISET = 128;
  pll_reg.Bits.VCO_DACISET_STRT = 300;

  pll_reg.Bits.VCO_CAPCTRL = 183;

  pll_reg.Bits.VCO_SEL_FORCE = 0;

  pll_reg.Bits.VCO_SEL = vco_4;

  pll_reg.Bits.CHDIV_DIV2 = 1;

  pll_reg.Bits.PLL_N_18_16 = 0;
  pll_reg.Bits.PLL_N_15_0 = 100;
  pll_reg.Bits.PFD_DLY_SEL = 3;
  pll_reg.Bits.MASH_SEED_EN = 0;

  pll_reg.Bits.PLL_DEN_31_16 = 0;
  pll_reg.Bits.PLL_DEN_15_0 = 4;

  pll_reg.Bits.MASH_SEED_31_16 = 0;
  pll_reg.Bits.MASH_SEED_15_0 = 0;

  pll_reg.Bits.PLL_NUM_31_16 = 0;
  pll_reg.Bits.PLL_NUM_15_0 = 1;

  pll_reg.Bits.MASH_ORDER = frac3;  // не ставить integer !
  pll_reg.Bits.MASH_RESET_N = 1;
  pll_reg.Bits.OUTA_PD = 0;
  pll_reg.Bits.OUTB_PD = 1;

  pll_reg.Bits.OUTA_PWR = PWR_Max;
  pll_reg.Bits.OUTB_PWR = 0;
  pll_reg.Bits.OUT_ISET = noboost;
  pll_reg.Bits.OUTA_MUX = vcoa;
  pll_reg.Bits.OUTB_MUX = vcob;

  pll_reg.Bits.INPIN_FMT = inpin_cmos_cmos;
  pll_reg.Bits.INPIN_LVL = inpin_vin4;
  pll_reg.Bits.INPIN_HYST = 0;
  pll_reg.Bits.INPIN_IGNORE = 1;
  pll_reg.Bits.LD_TYPE = 1;
  pll_reg.Bits.LD_DLY = 0;

  pll_reg.Bits.MASH_RST_COUNT_31_16 = 0;
  pll_reg.Bits.MASH_RST_COUNT_15_0 = 1000;  // было 50000

  pll_reg.Bits.SYSREF_REPEAT = 0;
  pll_reg.Bits.SYSREF_EN = 0;
  pll_reg.Bits.SYSREF_PULSE = 0;
  pll_reg.Bits.SYSREF_DIV_PRE = divpre_4;
  pll_reg.Bits.SYSREF_DIV = 1;

  pll_reg.Bits.JESD_DAC1_CTRL = 63;
  pll_reg.Bits.JESD_DAC2_CTRL = 0;
  pll_reg.Bits.JESD_DAC3_CTRL = 0;
  pll_reg.Bits.JESD_DAC4_CTRL = 0;

  pll_reg.Bits.SYSREF_PULSE_CNT = 0;

  pll_reg.Bits.CHDIV = div_4;

  pll_reg.Bits.VCO_CAPCTRL_STRT = 1;

  pll_reg.Bits.QUICK_RECAL_EN = 0;

  // если рампинг отключен - выходим
  if (pll_reg.Bits.RAMP_EN == 0)
    return;

  pll_reg.Bits.RAMP_THRESH_32 = 0;
  pll_reg.Bits.RAMP_THRESH_31_16 = 0;
  pll_reg.Bits.RAMP_THRESH_15_0 = 0;

  pll_reg.Bits.RAMP_LIMIT_HIGH_32 = 0;
  pll_reg.Bits.RAMP_LIMIT_HIGH_31_16 = 0;
  pll_reg.Bits.RAMP_LIMIT_HIGH_15_0 = 0;

  pll_reg.Bits.RAMP_LIMIT_LOW_32 = 0;
  pll_reg.Bits.RAMP_LIMIT_LOW_31_16 = 0;
  pll_reg.Bits.RAMP_LIMIT_LOW_15_0 = 0;

  pll_reg.Bits.RAMP_BURST_COUNT = 0;
  pll_reg.Bits.RAMP_BURST_EN = 0;
  pll_reg.Bits.RAMP_BURST_TRIG = ramp;
  pll_reg.Bits.RAMP_TRIGA = disabled;
  pll_reg.Bits.RAMP_TRIGB = disabled;

  pll_reg.Bits.RAMP0_RST = 0;
  pll_reg.Bits.RAMP0_DLY = 0;
  pll_reg.Bits.RAMP0_INC_29_16 = 0;
  pll_reg.Bits.RAMP0_INC_15_0 = 0;
  pll_reg.Bits.RAMP0_LEN = 0;
  pll_reg.Bits.RAMP0_NEXT_TRIG = ramp;
  pll_reg.Bits.RAMP0_NEXT = 0;

  pll_reg.Bits.RAMP1_RST = 0;
  pll_reg.Bits.RAMP1_DLY = 0;
  pll_reg.Bits.RAMP1_INC_29_16 = 0;
  pll_reg.Bits.RAMP1_INC_15_0 = 0;
  pll_reg.Bits.RAMP1_LEN = 0;
  pll_reg.Bits.RAMP1_NEXT_TRIG = ramp;
  pll_reg.Bits.RAMP1_NEXT = 0;

  pll_reg.Bits.RAMP_MANUAL = 0;
  pll_reg.Bits.RAMP_DLY_CNT = 0;
  pll_reg.Bits.RAMP_SCALE_COUNT = 7;
  pll_reg.Bits.RAMP_TRIG_CAL = 0;
}

//=============================================================================
// Расчет максимального значения частоты VCO PLL
// chdiv - значение делителя CHDIV
//=============================================================================
uint64_t LMX2594::fvco_max(uint32_t chdiv) {
  if (chdiv >= 8)
    return 11500000000;
  else
    return 15000000000;
}

//=============================================================================
// Алгоритм поиска дроби NUM/DEN для десятичного дробного числа alfa
//=============================================================================
void LMX2594::find_num_den(double alfa, uint32_t &num, uint32_t &den,
                           uint32_t &nint) {
  nint = uint32_t(alfa);

  alfa -= nint;  // оставляем дробную часть

  double tVal = 0.5 / DEN_MAX;

  if ((tVal <= alfa) && (alfa < 1.0 / DEN_MAX)) {
    den = DEN_MAX;
    num = 1;
    return;
  }

  if (alfa < tVal) {
    num = 0;
    den = DEN_MAX;
    return;
  }

  if (alfa > (1.0 - tVal)) {
    den = DEN_MAX;
    num = 0;
    ++nint;  // увеличиваем nint
    return;
  }

  if (((1.0 - 1.0 / DEN_MAX) < alfa) && (alfa <= (1 - tVal))) {
    den = DEN_MAX;
    num = DEN_MAX - 1;
    return;
  }

  uint64_t num0 = 0;
  uint64_t den0 = 1;
  uint64_t num1 = 1;
  uint64_t den1 = uint64_t(1.0 / alfa);
  double eps = 1 / alfa - den1;

  uint64_t a;
  uint64_t num2;
  uint64_t den2;

  // порог выхода из алгоритма должен быть меньше чем задаваемая точность alfa
  // например если alfa с точностью 1e-6 то zero должен быть 1e-7
  // минимальное значение равно 1/den_max = 2.33e-10, ставим zero = 1e-11
  static double zero = 1e-11;  // непонятно что при изменении den_max ??

  while (eps > zero) {
    a = uint64_t(1 / eps);
    den2 = den1 * a + den0;
    if (den2 >= DEN_MAX)
      break;
    num2 = num1 * a + num0;
    eps = 1 / eps - a;
    num0 = num1;
    den0 = den1;
    num1 = num2;
    den1 = den2;
  }

  num = uint32_t(num1);
  den = uint32_t(den1);
  return;
}

//=============================================================================
// Расчет минимального значения целочисленного делителя PLL
// frac - режим FRAC_ORDER PLL
// fvco - частота VCO PLL в Гц
//=============================================================================
uint32_t LMX2594::N_min(MASH_ORDER_type frac, uint64_t fvco) {
  switch (frac) {
    case integer:
      if (fvco > 12500000000)
        return 32;
      else
        return 28;
    case frac1:
      if (fvco <= 10000000000)
        return 28;
      else if (fvco <= 12500000000)
        return 32;
      else
        return 36;
    case frac2:
      if (fvco > 10000000000)
        return 36;
      else
        return 32;
    case frac3:
      if (fvco > 10000000000)
        return 40;
      else
        return 36;
    case frac4:
      if (fvco > 10000000000)
        return 48;
      else
        return 44;
    default:
      return 0;
  }
}

//=============================================================================
// Расчет значения PFD_DLY_SEL PLL
// frac - режим FRAC_ORDER PLL
// fvco - частота VCO PLL в Гц
//=============================================================================
uint8_t LMX2594::pfd_dly_sel(MASH_ORDER_type frac, uint64_t fvco) {
  switch (frac) {
    case integer:
      if (fvco > 12500000000)
        return 2;
      else
        return 1;
    case frac1:
      if (fvco <= 10000000000)
        return 1;
      else if (fvco <= 12500000000)
        return 2;
      else
        return 3;
    case frac2:
      if (fvco > 10000000000)
        return 3;
      else
        return 2;
    case frac3:
      if (fvco > 10000000000)
        return 4;
      else
        return 3;
    case frac4:
      if (fvco > 10000000000)
        return 6;
      else
        return 5;
    default:
      return 0;
  }
}

//=============================================================================
// Расчет минимального значения частоты сравнения PLL
// frac - режим FRAC_ORDER PLL
//=============================================================================
uint32_t LMX2594::fpd_min(MASH_ORDER_type frac) {
  if (frac == integer)
    return 125000;
  else
    return 5000000;
}

//=============================================================================
// Расчет максимального значения частоты сравнения PLL
// frac - режим FRAC_ORDER PLL
//=============================================================================
uint32_t LMX2594::fpd_max(MASH_ORDER_type frac) {
  if (frac == integer)
    return 400000000;
  else if (frac == frac4)
    return 240000000;
  else
    return 300000000;
}

//=============================================================================
// Расчет максимального значения частоты VCO PLL
// osc_2x - использование умножителя OSC_2X PLL
//=============================================================================
uint32_t LMX2594::fosc_max(bool osc_2x) {
  if (osc_2x)
    return 200000000;
  else
    return 1400000000;
}

//=============================================================================
// Установка параметра FCAL_HPFD_ADJ PLL
// fpd - частота сравнения PLL в Гц
// PLLsettings - ссылка на параметры PLL
//=============================================================================
LMX2594::FCAL_HPFD_ADJ_type LMX2594::set_fcal_hfpd(uint32_t fpd) {
  if (fpd <= 100000000)
    return low_100;
  else if (fpd <= 150000000)
    return in_100_150;
  else if (fpd <= 200000000)
    return in_150_200;
  else
    return up_200;
}

//=============================================================================
// Установка параметра FCAL_LPFD_ADJ PLL
// fpd - частота сравнения PLL
// PLLsettings - ссылка на параметры PLL
//=============================================================================
LMX2594::FCAL_LPFD_ADJ_type LMX2594::set_fcal_lfpd(uint32_t fpd) {
  if (fpd >= 10000000)
    return up_10;
  else if (fpd >= 5000000)
    return in_5_10;
  else if (fpd >= 2500000)
    return in_2_5_5;
  else
    return low_2_5;
}

//=============================================================================
// Установка параметра CAL_CLK_DIV PLL
// fosc - опорная частота PLL
// CAL_CLK_DIV - ссылка на параметр
//=============================================================================
LMX2594::CAL_CLK_DIV_type LMX2594::set_cal_clkdiv(uint32_t fosc) {
  if (fosc <= 200000000)
    return low_200;
  else if (fosc <= 800000000)
    return in_400_800;
  else
    return up_800;
}

//=============================================================================
// Поиск выходного делителя CHDIV PLL для функции set_f_frac
// Входные данные:
//   fout - требуемая выходная частота PLL
//   fpd - частота сравнения PLL
//   frac - режим FRAC_ORDER PLL
//   chdiv - ссылка для возврата искомого значения CHDIV
//   chdiv_i - ссылка для возврата настройки параметра CHDIV
//=============================================================================
bool LMX2594::find_chdiv(uint64_t fout, uint32_t fpd, MASH_ORDER_type frac,
                         uint32_t *chdiv, uint32_t *chdiv_i) {
  bool chdiv_find = false;

  // случай, когда выходной делитель CHDIV не используется (сигнал берется
  // напрямую c VCO)
  if ((fout >= fvco_min) && ((fout / fpd) >= N_min(frac, fout))) {
    *chdiv = 1;
    *chdiv_i = CHDIV_arr[0];
    chdiv_find = true;
  } else
    for (uint32_t ii = 0; ii <= CHDIV_arr_i_max; ii++) {
      *chdiv = CHDIV_val[ii];
      // если частота VCO ниже допустимой - пропускаем
      if ((fout * *chdiv) < fvco_min)
        continue;
      // если частота VCO выше допустимой - заканчиваем, не нашли
      if ((fout * *chdiv) > fvco_max(*chdiv))
        break;
      // если условие по N выполняется то делитель найден
      if ((fout * *chdiv / fpd) >= N_min(frac, fout * *chdiv)) {
        *chdiv = CHDIV_val[ii];
        *chdiv_i = CHDIV_arr[ii];
        chdiv_find = true;
        break;
      }
    }

  return chdiv_find;
}

//=============================================================================
// Возвращает индекс делителя chdiv_i по зданному делению
// результат false если данного делителя нет в массиве
//=============================================================================
bool LMX2594::getChdiv_i(uint32_t chdiv, uint32_t *chdiv_i) {
  bool res = false;
  for (uint32_t ii = 0; ii <= CHDIV_arr_i_max; ii++) {
    if (chdiv == CHDIV_val[ii]) {
      *chdiv_i = CHDIV_arr[ii];
      res = true;
      break;
    }
  }
  return res;
}

//=============================================================================
// Установка параметров в режиме Partial Assist
// ufvco - частота в МГц
//=============================================================================
void LMX2594::setPartialAssist(int32_t ufvco) {
  int32_t Cmin = 0;
  int32_t Cmin_max = 0;
  int32_t FCmin = 0;
  int32_t FCmax_min = 1;
  int32_t Amin = 0;
  int32_t Amax_min = 0;
  bool in_11900_12100 = false;

  if (ufvco < 8600) {
    pll_reg.Bits.VCO_SEL = 1;
    Cmin = 164;
    Cmin_max = 152;
    Amin = 299;
    Amax_min = -59;
    FCmin = 7500;
    FCmax_min = 1100;
  } else if (ufvco < 9800) {
    pll_reg.Bits.VCO_SEL = 2;
    Cmin = 165;
    Cmin_max = 149;
    Amin = 356;
    Amax_min = -109;
    FCmin = 8600;
    FCmax_min = 1200;
  } else if (ufvco < 10800) {
    pll_reg.Bits.VCO_SEL = 3;
    Cmin = 158;
    Cmin_max = 139;
    Amin = 324;
    Amax_min = -100;
    FCmin = 9800;
    FCmax_min = 1000;
  } else if (ufvco < 11900) {
    pll_reg.Bits.VCO_SEL = 4;
    Cmin = 140;
    Cmin_max = 140;
    Amin = 383;
    Amax_min = -139;
    FCmin = 10800;
    FCmax_min = 1200;
  } else if (ufvco < 12100) {
    pll_reg.Bits.VCO_SEL = 4;
    in_11900_12100 = true;
  } else if (ufvco < 12900) {
    pll_reg.Bits.VCO_SEL = 5;
    Cmin = 183;
    Cmin_max = 147;
    Amin = 205;
    Amax_min = -59;
    FCmin = 12000;
    FCmax_min = 900;
  } else if (ufvco < 13900) {
    pll_reg.Bits.VCO_SEL = 6;
    Cmin = 155;
    Cmin_max = 149;
    Amin = 242;
    Amax_min = -79;
    FCmin = 12900;
    FCmax_min = 1000;
  } else {
    pll_reg.Bits.VCO_SEL = 7;
    Cmin = 175;
    Cmin_max = 156;
    Amin = 323;
    Amax_min = -79;
    FCmin = 13900;
    FCmax_min = 1100;
  }

  if (in_11900_12100) {
    pll_reg.Bits.VCO_CAPCTRL_STRT = 1;
    pll_reg.Bits.VCO_DACISET_STRT = 300;
  } else {
    int32_t tmp = (ufvco - FCmin) / FCmax_min;
    pll_reg.Bits.VCO_CAPCTRL_STRT = uint16_t(Cmin - Cmin_max * tmp);
    pll_reg.Bits.VCO_DACISET_STRT = uint16_t(Amin + Amax_min * tmp);
  }

  pll_reg.Bits.VCO_CAPCTRL_FORCE = 0;
  pll_reg.Bits.VCO_DACISET_FORCE = 0;
  pll_reg.Bits.VCO_SEL_FORCE = 0;
  pll_reg.Bits.FCAL_EN = 1;
}

//=============================================================================
// функция выводит сообщения в экранный лог
//=============================================================================
extern void write_log_string(const char *str);

#define WRITE_LOG(str) \
  { /* write_log_string(str); */ }

//=============================================================================
// Функция установки выходной частоты синтезатора LMX2594
// Выходная частота задается через fout, Гц
// Опорная частота задается через fosc, Гц
//=============================================================================
bool LMX2594::Set_F(uint64_t fout, uint32_t fosc) {
  double fpd;                   // частота сравнения
  uint64_t fvco;                // частота VCO
  uint32_t chdiv, chdiv_i = 0;  // выходной делитель и его индекс
  uint32_t n, num;  // делитель и нумератор обратной связи
  uint32_t den = DEN_MAX;  // деноминатор по умолчанию
  double nnd;  // дробный делитель обратной связи
  double Kfosc = 1.0;

  //проверяем допустимые границы заданной частоты
  if ((fout > fout_max) || (fout < fout_min)) {
    WRITE_LOG("Set_F_2594: Недопустимое значение частоты fout")
    return false;
  }

  //проверяем допустимые границы опорной частоты
  if ((fosc < fosc_min) || (fosc > fosc_max(pll_reg.Bits.OSC_2X))) {
    WRITE_LOG("Set_F_2594: Недопустимое значение частоты fosc")
    return false;
  }

  // исходный режим работы
  pll_reg.Bits.MASH_ORDER = Init_MASH_ORDER;

  // коэффициент преобразования опорной частоты
  Kfosc = pll_reg.Bits.MULT * (uint32_t(pll_reg.Bits.OSC_2X) + 1)
      / double(pll_reg.Bits.PLL_R) / double(pll_reg.Bits.PLL_R_PRE);

  // частота сравнения
  fpd = fosc * Kfosc;

  //проверяем допустимые границы заданной частоты сравнения
  if ((uint32_t(fpd) > fpd_max(MASH_ORDER_type(pll_reg.Bits.MASH_ORDER)))
      || (uint32_t(fpd) < fpd_min(MASH_ORDER_type(pll_reg.Bits.MASH_ORDER)))) {
    WRITE_LOG("Set_F_2594: Недопустимое значение частоты fpd")
    return false;
  }

  if (!find_chdiv(fout, uint32_t(fpd), MASH_ORDER_type(pll_reg.Bits.MASH_ORDER),
                  &chdiv, &chdiv_i)) {
    WRITE_LOG("Set_F_2594: Ошибка при поиске делителя chdiv")
    return false;
  }

  uint32_t incDiv { 1 };

  fvco = fout * chdiv;

// проверяем допустимые границы Fvco
  if ((fvco < fvco_min) || (fvco > fvco_max(chdiv))) {
    WRITE_LOG("Set_F_2594: Недопустимое значение частоты fvco")
    return false;
  }

  nnd = (double(fvco) / fpd) / incDiv;

  // новый вариант
  find_num_den(nnd, num, den, n);

  // проверяем N
  if (n < N_min(MASH_ORDER_type(pll_reg.Bits.MASH_ORDER), fvco)) {
    WRITE_LOG("Set_F_2594: Недопустимое значение N")
    return false;
  }

  // если num вернулся 0 и нет режима forUsync - ставим integer mode
  if (num == 0) {
    pll_reg.Bits.MASH_ORDER = integer;
    WRITE_LOG("Set_F_2594: Установлен integer mode")
  }

  // максимизируем значения
  num *= DEN_MAX / den;
  den *= DEN_MAX / den;

  // фактическая выходная частота
  fout = uint64_t(0.5 + fpd * (n + double(num) / den) / double(chdiv));

// ==== финальная установка параметров ====

  if (pll_reg.Bits.MASH_ORDER == integer)
    pll_reg.Bits.MASH_RESET_N = 0;
  else
    pll_reg.Bits.MASH_RESET_N = 1;

  pll_reg.Bits.FCAL_HPFD_ADJ = set_fcal_hfpd(uint32_t(fpd));
  pll_reg.Bits.FCAL_LPFD_ADJ = set_fcal_lfpd(uint32_t(fpd));

  pll_reg.Bits.PFD_DLY_SEL = pfd_dly_sel(
      MASH_ORDER_type(pll_reg.Bits.MASH_ORDER), fvco);

// так быстрее будет проходить калибровка
// потом переставим на 6ma - так меньше помех
  pll_reg.Bits.CPG = ma_15;

  pll_reg.Bits.CHDIV = CHDIV_type(chdiv_i);

  if (chdiv < 3)
    pll_reg.Bits.CHDIV_DIV2 = 0;
  else
    pll_reg.Bits.CHDIV_DIV2 = 1;

  if (chdiv == 1)
    pll_reg.Bits.OUTA_MUX = vcoa;
  else
    pll_reg.Bits.OUTA_MUX = chdiva;

  pll_reg.Bits.PLL_N_18_16 = n >> 16;
  pll_reg.Bits.PLL_N_15_0 = uint16_t(n);
  pll_reg.Bits.PLL_NUM_31_16 = num >> 16;
  pll_reg.Bits.PLL_NUM_15_0 = uint16_t(num);
  pll_reg.Bits.PLL_DEN_31_16 = den >> 16;
  pll_reg.Bits.PLL_DEN_15_0 = uint16_t(den);

  setPartialAssist(int32_t(fvco / 1000000));

// включаем клокирование STM, выключаем в After_Set_F()
  pll_reg.Bits.STM_ON = 1;

  fpd_last = fpd;

  return true;
}

/* ==================================================================================================
 * Установка одного регистра
 *  Num - номер регистра
 *  Reg - значение регистра
 * Возвращаемое значение см. PSspi::Write
 * ==================================================================================================
 */
uint32_t LMX2594::WriteOneReg(uint8_t Num, uint16_t Reg) {
  uint8_t val[3] { };

  val[0] = Num;
  val[1] = Reg >> 8;
  val[2] = Reg;

  return SPI_Write(val, 3);
}

//extern HAL_StatusTypeDef SPI_Write(uint8_t *pData, uint16_t Size);

/* ==================================================================================================
 * Установка одного регистра pll_reg из уже сформированных
 *  Num - номер регистра
 * Возвращаемое значение см. PSspi::Write
 * ==================================================================================================
 */
uint32_t LMX2594::WriteItReg(uint8_t Num) {
  uint8_t val[3] { };

  val[0] = Num;
  val[1] = pll_reg.Arr[Num] >> 8;
  val[2] = pll_reg.Arr[Num];

  return SPI_Write(val, 3);
}

/************************************************************************
 *
 * Возвращает число регистров на запись и условие записи всех регистров
 *
 ************************************************************************/
uint8_t LMX2594::WriteRegsCount() {
  if (first_init) {
    if (pll_reg.Bits.RAMP_EN)
      return RegsCount;
    else
      return RegsCount_NoRamp;
  }

  return write_regs_count;
}

/* ===========================================================================
 * Установка мощности канала А
 ========================================================================== */
bool LMX2594::Set_PWR(uint8_t pwr) {
  if (pwr > PWR_Off)
    return false;

  if (pwr == PWR_Off) {
    pll_reg.Bits.OUTA_PD = 1;
    pll_reg.Bits.OUTA_PWR = 0;
  } else {
    pll_reg.Bits.OUTA_PD = 0;
    if (pwr > PWR_Max)
      pwr = PWR_Max;
    pll_reg.Bits.OUTA_PWR = pwr;
  }

  return WriteItReg(44) == HAL_OK;
}

/************************************************************************
 * Выполняет установку тока CPG
 ************************************************************************/
void LMX2594::SetCPG(uint8_t cpg) {
  if (cpg == CPG_type::ma_auto) {
    if (fpd_last <= 20000000)
      pll_reg.Bits.CPG = CPG_type::ma_15;
    else if (fpd_last <= 80000000)
      pll_reg.Bits.CPG = CPG_type::ma_9;
    else if (fpd_last <= 150000000)
      pll_reg.Bits.CPG = CPG_type::ma_6;
    else
      pll_reg.Bits.CPG = CPG_type::ma_3;
  } else
    pll_reg.Bits.CPG = CPG_type(cpg);
  WriteItReg(14);
}

/************************************************************************
 * Отключает стейт машину
 ************************************************************************/
void LMX2594::stmOff() {
  pll_reg.Bits.STM_ON = 0;
  WriteItReg(2);
}

/* ==================================================================================================
 * Установка регистров
 *  Regs[] - массив регистров размером NRegs_LMX2594
 * Возвращаемое значение см. PSspi::Write
 * ==================================================================================================
 */
void LMX2594::WriteRegs() {
  uint8_t LocRegsCount = WriteRegsCount();

// если пишем все регистры
  if (LocRegsCount > write_regs_count) {
    // считаем что это первая запись регистров, поэтому
    // сначала делаем программный RESET LMX
    pll_reg.Bits.RESET = 1;
    pll_reg.Bits.FCAL_EN = 0;    // TODO поставил 0, была 1
    WriteItReg(0);
    pll_reg.Bits.RESET = 0;
    WriteItReg(0);
    HAL_Delay(1);

    for (int i = LocRegsCount - 1; i >= 0; i--)
      WriteItReg(i);

    HAL_Delay(10);
    pll_reg.Bits.FCAL_EN = 1;
    WriteItReg(0);

  } else {  // если пишем регистры из массива Num_Write_regs
    for (uint8_t i = 0; i < LocRegsCount; i++)  // пишем указанные регистры
      WriteItReg(Num_Write_regs[i]);
  }
}

void LMX2594::MUXOUT(uint8_t LD_SEL) {
  pll_reg.Bits.RESET = 0;
  pll_reg.Bits.FCAL_EN = 0;
  pll_reg.Bits.MUXOUT_LD_SEL = LD_SEL;
  WriteItReg(0);
}

/* ===========================================================================
 * Чтение одного регистра
 *  Num - номер регистра
 *  Reg - значение регистра - два байта
 * Возвращаемое значение см. PSSpi::WriteRead
 * ===========================================================================
 */
uint32_t LMX2594::ReadOneReg(uint8_t Num, uint16_t *Reg) {
  uint8_t val[3] { };
  uint32_t res;
  MUXOUT(0);
  val[0] = Num | 0x80;
  val[1] = 0;
  val[2] = 0;
  // res = HAL_SPI_TransmitReceive(&hspi1, val, val, 3, 2);
  res = SPI_WriteRead(val, val, 3);
  *Reg = val[1] << 8 | val[2];
  MUXOUT(1);
  return res;
}

