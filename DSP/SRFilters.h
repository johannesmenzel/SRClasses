//
//  Biquad.h
//
//  Modifications on class:
//  Copyright Johannes Menzel 2018 MIT
//
//  This is an ever growing modified filter class, based on the 
//  biquad source by Nigel Redmon. See the original licence below.
//
//  Created by Nigel Redmon on 11/24/12
//  EarLevel Engineering: earlevel.com
//  Copyright 2012 Nigel Redmon
//
//  For a complete explanation of the Biquad code:
//  http://www.earlevel.com/main/2012/11/25/biquad-c-source-code/
//
//  License:
//
//  This source code is provided as is, without warranty.
//  You may copy and distribute verbatim copies of this document.
//  You may modify and use this source code to create binary code
//  for your own purposes, free or commercial.
//

#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace SR {

  namespace DSP {

    // TWO POLE FILTERS



    class SRFiltersTwoPole {
    public:
      enum FilterType {
        biquad_lowpass = 0,
        biquad_highpass,
        biquad_bandpass,
        biquad_notch,
        biquad_peak,
        biquad_peak_ncq,
        biquad_lowshelf,
        biquad_highshelf,
        iir_linkwitz_highpass,
        iir_linkwitz_lowpass
      };
      SRFiltersTwoPole();
      SRFiltersTwoPole(int type, double Fc, double Q, double peakGainDB, double samplerate);
      ~SRFiltersTwoPole();
      void setType(int type);
      void setQ(double Q);
      void setFc(double Fc);
      void setPeakGain(double peakGainDB);
      void setFilter(int type, double Fc, double Q, double peakGain, double samplerate);
      double process(double in);
      void GetFrequencyResponse(double* values, int points, double rangeDb, bool returnPhase);
      double GetFrequencyResponse(double atNormalizedFrequency, double rangeDb, bool returnPhase);

    protected:
      void calcBiquad(void);

      int type;
      double a0, a1, a2, b1, b2;
      double Fc, Q, peakGain;
      double z1, z2;
      double samplerate;
      double plotFC;
    };

    inline double SRFiltersTwoPole::process(double in) {
      double out = in * a0 + z1;
      z1 = in * a1 + z2 - b1 * out;
      z2 = in * a2 - b2 * out;
      return out;
    }


    // ONE POLE FILTERS

    class SRFiltersOnePole {
    public:
      SRFiltersOnePole();
      SRFiltersOnePole(int type, double Fc, double samplerate);
      void setFilter(int type, double Fc, double samplerate);
      void setType(int type);
      ~SRFiltersOnePole();
      void setFc(double Fc);
      void calcOnePole(void);
      double process(double in);

    protected:
      int type;
      double a0, b1, z1;
      double Fc;
      double samplerate;
      //double plotFC;
    };

    inline SRFiltersOnePole::~SRFiltersOnePole()
    {
    }

    inline double SRFiltersOnePole::process(double in) {
      //	return z1 = in * a0 + z1 * b1;
      z1 = in * a0 + z1 * b1;
      in -= z1;
      return in;
    }


    class SRParamSmooth
    {
    public:
      SRParamSmooth(double smoothingTimeInMs, double samplerate)
      {
        Reset(smoothingTimeInMs, samplerate);
      }

      ~SRParamSmooth() {}

      void Reset(double smoothingTimeInMs, double samplerate) {
        const double twoPi = 6.283185307179586476925286766559;
        a = exp(-twoPi / (smoothingTimeInMs * 0.001 * samplerate));
        b = 1.0 - a;
        z = 0.0;
      };

      inline double process(double in) { z = (in * b) + (z * a); return z; }

    private:
      double a, b, z;
    };

  }
}
// end namespace SRFilters