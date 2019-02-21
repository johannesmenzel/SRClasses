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
#if BUFFERIMPL == 3 // T**
        , mBuffer(new T*[nChannels])
#endif
      {
#if BUFFERIMPL == 1 // vector
        mBuffer.reserve(MAXNUMCHANNELS);
        mBuffer.resize(mNumChannels);
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Resize(mNumFrames);
        }
#elif BUFFERIMPL == 2 // ptrlist
        mBuffer.Resize()
          for (int c = 0; c < MAXNUMCHANNELS; c++) {
            mBuffer[c].Add(new WDL_TypedBuf<T>);
            mBuffer[c].Resize(mNumFrames);
          }
#elif BUFFERIMPL == 3 // T**
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c] = new T[mNumFrames];
        }
#endif
      }



      ~SRBuffer() {
#if BUFFERIMPL == 1 // vector
        mBuffer.clear();
#elif BUFFERIMPL == 2 //ptrlist
        for (int c = 0; c < MAXNUMCHANNELS; c++) {
          mBuffer[c].Empty();
        }
        mBuffer.Empty();
#elif BUFFERIMPL == 3 // T**
        // TODO
#endif

      }



      void SetNumFrames(int nFrames = MAXNUMFRAMES) {
        mNumFrames = nFrames;
        for (int c = 0; c < mNumChannels; c++) {
#if BUFFERIMPL == 1 // vector
          mBuffer[c].Resize(mNumFrames, true);
#elif BUFFERIMPL == 2 // ptrlist
          mBuffer[c].Resize(mNumFrames, true);
#elif BUFFERIMPL == 3 // T**
          if (nFrames != mNumFrames) ResetBuffer(mNumChannels, nFrames);
#endif
      }
    }



      void SetNumChannels(int nChannels) {
#if BUFFERIMPL == 3 // T**
        if (nChannels != mNumChannels) ResetBuffer(nChannels, mNumFrames);
#endif
  }



      void ResetBuffer(int nChannels = MAXNUMCHANNELS, int nFrames = MAXNUMFRAMES) {
#if BUFFERIMPL == 3 // T**
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
#else // not T**
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
#if BUFFERIMPL == 1 // vector
        mBuffer[channel].Get()[sample] = in;
#elif BUFFERIMPL == 2 // ptrlist
        mBuffer[channel]->Get()[sample] = in;
#elif BUFFERIMPL == 3 // T**
        mBuffer[channel][sample] = in;
#endif
}



      void ProcessBuffer(T* in, int channel) {
#if BUFFERIMPL == 3 // T**
        mBuffer[channel] = in;
#else // not T**
        mBuffer[channel].Get() = in;
#endif
      }



      void ProcessBuffer(T** in) {
#if BUFFERIMPL == 3 // T**
        mBuffer = in;
#else // not T**
        for (int c = 0; c < mNumChannels; c++) {
          mBuffer[c].Get() = in[c];
        }
#endif
      }



      T GetBuffer(int channel, int sample) {
#if BUFFERIMPL == 1 // vector

        return mBuffer[channel].Get()[sample];
#elif BUFFERIMPL == 2 // ptrlist
#elif BUFFERIMPL == 3 // T**
        return mBuffer[channel][sample];
#endif
      }



      T* GetBuffer(int channel) {
#if BUFFERIMPL == 1 // vector
        return mBuffer[channel].Get();
#elif BUFFERIMPL == 2 // ptrlist
#elif BUFFERIMPL == 3 // T**
        return mBuffer[channel];
#endif

      }



      T** GetBuffer() {
#if BUFFERIMPL == 1 // vector
        T** buffer = new T*[mNumChannels];
        for (int c = 0; c < mNumChannels; c++) {
          buffer[c] = mBuffer[c].Get();
        }
        return buffer;
#elif BUFFERIMPL == 2 // ptrlist
        mBuffer.GetList();
#elif BUFFERIMPL == 3 // T**
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
#if BUFFERIMPL == 3 // T**
        T sum = 0.0;
        for (int c = 0; c < mNumChannels; c++) {
          for (int s = 0; s < mNumFrames; s++) {
            sum += std::fabs(mBuffer[c][s]);
          }
        }
        return mNumChannels * sum / (T)mNumFrames;
#else // not T**
        return SumBuffer() / (mNumFrames * mNumChannels);
#endif
      }



      T AverageBuffer(int channel) {
#if BUFFERIMPL == 3 // T**
        T sum = 0.0;
        for (int s = 0; s < mNumFrames; s++) {
          sum += std::fabs(mBuffer[channel][s]);
        }
        return sum / (T)mNumFrames;
#endif
      }



    private:
      unsigned int mNumFrames;
      unsigned int mNumChannels;

#if BUFFERIMPL == 1 // vector
      std::vector<WDL_TypedBuf<T>> mBuffer;
#elif BUFFERIMPL == 2 // ptrlist
      WDL_PtrList<WDL_TypedBuf<T>> mBuffer;
#elif BUFFERIMPL == 3 // T**
      T **mBuffer;
#endif

    };



    // End of namespaces:
  }
}
#endif //SRBUFFER_H