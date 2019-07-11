#pragma once
//#include "IPlug/IPlug_include_in_plug_hdr.h"
#include "IControl.h"
#include "IPlugPluginBase.h"
#include "../Utils/SRHelpers.h"
#include "config.h"
// For meter, see IVMeterControl.h
#include "IPlugQueue.h"
#include "IPlugStructs.h"
#include "Controls/IVMeterControl.h"


//#include <string>

namespace SR {
  namespace Graphics {
    namespace Layout {

      class SRCustomStyles {
      public:
        enum ECustomStyles {
          kDefault = 0,
          kKnob,
          kButton,
          kFader,
          kMeter,
          kGraph,
          kNumCustomStyles
        };

        SRCustomStyles(
          IVStyle defaultStyle = DEFAULT_STYLE,
          IVStyle knobStyle = DEFAULT_STYLE,
          IVStyle buttonStyle = DEFAULT_STYLE,
          IVStyle faderStyle = DEFAULT_STYLE,
          IVStyle meterStyle = DEFAULT_STYLE,
          IVStyle graphStyle = DEFAULT_STYLE
        )
          : mStyles{ defaultStyle, knobStyle, buttonStyle, faderStyle, meterStyle, graphStyle } {}

        IVStyle GetStyle(ECustomStyles type) const { return mStyles[type]; }
        void SetStyle(ECustomStyles type, IVStyle style) { mStyles[type] = style; }

      private:
        IVStyle mStyles[ECustomStyles::kNumCustomStyles];
      };


      // Colors
      class SRCustomColors {
      public:
        enum ECustomColors {
          kDefault = 0,
          kPluginBg,
          kPanelBg,
          kBlue,
          kRed,
          kGreen,
          kOrange,
          kYellow,
          kBlack,
          kWhite,
          kNumCustomColors
        };

        SRCustomColors(
          IVColorSpec colorSpec,
          IColor defaultColor,
          IColor pluginColor,
          IColor panelColor,
          IColor blue,
          IColor red,
          IColor green,
          IColor orange,
          IColor yellow,
          IColor black,
          IColor white
        )
          : mColorSpec(colorSpec)
          , mColors{ defaultColor, pluginColor, panelColor, blue, red, green, orange, yellow, black, white }
        {}

        IColor GetColor(ECustomColors type = ECustomColors::kDefault) const { return mColors[type]; }
        IColor GetColorWithAlpha(ECustomColors type = ECustomColors::kDefault, float alpha = 255.f) const { IColor tempcolor = mColors[type]; tempcolor.A = alpha; return tempcolor; }
        IVColorSpec GetColorSpec() const { return mColorSpec; }
        void SetColor(ECustomColors type, IColor& color) { mColors[type] = color; }
        void SetColorSpec(IVColorSpec& colorSpec) { mColorSpec = colorSpec; }

      private:
        IColor mColors[ECustomColors::kNumCustomColors];
        IVColorSpec mColorSpec;
      };



      class SRCustomTexts {
      public:

        enum ECustomTexts {
          kDefault = 0,
          kKnobLabel,
          kKnobValue,
          kButtonLabel,
          kButtonValue,
          kVersionString,
          kPresetMenu,
          kNumCustomTexts
        };

        SRCustomTexts(
          float size,
          IText defaultText,
          IText knobLabel,
          IText knobValue,
          IText buttonLabel,
          IText buttonValue,
          IText versionString,
          IText presetMenu
        )
          : mSize(size)
          , mTexts{ defaultText, knobLabel, knobValue, buttonLabel, buttonValue, versionString, presetMenu }
        {}

        IText GetText(SR::Graphics::Layout::SRCustomTexts::ECustomTexts type = SR::Graphics::Layout::SRCustomTexts::ECustomTexts::kDefault) const { return mTexts[type]; }
        void SetText(SR::Graphics::Layout::SRCustomTexts::ECustomTexts type, IText& text) { mTexts[type] = text; }

      private:
        float mSize;
        IText mTexts[ECustomTexts::kNumCustomTexts];
      };



      class SRLayout
        : public SRCustomColors
        , public SRCustomTexts
        , public SRCustomStyles
      {
      public:
        SRLayout(SRCustomColors colors, SRCustomTexts texts, SRCustomStyles styles)
          : SRCustomColors(colors)
          , SRCustomTexts(texts)
          , SRCustomStyles(styles)
        {};
        ~SRLayout() {};

        //IColor GetCustomColor(SRCustomColors::ECustomColors color = SRCustomColors::ECustomColors::kDefault) { return mColors.GetColor(color); }
        //IColor GetColorSpecColor(EVColor type) { return mColors.GetColorSpec().GetColor(type); }
        //IVColorSpec GetColorSpec() { return mColors.GetColorSpec(); }
        //IText GetCustomText(SRCustomTexts::ECustomTexts text = SRCustomTexts::ECustomTexts::kDefault) { return mTexts.GetText(text); }
        //IVStyle GetStyle() { return mStyles; }

      private:
      };


      const SRCustomColors SR_DEFAULT_CUSTOM_COLORS = SRCustomColors(
        // Color Specs
        IVColorSpec{
          DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
          DEFAULT_FGCOLOR,            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
          IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
          IColor(255, 150, 150, 150),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
          IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
          IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
          IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0))
          IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
          IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
        },
        COLOR_LIGHT_GRAY,
        IColor(255, 12, 17, 23),
        IColor(255, 50, 75, 95),
        IColor(255, 62, 100, 121),
        IColor(255, 131, 18, 18),
        IColor(255, 103, 141, 52),
        IColor(255, 234, 158, 19),
        IColor(255, 219, 181, 30),
        IColor(255, 23, 23, 23),
        IColor(255, 243, 243, 243)
        );


      const SRCustomTexts SR_DEFAULT_CUSTOM_TEXTS = SRCustomTexts(
        DEFAULT_TEXT_SIZE,
        IText(DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Top),
        IText(DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Top),
        IText(DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Bottom),
        IText(DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Bottom),
        IText(DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Bottom),
        IText(1.5f * DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Near, EVAlign::Middle),
        IText(2.f * DEFAULT_TEXT_SIZE, SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kDefault), nullptr, EAlign::Center, EVAlign::Middle)
      );


      const SRCustomStyles SR_DEFAULT_CUSTOM_STYLE = SRCustomStyles(
        // DEFAULT:
        IVStyle(
          true,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            DEFAULT_FGCOLOR,            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150), // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kKnobLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kKnobValue),
          true,
          true,
          true,
          0.1f, // roundness
          2.f, // frame-thick, def: 1.f
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          ),

        // KNOB:
        IVStyle(
          true,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            DEFAULT_FGCOLOR,            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150), // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kKnobLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kKnobValue),
          true,
          true,
          true,
          0.1f, // roundness
          2.f, // frame-thick, def: 1.f
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          ),

        // BUTTON:
        IVStyle(
          false,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            IColor(255, 30, 50, 70),            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonValue),
          true,
          false,
          true,
          0.5f, // roundness
          2.f, // frame-thick
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          ),

        // FADER:
        IVStyle(
          false,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kPluginBg),            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonValue),
          true,
          false,
          true,
          0.5f, // roundness
          2.f, // frame-thick
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          ),

        // METER:
        IVStyle(
          false,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kPluginBg),            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            IColor(255, 234, 158, 19), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            IColor(30, 255, 255, 255),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            IColor(255, 234, 158, 19),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonValue),
          true,
          false,
          true,
          0.5f, // roundness
          2.f, // frame-thick
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          ),
              
        // GRAPH:
        IVStyle(
          false,
          true,
          {
            DEFAULT_BGCOLOR,            // Background (DEFAULT_BGCOLOR = COLOR_TRANSPARENT(0, 0, 0, 0))
            SR_DEFAULT_CUSTOM_COLORS.GetColor(SRCustomColors::ECustomColors::kPluginBg),            // Foreground (DEFAULT_FGCOLOR = COLOR_MID_GRAY(255, 200, 200, 200))
            SR_DEFAULT_CUSTOM_COLORS.GetColorWithAlpha(SRCustomColors::ECustomColors::kGreen, 60.f), // Pressed    (DEFAULT_PRCOLOR = COLOR_LIGHT_GRAY(255, 240, 240, 240))
            IColor(255, 150, 150, 150),    // Frame      (DEFAULT_FRCOLOR = COLOR_DARK_GRAY(255, 70, 70, 70))
            SR_DEFAULT_CUSTOM_COLORS.GetColorWithAlpha(SRCustomColors::ECustomColors::kGreen, 30.f),  // Higlight   (DEFAULT_HLCOLOR = COLOR_TRANSLUCENT(10, 0, 0, 0))
            IColor(100, 0, 0, 0),       // Shadow     (DEFAULT_SHCOLOR = IColor(60, 0, 0, 0)
            SR_DEFAULT_CUSTOM_COLORS.GetColorWithAlpha(SRCustomColors::ECustomColors::kGreen, 60.f),  // Extra 1    (DEFAULT_X1COLOR = COLOR_RED(255, 255, 0, 0)
            IColor(255, 48, 166, 186),  // Extra 2    (DEFAULT_X2COLOR = COLOR_GREEN(255, 0, 255, 0))
            IColor(255, 249, 206, 34),  // Extra 3    (DEFAULT_X3COLOR = COLOR_BLUE(255, 0, 0, 255))
          },
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonLabel),
          SR_DEFAULT_CUSTOM_TEXTS.GetText(SRCustomTexts::ECustomTexts::kButtonValue),
          true,
          false,
          true,
          0.5f, // roundness
          2.f, // frame-thick
          3.f, // shadow-off
          DEFAULT_WIDGET_FRAC,
          DEFAULT_WIDGET_ANGLE
          )
      );



      const SRLayout SR_DEFAULT_LAYOUT = SRLayout(
        SR_DEFAULT_CUSTOM_COLORS,
        SR_DEFAULT_CUSTOM_TEXTS,
        SR_DEFAULT_CUSTOM_STYLE
      );

      const SRLayout layout = SR_DEFAULT_LAYOUT;


      // Maybe we can make something out of this?
      struct SRStyle
        : public IVStyle {
        SRStyle(bool showLabel = DEFAULT_SHOW_LABEL,
          bool showValue = DEFAULT_SHOW_VALUE,
          const std::initializer_list<IColor>& colors = { DEFAULT_BGCOLOR, DEFAULT_FGCOLOR, DEFAULT_PRCOLOR, DEFAULT_FRCOLOR, DEFAULT_HLCOLOR, DEFAULT_SHCOLOR, DEFAULT_X1COLOR, DEFAULT_X2COLOR, DEFAULT_X3COLOR },
          const IText& labelText = DEFAULT_LABEL_TEXT,
          const IText& valueText = DEFAULT_VALUE_TEXT,
          bool hideCursor = DEFAULT_HIDE_CURSOR,
          bool drawFrame = DEFAULT_DRAW_FRAME,
          bool drawShadows = DEFAULT_DRAW_SHADOWS,
          float roundness = DEFAULT_ROUNDNESS,
          float frameThickness = DEFAULT_FRAME_THICKNESS,
          float shadowOffset = DEFAULT_SHADOW_OFFSET,
          float widgetFrac = DEFAULT_WIDGET_FRAC,
          float angle = DEFAULT_WIDGET_ANGLE)
          : IVStyle(showLabel, showValue, colors, labelText, valueText, hideCursor, drawFrame, drawShadows, roundness, frameThickness, shadowOffset, widgetFrac, angle)
        {}
      };


    }

    namespace Base {

      class SRRoomInfo {
      public:
        SRRoomInfo(float lightX = 0.f, float lightY = 0.f, float lightZ = 0.f, float plugHeight = 0.f, float plugWidth = 0.f)

        {
          Reset(lightX, lightY, lightZ);
        }
        ~SRRoomInfo() {};

        void Reset(float lightX = 0.f, float lightY = 0.f, float lightZ = 0.f, float plugHeight = 0.f, float plugWidth = 0.f) {
          mPlugHeight = plugHeight;
          mPlugWidth = plugWidth;
          mLightX = lightX;
          mLightY = lightY;
          mLightZ = lightZ;
        }

        float GetLightPositionX() { return mLightX; }
        float GetLightPositionY() { return mLightY; }
        float GetLightPositionZ() { return mLightZ; }

        float GetShadowOffsetX(float cx, float height) { return (cx - mLightX) * height / mLightZ; }
        float GetShadowOffsetY(float cy, float height) { return (cy - mLightY) * height / mLightZ; }
        IShadow GetShadowForPart(float cx, float cy, float height) {
          return IShadow(EPatternType::Solid, 1.f, GetShadowOffsetX(cx, height), GetShadowOffsetY(cy, height), 0.5f, true);
        }
      private:
        float mPlugHeight;
        float mPlugWidth;
        float mLightX;
        float mLightY;
        float mLightZ;
      };

      const SRRoomInfo SR_DEFAULT_ROOMINFO = SRRoomInfo();

      /** A base interface to be combined with IControl for vectorial controls "IVControls", in order for them to share a common style
   * If you need more flexibility, you're on your own! */
      class SRVectorBase
        : public IVectorBase
      {
      public:
        SRVectorBase(const IColor* pBGColor = &DEFAULT_BGCOLOR,
          const IColor* pFGColor = &DEFAULT_FGCOLOR,
          const IColor* pPRColor = &DEFAULT_PRCOLOR,
          const IColor* pFRColor = &DEFAULT_FRCOLOR,
          const IColor* pHLColor = &DEFAULT_HLCOLOR,
          const IColor* pSHColor = &DEFAULT_SHCOLOR,
          const IColor* pX1Color = &DEFAULT_X1COLOR,
          const IColor* pX2Color = &DEFAULT_X2COLOR,
          const IColor* pX3Color = &DEFAULT_X3COLOR)
          : IVectorBase(pBGColor, pFGColor, pPRColor, pFRColor, pHLColor, pSHColor, pX1Color, pX2Color, pX3Color)
        {
          AddColors(pBGColor, pFGColor, pPRColor, pFRColor, pHLColor, pSHColor, pX1Color, pX2Color, pX3Color);
        }

        SRVectorBase(const IVStyle& style, bool labelInWidget = false, bool valueInWidget = false, SRRoomInfo roomInfo = SR_DEFAULT_ROOMINFO)
          : IVectorBase(style, labelInWidget, valueInWidget)
          , mRoomInfo(roomInfo)
        {
          SetStyle(style);
        }

        //void AttachIControl(IControl* pControl, const char* label)
        //{
        //  mControl = pControl;
        //  mLabelStr.Set(label);
        //}

        //void AddColor(const IColor& color)
        //{
        //  mColors.Add(color);
        //}

        //void AddColors(const IColor* pBGColor = 0,
        //  const IColor* pFGColor = 0,
        //  const IColor* pPRColor = 0,
        //  const IColor* pFRColor = 0,
        //  const IColor* pHLColor = 0,
        //  const IColor* pSHColor = 0,
        //  const IColor* pX1Color = 0,
        //  const IColor* pX2Color = 0,
        //  const IColor* pX3Color = 0)
        //{
        //  if (pBGColor) AddColor(*pBGColor);
        //  if (pFGColor) AddColor(*pFGColor);
        //  if (pPRColor) AddColor(*pPRColor);
        //  if (pFRColor) AddColor(*pFRColor);
        //  if (pHLColor) AddColor(*pHLColor);
        //  if (pSHColor) AddColor(*pSHColor);
        //  if (pX1Color) AddColor(*pX1Color);
        //  if (pX2Color) AddColor(*pX2Color);
        //  if (pX3Color) AddColor(*pX3Color);
        //}

        //void SetColor(int colorIdx, const IColor& color)
        //{
        //  if (colorIdx < mColors.GetSize())
        //    mColors.Get()[colorIdx] = color;

        //  mControl->SetDirty(false);
        //}

        //void SetColors(const IColor& BGColor,
        //  const IColor& FGColor,
        //  const IColor& PRColor,
        //  const IColor& FRColor,
        //  const IColor& HLColor,
        //  const IColor& SHColor,
        //  const IColor& X1Color,
        //  const IColor& X2Color,
        //  const IColor& X3Color)
        //{
        //  mColors.Get()[kBG] = BGColor;
        //  mColors.Get()[kFG] = FGColor;
        //  mColors.Get()[kPR] = PRColor;
        //  mColors.Get()[kFR] = FRColor;
        //  mColors.Get()[kHL] = HLColor;
        //  mColors.Get()[kSH] = SHColor;
        //  mColors.Get()[kX1] = X1Color;
        //  mColors.Get()[kX2] = X2Color;
        //  mColors.Get()[kX3] = X3Color;
        //}

        //void SetColors(const IVColorSpec& spec)
        //{
        //  SetColors(spec.GetColor(kBG),
        //    spec.GetColor(kFG),
        //    spec.GetColor(kPR),
        //    spec.GetColor(kFR),
        //    spec.GetColor(kHL),
        //    spec.GetColor(kSH),
        //    spec.GetColor(kX1),
        //    spec.GetColor(kX2),
        //    spec.GetColor(kX3));
        //}

        //const IColor& GetColor(int colorIdx) const
        //{
        //  if (colorIdx < mColors.GetSize())
        //    return mColors.Get()[colorIdx];
        //  else
        //    return mColors.Get()[0];
        //}

        //void SetLabelStr(const char* label) { mLabelStr.Set(label); mControl->SetDirty(false); }
        //void SetValueStr(const char* value) { mValueStr.Set(value); mControl->SetDirty(false); }
        //void SetWidgetFrac(float frac) { mStyle.widgetFrac = Clip(frac, 0.f, 1.f);  mControl->OnResize(); mControl->SetDirty(false); }
        //void SetAngle(float angle) { mStyle.angle = Clip(angle, 0.f, 360.f);  mControl->SetDirty(false); }
        //void SetShowLabel(bool show) { mStyle.showLabel = show;  mControl->OnResize(); mControl->SetDirty(false); }
        //void SetShowValue(bool show) { mStyle.showValue = show;  mControl->OnResize(); mControl->SetDirty(false); }
        //void SetRoundness(float roundness) { mStyle.roundness = Clip(roundness, 0.f, 1.f); mControl->SetDirty(false); }
        //void SetDrawFrame(bool draw) { mStyle.drawFrame = draw; mControl->SetDirty(false); }
        //void SetDrawShadows(bool draw) { mStyle.drawShadows = draw; mControl->SetDirty(false); }
        //void SetShadowOffset(float offset) { mStyle.shadowOffset = offset; mControl->SetDirty(false); }
        //void SetFrameThickness(float thickness) { mStyle.frameThickness = thickness; mControl->SetDirty(false); }
        //void SetSplashRadius(float radius) { mSplashRadius = radius * mMaxSplashRadius; }

        //void SetStyle(const IVStyle& style)
        //{
        //  mStyle = style;
        //  mColors.Resize(kNumDefaultVColors); // TODO?
        //  SetColors(style.colorSpec);
        //}

      //  IRECT GetAdjustedHandleBounds(IRECT handleBounds) const
      //  {
      //    if (mStyle.drawFrame)
      //      handleBounds.Pad(-0.5f * mStyle.frameThickness);

      //    if (mStyle.drawShadows)
      //      handleBounds.Alter(0, 0, -mStyle.shadowOffset, -mStyle.shadowOffset);

      //    return handleBounds;
      //  }

      //  float GetRoundedCornerRadius(const IRECT& bounds) const
      //  {
      //    if (bounds.W() < bounds.H())
      //      return mStyle.roundness * (bounds.W() / 2.f);
      //    else
      //      return mStyle.roundness * (bounds.H() / 2.f);
      //  }

      //  void DrawSplash(IGraphics& g)
      //  {
      //    float mouseDownX, mouseDownY;
      //    g.GetMouseDownPoint(mouseDownX, mouseDownY);
      //    g.FillCircle(GetColor(kHL), mouseDownX, mouseDownY, mSplashRadius);
      //  }

      //  virtual void DrawBackGround(IGraphics& g, const IRECT& rect)
      //  {
      //    g.FillRect(GetColor(kBG), rect);
      //  }

      //  virtual void DrawWidget(IGraphics& g)
      //  {
      //    // no-op
      //  }

      //  virtual void DrawLabel(IGraphics& g)
      //  {
      //    if (mLabelBounds.H() && mStyle.showLabel)
      //      g.DrawText(mStyle.labelText, mLabelStr.Get(), mLabelBounds);
      //  }

      //  virtual void DrawValue(IGraphics& g, bool mouseOver)
      //  {
      //    if (mouseOver)
      //      g.FillRect(COLOR_TRANSLUCENT, mValueBounds);

      //    if (mStyle.showValue)
      //      g.DrawText(mStyle.valueText, mValueStr.Get(), mValueBounds);
      //  }

        void DrawHandle(IGraphics& g, EVShape shape, const IRECT& bounds, bool pressed, bool mouseOver)
        {
          switch (shape)
          {
          case EVShape::Ellipse:
            DrawPressableEllipse(g, bounds, pressed, mouseOver);
            break;
          case EVShape::Rectangle:
            DrawPressableRectangle(g, bounds, pressed, mouseOver);
            break;
          case EVShape::Triangle:
            DrawPressableTriangle(g, bounds, pressed, mouseOver, mStyle.angle);
            break;
          case EVShape::EndsRounded:
            DrawPressableRectangle(g, bounds, pressed, mouseOver, true, true, false, false);
            break;
          case EVShape::AllRounded:
            DrawPressableRectangle(g, bounds, pressed, mouseOver, true, true, true, true);
          default:
            break;
          }
        }

        void DrawPressableCircle(IGraphics&g, const IRECT& bounds, float radius, bool pressed, bool mouseOver)
        {
          const float cx = bounds.MW(), cy = bounds.MH();

          if (!pressed && mStyle.drawShadows)
            g.FillCircle(GetColor(kSH), cx + mStyle.shadowOffset, cy + mStyle.shadowOffset, radius);

          //    if(pressed)
          //      g.DrawCircle(GetColor(kON), cx, cy, radius * 0.9f, 0, mStyle.frameThickness);
          //    else
          g.FillCircle(GetColor(kFG), cx, cy, radius);

          if (mouseOver)
            g.FillCircle(GetColor(kHL), cx, cy, radius * 0.8f);

          if (mStyle.drawFrame)
            g.DrawCircle(GetColor(kFR), cx, cy, radius, 0, mStyle.frameThickness);
        }

        void DrawPressableEllipse(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver)
        {
          if (!pressed && mStyle.drawShadows)
            g.FillEllipse(GetColor(kSH), bounds.GetTranslated(mStyle.shadowOffset, mStyle.shadowOffset));

          if (pressed)
            g.FillEllipse(GetColor(kON), bounds);
          else
            g.FillEllipse(GetColor(kFG), bounds);

          if (mouseOver)
            g.FillEllipse(GetColor(kHL), bounds);

          if (mStyle.drawFrame)
            g.DrawEllipse(GetColor(kFR), bounds, nullptr, mStyle.frameThickness);
        }

        /** /todo
         @param IGraphics&g /todo
         @param bounds /todo
         @param pressed /todo
         @param mouseOver /todo
         @return /todo */
        IRECT DrawPressableRectangle(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver,
          bool roundTopLeft = true, bool roundTopRight = true, bool roundBottomLeft = true, bool roundBottomRight = true)
        {
          IRECT handleBounds = GetAdjustedHandleBounds(bounds);
          float cR = GetRoundedCornerRadius(handleBounds);

          const float topLeftR = roundTopLeft ? cR : 0.f;
          const float topRightR = roundTopRight ? cR : 0.f;
          const float bottomLeftR = roundBottomLeft ? cR : 0.f;
          const float bottomRightR = roundBottomRight ? cR : 0.f;

          if (pressed)
            g.FillRoundRect(GetColor(kPR), handleBounds, topLeftR, topRightR, bottomLeftR, bottomRightR);
          else
          {
            //outer shadow
            if (mStyle.drawShadows)
              g.FillRoundRect(GetColor(kSH), handleBounds.GetTranslated(
                mRoomInfo.GetShadowOffsetX(handleBounds.L + handleBounds.W() * 0.5f, handleBounds.GetLengthOfShortestSide()),
                mRoomInfo.GetShadowOffsetY(handleBounds.T + handleBounds.H() * 0.5f, handleBounds.GetLengthOfShortestSide())),
                topLeftR, topRightR, bottomLeftR, bottomRightR);

            g.FillRoundRect(GetColor(kFG), handleBounds, topLeftR, topRightR, bottomLeftR, bottomRightR);
          }

          if (mouseOver)
            g.FillRoundRect(GetColor(kHL), handleBounds, topLeftR, topRightR, bottomLeftR, bottomRightR);

          if (mControl->GetAnimationFunction())
            DrawSplash(g);

          if (mStyle.drawFrame)
            g.DrawRoundRect(GetColor(kFR), handleBounds, topLeftR, topRightR, bottomLeftR, bottomRightR, 0, mStyle.frameThickness);

          return handleBounds;
        }

        /** Draw a triangle-shaped vector button
         * @param g The IGraphics context used for drawing
         * @param bounds Where to draw the button
         * @param pressed Whether to draw the button pressed or unpressed
         * @param mouseOver Whether mouse is currently hovering on control */
        IRECT DrawPressableTriangle(IGraphics&g, const IRECT& bounds, bool pressed, bool mouseOver, float angle)
        {
          float x1, x2, x3, y1, y2, y3;

          float theta = DegToRad(angle);

          IRECT handleBounds = GetAdjustedHandleBounds(bounds);

          // Center bounds around origin for rotation
          float xT = handleBounds.L + handleBounds.W() * 0.5f;
          float yT = handleBounds.T + handleBounds.H() * 0.5f;
          IRECT centered = handleBounds.GetTranslated(-xT, -yT);

          // Do rotation and translate points back into view space
          float c = cosf(theta);
          float s = sinf(theta);
          x1 = centered.L * c - centered.B * s + xT;
          y1 = centered.L * s + centered.B * c + yT;
          x2 = centered.MW() * c - centered.T * s + xT;
          y2 = centered.MW() * s + centered.T * c + yT;
          x3 = centered.R * c - centered.B * s + xT;
          y3 = centered.R * s + centered.B * c + yT;

          if (pressed)
            g.FillTriangle(GetColor(kPR), x1, y1, x2, y2, x3, y3);
          else
          {
            //outer shadow
            if (mStyle.drawShadows)
              g.FillTriangle(GetColor(kSH), x1 + mStyle.shadowOffset, y1 + mStyle.shadowOffset, x2 + mStyle.shadowOffset, y2 + mStyle.shadowOffset, x3 + mStyle.shadowOffset, y3 + mStyle.shadowOffset);

            g.FillTriangle(GetColor(kFG), x1, y1, x2, y2, x3, y3);
          }

          if (mouseOver)
            g.FillTriangle(GetColor(kHL), x1, y1, x2, y2, x3, y3);

          if (mControl->GetAnimationFunction())
            DrawSplash(g);

          if (mStyle.drawFrame)
            g.DrawTriangle(GetColor(kFR), x1, y1, x2, y2, x3, y3, 0, mStyle.frameThickness);

          return handleBounds;
        }


        // MAKERECTS
        // ---------

        IRECT MakeRects(const IRECT& parent, bool hasHandle = false)
        {
          IRECT clickableArea = parent;

          // Hannes:
          bool labelValueSame = false;
          if (mStyle.labelText.mVAlign == mStyle.valueText.mVAlign) {
            labelValueSame = true;
          }



          if (!mLabelInWidget)
          {
            if (mStyle.showLabel && CStringHasContents(mLabelStr.Get()))
            {
              IRECT textRect;
              mControl->GetUI()->MeasureText(mStyle.labelText, mLabelStr.Get(), textRect);

              if (mStyle.labelText.mVAlign == EVAlign::Bottom) {
                mLabelBounds = parent.GetFromBottom(textRect.H());
                clickableArea = parent.GetReducedFromBottom(mLabelBounds.H());
              }
              else {
                mLabelBounds = parent.GetFromTop(textRect.H());
                clickableArea = parent.GetReducedFromTop(mLabelBounds.H());
              }
            }
            else
              mLabelBounds = IRECT();
          }

          if (mStyle.showValue && !mValueInWidget)
          {
            IRECT textRect;
            if (CStringHasContents(mValueStr.Get()))
              mControl->GetUI()->MeasureText(mStyle.valueText, mValueStr.Get(), textRect);

            const float valueDisplayWidth = textRect.W() * 0.5f;

            switch (mStyle.valueText.mVAlign)
            {
            case EVAlign::Middle:
              mValueBounds = clickableArea.GetMidVPadded(textRect.H() / 2.f).GetMidHPadded(valueDisplayWidth);
              mWidgetBounds = clickableArea.GetScaledAboutCentre(mStyle.widgetFrac);
              break;
            case EVAlign::Bottom:
            {
              mValueBounds = clickableArea.GetFromBottom(textRect.H()).GetMidHPadded(valueDisplayWidth);
              mWidgetBounds = clickableArea.GetReducedFromBottom(textRect.H()).GetScaledAboutCentre(mStyle.widgetFrac);
              break;
            }
            case EVAlign::Top:
              mValueBounds = clickableArea.GetFromTop(textRect.H()).GetMidHPadded(valueDisplayWidth);
              mWidgetBounds = clickableArea.GetReducedFromTop(textRect.H()).GetScaledAboutCentre(mStyle.widgetFrac);
              break;
            default:
              break;
            }
          }
          else
          {
            mWidgetBounds = clickableArea.GetScaledAboutCentre(mStyle.widgetFrac);
          }

          if (hasHandle)
            mWidgetBounds = GetAdjustedHandleBounds(clickableArea).GetScaledAboutCentre(mStyle.widgetFrac);

          if (mLabelInWidget)
            mLabelBounds = mWidgetBounds;

          if (mValueInWidget)
            mValueBounds = mWidgetBounds;

          return clickableArea;
        }

      protected:
      //  IControl* mControl = nullptr;
      //  WDL_TypedBuf<IColor> mColors;
      //  IVStyle mStyle;
      //  bool mLabelInWidget = false;
      //  bool mValueInWidget = false;
      //  float mSplashRadius = 0.f;
      //  float mMaxSplashRadius = 50.f;
      //  IRECT mWidgetBounds; // The knob/slider/button
      //  IRECT mLabelBounds; // A piece of text above the control
      //  IRECT mValueBounds; // Text below the contol, usually displaying the value of a parameter
      //  WDL_String mLabelStr;
      //  WDL_String mValueStr;

      //  // Hannes:
        SRRoomInfo mRoomInfo;
      };





    }

    namespace Controls {


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
      class SRVectorKnob
        : public IKnobControlBase
        , public Base::SRVectorBase
      {
      public:
        SRVectorKnob(const IRECT& bounds, int paramIdx,
          const char* label = "",
          const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& lightInfo = Base::SRRoomInfo(), const IColor& uniqueColor = DEFAULT_FGCOLOR,
          bool valueIsEditable = false, bool valueInWidget = false,
          float aMin = -135.f, float aMax = 135.f, float aAnchor = -135.f,
          EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING);

        SRVectorKnob(const IRECT& bounds, IActionFunction actionFunction,
          const char* label = "",
          const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& lightInfo = Base::SRRoomInfo(), const IColor& uniqueColor = DEFAULT_FGCOLOR,
          bool valueIsEditable = false, bool valueInWidget = false,
          float aMin = -135.f, float aMax = 135.f, float aAnchor = -135.f,
          EDirection direction = EDirection::Vertical, double gearing = DEFAULT_GEARING);

        virtual ~SRVectorKnob() {}

        void Draw(IGraphics& g) override;
        virtual void DrawWidget(IGraphics& g) override;
        virtual void DrawLabel(IGraphics & g) override;
        virtual void DrawValue(IGraphics & g, bool mouseOver) override;

        void OnMouseDown(float x, float y, const IMouseMod& mod) override;
        void OnMouseUp(float x, float y, const IMouseMod& mod) override;
        void OnMouseOver(float x, float y, const IMouseMod& mod) override;
        void OnMouseOut() override { mValueMouseOver = false; IKnobControlBase::OnMouseOut(); }

        //  void OnMouseDblClick(float x, float y, const IMouseMod& mod) override {  OnMouseDown(x, y, mod); }
        void OnResize() override;
        bool IsHit(float x, float y) const override;
        void SetDirty(bool push, int valIdx = kNoValIdx) override;
        void OnInit() override;
        //void DrawPressableCircle(IGraphics&g, const IRECT& bounds, float radius, bool pressed, bool mouseOver);



      protected:
        IColor mUniqueColor;
        float mAngleMin, mAngleMax;
        float mAnchorAngle; // for bipolar arc
        bool mValueMouseOver = false;
      };


      SRVectorKnob::SRVectorKnob(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, Base::SRRoomInfo& lightInfo, const IColor& uniqueColor, bool valueIsEditable, bool valueInWidget, float aMin, float aMax, float aAnchor, EDirection direction, double gearing)
        : IKnobControlBase(bounds, paramIdx, direction, gearing)
        , SRVectorBase(style, false, valueInWidget, lightInfo)
        , mAngleMin(aMin)
        , mAngleMax(aMax)
        , mAnchorAngle(aAnchor)
        , mUniqueColor(uniqueColor)
      {
        DisablePrompt(!valueIsEditable);
        mText = style.valueText;
        AttachIControl(this, label);

      }

      SRVectorKnob::SRVectorKnob(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, Base::SRRoomInfo& lightInfo, const IColor& uniqueColor, bool valueIsEditable, bool valueInWidget, float aMin, float aMax, float aAnchor, EDirection direction, double gearing)
        : IKnobControlBase(bounds, kNoParameter, direction, gearing)
        , SRVectorBase(style, false, valueInWidget, lightInfo)
        , mAngleMin(aMin)
        , mAngleMax(aMax)
        , mAnchorAngle(aAnchor)
        , mUniqueColor(uniqueColor)
      {
        DisablePrompt(!valueIsEditable);
        mText = style.valueText;
        SetActionFunction(actionFunc);
        AttachIControl(this, label);
      }

      void SRVectorKnob::Draw(IGraphics& g)
      {
        DrawBackGround(g, mRECT);
        DrawLabel(g);
        DrawWidget(g);
        DrawValue(g, mValueMouseOver);
      }

      void SRVectorKnob::DrawWidget(IGraphics& g)
      {
        const float radius = (mWidgetBounds.W() > mWidgetBounds.H())
          ? (mWidgetBounds.H() / 2.f) * 0.8f
          : (mWidgetBounds.W() / 2.f) * 0.8f;
        const float cx = mWidgetBounds.MW(), cy = mWidgetBounds.MH();
        const IRECT knobRect = { cx - radius, cy - radius, cx + radius, cy + radius };
        const float shadowOffsetX = mRoomInfo.GetShadowOffsetX(cx, radius);
        const float shadowOffsetY = mRoomInfo.GetShadowOffsetY(cy, radius);
        const float frameThickness = radius * 0.1 * mStyle.frameThickness;


        if (!IsGrayed())
        {
          /*TODO: constants! */
          const float v = mAngleMin + ((float)GetValue() * (mAngleMax - mAngleMin));

          // FRAME
          if (mStyle.drawFrame) {
            g.DrawCircle((!mMouseDown) ? GetColor(kFR) : GetColor(kX1), cx, cy, radius, 0, frameThickness);
          }

          // ARC
          g.DrawArc(mUniqueColor, cx, cy, (radius)+3.f, v >= mAnchorAngle ? mAnchorAngle : mAnchorAngle - (mAnchorAngle - v), v >= mAnchorAngle ? v : mAnchorAngle, 0, 2.f);

          // WIDGET

          // SHADOW
          if (mStyle.drawShadows)
            //g.FillCircle(GetColor(kSH), cx + mRoomInfo.GetShadowOffsetX(cx, radius * 0.1f), cy + mRoomInfo.GetShadowOffsetY(cy, radius * 0.1f), radius);
            g.FillEllipse(
              GetColor(kSH),
              knobRect.GetPadded(
              (shadowOffsetX < 0.f) ? -shadowOffsetX : 0.f,
                (shadowOffsetY < 0.f) ? -shadowOffsetY : 0.f,
                (shadowOffsetX > 0.f) ? shadowOffsetX : 0.f,
                (shadowOffsetY > 0.f) ? shadowOffsetY : 0.f
              )
            );

          // KNOB
          g.FillCircle(mUniqueColor, cx, cy, radius);

          // INNER RING
          g.DrawCircle(GetColor(kHL), cx, cy, radius * 0.9f);

          // ARROW
          if (mStyle.drawShadows)
            g.DrawRadialLine(GetColor(kSH), cx + shadowOffsetX * 0.1f, cy + shadowOffsetY * 0.1f, v, 0.0f, radius, 0, frameThickness >= 1.f ? frameThickness : 1.f);
          g.DrawRadialLine(GetColor(kFR), cx, cy, v, 0.0f, radius, 0, frameThickness >= 1.f ? frameThickness : 1.f);


          // LIGHTPATTERN, Should be optimized
          const IPattern patternLight = IPattern::CreateRadialGradient(
            cx - shadowOffsetX,
            cy - shadowOffsetY,
            radius,
            {
              IColorStop(IColor(200, 255, 200, 200), 0.1f),
              IColorStop(COLOR_TRANSLUCENT, 0.9f)
            }
          );
          g.PathCircle(cx, cy, radius);
          g.PathFill(patternLight);



          // MOUSEOVER
          if (mMouseIsOver & !mValueMouseOver)
            g.FillCircle(GetColor(kHL), cx, cy, radius);





        }
        else
        {
          // GRAYED
          g.FillCircle(GetColor(kOFF), cx, cy, radius);
        }
      }

      void SRVectorKnob::DrawLabel(IGraphics& g)
      {
        if (mLabelBounds.H() && mStyle.showLabel)
          g.DrawText(mStyle.labelText, mLabelStr.Get(), mLabelBounds);
      }

      void SRVectorKnob::DrawValue(IGraphics& g, bool mouseOver)
      {
        if (mouseOver)
          g.FillRect(COLOR_TRANSLUCENT, mValueBounds);

        if (mStyle.showValue)
          g.DrawText(mStyle.valueText, mValueStr.Get(), mValueBounds);
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





      class SRVectorSwitch
        : public ISwitchControlBase
        , public Base::SRVectorBase
      {
      public:
        SRVectorSwitch(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& roomInfo = Base::SRRoomInfo(), bool valueInButton = true);

        SRVectorSwitch(const IRECT& bounds, IActionFunction actionFunc = SplashClickActionFunc, const char* label = "", const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& roomInfo = Base::SRRoomInfo(), int numStates = 2, bool valueInButton = true);

        void Draw(IGraphics& g) override;
        virtual void DrawWidget(IGraphics& g) override;
        bool IsHit(float x, float y) const override;
        void SetDirty(bool push, int valIdx = kNoValIdx) override;
        void OnResize() override;
        void OnInit() override;
      };


      SRVectorSwitch::SRVectorSwitch(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, Base::SRRoomInfo& roomInfo, bool valueInButton)
        : ISwitchControlBase(bounds, paramIdx, DefaultClickActionFunc)
        , SRVectorBase(style, false, valueInButton, roomInfo)
      {
        AttachIControl(this, label);
        mText = style.valueText;

        if (valueInButton)
          mText.mVAlign = mStyle.valueText.mVAlign = EVAlign::Middle;

        mDblAsSingleClick = true;
      }

      SRVectorSwitch::SRVectorSwitch(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, Base::SRRoomInfo& roomInfo, int numStates, bool valueInButton)
        : ISwitchControlBase(bounds, kNoParameter, actionFunc, numStates)
        , SRVectorBase(style, false, valueInButton, roomInfo)
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

      void SRVectorSwitch::DrawWidget(IGraphics& g) {
        DrawPressableRectangle(g, mWidgetBounds, mMouseDown, mMouseIsOver);
      }

      void SRVectorSwitch::SetDirty(bool push, int valIdx) {
        IControl::SetDirty(push);

        const IParam* pParam = GetParam();

        if (pParam)
          pParam->GetDisplayForHost(mValueStr);
      }

      void SRVectorSwitch::OnResize() {
        SetTargetRECT(MakeRects(mRECT, true));
        SetDirty(false);
      }

      bool SRVectorSwitch::IsHit(float x, float y) const {
        return mWidgetBounds.Contains(x, y);
      }

      void SRVectorSwitch::OnInit() {
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
      class SRVectorToggle
        : public SRVectorSwitch
      {
      public:
        SRVectorToggle(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& roomInfo = Base::SRRoomInfo(), const char* offText = "OFF", const char* onText = "ON");

        SRVectorToggle(const IRECT& bounds, IActionFunction actionFunc = SplashClickActionFunc, const char* label = "", const IVStyle& style = DEFAULT_STYLE, Base::SRRoomInfo& roomInfo = Base::SRRoomInfo(), const char* offText = "OFF", const char* onText = "ON", bool initialState = false);

        void DrawValue(IGraphics& g, bool mouseOver) override;
        void DrawWidget(IGraphics& g) override;
      protected:
        WDL_String mOffText;
        WDL_String mOnText;
      };

      SRVectorToggle::SRVectorToggle(const IRECT& bounds, int paramIdx, const char* label, const IVStyle& style, Base::SRRoomInfo& roomInfo, const char* offText, const char* onText)
        : SRVectorSwitch(bounds, paramIdx, label, style, roomInfo, true)
        , mOnText(onText)
        , mOffText(offText)
      {
        //TODO: assert boolean?
      }

      SRVectorToggle::SRVectorToggle(const IRECT& bounds, IActionFunction actionFunc, const char* label, const IVStyle& style, Base::SRRoomInfo& roomInfo, const char* offText, const char* onText, bool initialState)
        : SRVectorSwitch(bounds, actionFunc, label, style, roomInfo, 2, true)
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
          mText.mSize = std::max(12.f, std::min(mRECT.W() * 0.25f, mRECT.H() / mNumLabels));
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

      // Preset Menu
  // -----------------------------------

      class SRPresetMenu
        : public IControl {
      public:
        SRPresetMenu(IRECT bounds, IText pText, const char** pNamedParams)
          : IControl(bounds, -1) {
          mTextEntryLength = MAX_PRESET_NAME_LEN - 3;
          mText = pText;
          mNamedParams = pNamedParams;
        }
        void Draw(IGraphics& g) override {
          auto* pPluginBase = static_cast<::IPluginBase*>(GetDelegate());
          int pNumber = pPluginBase->GetCurrentPresetIdx();

          mDisp.SetFormatted(32, "%02d: %s", pNumber + 1, pPluginBase->GetPresetName(pNumber));

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
          auto* pPluginBase = static_cast<::IPluginBase*>(GetDelegate());
          if (mod.R) {
            const char* pname = pPluginBase->GetPresetName(pPluginBase->GetCurrentPresetIdx());
            GetUI()->CreateTextEntry(*this, mText, mRECT, pname);
          }
          else {
            doPopupMenu(*GetUI());
          }
          //Redraw(); // seems to need this
          SetDirty();
        }
        void doPopupMenu(IGraphics& g) {
          auto* pPluginBase = static_cast<::IPluginBase*>(GetDelegate());

          const int numPresets = pPluginBase->NPresets();
          IPopupMenu menuMain;
          int currentPresetIdx = pPluginBase->GetCurrentPresetIdx();

          for (int i = 0; i < numPresets; i++) {
            const char* str = pPluginBase->GetPresetName(i);
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
            pPluginBase->RestorePreset(itemChosen);
            pPluginBase->InformHostOfProgramChange();
            pPluginBase->DirtyParametersFromUI();
          }
          else {
            //const int numParams = mPlug->NParams();
            //const char** enumNames = new const char*[numParams];
            //for (int i = 0; i < mPlug->NParams(); i++) {
            //  enumNames[i] = mPlug->GetParamGroupName(i);
            //}
            WDL_String filename, path;
            GetUI()->PromptForFile(filename, path, EFileAction::Save, "txt");
            pPluginBase->DumpPresetSrcCode(filename.Get(), mNamedParams);
          }
        }
        void TextFromTextEntry(const char* txt) {
          auto* pPluginBase = static_cast<::IPluginBase*>(GetDelegate());
          WDL_String safeName;
          safeName.Set(txt, MAX_PRESET_NAME_LEN);

          pPluginBase->ModifyCurrentPreset(safeName.Get());
          pPluginBase->InformHostOfProgramChange();
          pPluginBase->DirtyParametersFromUI();
          SetDirty(false);
        }
      private:
        WDL_String mDisp;
        const char** mNamedParams;
      };



      // TODO: Draw with PathConvexShape from ptr to member array updated from Updatefunction
      class SRGraphBase
        : public IControl
        , public IVectorBase
      {
      public:
        SRGraphBase(IRECT bounds, int numValues, float* values, const IVStyle& style = DEFAULT_STYLE)
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
          g.PathStroke(GetColor(kPR), mStyle.frameThickness);
        };

        void OnResize() override {
          for (int i = 0; i < mNumValues; i++) {
            mX[i] = mRECT.L + (mRECT.W() / (mNumValues - 1.f)) * float(i);
            mY[i] = mRECT.MH() - (mRECT.H() * (float)mValues[i] * 0.5f);
          }
          SetDirty(false);
        }

        void Process(float* values) {
          mValues = values;
          for (int i = 0; i < mNumValues; i++) {
            mValues[i] = Clip<float>(mValues[i], -1.f, 1.f);
          }
          OnResize();
        };
        //void OnMouseDown(float x, float y, const IMouseMod& mod) override;
      private:
        //WDL_String mDisp;
        float* mValues;
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
  }
} // End namespaces