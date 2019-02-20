/*
How is the buffer implemented?
1 - std::vector
2 - Ptrlist
*/
#define BUFFERIMPL 2

#ifndef SRBUFFER_H
#define SRBUFFER_H
#include "IPlug_include_in_plug_hdr.h"
#include <cassert>
#include <vector>

namespace SR {
  namespace DSP {


    /*
    Buffer in progress with PtrList of WDL_Typedbuf or vectors
    */
    template<typename T = double, int MAXNUMCHANNELS = 1, int MAXNUMFRAMES = 1024>
    class SRBuffer
    {
    public:
      SRBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES)
        : mNumChannels(nChannels)
        , mNumFrames(nFrames)
      {
#if BUFFERIMPL == 1
        mBuffer.reserve(MAXNUMCHANNELS);
        mBuffer.resize(mNumChannels);
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Resize(mNumFrames);
    }
#elif BUFFERIMPL == 2
        mBuffer.Resize()
          for (int c = 0; c < MAXNUMCHANNELS; c++) {
            mBuffer[c].Add(new WDL_TypedBuf<T>);
            mBuffer[c].Resize(mNumFrames);
          }
#endif
  }

      ~SRBuffer() {
#if BUFFERIMPL == 1
        mBuffer.clear();
#elif BUFFERIMPL == 2
        for (int c = 0; c < MAXNUMCHANNELS; c++) {
          mBuffer[c].Empty();
        }
        mBuffer.Empty();
#endif
}

      void SetNumFrames(int nFrames = MAXNUMFRAMES) {
        mNumFrames = nFrames;
        for (int c = 0; c < mNumChannels; c++) {
#if BUFFERIMPL == 1
          mBuffer[c].Resize(mNumFrames, true);
#elif BUFFERIMPL == 2
          mBuffer[c].Resize(mNumFrames, true);
#endif
      }
      }

      void ResetBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES) {
        assert(nChannels <= MAXNUMCHANNELS);
        mNumChannels = nChannels;
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Resize(mNumFrames, true);
        }
        SetNumFrames(nFrames);
        //ClearBuffer();
      }

      void ClearBuffer() {
        for (int c = 0; c < mNumChannels; c++) {
          //memset(mBuffer[c].Get(), 0, mNumChannels * mBuffer[c].GetSize() * sizeof(T));
          mBuffer.clear();
        }
      }

      void ProcessBuffer(T in, int channel, int sample) {
#if BUFFERIMPL == 1
        mBuffer[channel].Get()[sample] = in;
#elif BUFFERIMPL == 2
        mBuffer[channel]->Get()[sample] = in;
#endif
      }

      void ProcessBuffer(T* in, int channel) {
        mBuffer[channel].Get() = in;
      }

      void ProcessBuffer(T** in) {
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Get() = in[c];
        }
      }

      T GetBuffer(int channel, int sample) {
        return mBuffer[channel].Get()[sample];
      }

      T* GetBuffer(int channel) {
        return mBuffer[channel].Get();
      }

      T** GetBuffer() {
        //T** buffer = new T*[mNumChannels];
        //for (int c = 0; c < mNumChannels; c++) {
        //  buffer[c] = mBuffer[c].Get();
        //}
        //return buffer;

        mBuffer.GetList();
      }

      T SumBuffer() {
        double sum = (T)0.0;
        for (int c = 0; c < mNumChannels; c++) {
          for (int s = 0; s < mNumFrames; s++) {
            sum += mBuffer[c].Get()[s];
          }
        }
        return sum;
      }

      T AverageBuffer() {
        return SumBuffer() / (mNumFrames * mNumChannels);
      }

    private:
      WDL_PtrList<WDL_TypedBuf<T>> mBuffer;
      //std::vector<WDL_TypedBuf<T>> mBuffer;
      unsigned int mNumFrames;
      unsigned int mNumChannels;
    };


    /*
    Working Buffer with ordinary T** double pointers
    */
    template<class T = double, int MAXNUMCHANNELS = 1, int MAXNUMFRAMES = 1024>
    class SRBuffer2 {
    public:
      SRBuffer2(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES)
        : mNumChannels(nChannels)
        , mNumFrames(nFrames)
        , mBuffer(new T*[nChannels])
      {
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c] = new T[mNumFrames];
        }
      }


      ~SRBuffer2() {

      }

      void SetNumFrames(int nFrames) {
        if (nFrames != mNumFrames) ResetBuffer(mNumChannels, nFrames);
      }

      void SetNumChannels(int nChannels) {
        if (nChannels != mNumChannels) ResetBuffer(nChannels, mNumFrames);
      }

      void ResetBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES) {
        // delete dynamic 2D array
        for (int c = 0; c < mNumChannels; c++) {
          if (mBuffer[c]) delete[] mBuffer[c];
        }
        if (mBuffer) delete[] mBuffer;

        // change channel and frames count
        assert(nChannels <= MAXNUMCHANNELS);
        mNumChannels = nChannels;
        mNumFrames = nFrames;

        // create new 2D array;
        mBuffer = new T*[mNumChannels];
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c] = new T[mNumFrames];
        }
      }

      void ProcessBuffer(T **in) { mBuffer = in; }
      void ProcessBuffer(T *in, int channel = 0) { mBuffer[channel] = in; }
      void ProcessBuffer(T in, int channel = 0, int sample = 0) { mBuffer[channel][sample] = in; }
      T** GetBuffer() { return mBuffer; }
      T*  GetBuffer(int channel) { return mBuffer[channel]; }
      T   GetBuffer(int channel, int sample) { return mBuffer[channel][sample]; }

      T AverageBuffer() {
        T sum = 0.0;
        for (int c = 0; c < mNumChannels; c++) {
          for (int s = 0; s < mNumFrames; s++) {
            sum += std::fabs(mBuffer[c][s]);
          }
        }
        return mNumChannels * sum / (T)mNumFrames;
      }

      T AverageBuffer(int channel) {
        T sum = 0.0;
        for (int s = 0; s < mNumFrames; s++) {
          sum += std::fabs(mBuffer[channel][s]);
        }
        return sum / (T)mNumFrames;
      }


    private:
      int mNumChannels;
      int mNumFrames;
      T **mBuffer;

    };
    // End of namespaces:
  }
}
#endif //SRBUFFER_H