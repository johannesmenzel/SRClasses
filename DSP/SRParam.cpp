#include "SRParam.h"

namespace SR {
  namespace DSP {

    SRParam::SRParam(int numSmoothSamples)
      : mNumSmoothSamples(numSmoothSamples)
      , mNumParams(0)
      , mParams()
    {
    }



    SRParam::~SRParam()
    {
      for (int i = 0; i < mNumParams; i++) {
        delete(mParams.Get(i));
        mParams.Delete(i);
      }
      mParams.Empty();
    }

    void SRParam::Add(int paramIdx, int type, bool smoothing)
    {
      mParams.Add(new Param);
      mNumParams++;
      Param* thisParam = mParams.Get(paramIdx);
      thisParam->mType = type;
      SetSmoothing(paramIdx, smoothing);
      switch (thisParam->mType) {
      case EType::kDouble:
        thisParam->mValue = 0.0;
        thisParam->mTargetValue = 0.0;
        break;
      case EType::kFloat:
        thisParam->mValue = 0.f;
        thisParam->mTargetValue = 0.f;
        break;
      case EType::kInt:
        thisParam->mValue = 0;
        thisParam->mTargetValue = 0;
        break;
      case EType::kBool:
        thisParam->mValue = false;
        thisParam->mTargetValue = false;
        break;
      default:
        break;
      }


    }

    // Set double value at index
    void SRParam::SetDouble(int paramIdx, double value, bool smooth) {
      Param* paramToSet = mParams.Get(paramIdx);
      paramToSet->mTargetValue = (double)value;
      if (paramToSet->mType != EType::kDouble) paramToSet->mType = EType::kDouble;
      if (smooth) paramToSet->mSmoothSamplesLeft = mNumSmoothSamples;
      else paramToSet->mValue = (double)value;
      //paramToSet->mValue = value;

      //if (paramIdx == 0) DBGMSG("Set: i: %i; Tp: %i; v: %f; V: %f; TV: %f\n", paramIdx, paramToSet->mType, value, paramToSet->mValue, paramToSet->mTargetValue);
    }
    // Set floating value at index
    void SRParam::SetFloat(int paramIdx, float value, bool smooth) {
    Param* paramToSet = mParams.Get(paramIdx);
    paramToSet->mTargetValue = (float)value;
      if (paramToSet->mType != EType::kFloat)paramToSet->mType = EType::kFloat;
      if (smooth) paramToSet->mSmoothSamplesLeft = mNumSmoothSamples;
      else paramToSet->mValue = (float)value;
      //paramToSet->mValue = value;

    }
    // Set integer value at index
    void SRParam::SetInt(int paramIdx, int value) {
    Param* paramToSet = mParams.Get(paramIdx);
    paramToSet->mValue = value;
      if (paramToSet->mType != EType::kInt) paramToSet->mType = EType::kInt;
    }
    // Set boolean value at index
    void SRParam::SetBool(int paramIdx, bool value) {
    Param* paramToSet = mParams.Get(paramIdx);
    paramToSet->mValue = value;
      if (paramToSet->mType != EType::kBool) paramToSet->mType = EType::kBool;
    }

    double SRParam::GetDouble(int paramIdx) {
      //if (paramIdx == 0) DBGMSG("Get: i: %i; Tp: %i; V: %f; TV: %f\n", paramIdx, mParams.Get(paramIdx)->mType, std::get<double>(mParams.Get(paramIdx)->mValue), std::get<double>(mParams.Get(paramIdx)->mTargetValue));
      return std::get<double>(mParams.Get(paramIdx)->mValue);
    } // Get double value at index
    float SRParam::GetFloat(int paramIdx) { return std::get<float>(mParams.Get(paramIdx)->mValue); } // Get floating value at index
    int SRParam::GetInt(int paramIdx) { return std::get<int>(mParams.Get(paramIdx)->mValue); } // Get integer value at index
    bool SRParam::GetBool(int paramIdx) { return std::get<bool>(mParams.Get(paramIdx)->mValue); } // Get boolean value at index

    // Set if a parameter is smoothed. Set smoothSamples = 0 to unable.
    void SRParam::SetSmoothing(int paramIdx, bool smoothing)
    {
      mParams.Get(paramIdx)->mSmooth = (mNumSmoothSamples > 0) ? smoothing : false;
    }

    bool SRParam::IsCurrentlySmoothing(int paramIdx) {
      return (mParams.Get(paramIdx)->mSmoothSamplesLeft > 0) ? true : false;
    }

    void SRParam::Process(int paramIdx) {
      Param* paramToProcess = mParams.Get(paramIdx);
      if (paramToProcess->mSmoothSamplesLeft > 0) {
        switch (mParams.Get(paramIdx)->mType) {
        case EType::kDouble:
          //if (paramIdx == 7) DBGMSG("%f %f %i", std::get<double>(paramToProcess->mTargetValue), std::get<double>(paramToProcess->mValue), paramToProcess->mIsCurrentlySmoothing);
            //paramToProcess->mValue = paramToProcess->mSmoothFilter.Process(std::get<double>(paramToProcess->mTargetValue));
            paramToProcess->mValue = double(std::get<double>(paramToProcess->mValue) + (std::get<double>(paramToProcess->mTargetValue) - std::get<double>(paramToProcess->mValue)) * (1. / paramToProcess->mSmoothSamplesLeft));
            paramToProcess->mSmoothSamplesLeft--;
            if (paramToProcess->mSmoothSamplesLeft == 0) paramToProcess->mValue = paramToProcess->mTargetValue;
          break;
        case EType::kFloat:
          paramToProcess->mValue = float(std::get<float>(paramToProcess->mValue) + (std::get<float>(paramToProcess->mTargetValue) - std::get<float>(paramToProcess->mValue)) * (1.f / paramToProcess->mSmoothSamplesLeft));
          paramToProcess->mSmoothSamplesLeft--;
          if (paramToProcess->mSmoothSamplesLeft == 0) paramToProcess->mValue = paramToProcess->mTargetValue;
          break;
        case EType::kInt:
        case EType::kBool:
          if (paramToProcess->mValue != paramToProcess->mTargetValue)
            paramToProcess->mValue = paramToProcess->mTargetValue;
          break;
        default:
          if (paramToProcess->mValue != paramToProcess->mTargetValue)
            paramToProcess->mValue = paramToProcess->mTargetValue;
          break;
        }
      }
    }

    void SRParam::ProcessAll() {
      for (int paramIdx = 0; paramIdx < mNumParams; paramIdx++) {
        Process(paramIdx);
      }
    }
  }
}