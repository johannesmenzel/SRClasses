#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include <variant>

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
  SRParam(int numParams);
  ~SRParam();

  // Getters and setters
  void SetDouble(int paramIdx, double value);
  void SetFloat(int paramIdx, float value);
  void SetInt(int paramIdx, int value);
  void SetBool(int paramIdx, bool value);
  double GetDouble(int paramIdx);
  int GetInt(int paramIdx);
  float GetFloat(int paramIdx);
  bool GetBool(int paramIdx);

  void ProcessAll();

private:
  // Holds values of each parameter
  struct Param
  {
    Values mValue;
    Values mTargetValue;
    bool mSmooth;
    int mNumSmoothSamples;
    bool mIsCurrentlySmoothing;
    int mType;
    Param()
      : mValue(0.0)
      , mTargetValue(0.0)
      , mSmooth(false)
      , mNumSmoothSamples(0)
      , mIsCurrentlySmoothing(false)
      , mType(EType::kNoninit)
    {
    }
  };
  int mNumParams;
  WDL_PtrList<Param> mParams;
};

