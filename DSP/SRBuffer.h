/*
How is the buffer implemented?
1 - std::vector
2 - Ptrlist
3 - T**
*/
#define BUFFERIMPL 3

#ifndef SRBUFFER_H
#define SRBUFFER_H
#include "IPlug_include_in_plug_hdr.h"
#include <cassert>
#if BUFFERIMPL == 1
#include <vector>
#endif

namespace SR {
  namespace DSP {


    /*
    Buffer in progress with PtrList of WDL_Typedbuf or vectors
    */
    template<typename T = double, int MAXNUMCHANNELS = 1, int MAXNUMFRAMES = 1024>
    class SRBuffer {
    public:

      SRBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES)
        : mNumChannels(nChannels)
        , mNumFrames(nFrames)
#if BUFFERIMPL == 3
        , mBuffer(new T*[nChannels])
#endif
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
#elif BUFFERIMPL == 3
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c] = new T[mNumFrames];
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
#elif BUFFERIMPL == 3
        // TODO
#endif

      }



      void SetNumFrames(int nFrames = MAXNUMFRAMES) {
        mNumFrames = nFrames;
        for (int c = 0; c < mNumChannels; c++) {
#if BUFFERIMPL == 1
          mBuffer[c].Resize(mNumFrames, true);
#elif BUFFERIMPL == 2
          mBuffer[c].Resize(mNumFrames, true);
#elif BUFFERIMPL == 3
          if (nFrames != mNumFrames) ResetBuffer(mNumChannels, nFrames);
#endif
      }
    }



      void SetNumChannels(int nChannels) {
#if BUFFERIMPL == 3
        if (nChannels != mNumChannels) ResetBuffer(nChannels, mNumFrames);
#endif
  }



      void ResetBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES) {
#if BUFFERIMPL == 3
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
#else
        assert(nChannels <= MAXNUMCHANNELS);
        mNumChannels = nChannels;
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Resize(mNumFrames, true);
        }
        SetNumFrames(nFrames);
        //ClearBuffer();
#endif
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
#elif BUFFERIMPL == 3
        mBuffer[channel][sample] = in;
#endif
}



      void ProcessBuffer(T* in, int channel) {
#if BUFFERIMPL == 3
        mBuffer[channel] = in;
#else
        mBuffer[channel].Get() = in;
#endif
      }



      void ProcessBuffer(T** in) {
#if BUFFERIMPL == 3
        mBuffer = in;
#else
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Get() = in[c];
        }
#endif
      }



      T GetBuffer(int channel, int sample) {
#if BUFFERIMPL == 1

        return mBuffer[channel].Get()[sample];
#elif BUFFERIMPL == 2
#elif BUFFERIMPL == 3
        return mBuffer[channel][sample];
#endif
      }



      T* GetBuffer(int channel) {
#if BUFFERIMPL == 1
        return mBuffer[channel].Get();
#elif BUFFERIMPL == 2
#elif BUFFERIMPL == 3
        return mBuffer[channel];
#endif

      }



      T** GetBuffer() {
#if BUFFERIMPL == 1
        T** buffer = new T*[mNumChannels];
        for (int c = 0; c < mNumChannels; c++) {
          buffer[c] = mBuffer[c].Get();
        }
        return buffer;
#elif BUFFERIMPL == 2
        mBuffer.GetList();
#elif BUFFERIMPL == 3
        return mBuffer;
#endif
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
#if BUFFERIMPL == 3
        T sum = 0.0;
        for (int c = 0; c < mNumChannels; c++) {
          for (int s = 0; s < mNumFrames; s++) {
            sum += std::fabs(mBuffer[c][s]);
          }
        }
        return mNumChannels * sum / (T)mNumFrames;
#else
        return SumBuffer() / (mNumFrames * mNumChannels);
#endif
      }



      T AverageBuffer(int channel) {
#if BUFFERIMPL == 3
        T sum = 0.0;
        for (int s = 0; s < mNumFrames; s++) {
          sum += std::fabs(mBuffer[channel][s]);
        }
        return sum / (T)mNumFrames;
#endif
      }



    private:

#if BUFFERIMPL == 1
      std::vector<WDL_TypedBuf<T>> mBuffer;
#elif BUFFERIMPL == 2
      WDL_PtrList<WDL_TypedBuf<T>> mBuffer;
#elif BUFFERIMPL == 3
      T **mBuffer;
#endif
      unsigned int mNumFrames;
      unsigned int mNumChannels;
    };



    // End of namespaces:
  }
}
#endif //SRBUFFER_H