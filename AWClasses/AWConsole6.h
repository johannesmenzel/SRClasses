#pragma once
#include <math.h>

namespace AW {
  class AWNoiseShaper {
  public:
    AWNoiseShaper() {}
    ~AWNoiseShaper() {}
    inline void ProcessBlock(double** inputs, double** outputs, int nChannels, int nFrames) {
      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {
          if (inputs[c][s] < 1.2e-38 && -inputs[c][s] < 1.2e-38) {
            static int noisesource = 0;
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            inputs[c][s] = applyresidue;
          }
        }
      }
    }
    inline void ProcessBlock(float** inputs, float** outputs, int nChannels, int nFrames) {
      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {
          if (inputs[c][s] < 1.2e-38 && -inputs[c][s] < 1.2e-38) {
            static int noisesource = 0;
            noisesource = noisesource % 1700021; noisesource++;
            int residue = noisesource * noisesource;
            residue = residue % 170003; residue *= residue;
            residue = residue % 17011; residue *= residue;
            residue = residue % 1709; residue *= residue;
            residue = residue % 173; residue *= residue;
            residue = residue % 17;
            double applyresidue = residue;
            applyresidue *= 0.00000001;
            applyresidue *= 0.00000001;
            outputs[c][s] = applyresidue;
          }
        }
      }
    }
  };

  template<int MAXNUMCHANNELS = 2>
  class AWDither {
  public:
    AWDither() {
      Reset();
    }

    ~AWDither() {}

    void Reset() {
      for (int c = 0; c < MAXNUMCHANNELS; c++) {
        fpNShape[c] = 0.0;
      }
    }

    inline void ProcessBlock(double** inputs, double** outputs, int nChannels, int nFrames) {
      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {
          outputs[c][s] = inputs[c][s];
          int expon;
          frexp((double)inputs[c][s], &expon);
          long double dither = (rand() / (RAND_MAX*7.737125245533627e+25))*pow(2, expon + 62);
          dither /= 536870912.0;
          outputs[c][s] += (dither - fpNShape[c]); fpNShape[c] = dither;
        }
      }
    }

    inline void ProcessBlock(float** inputs, float** outputs, int nChannels, int nFrames) {
      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {
          int expon;
          frexpf((float)inputSampleL, &expon);
          long double dither = (rand() / (RAND_MAX*7.737125245533627e+25))*pow(2, expon + 62);
          outputs[c][s] += (dither - fpNShape[c]); fpNShape[c] = dither;
        }
      }
    }

  private:
    long double fpNShape[MAXNUMCHANNELS] = {};
  };

  class AWBase {
  public:
    AWBase()
      : mSampleRate(44100.)
      , mOverallScale(1.)
    {}
    ~AWBase() {}
    void Reset(double samplerate) {
      mSampleRate = samplerate;
      mOverallScale = mSampleRate / 44100.;
    }
  protected:
    double mSampleRate;
    double mOverallScale;
  };

  template<int MAXNUMCHANNELS = 2>
  class AWConsoleChannel
    : AWBase
  {
  public:
    enum EConsoleType {
      kConsole6 = 0,
      kConsole5,
      kNumConsoleTypes
    };

    AWConsoleChannel()
      : AWBase()
      , mInput(1.0)
      , mType(EConsoleType::kConsole6)
    {
      Reset();
    }

    ~AWConsoleChannel() {}

    void Reset(double samplerate = 44100.) {
      for (int c = 0; c < MAXNUMCHANNELS; c++) {
        AWBase::Reset(samplerate);
        lastSampleChannel[c] = 0.0;
        lastFXChannel[c] = 0.0;
        iirCorrect[c] = 0.0;
      }
      gainchase = -90.0;
      settingchase = -90.0;
      chasespeed = 350.0;
    }
    void SetType(EConsoleType type) { mType = type; };
    void SetInput(double input) { mInput = input; };
    EConsoleType GetType() { return mType; };
    double GetInput() { return mInput; };

    inline void ProcessBlock(double** inputs, double** outputs, int nChannels, int nFrames) {
      double difference;
      double nearZero;
      double servoTrim = 0.0000001 / mOverallScale;
      double bassTrim = 0.005 / mOverallScale;
      if (settingchase != mInput) {
        chasespeed *= 2.0;
        settingchase = mInput;
      }
      if (chasespeed > 2500.0) chasespeed = 2500.0;
      if (gainchase < 0.0) gainchase = mInput;
      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {
          outputs[c][s] = inputs[c][s];
          switch (mType)
          {

          case EConsoleType::kConsole6:
            if (mInput != 1.0)
              outputs[c][s] *= mInput;
            if (outputs[c][s] > 1.0) outputs[c][s] = 1.0;
            else if (outputs[c][s] > 0.0) outputs[c][s] = 1.0 - pow(1.0 - outputs[c][s], 2.0);

            if (outputs[c][s] < -1.0) outputs[c][s] = -1.0;
            else if (outputs[c][s] < 0.0) outputs[c][s] = -1.0 + pow(1.0 + outputs[c][s], 2.0);

            break;

          case EConsoleType::kConsole5:
            chasespeed *= 0.9999;
            chasespeed -= 0.01;
            if (chasespeed < 350.0) chasespeed = 350.0;

            gainchase = (((gainchase * chasespeed) + mInput) / (chasespeed + 1.0));

            if (1.0 != gainchase) {
              outputs[c][s] *= gainchase;
            }

            difference = lastSampleChannel[c] - outputs[c][s];
            lastSampleChannel[c] = outputs[c][s];

            if (difference > 1.0) difference = 1.0;
            if (difference < -1.0) difference = -1.0;

            difference = lastFXChannel[c] + asin(difference);

            iirCorrect[c] += outputs[c][s] - difference;
            outputs[c][s] = difference;

            lastFXChannel[c] = outputs[c][s];
            if (lastFXChannel[c] > 1.0) lastFXChannel[c] = 1.0;
            if (lastFXChannel[c] < -1.0) lastFXChannel[c] = -1.0;

            nearZero = pow(fabs(fabs(lastFXChannel[c]) - 1.0), 2);
            if (iirCorrect[c] > 0) iirCorrect[c] -= servoTrim;
            if (iirCorrect[c] < 0) iirCorrect[c] += servoTrim;
            lastFXChannel[c] += (iirCorrect[c] * 0.0000005);
            lastFXChannel[c] *= (1.0 - (nearZero * bassTrim));

            if (outputs[c][s] > 1.57079633) outputs[c][s] = 1.57079633;
            if (outputs[c][s] < -1.57079633) outputs[c][s] = -1.57079633;
            outputs[c][s] = sin(outputs[c][s]);
            break;
          default:
            break;
          }

        }
      }
    }

  private:
    double mInput;
    EConsoleType mType;
    double lastSampleChannel[MAXNUMCHANNELS];
    double lastFXChannel[MAXNUMCHANNELS];
    double iirCorrect[MAXNUMCHANNELS];
    double gainchase;
    double settingchase;
    double chasespeed;
  };




  template<int MAXNUMCHANNELS = 2>
  class AWConsoleBuss
    : AWBase
  {
  public:
    enum EConsoleType {
      kConsole6 = 0,
      kConsole5,
      kNumConsoleTypes
    };
    AWConsoleBuss()
      : AWBase()
      , mInput(1.0)
      , mType(EConsoleType::kConsole6)
    {
      Reset();
    }
    ~AWConsoleBuss() {

    }

    void Reset(double samplerate = 44100.) {
      for (int c = 0; c < MAXNUMCHANNELS; c++) {
        AWBase::Reset(samplerate);
        lastSampleBuss[c] = 0.0;
        lastFXBuss[c] = 0.0;
        iirCorrect[c] = 0.0;
      }
      gainchase = -90.0;
      settingchase = -90.0;
      chasespeed = 350.0;
    }

    void SetType(EConsoleType type) { mType = type; };
    void SetInput(double input) { mInput = input; };
    EConsoleType GetType() { return mType; };
    double GetInput() { return mInput; };

    inline void ProcessBlock(double** inputs, double** outputs, int nChannels, int nFrames) {
      double difference;
      double nearZero;
      double servoTrim = 0.0000001 / mOverallScale;
      double bassTrim = 0.005 / mOverallScale;

      if (settingchase != mInput) {
        chasespeed *= 2.0;
        settingchase = mInput;
      }
      if (chasespeed > 2500.0) chasespeed = 2500.0;
      if (gainchase < 0.0) gainchase = mInput;

      for (int s = 0; s < nFrames; s++) {
        for (int c = 0; c < nChannels; c++) {

          outputs[c][s] = inputs[c][s];

          switch (mType)
          {
          case EConsoleType::kConsole6:
            if (mInput != 1.0)
              outputs[c][s] *= mInput;

            if (outputs[c][s] > 1.0) outputs[c][s] = 1.0;
            else if (outputs[c][s] > 0.0) outputs[c][s] = 1.0 - pow(1.0 - outputs[c][s], 0.5);

            if (outputs[c][s] < -1.0) outputs[c][s] = -1.0;
            else if (outputs[c][s] < 0.0) outputs[c][s] = -1.0 + pow(1.0 + outputs[c][s], 0.5);
            break;
          case EConsoleType::kConsole5:
            chasespeed *= 0.9999;
            chasespeed -= 0.01;
            if (chasespeed < 350.0) chasespeed = 350.0;

            gainchase = (((gainchase*chasespeed) + mInput) / (chasespeed + 1.0));

            if (1.0 != gainchase)
              outputs[c][s] *= gainchase;

            if (outputs[c][s] > 1.0) outputs[c][s] = 1.0;
            if (outputs[c][s] < -1.0) outputs[c][s] = -1.0;
            outputs[c][s] = asin(outputs[c][s]);


            difference = lastSampleBuss[c] - outputs[c][s];
            lastSampleBuss[c] = outputs[c][s];

            if (difference > 1.57079633) difference = 1.57079633;
            if (difference < -1.57079633) difference = -1.57079633;

            difference = lastFXBuss[c] + sin(difference);

            iirCorrect[c] += outputs[c][s] - difference;
            outputs[c][s] = difference;

            lastFXBuss[c] = outputs[c][s];
            if (lastFXBuss[c] > 1.0) lastFXBuss[c] = 1.0;
            if (lastFXBuss[c] < -1.0) lastFXBuss[c] = -1.0;

            nearZero = pow(fabs(fabs(lastFXBuss[c]) - 1.0), 2);
            if (iirCorrect[c] > 0) iirCorrect[c] -= servoTrim;
            if (iirCorrect[c] < 0) iirCorrect[c] += servoTrim;

            lastFXBuss[c] += (iirCorrect[c] * 0.0000005);
            lastFXBuss[c] *= (1.0 - (nearZero * bassTrim));
            break;
          default:
            break;
          }

        }
      }
    }
  private:
    double mInput;
    EConsoleType mType;
    double lastSampleBuss[MAXNUMCHANNELS];
    double lastFXBuss[MAXNUMCHANNELS];
    double iirCorrect[MAXNUMCHANNELS];
    double gainchase;
    double settingchase;
    double chasespeed;
  };
}