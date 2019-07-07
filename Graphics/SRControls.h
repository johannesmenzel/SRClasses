#pragma once
//#include "IPlug/IPlug_include_in_plug_hdr.h"
#include "IControl.h"
//#include "IPlugPluginBase.h"
#include "../Utils/SRHelpers.h"
// For meter, see IVMeterControl.h
#include "IPlugQueue.h"
#include "IPlugStructs.h"
#include "Controls/IVMeterControl.h"
#include "IPlugPluginBase.h"
//#include <string>

namespace SR {
  namespace Graphics {

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
      const IText textKnobLabel = IText(textSize, colorDefaultText, nullptr, EAlign::Center, EVAlign::Bottom);
      const IText textKnobValue = IText(textSize, colorDefaultText, nullptr, EAlign::Center, EVAlign::Bottom);
      const IText textVersionString = IText(20.f, colorDefaultText, nullptr, EAlign::Near, EVAlign::Middle);
      const IText textPresetMenu = IText(30.f, colorDefaultText, nullptr, EAlign::Center, EVAlign::Middle);
      // THESE ARE THE OLD ITEXT CONSTS
      //const IText textKnobLabel = IText(textSize, colorDefaultText, nullptr, IText::kStyleBold, IText::kAlignCenter, IText::kVAlignBottom, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
      //const IText textKnobValue = IText(textSize, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignCenter, IText::kVAlignBottom, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
      //const IText textVersionString = IText(20, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignNear, IText::kVAlignMiddle, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
      //const IText textPresetMenu = IText(30, colorDefaultText, nullptr, IText::kStyleNormal, IText::kAlignCenter, IText::kVAlignMiddle, 0, IText::kQualityClearType, colorPanelBG, colorDefaultText);
      const IVStyle style = IVStyle(
        true,
        true,
        { DEFAULT_BGCOLOR, DEFAULT_FGCOLOR, IColor(255, 200, 200, 200),IColor(255, 70, 70, 70),IColor(30, 255, 255, 255),IColor(100, 0, 0, 0),IColor(255, 249, 206, 34),IColor(255, 48, 166, 186),IColor(255, 249, 206, 34) },
        textKnobLabel,
        textKnobValue,
        true,
        true,
        true,
        DEFAULT_ROUNDNESS,
        DEFAULT_FRAME_THICKNESS,
        DEFAULT_SHADOW_OFFSET,
        DEFAULT_WIDGET_FRAC,
        DEFAULT_WIDGET_ANGLE
      );

    } SRLayout;

    struct SRLightInfo {
    public:
      SRLightInfo(float x, float y, float z)
        : mLightX(x)
        , mLightY(y)
        , mLightZ(z)
      {
      }
    private:
      float mLightX;
      float mLightY;
      float mLightZ;
    };


    // --------------------------------------------------------------------------------
    // KNOB PARTS
    // --------------------------------------------------------------------------------

    class SRKnobParts {
    public:
      enum EKnobParts {
        kLabelring = 0,
        kFrame,
        kHandle,
        kNumKnobParts
      };

      enum EKnobTypes {
        kSolid = 0,
        kChicken,
        kTec,
        kMoog,
        kNumKnobTypes
      };

      SRKnobParts(EKnobParts knobpart, EKnobTypes knobtype, float cx = 0.f, float cy = 0.f, float r = 50.f, float lightOffsetX = 0.f, float lightOffsetY = 0.f)
      {
        SetProperties(knobpart, knobtype, cx, cy, r, lightOffsetX, lightOffsetY);
      }

      ~SRKnobParts() {}

      void SetProperties(EKnobParts knobpart, EKnobTypes knobtype, float cx, float cy, float r, float lightOffsetX, float lightOffsetY) {
        mKnobtype = knobtype;
        mKnobpart = knobpart;
        mCx = cx;
        mCy = cy;
        mR = r;
        mLightOffsetX = lightOffsetX;
        mLightOffsetY = lightOffsetY;
      }

      void DrawPart(IGraphics& g) {

      }

      struct Label {
        IColor mTextColor = DEFAULT_TEXT_FGCOLOR;
      } mLabel;

      struct Frame {
        IPattern mPattern = IPattern(DEFAULT_FRCOLOR);
      } mFrame;

      struct Handle {
        IPattern mPattern = IPattern(DEFAULT_FGCOLOR);
      } mHandle;

    protected:

    private:
      EKnobParts mKnobpart = EKnobParts::kFrame;
      EKnobTypes mKnobtype = EKnobTypes::kSolid;
      float mCx = 0.f;
      float mCy = 0.f;
      float mR = 50.f;
      float mLightOffsetX = 0.f;
      float mLightOffsetY = 0.f;
    };


    // --------------------------------------------------------------------------------
    // KNOB
    // --------------------------------------------------------------------------------

    /** A vector knob control drawn using graphics primitves */
    class SRVectorKnob : public IKnobControlBase
      , public IVectorBase
    {
    public:
      SRVectorKnob(const IRECT& bounds, int paramIdx,
        const char* label = "",
        const IVStyle& style = DEFAULT_STYLE,
        bool valueIsEditable = false, bool valueInWidget = false,
        float aMin = -135.f, float aMax = 135.f, float aAnchor = -135.f,
        EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING);

      SRVectorKnob(const IRECT& bounds, IActionFunction actionFunction,
        const char* label = "",
        const IVStyle& style = DEFAULT_STYLE,
        bool valueIsEditable = false, bool valueInWidget = false,
        float aMin = -135.f, float aMax = 135.f, float aAnchor = -135.f,
        EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING);

      virtual ~SRVectorKnob() {}

      void Draw(IGraphics& g) override;
      virtual void DrawWidget(IGraphics& g) override;

      void OnMouseDown(float x, float y, const IMouseMod& mod) override;
      void OnMouseUp(float x, float y, const IMouseMod& mod) override;
      void OnMouseOver(float x, float y, const IMouseMod& mod) override;
      void OnMouseOut() override { mValueMouseOver = false; IKnobControlBase::OnMouseOut(); }

      //  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {  OnMouseDown(x, y, mod); }
      void OnResize() override;
      bool IsHit(float x, float y) const override;
      void SetDirty(bool push, int valIdx = kNoValIdx) override;
      void OnInit() override;

    protected:
      float mAngleMin, mAngleMax;
      float mAnchorAngle; // for bipolar arc
      bool mValueMouseOver = false;
    };


    SRVectorKnob::SRVectorKnob(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, bool valueIsEditable, bool valueInWidget, float aMin, float aMax, float aAnchor, EDirection direction, double gearing)
      : IKnobControlBase(bounds, paramIdx, direction, gearing)
      , IVectorBase(style, false, valueInWidget)
      , mAngleMin(aMin)
      , mAngleMax(aMax)
      , mAnchorAngle(aAnchor)
    {
      DisablePrompt(!valueIsEditable);
      mText = style.valueText;
      AttachIControl(this, label);
    }

    SRVectorKnob::SRVectorKnob(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, bool valueIsEditable, bool valueInWidget, float aMin, float aMax, float aAnchor, EDirection direction, double gearing)
      : IKnobControlBase(bounds, kNoParameter, direction, gearing)
      , IVectorBase(style, false, valueInWidget)
      , mAngleMin(aMin)
      , mAngleMax(aMax)
      , mAnchorAngle(aAnchor)
    {
      DisablePrompt(!valueIsEditable);
      mText = style.valueText;
      SetActionFunction(actionFunc);
      AttachIControl(this, label);
    }

    void SRVectorKnob::Draw(IGraphics& g)
    {
      DrawBackGround(g, mRECT);
      DrawWidget(g);
      DrawLabel(g);
      DrawValue(g, mValueMouseOver);
    }

    void SRVectorKnob::DrawWidget(IGraphics& g)
    {
      float radius;

      if (mWidgetBounds.W() > mWidgetBounds.H())
        radius = (mWidgetBounds.H() / 2.f /*TODO: fix bodge*/);
      else
        radius = (mWidgetBounds.W() / 2.f /*TODO: fix bodge*/);

      const float cx = mWidgetBounds.MW(), cy = mWidgetBounds.MH();

      if (!IsGrayed())
      {
        /*TODO: constants! */
        const float v = mAngleMin + ((float)GetValue() * (mAngleMax - mAngleMin));

        g.DrawArc(GetColor(kX1), cx, cy, (radius * 0.8f) + 3.f, v >= mAnchorAngle ? mAnchorAngle : mAnchorAngle - (mAnchorAngle - v), v >= mAnchorAngle ? v : mAnchorAngle, 0, 2.f);

        DrawPressableCircle(g, mWidgetBounds, radius * 0.8f, false/*mMouseDown*/, mMouseIsOver & !mValueMouseOver);

        g.DrawCircle(GetColor(kHL), cx, cy, radius * 0.7f);

        if (mMouseDown)
          g.FillCircle(GetColor(kON), cx, cy, radius * 0.7f);

        g.DrawRadialLine(GetColor(kFR), cx, cy, v, 0.6f * radius, 0.8f * radius, 0, mStyle.frameThickness >= 1.f ? mStyle.frameThickness : 1.f);
      }
      else
      {
        g.FillCircle(GetColor(kOFF), cx, cy, radius);
      }
    }

    void SRVectorKnob::OnMouseDown(float x, float y, const IMouseMod& mod)
    {
      if (mStyle.showValue && mValueBounds.Contains(x, y))
      {
        PromptUserInput(mValueBounds);
      }
      else
      {
        if (mStyle.hideCursor)
          GetUI()->HideMouseCursor(true, true);

        IKnobControlBase::OnMouseDown(x, y, mod);
      }
    }

    void SRVectorKnob::OnMouseUp(float x, float y, const IMouseMod& mod)
    {
      if (mStyle.hideCursor)
        GetUI()->HideMouseCursor(false);

      IKnobControlBase::OnMouseUp(x, y, mod);

      SetDirty(true);
    }

    void SRVectorKnob::OnMouseOver(float x, float y, const IMouseMod& mod)
    {
      if (mStyle.showValue && !mDisablePrompt)
        mValueMouseOver = mValueBounds.Contains(x, y);

      IKnobControlBase::OnMouseOver(x, y, mod);
    }

    void SRVectorKnob::OnResize()
    {
      SetTargetRECT(MakeRects(mRECT));
      SetDirty(false);
    }

    bool SRVectorKnob::IsHit(float x, float y) const
    {
      if (!mDisablePrompt)
      {
        if (mValueBounds.Contains(x, y))
          return true;
      }

      return mWidgetBounds.Contains(x, y);
    }

    void SRVectorKnob::SetDirty(bool push, int valIdx)
    {
      IKnobControlBase::SetDirty(push);

      const IParam* pParam = GetParam();

      if (pParam)
        pParam->GetDisplayForHostWithLabel(mValueStr);
    }

    void SRVectorKnob::OnInit()
    {
      const IParam* pParam = GetParam();

      if (pParam)
      {
        pParam->GetDisplayForHostWithLabel(mValueStr);

        if (!mLabelStr.GetLength())
          mLabelStr.Set(pParam->GetNameForHost());
      }
    }






/*

    class SRVectorKnobText
      : public IKnobControlBase
      , public IVectorBase
    {
    public:
      SRVectorKnobText(IRECT bounds, int paramIdx,
        const char* label = "", const char* minLabel = "", const char* maxLabel = "", const char* ctrLabel = "", bool drawCircleLabels = false, bool displayParamValue = false,
        const IVStyle& style = DEFAULT_STYLE, const IColor& color = DEFAULT_FGCOLOR,
        const IText& labelText = IText(DEFAULT_TEXT_SIZE + 5, EVAlign::Top), const IText& valueText = IText(DEFAULT_TEXT_SIZE, EVAlign::Bottom),
        float aMin = -135.f, float aMax = 135.f, float knobFrac = 0.50f,
        EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING)
        : IKnobControlBase(bounds, paramIdx, direction, gearing)
        , IVectorBase(style, true, displayParamValue)
        , mAngleMin(aMin)
        , mAngleMax(aMax)
        , mAngleDefault(aMin + 0.5f * (aMax - aMin))
        , mLabel(label)
        , mLabelMin(minLabel)
        , mLabelMax(maxLabel)
        , mLabelCtr(ctrLabel)
        , mDrawCircleLabels(drawCircleLabels)
        , mDisplayParamValue(displayParamValue)
        , mLabelText(labelText)
        , mKnobFrac(knobFrac)
        , mColor(color)
        , mTextCircleLabelMin(10.f, COLOR_LIGHT_GRAY, nullptr, EAlign::Far)
        , mTextCircleLabelMax(10.f, COLOR_LIGHT_GRAY, nullptr, EAlign::Near)
        , mTextCircleLabelCtr(10.f, COLOR_LIGHT_GRAY, nullptr, EAlign::Center)
        , mPatternShadow(IPattern(EPatternType::Solid))
        , mPatternHead(IPattern(EPatternType::Radial))
        , mPatternHeadLights(IPattern(EPatternType::Radial))
        , mPatternRim(IPattern(EPatternType::Linear))
        , mPatternEdge(IPattern(EPatternType::Linear))
        , mShadowFrame(mPatternShadow, 1.f, style.shadowOffset, style.shadowOffset, 0.5f, true)
        , mShadowHead(mPatternShadow, 1.f, style.shadowOffset, style.shadowOffset, 0.5f, true)
        , mShadowArrow(mPatternShadow, 1.f, style.shadowOffset, style.shadowOffset, 0.5f, true)

      {
        if (mDisplayParamValue)
          DisablePrompt(false);
        mValueText = valueText;
        mStrokeOptions.mPreserve = true;
        mFillOptions.mPreserve = true;
        AttachIControl(this, label);
      }

      virtual ~SRVectorKnobText() {}

      void Draw(IGraphics& g) override {
        // These values have to be calculated if value changed
        const float mAngleValue = mAngleMin + ((float)GetValue() * (mAngleMax - mAngleMin));
        const float colorIntensity = fabsf((float)GetValue() - (float)GetParam()->GetDefault(true) / fmaxf((float)GetParam()->GetDefault(true), (1.f - (float)GetParam()->GetDefault(true))));
        IColor arcColor;
        IColor::LinearInterpolateBetween(GetColor(kBG), mColor, arcColor, 0.3f + 0.5f * colorIntensity);

        // Background
        //g.FillRect(GetColor(kBG), mRECT);

        // Value Arc
        if (mAngleValue <= mAngleDefault)
          g.DrawArc(arcColor, mCenterX, mCenterY, knobScales.valArc.relRadius * mRadius, mAngleValue, mAngleDefault, 0, knobScales.valArc.relThickness * mRelThickness);
        else
          g.DrawArc(arcColor, mCenterX, mCenterY, knobScales.valArc.relRadius * mRadius, mAngleDefault, mAngleValue, 0, knobScales.valArc.relThickness * mRelThickness);

        // Dots
        for (int i = 0; i <= 10; i++) {
          g.DrawRadialLine(COLOR_MID_GRAY, mCenterX, mCenterY, mAngleMin + (float(i) * 0.1f) * (mAngleMax - mAngleMin), mRadius, mRadius + knobScales.dots.relThickness * mRelThickness, 0, knobScales.dots.relThickness * mRelThickness);
        }

        // Draw Circle Labels
        if (mDrawCircleLabels) {
          if (mMinLabelBounds.H()) {
            g.DrawText(mTextCircleLabelMin, mLabelMin.Get(), mMinLabelBounds, 0);
          }
          if (mMaxLabelBounds.H()) {
            g.DrawText(mTextCircleLabelMax, mLabelMax.Get(), mMaxLabelBounds, 0);
          }
          if (mCtrLabelBounds.H()) {
            g.DrawText(mTextCircleLabelCtr, mLabelCtr.Get(), mCtrLabelBounds, 0);
          }
        }

        // Text Label
        if (mLabelBounds.H()) {
          if (!mMouseIsOver && !mGrayed)
            g.DrawText(mLabelText, mLabel.Get(), mLabelBounds);
          else {
            // Text Value
            if (mDisplayParamValue)
            {
              WDL_String str;
              GetParam()->GetDisplayForHost(str);

              if (mShowParamLabel)
              {
                str.Append(" ");
                str.Append(GetParam()->GetLabelForHost());
              }

              //g.FillRoundRect(GetColor(kFR), mValueBounds, 2.f, 0);
              g.DrawText(mValueText, str.Get(), mValueBounds);
            }
          }
        }

        g.StartLayer(mRECT);
        // Draw Frame
        //g.DrawCircle(GetColor(kFR), mCenterX, mCenterY, mRadius * 0.9f, 0, mRelThickness);

        // Draw Rim
        g.PathClear();
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 7.f, mAngleValue + 53.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 55.f, mAngleValue + 65.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 67.f, mAngleValue + 113.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 115.f, mAngleValue + 125.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 127.f, mAngleValue + 173.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 175.f, mAngleValue + 185.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 187.f, mAngleValue + 233.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 235.f, mAngleValue + 245.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 247.f, mAngleValue + 293.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 295.f, mAngleValue + 305.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.outerRim.relRadius, mAngleValue + 307.f, mAngleValue + 353.f);
        g.PathArc(mCenterX, mCenterY, mRadius * knobScales.rim.relRadius, mAngleValue + 355.f, mAngleValue + 5.f);
        g.PathClose();
        //g.PathStroke(mPatternEdge, knobScales.outerRim.relThickness * mRadius, strokeOptions);
        g.PathFill(mPatternRim);
        mLayer = g.EndLayer();
        if (!Getstyle && !mGrayed) g.ApplyLayerDropShadow(mLayer, mShadowFrame);
        g.DrawLayer(mLayer);

        // Draw Head
        g.StartLayer(mRECT);
        g.PathClear();
        g.PathCircle(mCenterX, mCenterY, mRadius * knobScales.head.relRadius);
        if (!mGrayed) g.PathFill(mPatternHead, mFillOptions);
        else g.PathFill(COLOR_GRAY, mFillOptions);
        mLayer = g.EndLayer();
        if (!mEmboss && !mGrayed) g.ApplyLayerDropShadow(mLayer, mShadowHead);
        g.DrawLayer(mLayer);

        if (!mEmboss && !mGrayed) {
          g.PathClear();
          g.PathCircle(mCenterX, mCenterY, mRadius * knobScales.head.relRadius);
          g.PathFill(mPatternHeadLights, mFillOptions);
        }


        // Head Lights
        if (!mEmboss && !mGrayed) {
          g.PathClear();
          g.PathCircle(mCenterX, mCenterY, mRadius * knobScales.head.relRadius - 0.5f * knobScales.head.relThickness * mRadius);
          g.PathStroke(mPatternEdge, mRadius * knobScales.head.relThickness, mStrokeOptions);
        }

        // Outer Arrow
        g.DrawRadialLine(GetColor(kFR), mCenterX, mCenterY, mAngleValue, knobScales.outerArrow.relInnerRadius * mRadius, mRadius * knobScales.outerArrow.relRadius, 0, knobScales.outerArrow.relThickness * mRelThickness);

        // Arrow
        g.StartLayer(mRECT);
        if (mColor.B + mColor.R + mColor.G > 600) {
          g.DrawRadialLine(GetColor(kFR), mCenterX, mCenterY, mAngleValue, knobScales.arrow.relInnerRadius, knobScales.arrow.relRadius * mRadius, 0, knobScales.arrow.relThickness * mRelThickness);
          g.FillCircle(GetColor(kFR), mCenterX, mCenterY, knobScales.arrow.relThickness * mRelThickness * 0.5f, 0);
        }
        else {
          g.DrawRadialLine(GetColor(kON), mCenterX, mCenterY, mAngleValue, knobScales.arrow.relInnerRadius, knobScales.arrow.relRadius * mRadius, 0, knobScales.arrow.relThickness * mRelThickness);
          g.FillCircle(GetColor(kON), mCenterX, mCenterY, knobScales.arrow.relThickness * mRelThickness * 0.5f, 0);
        }
        mLayer = g.EndLayer();
        if (!mEmboss && !mGrayed) g.ApplyLayerDropShadow(mLayer, mShadowArrow);
        g.DrawLayer(mLayer);


        // Mouseover
        if (mMouseIsOver && !mGrayed) g.FillCircle(GetColor(kHL), mCenterX, mCenterY, mRadius * knobScales.head.relRadius);

      }
      void OnMouseDown(float x, float y, const IMouseMod& mod) override {
        if (mDisplayParamValue && mValueBounds.Contains(x, y))
        {
          PromptUserInput(mValueBounds);
        }
        else {
          IKnobControlBase::OnMouseDown(x, y, mod);
        }
      }
      void OnInit() override
      {
        mAngleDefault = mAngleMin + (float)GetParam()->GetDefault(true) * (mAngleMax - mAngleMin);
      }
      void OnResize() override {

        mTargetRECT = mHandleBounds = mRECT;
        mTextCircleLabelMin.mSize = mTextCircleLabelMax.mSize = mTextCircleLabelCtr.mSize = int(mRECT.H() * 0.18f);
        mLabelText.mSize = mValueText.mSize = int(mRECT.H() * 0.22f);


        // LABEL
        if (mLabel.GetLength())
        {
          IRECT textRect;
          GetUI()->MeasureText(mLabelText, mLabel.Get(), textRect);
          const float labelDisplayWidth = mTargetRECT.W() * mKnobFrac * 0.5f;
          switch (mLabelText.mVAlign)
          {
          case EVAlign::Top:
            mLabelBounds = mTargetRECT.GetFromTop(textRect.H()).GetMidHPadded(labelDisplayWidth);
            mHandleBounds = mHandleBounds.GetReducedFromTop(textRect.H());
            break;
          case EVAlign::Middle:
            mLabelBounds = mTargetRECT.GetMidVPadded(textRect.H() / 2.f).GetMidHPadded(labelDisplayWidth);
            break;
          case EVAlign::Bottom:
            mLabelBounds = mTargetRECT.GetFromBottom(textRect.H()).GetMidHPadded(labelDisplayWidth);
            mHandleBounds = mHandleBounds.GetReducedFromBottom(textRect.H());
            break;
          default:
            break;
          }
          if (mLabelBounds.W() < textRect.W()) mLabelBounds = mLabelBounds.GetMidHPadded(mTargetRECT.W() / 2.f);
        }
        else {
          mLabelBounds = IRECT();
        }

        // DISPLAY VALUE
        if (mDisplayParamValue)
        {
          IRECT textRect;
          WDL_String str;
          GetParam()->GetDisplayForHost(str);

          GetUI()->MeasureText(mValueText, str.Get(), textRect);

          const float valueDisplayWidth = mTargetRECT.W() * mKnobFrac * 0.5f;

          switch (mValueText.mVAlign) {
          case EVAlign::Top:
            mValueBounds = mTargetRECT.GetFromTop(textRect.H()).GetMidHPadded(valueDisplayWidth);
            if (mLabelText.mVAlign != EVAlign::Top)
              mHandleBounds = mHandleBounds.GetReducedFromTop(textRect.H());
            break;
          case EVAlign::Middle:
            mValueBounds = mTargetRECT.GetMidVPadded(textRect.H() / 2.f).GetMidHPadded(valueDisplayWidth);
            break;
          case EVAlign::Bottom:
            mValueBounds = mTargetRECT.GetFromBottom(textRect.H()).GetMidHPadded(valueDisplayWidth);
            if (mLabelText.mVAlign != EVAlign::Bottom)
              mHandleBounds = mHandleBounds.GetReducedFromBottom(textRect.H());
            break;
          default:
            break;
          }

          if (mValueBounds.W() < textRect.W()) mValueBounds = mValueBounds.GetMidHPadded(mTargetRECT.W() / 2.f);
        }
        else {
          mValueBounds = IRECT();
        }

        // CIRCLE LABELS
        if (mDrawCircleLabels) {
          mHandleBounds = mHandleBounds.GetReducedFromTop((float)mTextCircleLabelCtr.mSize);
        }

        mHandleBounds = mHandleBounds.GetMidHPadded(mHandleBounds.H() * 0.5f);
        mTargetRECT = mTargetRECT.GetMidHPadded(mHandleBounds.W() * 0.5f);

        if (mDrawCircleLabels) {
          if (mLabelMin.GetLength()) {
            IRECT textRect;
            GetUI()->MeasureText(mTextCircleLabelMin, mLabelMin.Get(), textRect);
            mMinLabelBounds = IRECT(mRECT.L, mHandleBounds.B - textRect.H(), mHandleBounds.L, mHandleBounds.B);
          }
          if (mLabelMax.GetLength()) {
            IRECT textRect;
            GetUI()->MeasureText(mTextCircleLabelMax, mLabelMax.Get(), textRect);
            mMaxLabelBounds = IRECT(mHandleBounds.R, mHandleBounds.B - textRect.H(), mRECT.R, mHandleBounds.B);
          }
          // Center Label need rework
          if (mLabelCtr.GetLength()) {
            IRECT textRect;
            GetUI()->MeasureText(mTextCircleLabelCtr, mLabelCtr.Get(), textRect);
            mCtrLabelBounds = mRECT.GetFromTop(textRect.H()).GetMidHPadded(textRect.W() * 0.5f);
          }

        }

        //mHandleBounds = GetAdjustedHandleBounds(mTargetRECT).GetReducedFromTop(mCtrLabelBounds.H()).GetReducedFromBottom(;


        // ----------------------------------------------------------------------------










        mRadius = (mHandleBounds.W() * 0.5f);
        mCenterX = mHandleBounds.MW();
        mCenterY = mHandleBounds.MH();
        mRelThickness = mRadius * mFrameThickness * 0.05f / 2.f;
        mRelShadowOffset = mRadius * mShadowOffset * 0.2f / 3.f;

        // PATTERN TEST
        // Patterns
        mPatternShadow = IPattern(COLOR_BLACK);
        mPatternHead = IPattern::CreateRadialGradient(
          mCenterX,
          mCenterY,
          mRadius,
          {
            IColorStop(mColor, 0.9f),
            //IColorStop(mColor.GetContrasted(1.0f), 0.9f),
            //IColorStop(mColor, 0.95f),
            IColorStop(ColorGetAltered(mColor, 0.5f), 1.f)
          }
        );
        mPatternHeadLights = IPattern::CreateRadialGradient(
          mCenterX - GetPercW() * 0.5f * mRadius,
          mCenterY - GetPercH() * 0.5f * mRadius,
          mRadius * knobScales.head.relRadius,
          {
            IColorStop(IColor(200, 255,255,255), 0.0f),
            IColorStop(COLOR_TRANSPARENT, 1.0f)
          }
        );
        mPatternRim = IPattern::CreateLinearGradient(
          mCenterX - mRadius, mCenterY - mRadius,
          mCenterX + mRadius, mCenterY + mRadius,
          {
            IColorStop(GetColor(kON), 0.5f),
            IColorStop(GetColor(kFR), 1.0f)
          }
        );
        mPatternEdge = IPattern::CreateLinearGradient(
          mCenterX - 0.5f * mRadius, mCenterY - 0.5f * mRadius,
          mCenterX + 0.5f * mRadius, mCenterY + 0.5f * mRadius,
          {
            IColorStop(IColor(130, 255, 255, 255), 0.0f),
            IColorStop(IColor(20, 130, 130, 130), 0.5f),
            IColorStop(IColor(130, 0, 0, 0), 1.0f)
          }
        );

        // Shadows
        mShadowFrame = IShadow(
          mPatternShadow,
          mRadius * 0.1f,
          0.6f * mRadius * GetPercW(),
          0.6f * mRadius * GetPercH(),
          0.5f,
          true
        );

        mShadowHead = IShadow(
          mPatternShadow,
          mRadius * 0.01f,
          0.3f * mRadius * GetPercW(),
          0.3f * mRadius * GetPercH(),
          0.5f,
          true
        );

        mShadowArrow = IShadow(
          mPatternShadow,
          mRadius * 0.01f,
          0.1f * mRadius * GetPercW(),
          0.1f * mRadius * GetPercH(),
          0.5f,
          true
        );
        // ---}

        SetDirty(false);
      }
      //void GrayOut(bool gray) override;
      float GetPercW() { return mCenterX / GetDelegate()->GetEditorWidth(); }
      float GetPercH() { return mCenterY / GetDelegate()->GetEditorHeight(); }

      IColor ColorGetAltered(IColor pColor, float change) {
        IColor color = pColor;
        color.R = int(color.R * change); color.R = (color.R > 255) ? 255 : color.R;
        color.B = int(color.B * change); color.B = (color.B > 255) ? 255 : color.B;
        color.G = int(color.G * change); color.G = (color.G > 255) ? 255 : color.G;
        return color;
      }


    protected:
      bool mDisplayParamValue;
      bool mShowParamLabel = true;
      bool mDrawCircleLabels;
      IRECT mHandleBounds;
      IRECT mLabelBounds;
      IRECT mValueBounds;
      IRECT mMinLabelBounds;
      IRECT mMaxLabelBounds;
      IRECT mCtrLabelBounds;
      float mAngleMin, mAngleMax, mAngleDefault;
      float mKnobFrac;
      float mRadius, mAngleValue, mCenterX, mCenterY, mRelThickness, mRelShadowOffset;
      WDL_String mLabel;
      WDL_String mLabelMin;
      WDL_String mLabelMax;
      WDL_String mLabelCtr;
      IText mLabelText;
      IText& mValueText = mText;
      IText mTextCircleLabelMin;
      IText mTextCircleLabelMax;
      IText mTextCircleLabelCtr;
      IColor mColor;
      IPattern mPatternShadow;
      IPattern mPatternHead;
      IPattern mPatternHeadLights;
      IPattern mPatternRim;
      IPattern mPatternEdge;
      ILayerPtr mLayer;
      IShadow mShadowFrame;
      IShadow mShadowHead;
      IShadow mShadowArrow;
      IStrokeOptions mStrokeOptions;
      IFillOptions mFillOptions;
      const struct KnobScaleVals
      {
        const float relRadius = 1.f;
        const float relThickness = 2.f;
        const float relInnerRadius = 0.f;
      };
      const struct KnobScales
      {
        const KnobScaleVals valArc = { 1.f, 4.f };
        const KnobScaleVals dots = { 1.f, 2.f };
        const KnobScaleVals rim = { 0.825f, -1.f };
        const KnobScaleVals outerRim = { 0.9f, 0.06f };
        const KnobScaleVals head = { 0.75f, 0.06f };
        const KnobScaleVals outerArrow = { 0.825f, 3.f, 0.7f };
        const KnobScaleVals arrow = { 0.75f, 3.f, 0.f };

      } knobScales;
    };

*/





class SRVectorSwitch : public ISwitchControlBase
  , public IVectorBase
{
public:
  SRVectorSwitch(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, bool valueInButton = true);

  SRVectorSwitch(const IRECT& bounds, IActionFunction actionFunc = SplashClickActionFunc, const char* label = "", const IVStyle& style = DEFAULT_STYLE, int numStates = 2, bool valueInButton = true);

  void Draw(IGraphics& g) override;
  virtual void DrawWidget(IGraphics& g) override;
  bool IsHit(float x, float y) const override;
  void SetDirty(bool push, int valIdx = kNoValIdx) override;
  void OnResize() override;
  void OnInit() override;
};


SRVectorSwitch::SRVectorSwitch(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, bool valueInButton)
  : ISwitchControlBase(bounds, paramIdx, SplashClickActionFunc)
  , IVectorBase(style, false, valueInButton)
{
  AttachIControl(this, label);
  mText = style.valueText;

  if (valueInButton)
    mText.mVAlign = mStyle.valueText.mVAlign = EVAlign::Middle;

  mDblAsSingleClick = true;
}

SRVectorSwitch::SRVectorSwitch(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, int numStates, bool valueInButton)
  : ISwitchControlBase(bounds, kNoParameter, actionFunc, numStates)
  , IVectorBase(style, false, valueInButton)
{
  AttachIControl(this, label);
  mText = style.valueText;

  if (valueInButton)
    mText.mVAlign = mStyle.valueText.mVAlign = EVAlign::Middle;

  mDblAsSingleClick = true;
}

void SRVectorSwitch::Draw(IGraphics& g)
{
  DrawBackGround(g, mRECT);
  DrawWidget(g);
  DrawLabel(g);
  DrawValue(g, false);
}

void SRVectorSwitch::DrawWidget(IGraphics& g)
{
  DrawPressableRectangle(g, mWidgetBounds, mMouseDown, mMouseIsOver);
}

void SRVectorSwitch::SetDirty(bool push, int valIdx)
{
  IControl::SetDirty(push);

  const IParam* pParam = GetParam();

  if (pParam)
    pParam->GetDisplayForHost(mValueStr);
}

void SRVectorSwitch::OnResize()
{
  SetTargetRECT(MakeRects(mRECT, true));
  SetDirty(false);
}

bool SRVectorSwitch::IsHit(float x, float y) const
{
  return mWidgetBounds.Contains(x, y);
}

void SRVectorSwitch::OnInit()
{
  ISwitchControlBase::OnInit();

  const IParam* pParam = GetParam();

  if (pParam)
  {
    pParam->GetDisplayForHostWithLabel(mValueStr);

    if (!mLabelStr.GetLength())
      mLabelStr.Set(pParam->GetNameForHost());
  }
}

/** A vector toggle control. Click to cycle through two states. */
class SRVectorToggle : public SRVectorSwitch
{
public:
  SRVectorToggle(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "OFF", const char* onText = "ON");

  SRVectorToggle(const IRECT& bounds, IActionFunction actionFunc = SplashClickActionFunc, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "OFF", const char* onText = "ON", bool initialState = false);

  void DrawValue(IGraphics& g, bool mouseOver) override;
  void DrawWidget(IGraphics& g) override;
protected:
  WDL_String mOffText;
  WDL_String mOnText;
};

SRVectorToggle::SRVectorToggle(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, const char* offText, const char* onText)
  : SRVectorSwitch(bounds, paramIdx, label, style, true)
  , mOnText(onText)
  , mOffText(offText)
{
  //TODO: assert boolean?
}

SRVectorToggle::SRVectorToggle(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, const char* offText, const char* onText, bool initialState)
  : SRVectorSwitch(bounds, actionFunc, label, style, 2, true)
  , mOnText(onText)
  , mOffText(offText)
{
  SetValue((double)initialState);
}

void SRVectorToggle::DrawWidget(IGraphics& g)
{
  DrawPressableRectangle(g, mWidgetBounds, GetValue() > 0.5, mMouseIsOver);
}

void SRVectorToggle::DrawValue(IGraphics& g, bool mouseOver)
{
  if (mouseOver)
    g.FillRect(COLOR_TRANSLUCENT, mValueBounds);

  if (GetValue() > 0.5)
    g.DrawText(mStyle.valueText, mOnText.Get(), mValueBounds);
  else
    g.DrawText(mStyle.valueText, mOffText.Get(), mValueBounds);
}

// Todo: Commented until reimplementation


    /** A vector switch control. Click to cycle through states. */

/*

    class SRVectorSwitch : public ISwitchControlBase
      , public IVectorBase
    {
    public:
      SRVectorSwitch(IRECT bounds, int paramIdx = kNoParameter, IActionFunction actionFunc = SplashClickActionFunc,
        const char* label = "", const IVStyle& style = DEFAULT_STYLE, int numStates = 2)
        : ISwitchControlBase(bounds, paramIdx, actionFunc, numStates)
        , IVectorBase(style)
      {
        AttachIControl(this, ""); // TODO: Should hand label, but may be solved through adding IVSwitch
        mDblAsSingleClick = true;
        mStr.Set(label);
      }

      void Draw(IGraphics& g) override
      {
        IRECT handleBounds = DrawVectorButton(g, mRECT, mMouseDown, mMouseIsOver);

        if (CStringHasContents(mStr.Get()))
          g.DrawText(mText, mStr.Get(), handleBounds);
      }

      void SetDirty(bool push, int valIdx = kNoValIdx) override
      {
        IControl::SetDirty(push);

        const IParam* pParam = GetParam();

        if (pParam)
          pParam->GetDisplayForHost(mStr);
      }

      IRECT DrawVectorButton(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver)
      {
        // background
        g.FillRect(GetColor(kBG), bounds);

        IRECT handleBounds = GetAdjustedHandleBounds(bounds);
        const float cornerRadius = IVectorBase::mStyle.roundness * (handleBounds.W() / 2.f);

        // Pressed
        if (pressed) {
          g.FillRoundRect(GetColor(kPR), handleBounds, cornerRadius);

          //inner shadow
          if (mStyle.drawShadows) {
            g.PathRect(handleBounds.GetHSliced(mStyle.shadowOffset));
            g.PathRect(handleBounds.GetVSliced(mStyle.shadowOffset));
            g.PathFill(GetColor(kSH));
          }

          if (mouseOver) g.FillRoundRect(GetColor(kHL), handleBounds, cornerRadius);
          if (mControl->GetAnimationFunction()) DrawSplash(g);
          if (mStyle.drawFrame) g.DrawRoundRect(GetColor(kFR), handleBounds, cornerRadius, 0, mStyle.frameThickness);
        }
        else {
          // Normal button state
          if (mNumStates > 2 || GetValue() == 0) {
            //outer shadow
            if (mStyle.drawShadows) g.FillRoundRect(GetColor(kSH), handleBounds, cornerRadius);
            g.FillRoundRect(GetColor(kFG), handleBounds, cornerRadius);
            if (mouseOver) g.FillRoundRect(GetColor(kHL), handleBounds, cornerRadius);
            if (mControl->GetAnimationFunction()) DrawSplash(g);
            if (mStyle.drawFrame) g.DrawRoundRect(GetColor(kFR), handleBounds, cornerRadius, 0, mStyle.frameThickness);
          }

          // If button should be "inside"
          else {
            g.FillRoundRect(GetColor(kPR), handleBounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset), cornerRadius);
            //inner shadow
            if (mStyle.drawShadows) {
              g.PathRect(handleBounds.GetHSliced(mStyle.shadowOffset));
              g.PathRect(handleBounds.GetVSliced(mStyle.shadowOffset));
              g.PathFill(GetColor(kSH));
            }
            if (mouseOver) g.FillRoundRect(GetColor(kHL), handleBounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset), cornerRadius);
            if (mControl->GetAnimationFunction()) DrawSplash(g);
            if (mStyle.drawShadows) g.DrawRoundRect(GetColor(kFR), handleBounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset), cornerRadius, 0, mStyle.frameThickness);
          }
        }

        return handleBounds;
      }


    protected:
      WDL_String mStr;
    };
    */



    /** A basic control to display some text */
    class SRLogo
      : public IControl
    {
    public:
      SRLogo(IRECT bounds, const char* str = "", const IText& text = DEFAULT_TEXT, const IColor& BGColor = DEFAULT_BGCOLOR)
        : IControl(bounds)
        , mStr(str)
        , mBGColor(BGColor)
      {
        mIgnoreMouse = true;
        IControl::mText = text;
      }

      void Draw(IGraphics& g) override
      {
        g.FillRect(mBGColor, mRECT);

        if (mStr.GetLength())
          g.DrawText(mText, mStr.Get(), mRECT);
      }

      virtual void SetStr(const char* str)
      {
        if (strcmp(mStr.Get(), str))
        {
          mStr.Set(str);
          SetDirty(false);
        }
      }
      virtual void SetStrFmt(int maxlen, const char* fmt, ...)
      {
        va_list arglist;
        va_start(arglist, fmt);
        mStr.SetAppendFormattedArgs(false, maxlen, fmt, arglist);
        va_end(arglist);
        SetDirty(false);
      }
      virtual void ClearStr() { SetStr(""); }

    protected:
      WDL_String mStr;
      IColor mBGColor;
    };



    /** A base class for mult-strip/track controls, such as multi-sliders, meters */
    class SRTrackControlBase
      : public IControl
      , public IVectorBase
    {
    public:
      SRTrackControlBase(IRECT bounds, int maxNTracks = 1, float minTrackValue = 0.f, float maxTrackValue = 1.f, const char* trackNames = 0, ...)
        : IControl(bounds)
        , mMaxNTracks(maxNTracks)
        , mMinTrackValue(minTrackValue)
        , mMaxTrackValue(maxTrackValue)
      {
        for (int i = 0; i < maxNTracks; i++)
        {
          mTrackData.Add(0.f);
          mTrackBounds.Add(IRECT());
        }

        AttachIControl(this, ""); // TODO: Should hand label
      }

      void MakeRects()
      {
        for (int ch = 0; ch < MaxNTracks(); ch++)
        {
          mTrackBounds.Get()[ch] = mRECT.GetPadded(-mOuterPadding).
            SubRect(EDirection(!(bool)mDirection), MaxNTracks(), ch).
            GetPadded(0, -mTrackPadding * (float)mDirection, -mTrackPadding * (float)!(bool)mDirection, -mTrackPadding);
        }
      }

      void Draw(IGraphics& g) override
      {
        g.FillRect(GetColor(kBG), mRECT);

        for (int ch = 0; ch < MaxNTracks(); ch++)
        {
          DrawTrack(g, mTrackBounds.Get()[ch], ch);
        }

        if (mStyle.drawFrame)
          DrawFrame(g);
      }

      int NTracks() { return mNTracks; }
      int MaxNTracks() { return mMaxNTracks; }
      void SetTrackData(int trackIdx, float val) { mTrackData.Get()[trackIdx] = Clip(val, mMinTrackValue, mMaxTrackValue); }
      float* GetTrackData(int trackIdx) { return &mTrackData.Get()[trackIdx]; }
      void SetAllTrackData(float val) { memset(mTrackData.Get(), (int)Clip(val, mMinTrackValue, mMaxTrackValue), mTrackData.GetSize() * sizeof(float)); }
    private:
      virtual void DrawFrame(IGraphics& g)
      {
        g.DrawRect(GetColor(kFR), mRECT, nullptr, mStyle.frameThickness);
      }

      virtual void DrawTrack(IGraphics& g, IRECT& r, int chIdx)
      {
        DrawTrackBG(g, r, chIdx);
        DrawTrackHandle(g, r, chIdx);

        if (mDrawTrackFrame)
          g.DrawRect(GetColor(kFR), r, nullptr, mStyle.frameThickness);
      }

      virtual void DrawTrackBG(IGraphics& g, IRECT& r, int chIdx)
      {
        g.FillRect(GetColor(kSH), r);
      }

      virtual void DrawTrackHandle(IGraphics& g, IRECT& r, int chIdx)
      {
        IRECT fillRect = r.FracRect(mDirection, *GetTrackData(chIdx));

        g.FillRect(GetColor(kFG), fillRect); // TODO: shadows!

        IRECT peakRect;

        if (mDirection == EDirection::Vertical)
          peakRect = IRECT(fillRect.L, fillRect.T, fillRect.R, fillRect.T + mPeakSize);
        else
          peakRect = IRECT(fillRect.R - mPeakSize, fillRect.T, fillRect.R, fillRect.B);

        DrawPeak(g, peakRect, chIdx);
      }

      virtual void DrawPeak(IGraphics& g, IRECT& r, int chIdx)
      {
        g.FillRect(GetColor(kHL), r);
      }

      void OnResize() override
      {
        MakeRects();
      }

    protected:

      EDirection mDirection = EDirection::Vertical;
      int mMaxNTracks;
      WDL_TypedBuf<float> mTrackData; // real values of sliders/meters
      WDL_TypedBuf<IRECT> mTrackBounds;

      int mNTracks = 1;

      float mMinTrackValue;
      float mMaxTrackValue;
      float mOuterPadding = 10.;
      float mTrackPadding = 2;
      float mPeakSize = 5.;
      bool mDrawTrackFrame = true;
    };

    // End TrackControl class


    // METER CLASS
    // ------------

    template <int MAXNC = 1, int QUEUE_SIZE = 1024>
    class SRMeter
      : public SRTrackControlBase {
    public:

      static constexpr int kUpdateMessage = 0;

      /** Data packet */
      struct Data
      {
        int nchans = MAXNC;
        float vals[MAXNC] = {};
        bool AboveThreshold() {
          static const float threshold = (float)DBToAmp(-90.);
          float sum = 0.f;
          for (int i = 0; i < MAXNC; i++) {
            sum += vals[i];
          }
          return std::abs(sum) > threshold;
        }
      };

      /** Used on the DSP side in order to queue sample values and transfer data to low priority thread. */
      class SRMeterBallistics {

      public:
        SRMeterBallistics(int controlTag)
          : mControlTag(controlTag)
        {
        }

        void ProcessBlock(sample** inputs, int nFrames)
        {
          Data d;

          for (auto s = 0; s < nFrames; s++) {
            for (auto c = 0; c < MAXNC; c++) {
              d.vals[c] += std::fabs((float)inputs[c][s]);
            }
          }

          for (auto c = 0; c < MAXNC; c++) {
            d.vals[c] /= (float)nFrames;
          }

          if (mPrevAboveThreshold)
            mQueue.Push(d); // TODO: expensive?

          mPrevAboveThreshold = d.AboveThreshold();
        }

        // this must be called on the main thread - typically in MyPlugin::OnIdle()
        void TransmitData(IEditorDelegate& dlg) {
          while (mQueue.ElementsAvailable())
          {
            Data d;
            mQueue.Pop(d);
            dlg.SendControlMsgFromDelegate(mControlTag, kUpdateMessage, sizeof(Data), (void*)&d);
          }
        }

      private:
        int mControlTag;
        bool mPrevAboveThreshold = true;
        IPlugQueue<Data> mQueue{ QUEUE_SIZE };
      };

      struct LabelFrames {
        IRECT rectLabel = IRECT();
        WDL_String str = WDL_String();
        void SetLabelFrame(IRECT r, float val) {
          str.SetFormatted(MAX_PARAM_DISPLAY_LEN, "%d", static_cast<int>(roundf(val)));
          rectLabel = r;
        }
      };

      struct LinePos {
        float position = 0.0f;
        float thickness = 1.0f;
        void SetLine(float pos, float thick) {
          position = pos;
          thickness = thick;
        }
      };

      // If you want to create a meter with decibel values
      SRMeter(IRECT bounds, bool drawFromTop = false, bool drawInverted = false, float minDb = 90., float maxDb = 0., float shape = 1.0, int markStep = 1, int labelStep = 6, const char* trackNames = 0, ...)
        : SRTrackControlBase(bounds, MAXNC, DBToAmp(minDb), DBToAmp(maxDb), trackNames)
        , mDrawFromTop(drawFromTop)
        , mDrawInverted(drawInverted)
        , mMinDb(minDb)
        , mMaxDb(maxDb)
        , mShape(shape)
        , mMaxTrackValue(4.f)
        , mMarkStep(markStep)
        , mLabelStep(labelStep)
        , mText(14.f, COLOR_LIGHT_GRAY, DEFAULT_FONT, EAlign::Center, EVAlign::Middle, 0.f)
      , mNumLines(int(maxDb - minDb))
        , mNumLabels(int((maxDb - minDb) / labelStep))
        , rectLabelFrame(IRECT())
        //, mPattern(kLinearPattern)
        , mDrawDb(true)
      {
        mLabelFrames = new LabelFrames[mNumLabels + 1];
        mLineCoord = new LinePos[mNumLines + 1];
      }

      // If you want to create a meter with normalized values 0..1
      SRMeter(IRECT bounds, bool drawFromTop = false, bool drawInverted = false, const char* trackNames = 0, ...)
        : IVTrackControlBase(bounds, MAXNC, 0, 1., trackNames)
        , mDrawFromTop(drawFromTop)
        , mDrawInverted(drawInverted)
        , mText(14.f, COLOR_LIGHT_GRAY, DEFAULT_FONT, EAlign::Center, EVAlign::Middle, 0.f)
        //, mPattern(kLinearPattern)
        , mDrawDb(false)
      {
      }

      void OnResize() override {
        MakeRects();
        mText.mSize = std::min(int(mRECT.W() * 0.25f), int(mRECT.H() / mNumLabels));
      };

      //  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override;
      //  void OnMouseDown(float x, float y, const IMouseMod& mod) override;

      void MakeRects() {
        for (int ch = 0; ch < MaxNTracks(); ch++) {
          mTrackBounds.Get()[ch] = mRECT.GetPadded(-mOuterPadding).
            SubRect(EDirection::Horizontal, MaxNTracks(), ch).
            GetPadded(0, -mTrackPadding, -mTrackPadding * (float)EDirection::Horizontal, -mTrackPadding * (float)EDirection::Vertical);
        }

        if (mDrawDb) {

          rectLabelFrame = mRECT.GetPadded(-mOuterPadding);

          for (int i = 0, j = 0; i <= mNumLines; i++) {
            const float val = std::roundf(mMinDb + ((float)i / (float)mNumLines) * (mMaxDb - mMinDb));
            const float vPosition = std::pow<float>((val - mMinDb) / (mMaxDb - mMinDb), 1.0f / mShape);
            mLineCoord[i].SetLine(rectLabelFrame.B - rectLabelFrame.H() * vPosition, 1.f);
            if (i % mLabelStep == 0) {
              mLineCoord[i].thickness = mStyle.frameThickness;
              mLabelFrames[j].SetLabelFrame(
                IRECT(rectLabelFrame.L, -mText.mSize + rectLabelFrame.B - vPosition * rectLabelFrame.H(), rectLabelFrame.R, mText.mSize + rectLabelFrame.B - vPosition * rectLabelFrame.H()),
                val
              );
              j++;
            }
          }
        }

      }

      void Draw(IGraphics& g) override {
        // Don't want to fill with transparent IColor
        // g.FillRect(GetColor(kBG), mRECT);

        for (int ch = 0; ch < MaxNTracks(); ch++) {
          DrawTrack(g, mTrackBounds.Get()[ch], ch);
        }

        // Don't want to draw a Frame
        //if (mDrawFrame)
        //  DrawFrame(g);

        if (mDrawDb) {
          for (int i = 0; i <= mNumLabels; i++) {
            g.DrawText(mText, mLabelFrames[i].str.Get(), mLabelFrames[i].rectLabel);
          }
          for (int i = 0; i <= mNumLines; i++) {
            g.DrawLine(GetColor(kHL), rectLabelFrame.L, mLineCoord[i].position, rectLabelFrame.R, mLineCoord[i].position, 0, mLineCoord[i].thickness);
          }
        }

      }

      //virtual void DrawFrame(IGraphics& g)
      //{
      //  //g.DrawRect(GetColor(kFR), mRECT, nullptr, mFrameThickness);
      //}

      virtual void DrawTrack(IGraphics& g, IRECT& r, int chIdx) {
        DrawTrackBG(g, r, chIdx);
        DrawTrackHandle(g, r, chIdx);
        //if (mDrawTrackFrame)
        //  g.DrawRect(GetColor(kBG), r, nullptr, mFrameThickness);
      }

      virtual void DrawTrackBG(IGraphics& g, IRECT& r, int chIdx) {
        g.FillRect(GetColor(kHL), r);
      }

      virtual void DrawTrackHandle(IGraphics& g, IRECT& r, int chIdx) {
        IRECT fillRect;
        float value = *GetTrackData(chIdx);
        if (mDrawDb) value = std::pow<float>(((float)AmpToDB(value) - mMinDb) / (mMaxDb - mMinDb), 1.0f / mShape);
        fillRect = r.FracRect(mDirection, (!mDrawInverted) ? value : 1.f - value, mDrawFromTop); // HERE the value rect is drawn!
        g.FillRect(GetColor(kFG), fillRect); // TODO: shadows!

        IRECT peakRect;
        if (mDirection == EDirection::Vertical)
          peakRect = (!mDrawFromTop) ? fillRect.GetFromTop(mPeakSize) : fillRect.GetFromBottom(mPeakSize);
        else
          peakRect = (!mDrawFromTop) ? fillRect.GetFromRight(mPeakSize) : fillRect.GetFromLeft(mPeakSize);
        DrawPeak(g, peakRect, chIdx);
      }

      virtual void DrawPeak(IGraphics& g, IRECT& r, int chIdx) {
        g.FillRect(GetColor(kX1), r);
      }

      void OnMsgFromDelegate(int messageTag, int dataSize, const void* pData) override {
        IByteStream stream(pData, dataSize);

        int pos = 0;
        Data data;
        pos = stream.Get(&data.nchans, pos);

        while (pos < stream.Size()) {
          for (auto i = 0; i < data.nchans; i++) {
            pos = stream.Get(&data.vals[i], pos);
            float* pVal = GetTrackData(i);
            *pVal = Clip(data.vals[i], 0.f, 1.f);
          }
        }

        SetDirty(false);
      }




    protected:
      bool mDrawFromTop;
      bool mDrawInverted;
      bool mDrawDb;
      float mMinDb;
      float mMaxDb;
      float mMaxTrackValue;
      int mLabelStep;
      int mMarkStep;
      int mNumLabels;
      int mNumLines;
      float mShape;
      IText mText;
      LabelFrames* mLabelFrames;
      LinePos* mLineCoord;
      IRECT rectLabelFrame;
      //IPattern mPattern;
    };

    /** A basic control to fill a rectangle with a color or gradient */
    class SRPanel
      : public IControl {
    public:
      SRPanel(IRECT bounds, const IColor& color, bool drawFrame = false)
        : IControl(bounds, kNoParameter)
        , mPattern(color)
        , mDrawFrame(drawFrame)
      {
        mIgnoreMouse = true;
      }

      SRPanel(IRECT bounds, const IPattern& pattern, bool drawFrame = false)
        : IControl(bounds, kNoParameter)
        , mPattern(pattern)
        , mDrawFrame(drawFrame)
      {
        mIgnoreMouse = true;
      }

      void Draw(IGraphics& g) override {
        if (g.HasPathSupport()) {
          g.PathRect(mRECT);
          g.PathFill(mPattern);
        }
        else {
          g.FillRect(mPattern.GetStop(0).mColor, mRECT);
        }

        if (mDrawFrame) {
          g.DrawRect(COLOR_LIGHT_GRAY, mRECT);
        }
      }

      void SetPattern(const IPattern& pattern) {
        mPattern = pattern;
        SetDirty(false);
      }

    private:
      IPattern mPattern;
      bool mDrawFrame;
    };

    // TODO: THIS IS COMMENTED UNTIL CONFLICT WITH STEINBERG::IPLUGINBASE IS SOLVED
    /*
    // Preset Menu
// -----------------------------------

    class SRPresetMenu
      : public IControl {
    public:
      SRPresetMenu(IPluginBase *pPlug, IRECT bounds, IText pText, const char** pNamedParams)
        : IControl(bounds, -1) {
        mTextEntryLength = MAX_PRESET_NAME_LEN - 3;
        mText = pText;
        mPlug = pPlug;
        mNamedParams = pNamedParams;
      }
      void Draw(IGraphics& g) override {
        int pNumber = mPlug->GetCurrentPresetIdx();

        mDisp.SetFormatted(32, "%02d: %s", pNumber + 1, mPlug->GetPresetName(pNumber));

        IColor colorBg = IColor(50, 0, 0, 0);
        IColor colorFrame = IColor(50, 255, 255, 255);
        g.FillRoundRect(colorBg, mRECT, 5.f);
        g.DrawRoundRect(colorFrame, mRECT, 5.f);

        if (mDisp.Get())
        {
          g.DrawText(mText, mDisp.Get(), mRECT);
        }
      }
      void OnMouseDown(float x, float y, const IMouseMod& mod) override {
        if (mod.R) {
          const char* pname = mPlug->GetPresetName(mPlug->GetCurrentPresetIdx());
          GetUI()->CreateTextEntry(*this, mText, mRECT, pname);
        }
        else {
          doPopupMenu(*GetUI());
        }
        //Redraw(); // seems to need this
        SetDirty();
      }
      void doPopupMenu(IGraphics& g) {
        const int numPresets = mPlug->NPresets();
        IPopupMenu menuMain;
        int currentPresetIdx = mPlug->GetCurrentPresetIdx();

        for (int i = 0; i < numPresets; i++) {
          const char* str = mPlug->GetPresetName(i);
          if (i == currentPresetIdx)
            menuMain.AddItem(str, i, IPopupMenu::Item::Flags::kChecked);
          else
            menuMain.AddItem(str, i, IPopupMenu::Item::Flags::kNoFlags);
        }
        menuMain.AddItem("Export Preset", numPresets);

        menuMain.SetPrefix(0);

        g.CreatePopupMenu(*this, menuMain, mRECT);
        int itemChosen = menuMain.GetChosenItemIdx();

        if (itemChosen > -1 && itemChosen < numPresets) {
          mPlug->RestorePreset(itemChosen);
          mPlug->InformHostOfProgramChange();
          mPlug->DirtyParametersFromUI();
        }
        else {
          //const int numParams = mPlug->NParams();
          //const char** enumNames = new const char*[numParams];
          //for (int i = 0; i < mPlug->NParams(); i++) {
          //  enumNames[i] = mPlug->GetParamGroupName(i);
          //}
          WDL_String filename, path;
          GetUI()->PromptForFile(filename, path, EFileAction::Save, "txt");
          mPlug->DumpPresetSrcCode(filename.Get(), mNamedParams);
        }
      }
      void TextFromTextEntry(const char* txt) {
        WDL_String safeName;
        safeName.Set(txt, MAX_PRESET_NAME_LEN);

        mPlug->ModifyCurrentPreset(safeName.Get());
        mPlug->InformHostOfProgramChange();
        mPlug->DirtyParametersFromUI();
        SetDirty(false);
      }
    private:
      WDL_String mDisp;
      IPluginBase *mPlug;
      const char** mNamedParams;
    };
    */

    // TODO: Draw with PathConvexShape from ptr to member array updated from Updatefunction
    class SRGraphBase
      : public IControl
      , public IVectorBase
    {
    public:
      SRGraphBase(IRECT bounds, int numValues, double* values, const IVStyle& style = DEFAULT_STYLE)
        : IControl(bounds, -1)
        , IVectorBase(style)
        , mValues(values)
        , mNumValues(numValues)
        , mX(new float[numValues])
        , mY(new float[numValues])
      {
        AttachIControl(this, ""); // TODO: Should hand label
      }

      ~SRGraphBase() {
        //delete[] mX;
        //delete[] mY;
        //delete[] mValues;
      }

      void Draw(IGraphics& g) override {
        // Fill Graph
        g.PathClear();
        g.PathMoveTo(mRECT.L, mRECT.MH());
        for (int i = 0; i < mNumValues; i++) {
          g.PathLineTo(mX[i], mY[i]);
        }
        g.PathLineTo(mRECT.R, mRECT.MH());
        g.PathClose();
        g.PathFill(GetColor(kHL));

        // Draw Graph
        g.PathClear();
        g.PathMoveTo(mRECT.L, mRECT.MH());
        for (int i = 0; i < mNumValues; i++) {
          //if (mY[i] == mRECT.T && mY[i-1] == mRECT.T || mY[i] == mRECT.B && mY[i - 1] == mRECT.B)
          //  g.PathMoveTo(mX[i], mY[i]);
          //else
          g.PathLineTo(mX[i], mY[i]);
        }
        g.PathLineTo(mRECT.R, mRECT.MH());
        g.PathStroke(GetColor(kFG), mStyle.frameThickness);
      };

      void OnResize() override {
        for (int i = 0; i < mNumValues; i++) {
          mX[i] = mRECT.L + (mRECT.W() / (mNumValues - 1.f)) * float(i);
          mY[i] = mRECT.MH() - (mRECT.H() * (float)mValues[i] * 0.5f);
        }
        SetDirty(false);
      }

      void Process(double* values) {
        mValues = values;
        for (int i = 0; i < mNumValues; i++) {
          mValues[i] = Clip<double>(mValues[i], -1., 1.);
        }
        OnResize();
      };
      //void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    private:
      //WDL_String mDisp;
      double* mValues;
      int mNumValues;
      float* mX;
      float* mY;
    };


    // TODO: Draw with PathConvexShape from ptr to member array updated from Updatefunction
    class SRFrequencyResponseMeter
      : public IControl
      , public IVectorBase
    {
    public:
      SRFrequencyResponseMeter(IRECT bounds, int numValues, double* values, double shape = 1.0, const IVStyle& style = DEFAULT_STYLE)
        : IControl(bounds, -1)
        , IVectorBase(style)
        , mValues(values)
        , mNumValues(numValues)
        , mShape(shape)
        , mPatternFill(IPattern(EPatternType::Solid))
        , mPatternStroke(IPattern(EPatternType::Solid))
      {
        mStrokeOptions.mPreserve = true;
        mFillOptions.mPreserve = true;
        mPatternStroke = IPattern(GetColor(kFG));
        mPatternFill = IPattern(GetColor(kHL));
        AttachIControl(this, ""); // TODO: shoud hand label
      }

      ~SRFrequencyResponseMeter() {}

      void Draw(IGraphics& g) override {
        g.PathClear();
        g.PathMoveTo(mRECT.L, mRECT.MH());

        for (int i = 0; i < mNumValues; i++) {
          const float y = mRECT.MH() - ((float)mValues[i] * 0.5f * mRECT.H());
          const float x = mRECT.L + ((float)i / ((float)mNumValues - 1.f)) * mRECT.W();
          g.PathLineTo(x, y);
        }

        g.PathLineTo(mRECT.R, mRECT.MH());
        //g.PathClose();
        g.PathFill(mPatternFill, mFillOptions, 0);
        g.PathStroke(mPatternStroke, 1.f, mStrokeOptions, 0);
      };

      void Process(double* values) {
        mValues = values;
        for (int i = 0; i < mNumValues; i++) {
          mValues[i] = Clip<double>(mValues[i], -1, 1);
        }
        SetDirty(false);
      };
      //void OnMouseDown(float x, float y, const IMouseMod& mod) override;
    private:
      //WDL_String mDisp;
      double* mValues;
      int mNumValues;
      double mShape;
      IStrokeOptions mStrokeOptions;
      IFillOptions mFillOptions;
      IPattern mPatternFill;
      IPattern mPatternStroke;
    };

  }
} // End namespaces