#pragma once
#include "IPlug_include_in_plug_hdr.h"

// PLUG ELEMENTS
// -------------

//inline double SetShapeCentered(double cMinValue, double cMaxValue, double cCenteredValue, double cControlPosition) {
//  return log((cCenteredValue - cMinValue) / (cMaxValue - cMinValue)) / log(cControlPosition);
//}
//
//void InitDoubleShapeFromMiddlePosition(IPluginBase* plug, int paramIdx, const char* name, double defaultVal = 0., double minVal = 0., double maxVal = 1., double step = 0.001, const char* label = "", int flags = 0, const char* group = "", double centerVal = 0.5, double centerPoint = 0.5, IParam::EParamUnit unit = IParam::kUnitCustom, IParam::DisplayFunc displayFunc = nullptr) {
//  plug->GetParam(paramIdx)->InitDouble(name, defaultVal, minVal, maxVal, step, label, flags, group, new IParam::ShapePowCurve(SetShapeCentered(minVal, maxVal, centerVal, centerPoint)), unit, displayFunc);
//}


  /** custom parameter shaping from middle position */
struct ShapeFromMiddle : public IParam::Shape
{
  ShapeFromMiddle(double minVal = 0.0, double maxVal = 1.0, double centeredVal = 0.5, double pos = 0.5)
    : mShape(log((centeredVal - minVal) / (maxVal - minVal)) / log(pos))
    , mMin(minVal)
    , mMax(maxVal)
    , mCtr(centeredVal)
    , mPos(pos)
  {
  };
  Shape* Clone() const override { return new ShapeFromMiddle(mMin, mMax, mCtr, mPos); };
  IParam::EDisplayType GetDisplayType() const override
  {
    if (mShape > 2.5) return IParam::EDisplayType::kDisplayCubeRoot;
    if (mShape > 1.5) return IParam::EDisplayType::kDisplaySquareRoot;
    if (mShape < (2.0 / 5.0)) return IParam::EDisplayType::kDisplayCubed;
    if (mShape < (2.0 / 3.0)) return IParam::EDisplayType::kDisplaySquared;

    return IParam::kDisplayLinear;
  }
  double NormalizedToValue(double value, const IParam& param) const override {
    return param.GetMin() + std::pow(value, mShape) * (param.GetMax() - param.GetMin());
  };
  double ValueToNormalized(double value, const IParam& param) const override {
    return std::pow((value - param.GetMin()) / (param.GetMax() - param.GetMin()), 1.0 / mShape);
  };
  double mShape;
  double mMin, mMax, mCtr, mPos;
};

// Struct object containing possible parameters properties
struct SRParamProperties {
  const int paramIdx = -1;
  const int ctrlTag = -1;
  const char* name = ""; // Name of parameter displayed in host
  const char* label = ""; // Short name for GUI display
  const double defaultVal = 0.0; // Plugin loads in this state, return by double click
  const double minVal = 0.0; // Minimum value of parameter
  const double maxVal = 1.0; // Maximum value of parameter
  const double stepValue = 0.001; // Controls dial steps / accuracy
  const double centerVal = 0.5; // Value that void setShapeCentered() will center (knobs middle position)
  const double centerPoint = 0.5; // Value WHERE void setShapeCentered() will center centerVal (0.5 means: real middle position)
  const char* unit = ""; // Measuring unit of parameter
  const char* group = ""; // Parameter group, not supported by every host
  const IParam::EParamType Type = IParam::EParamType::kTypeNone; // Data type of parameter
  const IParam::EFlags Flags = IParam::EFlags::kFlagsNone;
  const IParam::EParamUnit Unit = IParam::EParamUnit::kUnitCustom;
  const int Knobs = 0; // Used control bitmap
  const struct Position
  {
    const int AttachToControlPanel = 0; // Panel where control should appear
    const int x = 0; // Controls horizontal position within panel
    const int y = 0; // Controls vertical position within panel
    const float w = 2.f; // Controls width (in cells)
    const float h = 2.f; // Controls height (in cells)
  } position;
  const char* labelMin = ""; // GUI display string of minimum
  const char* labelMax = ""; // GUI display string of maximum
  const char* labelCtr = ""; // GUI display string of middle position
  const char* tooltip = ""; // Tooltip on mouseover
};

// DSP ELEMENTS
// ------------

// Pass Filter Slopes in dB/oct listed here
enum EFilterSlope {
  dbo6 = 0,
  dbo12,
  dbo18,
  dbo24,
  dbo36,
  dbo48,
  dbo60,
  dbo72,
  dbo120,
  kNumOrders
};

const double halfpi = PI / 2;								// Half pi defined
const double dcoff = 1e-15;									// amount of DC offset added and subtracted respectivly at the beginning and the end of DoubleReplacing loop
const double stQ = sqrt(0.5);

// Setting the Q constants for higher order low- and highpass filter.
// [Orders 2, 3, 4, 6, 8, 10, 12, 20][Filter]
const double cascadedFilterQs[EFilterSlope::kNumOrders][10] = {
  {0.}, // dummy for order 1
  {sqrt(0.5)}, // order 2
  {1.}, // order 3
  {0.54119610, 1.3065630}, // order 4
  {0.51763809, 0.70710678, 1.9318517}, // order 6
  {0.50979558, 0.60134489, 0.89997622, 2.5629154}, // order 8
  {0.50623256, 0.56116312, 0.70710678, 1.1013446, 3.1962266}, // order 10
  {0.50431448, 0.54119610, 0.63023621, 0.82133982, 1.3065630, 3.8306488}, // order 12
  {0.50154610, 0.51420760, 0.54119610, 0.58641385, 0.65754350, 0.76988452, 0.95694043, 1.3065630, 2.1418288, 6.3727474} // order 20
};



// UI ELEMENTS
// -----------

// All possible controls listed here
enum EControlImages {
  kKnobNone = 0,
  SslBlue,
  SslGreen,
  SslRed,
  SslOrange,
  SslYellow,
  SslBlack,
  SslWhite,
  AbbeyChicken,
  Button,
  Fader,
  kNumKnobs
};

// Plugin constants
const struct {
  const IColor colorPanelBG = IColor(255, 37, 53, 69);
  const IColor colorPluginBG = IColor(255, 13, 18, 23);
  const IColor colorKnobSslBlue = IColor(255, 62, 100, 121);
  const IColor colorKnobSslRed = IColor(255, 131, 18, 18);
  const IColor colorKnobSslGreen = IColor(255, 103, 141, 52);
  const IColor colorKnobSslOrange = IColor(255, 234, 158, 19);
  const IColor colorKnobSslYellow = IColor(255, 219, 181, 30);
  const IColor colorKnobSslBlack = IColor(255, 23, 23, 23);
  const IColor colorKnobSslWhite = IColor(255, 243, 243, 243);
  const IColor colorDefaultText = COLOR_LIGHT_GRAY;
  const IVColorSpec colorSpec = {
    DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
    DEFAULT_FGCOLOR,            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
    IColor(255, 200, 200, 200), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
    IColor(255, 70, 70, 70),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
    IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
    IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
    IColor(255, 249, 206, 34),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0))
    IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
    IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
  };
  const int textSize = 14;
  const IText textKnobLabel = IText(textSize, colorDefaultText, nullptr, IText::kStyleBold, IText::kAlignCenter, IText::kVAlignBottom, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
  const IText textKnobValue = IText(textSize, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignCenter, IText::kVAlignBottom, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
  const IText textVersionString = IText(20, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignNear, IText::kVAlignMiddle, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
  const IText textPresetMenu = IText(30, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignCenter, IText::kVAlignMiddle, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
} SRLayout;

