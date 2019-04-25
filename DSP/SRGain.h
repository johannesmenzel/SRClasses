//  SRGain.h

// Implementation:
//   Note that you might need a class for each channel
//   (so 2 for stereo processing.)
//
// Header:
//   private:
//     Impementation as object:
//       SRPan name;
//     Implementation as pointer:
//       SRPan *name = new SRPan();
//
// Class: Constructor, Reset()
//     Implementation as object:
//       name.setClass(pType, pVar1, pVar2, pVar3);
//     Implementation as pointer:
//       name->setClass(pType, pVar1, pVar2, pVar3);
//
// Class: ProcessDoubleReplacing()
//   Per sample and channel:
//     Implementation as object:
//       *out1 = name.process(*in1);
//     Implementation as pointer:
//       *out1 = name->process(*in1);

#pragma once

// #include this and that
#include "../Utils/SRHelpers.h" // optional
#include "../DSP/SRParam.h" // param smoother
#include <cassert>

namespace SR
{
  namespace DSP
  {


    class SRGain
    {
    public:
      enum PanType
      {
        kLinear = 0,
        kSquareroot,
        kSinusodial,
        kTanh,
        // ...
        kNumTypes
      };

      SRGain(
        int pRampNumSamples = 0,
        PanType pType = kLinear,
        double pPanNormalized = 0.5,
        bool pLinearMiddlePosition = true,
        double pWidthNormalized = 1.0,
        bool pBypassed = false
      );
      ~SRGain();

      void InitGain(
        int pRampNumSamples = 0,
        PanType pType = kLinear,
        double pPanNormalized = 0.5,
        bool pLinearMiddlePosition = true,
        double pWidthNormalized = 1.0,
        bool pBypassed = false
      );

      // Gain methods
      void SetGain(double pGainLin) { mGainLin = pGainLin; Update(); } // Set gain with linear voltage
      void SetGainDb(double pGainDb) { SetGain(SR::Utils::DBToAmp(pGainDb)); /* don't update because SetGain does */ } // Set gain with decibel value

      double GetGain() { return mGainLin; } // Get current gain value (linear voltage)
      double GetGainDb() { return SR::Utils::AmpToDB(mGainLin); } // Get current gain value (decibels)

      // Panning methods
      void SetPanType(PanType pType) { mPanType = pType; Update(); }
      void SetPanLinearMiddlePosition(bool pLinearMiddlePosition) { mLinearMiddlePosition = pLinearMiddlePosition; Update(); }
      void SetPanPosition(double pPanNormalized) { mPanNormalized = pPanNormalized; Update(); }

      PanType GetPanType() { return mPanType; }
      double GetPanPosition() { return mPanNormalized; }

      // Width methods
      void SetWidth(double pWidthNormalized) {
        mWidthNormalized = pWidthNormalized;
        double tmp = 1. / std::max(1. + mWidthNormalized, 2.);
        mMidCoeff = 1. * tmp;
        mSideCoeff = mWidthNormalized * tmp;
        Update();
      }
      double GetWidth() { return mWidthNormalized; }

      // Global methods
      void SetBypassed(bool pBypassed) { mBypassed = pBypassed; Update(); } // Set if currently bypassed
      void SetRamp(int pRampNumSamples) { mRampNumSamples = (pRampNumSamples < 1) ? 1 : pRampNumSamples; Update(); } // Set number of samples until smoother reaches target value

      bool GetBypassed() { return mBypassed; } // Get if currently bypassed
      int GetRamp() { return mRampNumSamples; } // Get number of samples until smoother reaches target value

      // Runtime method
      void Process(double &in1, double &in2); // for stereo

    protected:
      // Update gain values after any value changed
      void Update(void);

      // Gain members
      double mGainLin; // Enter normalized gain >= 0.0 (1.0 = unity)

      // Pan members
      PanType mPanType;
      double mPanNormalized; // Enter normalized pan between 0.0 (left) and 1.0 (right)
      bool mLinearMiddlePosition; // Center volume doens't change if true

      // Width members
      double mWidthNormalized; // Normalized width where 0 = mono, 1 = normal stereo, > 1 = wider
      double mMidCoeff, mSideCoeff;

      // Global members
      SRParamSmooth mGainEnv1, mGainEnv2; // Holds gain smoothing functionality
      int mRampNumSamples; // Enter lenght of gain ramp in samples
      bool mBypassed; // bool is gain bypassed
    };



    inline void SRGain::Process(double &in1, double &in2)
    {
      if (!mBypassed) {
        // Update values when smoothing
        mGainEnv1.Process();
        mGainEnv2.Process();

        // width per sample
        double mid = (in1 + in2) * mMidCoeff;
        double side = (in2 - in1) * mSideCoeff;

        // Get processed signal from mid-side
        in1 = mid - side;
        in2 = mid + side;

        // Apply final gain
        in1 *= mGainEnv1.Get();
        in2 *= mGainEnv2.Get();
      }
    }

  } // namespace SRGain
} // namespace SR