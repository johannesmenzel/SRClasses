#include "SRGain.h"
//#include this and that
//#include <math.h>

namespace SR
{
  namespace DSP
  {

    // Constructors & Destructor
    SRGain::SRGain(
      int pRampNumSamples,
      PanType pType,
      double pPanNormalized,
      bool pLinearMiddlePosition,
      double pWidthNormalized,
      bool pBypassed
    )
      : mGainLin(1.0)
      , mRampNumSamples(1)
      , mBypassed(false)
      , mPanType(kLinear)
      , mPanNormalized(0.5)
      , mLinearMiddlePosition(true)
      , mWidthNormalized(1.0)
      , mMidCoeff(0.5)
      , mSideCoeff(0.5)
    {
      InitGain(pRampNumSamples, pType, pPanNormalized, pLinearMiddlePosition, pWidthNormalized, pBypassed);
    }

    SRGain::~SRGain() {}


    void SRGain::InitGain(int pRampNumSamples, PanType pType, double pPanNormalized, bool pLinearMiddlePosition, double pWidthNormalized, bool pBypassed) {
      mGainLin = 1.0;
      mRampNumSamples = (pRampNumSamples < 1) ? 1 : pRampNumSamples;
      mBypassed = pBypassed;
      mPanType = pType;
      mPanNormalized = pPanNormalized;
      mWidthNormalized = pWidthNormalized;
      mLinearMiddlePosition = pLinearMiddlePosition;
      mGainEnv1.SetStrict(mGainLin);
      mGainEnv2.SetStrict(mGainLin);
      mGainEnv1.SetNumSmoothSamples(mRampNumSamples);
      mGainEnv2.SetNumSmoothSamples(mRampNumSamples);
      Update();
    }

    void SRGain::Update(void) {
      double gain1 = mGainLin;
      double gain2 = mGainLin;

      // Pan position calculation
      if (mPanNormalized != 0.5) {

        switch (mPanType) {
        case kLinear:
          if (mLinearMiddlePosition == true) {
            gain1 = (mPanNormalized > 0.5) ? (1. - mPanNormalized) * 2 : 1.0;
            gain2 = (mPanNormalized < 0.5) ? mPanNormalized * 2 : 1.0;
          }
          else {
            gain1 = (1. - mPanNormalized);
            gain2 = (mPanNormalized);
          }
          break;
        case kSquareroot:
          if (mLinearMiddlePosition == true) {
            gain1 = (mPanNormalized > 0.5) ? sqrt(mPanNormalized) * sqrt(2.) : 1.0;
            gain2 = (mPanNormalized < 0.5) ? sqrt(1 - mPanNormalized) * sqrt(2.) : 1.0;
          }
          else {
            gain1 = sqrt(1. - mPanNormalized);
            gain2 = sqrt(mPanNormalized);
          }
          break;
        case kSinusodial:
          if (mLinearMiddlePosition == true) {
            gain1 = (mPanNormalized > 0.5) ? sin(2. * mPanNormalized * M_PI_2) : 1.0;
            gain2 = (mPanNormalized < 0.5) ? cos((1. - 2. * mPanNormalized) * M_PI_2) : 1.0;
          }
          else {
            gain1 = sin(mPanNormalized * M_PI_2);
            gain2 = cos(mPanNormalized * M_PI_2);
          }
          break;
        case kTanh:
          gain1 = (mPanNormalized > 0.5) ? tanh(4. * (1 - mPanNormalized)) / tanh(2.) : 1.0; // you can scale it by replacing 4. and 2. by 2n and n (10. and 5.)
          gain2 = (mPanNormalized < 0.5) ? tanh(4. * mPanNormalized) / tanh(2.) : 1.0;
          break;
        default:
          break;
        }
      }

      mGainEnv1.Set(gain1);
      mGainEnv2.Set(gain2);
    }


    } // namespace SRGain
} // namespace SR