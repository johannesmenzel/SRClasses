#include "SRParam.h"



SRParam::SRParam(int numParams)
  : mNumParams(numParams)
{
  for (int i = 0; i > mNumParams; i++) {
    mParams.Add(new Param);
    mParams.Get(i)->mTargetValue = 0.0;
  }
}


SRParam::~SRParam()
{
  mParams.Empty();
}

// Set double value at index
void SRParam::SetDouble(int paramIdx, double value) {
  mParams.Get(paramIdx)->mTargetValue = value;
  mParams.Get(paramIdx)->mType = EType::kDouble;
}
// Set floating value at index
void SRParam::SetFloat(int paramIdx, float value) {
  mParams.Get(paramIdx)->mTargetValue = value;
  mParams.Get(paramIdx)->mType = EType::kFloat;
}
// Set integer value at index
void SRParam::SetInt(int paramIdx, int value) {
  mParams.Get(paramIdx)->mTargetValue = value;
  mParams.Get(paramIdx)->mType = EType::kInt;
}
// Set boolean value at index
void SRParam::SetBool(int paramIdx, bool value) {
  mParams.Get(paramIdx)->mTargetValue = value;
  mParams.Get(paramIdx)->mType = EType::kBool;
}

// Set if a parameter is smoothed. Set smoothSamples = 0 to unable.
void SRParam::SetSmoothing(int paramIdx, int smoothSamples)
{
  mParams.Get(paramIdx)->mSmooth = (smoothSamples > 0) ? true : false;
  mParams.Get(paramIdx)->mNumSmoothSamples = smoothSamples;
  mParams.Get(paramIdx)->mSmoothFilter.SetCoeff(1. / (double)smoothSamples);
}

double SRParam::GetDouble(int paramIdx) { return std::get<double>(mParams.Get(paramIdx)->mValue); } // Get double value at index
float SRParam::GetFloat(int paramIdx) { return std::get<float>(mParams.Get(paramIdx)->mValue); } // Get floating value at index
int SRParam::GetInt(int paramIdx) { return std::get<int>(mParams.Get(paramIdx)->mValue); } // Get integer value at index
bool SRParam::GetBool(int paramIdx) { return std::get<bool>(mParams.Get(paramIdx)->mValue); } // Get boolean value at index

void SRParam::ProcessAll()
{
  for (int paramIdx = 0; paramIdx < mNumParams; paramIdx++) {
    Param* p = mParams.Get(paramIdx);
    if (p->mIsCurrentlySmoothing) {
      switch (mParams.Get(paramIdx)->mType)
      {
      case EType::kDouble:
        p->mValue = p->mSmoothFilter.Process(std::get<double>(p->mTargetValue));
        if (p->mValue == p->mTargetValue) p->mIsCurrentlySmoothing = false;
        break;
      case EType::kFloat:
        p->mValue = p->mSmoothFilter.Process(std::get<float>(p->mTargetValue));
        if (p->mValue == p->mTargetValue) p->mIsCurrentlySmoothing = false;
        break;
      default:
        break;
      }
    }
  }
}
