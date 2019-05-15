#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "IPlugLogger.h"
#include <variant>
#include "SRFilters.h"


namespace SR {
  namespace DSP {

    /*
    Class acting as a parameter value smoother.
    Works with linear paramter changes.
    */
    class SRParamSmooth {
    public:
      // Constructors & Destructors
      SRParamSmooth()
        : mCurrentValue(1.0)
        , mTargetValue(1.0)
        , mNumSmoothSamples(0)
        , mNumSmoothSamplesLeft(0)
        , mValueChangePerSample(0.0)
      {
      }

      SRParamSmooth(double value, int numSmoothSamples)
        : mTargetValue(value)
        , mCurrentValue(value)
        , mNumSmoothSamples(numSmoothSamples)
      {
      }

      ~SRParamSmooth()
      {
      }

      // Setters
      void Set(double value) {
        if (mTargetValue != value) {
          mTargetValue = value;
          mNumSmoothSamplesLeft = mNumSmoothSamples;
          mValueChangePerSample = (mTargetValue - mCurrentValue) / mNumSmoothSamples;
        }
      }

      void SetStrict(double value) {
        mCurrentValue = mTargetValue = value;
        mNumSmoothSamplesLeft = 0;
        mValueChangePerSample = 0.0;
      }

      void SetNumSmoothSamples(int numSmoothSamples) {
        mNumSmoothSamples = numSmoothSamples;
      }

      // Getters
      double Get() { return mCurrentValue; }
      int GetNumSmoothSamples() { return mNumSmoothSamples; }
      int GetNumSmoothSamplesLeft() { return mNumSmoothSamplesLeft; }

      inline void Process() {
        if (mNumSmoothSamplesLeft > 0) {
          mCurrentValue += mValueChangePerSample;
          mNumSmoothSamplesLeft--;
          if (mNumSmoothSamplesLeft == 0) {
            mValueChangePerSample = 0.0;
            mCurrentValue = mTargetValue;
          }
        }
      }

    protected:

    private:
      double mCurrentValue, mTargetValue;
      int mNumSmoothSamples;
      int mNumSmoothSamplesLeft;
      double mValueChangePerSample;
    };

    using Values = std::variant<double, float, int, bool>;
    enum EType {
      kDouble = 0,
      kFloat,
      kInt,
      kBool,
      kNoninit,
      kNumTypes
    };
    class SRParam
    {
    public:
      SRParam(int numSmoothSamples = 0);
      ~SRParam();

      void Add(int paramIdx, int type, bool smoothing = false); // Add Parameter, use EParams loop. You have to set value and if it's smoothing after that

      void SetDouble(int paramIdx, double value, bool smooth = true); // Set double value at index
      void SetFloat(int paramIdx, float value, bool smooth = true); // Set floating value at index
      void SetInt(int paramIdx, int value); // Set integer value at index
      void SetBool(int paramIdx, bool value); // Set boolean value at index

      double GetDouble(int paramIdx); // Get double value at index
      float GetFloat(int paramIdx); // Get floating value at index
      int GetInt(int paramIdx); // Get integer value at index
      bool GetBool(int paramIdx); // Get boolean value at index

      void SetSmoothing(int paramIdx, bool smoothing); // Set if parameter changes should be smoothed
      void SetGlobalNumSmoothSamples(int samples);
      bool IsCurrentlySmoothing(int paramIdx); // Is this parameter currently in its smoothing loop?

      void Process(int paramIdx);
      void ProcessAll();

    private:
      // Holds values of each parameter
      struct Param
      {
        Values mValue;
        Values mTargetValue;
        bool mSmooth;
        int mSmoothSamplesLeft;
        int mType;
        Param()
          : mType(EType::kNoninit)
          , mValue(0.0)
          , mTargetValue(0.0)
          , mSmooth(false)
          , mSmoothSamplesLeft(0)
        {
        }
      };
      int mNumParams;
      int mNumSmoothSamples;
      WDL_PtrList<Param> mParams;
    };

  }
}