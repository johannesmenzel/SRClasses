/*
*	copyright 2018, Johannes Menzel, MIT
*
*	This is an ongoing modification of the code provided in the Simple Source
*	library by Bojan Markovic. Licence disclaimer is merged here, since the
*	classes and functions were merged to one .h and .cpp source file each.
*	See the original disclaimer below:
*
*	copyright 2006, ChunkWare Music Software, OPEN-SOURCE
*
*	Permission is hereby granted, free of charge, to any person obtaining a
*	copy of this software and associated documentation files (the "Software"),
*	to deal in the Software without restriction, including without limitation
*	the rights to use, copy, modify, merge, publish, distribute, sublicense,
*	and/or sell copies of the Software, and to permit persons to whom the
*	Software is furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in
*	all copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
*	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*	DEALINGS IN THE SOFTWARE.
*/

#include "SRDynamics.h"
namespace SR {

  namespace DSP {

    //-------------------------------------------------------------
    // envelope detector
    //-------------------------------------------------------------
    EnvelopeDetector::EnvelopeDetector(double ms, double sampleRate)
    {
      assert(sampleRate > 0.0);
      assert(ms > 0.0);
      mSampleRate = sampleRate;
      mTimeConstantMs = (ms > 1000. / mSampleRate) ? ms : 1000. / mSampleRate;
      setCoef();
    }



    //-------------------------------------------------------------
    void EnvelopeDetector::setTc(double ms)
    {
      assert(ms > 0.0);
      mTimeConstantMs = (ms > 1000. / mSampleRate) ? ms : 1000. / mSampleRate;
      setCoef();
    }

    //-------------------------------------------------------------
    void EnvelopeDetector::setSampleRate(double sampleRate)
    {
      assert(sampleRate > 0.0);
      mSampleRate = sampleRate;
      setCoef();
    }

    //-------------------------------------------------------------
    void EnvelopeDetector::setCoef(void)
    {
      mRuntimeCoeff = exp(-1000.0 / (mTimeConstantMs * mSampleRate));
    }

    //-------------------------------------------------------------
    // attack/release envelope
    //-------------------------------------------------------------
    AttRelEnvelope::AttRelEnvelope(double att_ms, double rel_ms, double sampleRate)
      : mEnvelopeDetectorAttack(att_ms, sampleRate)
      , mEnvelopeDetectorRelease(rel_ms, sampleRate)
    {
    }

    //-------------------------------------------------------------
    void AttRelEnvelope::setAttack(double ms)
    {
      mEnvelopeDetectorAttack.setTc(ms);
    }

    //-------------------------------------------------------------
    void AttRelEnvelope::setRelease(double ms)
    {
      mEnvelopeDetectorRelease.setTc(ms);
    }

    //-------------------------------------------------------------
    void AttRelEnvelope::setSampleRate(double sampleRate)
    {
      mEnvelopeDetectorAttack.setSampleRate(sampleRate);
      mEnvelopeDetectorRelease.setSampleRate(sampleRate);
    }



    //-------------------------------------------------------------
    // simple compressor
    //-------------------------------------------------------------
    SRCompressor::SRCompressor()
      : AttRelEnvelope(10.0, 100.0)
      , mThreshDb(0.0)
      , mRatio(1.0)
      , currentOvershootDb(DC_OFFSET)
      , mSidechainFc(0.0)
      , mGrLin(1.0)
      , mGrDb(0.0)
      , mTopologyFeedback(false)
      , mKneeWidthDb(0.0)
      , mMaxGr(0.0)
      , sidechainSignal1(0.0)
      , sidechainSignal2(0.0)
    {
    }

    void SRCompressor::initCompressor(double dB, double ratio, double attackMs, double releaseMs, double sidechainFc, double kneeDb, bool feedback, double samplerate) {
      setThresh(dB);
      setRatio(ratio);
      setAttack(attackMs);
      setRelease(releaseMs);
      initSidechainFilter(sidechainFc);
      setSampleRate(samplerate);
      setKnee(kneeDb);
      setTopologyFeedback(feedback);
      fSidechainFilter.SetFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadHighpass, sidechainFc, 0.7071, 0., samplerate);
    }

    //-------------------------------------------------------------
    void SRCompressor::setThresh(double dB)
    {
      mThreshDb = dB;
    }

    //-------------------------------------------------------------
    void SRCompressor::setRatio(double ratio)
    {
      assert(ratio >= 0.0);
      mRatio = ratio;
      mMaxGr = 73.4979484210802 - 88.939188010773 * (1 - exp(-1.75091102973106 * (1 / ratio)));
    }

    void SRCompressor::setKnee(double kneeDb)
    {
      this->mKneeWidthDb = kneeDb;
    }

    void SRCompressor::initSidechainFilter(double sidechainFc) {
      this->mSidechainFc = sidechainFc;
      fSidechainFilter.SetFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadHighpass, mSidechainFc, 0.7071, 0., getSampleRate());
    }

    void SRCompressor::setSidechainFilterFreq(double sidechainFc)
    {
      this->mSidechainFc = sidechainFc;
      fSidechainFilter.SetFreq(mSidechainFc);
    }

    void SRCompressor::setTopologyFeedback(bool feedback)
    {
      this->mTopologyFeedback = feedback;
    }

    //-------------------------------------------------------------
    void SRCompressor::initRuntime(void)
    {
      this->currentOvershootDb = DC_OFFSET;
    }

    //-------------------------------------------------------------
    // simple compressor with RMS detection
    //-------------------------------------------------------------
    SRCompressorRMS::SRCompressorRMS()
      : mEnvelopeDetectorAverager(5.0)
      , mAverageOfSquares(DC_OFFSET)
    {
    }

    void SRCompressorRMS::initCompressor(double dB, double ratio, double attackMs, double releaseMs, double sidechainFc, double kneeDb, double rmsWindowMs, bool feedback, double samplerate) {
      setThresh(dB);
      setRatio(ratio);
      setAttack(attackMs);
      setRelease(releaseMs);
      initSidechainFilter(sidechainFc);
      setSampleRate(samplerate);
      setKnee(kneeDb);
      setTopologyFeedback(feedback);
      mEnvelopeDetectorAverager.setTc(rmsWindowMs);
      fSidechainFilter.SetFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadHighpass, sidechainFc, 0.7071, 0., samplerate);
    }

    //-------------------------------------------------------------
    void SRCompressorRMS::setSampleRate(double sampleRate)
    {
      SRCompressor::setSampleRate(sampleRate);
      mEnvelopeDetectorAverager.setSampleRate(sampleRate);
    }

    //-------------------------------------------------------------
    void SRCompressorRMS::setWindow(double ms)
    {
      mEnvelopeDetectorAverager.setTc(ms);
    }

    //-------------------------------------------------------------
    void SRCompressorRMS::initRuntime(void)
    {
      SRCompressor::initRuntime();
      mAverageOfSquares = DC_OFFSET;
    }

    //-------------------------------------------------------------
    SRLimiter::SRLimiter()
      : mThreshDb(0.0)
      , mThreshLin(1.0)
      , mPeakHoldSamples(0)
      , mPeakHoldTimer(0)
      , mMaxPeak(1.0)
      , mEnvelopeDetectorAttack(1.0)
      , mEnvelopeDetectorRelease(10.0)
      , currentOvershootLin(1.0)
      , mBufferMask(BUFFER_SIZE - 1)
      , mCursor(0)
    {
      setAttack(1.0);
      mOutputBuffer[0].resize(BUFFER_SIZE, 0.0);
      mOutputBuffer[1].resize(BUFFER_SIZE, 0.0);
    }

    //-------------------------------------------------------------
    void SRLimiter::setThresh(double dB)
    {
      mThreshDb = dB;
      mThreshLin = SR::Utils::DBToAmp(dB);
    }

    //-------------------------------------------------------------
    void SRLimiter::setAttack(double ms)
    {
      unsigned int samp = int(0.001 * ms * mEnvelopeDetectorAttack.getSampleRate());

      assert(samp < BUFFER_SIZE);

      mPeakHoldSamples = samp;
      mEnvelopeDetectorAttack.setTc(ms);
    }

    //-------------------------------------------------------------
    void SRLimiter::setRelease(double ms)
    {
      mEnvelopeDetectorRelease.setTc(ms);
    }

    //-------------------------------------------------------------
    void SRLimiter::setSampleRate(double sampleRate)
    {
      mEnvelopeDetectorAttack.setSampleRate(sampleRate);
      mEnvelopeDetectorRelease.setSampleRate(sampleRate);
    }

    //-------------------------------------------------------------
    void SRLimiter::initRuntime(void)
    {
      mPeakHoldTimer = 0;
      mMaxPeak = mThreshLin;
      currentOvershootLin = mThreshLin;
      mCursor = 0;
      mOutputBuffer[0].assign(BUFFER_SIZE, 0.0);
      mOutputBuffer[1].assign(BUFFER_SIZE, 0.0);
    }

    //-------------------------------------------------------------
    void SRLimiter::FastEnvelope::setCoef(void)
    {
      // rises to 99% of in value over duration of time constant
      mRuntimeCoeff = pow(0.01, (1000.0 / (mTimeConstantMs * mSampleRate)));
    }



    //-------------------------------------------------------------
    SRGate::SRGate()
      : AttRelEnvelope(1.0, 100.0)
      , mThreshDb(0.0)
      , mThreshLin(1.0)
      , currentOvershootLin(DC_OFFSET)
    {
    }

    //-------------------------------------------------------------
    void SRGate::setThresh(double dB)
    {
      mThreshDb = dB;
      mThreshLin = SR::Utils::DBToAmp(dB);
    }

    //-------------------------------------------------------------
    void SRGate::initRuntime(void)
    {
      currentOvershootLin = DC_OFFSET;
    }

    //-------------------------------------------------------------
    // simple gate with RMS detection
    //-------------------------------------------------------------
    SRGateRms::SRGateRms()
      : mEnvelopeDetectorAverager(5.0)
      , mAverageOfSquares(DC_OFFSET)
    {
    }

    //-------------------------------------------------------------
    void SRGateRms::setSampleRate(double sampleRate)
    {
      SRGate::setSampleRate(sampleRate);
      mEnvelopeDetectorAverager.setSampleRate(sampleRate);
    }

    //-------------------------------------------------------------
    void SRGateRms::setWindow(double ms)
    {
      mEnvelopeDetectorAverager.setTc(ms);
    }

    //-------------------------------------------------------------
    void SRGateRms::initRuntime(void)
    {
      SRGate::initRuntime();
      mAverageOfSquares = DC_OFFSET;
    }


    //-------------------------------------------------------------
    // DEESSER
    //-------------------------------------------------------------
    SRDeesser::SRDeesser()
      : AttRelEnvelope(10.0, 100.0)
      , mThreshDb(0.0)
      , mRatio(1.0)
      , currentOvershootDb(DC_OFFSET)
      , mFilterFreq(0.5)
      , mFilterQ(0.707)
      , mFilterGain(0.0)
      , mGrLin(1.0)
      , mGrDb(0.0)
      , mKneeWidthDb(0.0)
      //, fSidechainBandpass(SRFiltersIIR<double, 2>::EFilterType::BiquadBandpass, 0.5, 0.707, 0.0, 44100.0)
      //, fDynamicEqFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadPeak, 0.5, 0.707, 0.0, 44100.0)
    {
    }

    void SRDeesser::setDeesser(double threshDb, double ratio, double attackMs, double releaseMs, double normalizedFreq, double q, double kneeDb, double samplerate) {
      setThresh(threshDb);
      setRatio(ratio);
      setAttack(attackMs);
      setRelease(releaseMs);
      initFilter(normalizedFreq, q);
      setSampleRate(samplerate);
      setKnee(kneeDb);
    }

    //-------------------------------------------------------------
    void SRDeesser::setThresh(double dB)
    {
      mThreshDb = dB;
    }

    //-------------------------------------------------------------
    void SRDeesser::setRatio(double ratio)
    {
      assert(ratio >= 0.0);
      mRatio = ratio;
    }

    void SRDeesser::setKnee(double kneeDb)
    {
      mKneeWidthDb = kneeDb;
    }

    void SRDeesser::initFilter(double freq, double q)
    {
      mFilterFreq = freq;
      mFilterQ = q;
      fSidechainBandpass.SetFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadBandpass, mFilterFreq, mFilterQ, 0.0, getSampleRate());
      fDynamicEqFilter.SetFilter(SRFiltersIIR<double, 2>::EFilterType::BiquadPeak, mFilterFreq, mFilterQ, 0.0, getSampleRate());
    }

    void SRDeesser::setFrequency(double freq)
    {
      mFilterFreq = freq;
      fSidechainBandpass.SetFreq(mFilterFreq);
      fDynamicEqFilter.SetFreq(mFilterFreq);
    }

    void SRDeesser::setQ(double q)
    {
      mFilterQ = q;
      fSidechainBandpass.SetQ(q);
      fDynamicEqFilter.SetQ(q);
    }

    //-------------------------------------------------------------
    void SRDeesser::initRuntime(void)
    {
      currentOvershootDb = DC_OFFSET;
    }

  }
} // end namespace
