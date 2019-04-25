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
    // Envelope detector
    //-------------------------------------------------------------
    EnvelopeDetector::EnvelopeDetector(double ms, double sampleRate)
    {
      assert(sampleRate > 0.0);
      assert(ms > 0.0);
      mSampleRate = sampleRate;
      mTimeConstantMs = (ms > 1000. / mSampleRate) ? ms : 1000. / mSampleRate;
      setCoef();
    }

    void EnvelopeDetector::setTc(double ms) {
      assert(ms > 0.0);
      mTimeConstantMs = (ms > 1000. / mSampleRate) ? ms : 1000. / mSampleRate;
      setCoef();
    }

    void EnvelopeDetector::setSampleRate(double sampleRate) {
      assert(sampleRate > 0.0);
      mSampleRate = sampleRate;
      setCoef();
    }

    void EnvelopeDetector::setCoef(void) {
      mRuntimeCoeff = exp(-1000.0 / (mTimeConstantMs * mSampleRate));
    }


    //-------------------------------------------------------------
    // Attack/release envelope
    //-------------------------------------------------------------
    AttRelEnvelope::AttRelEnvelope(double att_ms, double rel_ms, double sampleRate)
      : mEnvelopeDetectorAttack(att_ms, sampleRate)
      , mEnvelopeDetectorRelease(rel_ms, sampleRate)
    {
    }

    void AttRelEnvelope::SetAttack(double ms) {
      mEnvelopeDetectorAttack.setTc(ms);
    }

    void AttRelEnvelope::SetRelease(double ms) {
      mEnvelopeDetectorRelease.setTc(ms);
    }

    void AttRelEnvelope::SetSampleRate(double sampleRate) {
      mEnvelopeDetectorAttack.setSampleRate(sampleRate);
      mEnvelopeDetectorRelease.setSampleRate(sampleRate);
    }


    //-------------------------------------------------------------
    // SRDynamicsBase methods
    //-------------------------------------------------------------
    SRDynamicsBase::SRDynamicsBase(double threshDb, double ratio, bool autoMakeup)
      : mThreshDb(threshDb)
      , mThreshLin(Utils::DBToAmp(threshDb))
      , mRatio(ratio)
      , mMakeup(1.0)
      , mKneeWidthDb(0.0)
      , mGrDb(0.0)
      , mGrLin(1.0)
      , currentOvershootDb(DC_OFFSET)
      , currentOvershootLin(SR::Utils::DBToAmp(DC_OFFSET))
      , mAverageOfSquares(DC_OFFSET)
      , mIsAutoMakeup(autoMakeup)
      , mAutoMakeup(1.0)
    {
    }

    void SRDynamicsBase::SetThresh(double threshDb) {
      mThreshDb = threshDb;
      mThreshLin = SR::Utils::DBToAmp(threshDb);
      if (mIsAutoMakeup)
        mAutoMakeup = AutoMakeup(mThreshDb, mRatio);
    }

    void SRDynamicsBase::SetRatio(double ratio) {
      assert(ratio >= 0.0);
      mRatio = ratio;
      if (mIsAutoMakeup)
        mAutoMakeup = AutoMakeup(mThreshDb, mRatio);
    }

    void SRDynamicsBase::SetMakeup(double makeupDb) {
      mMakeup = SR::Utils::DBToAmp(makeupDb);
    }

    void SRDynamicsBase::SetIsAutoMakeup(bool autoMakeup) {
      mIsAutoMakeup = autoMakeup;
    }

    void SRDynamicsBase::SetKnee(double kneeDb) {
      assert(kneeDb >= 0.0);
      mKneeWidthDb = kneeDb;
    }

    void SRDynamicsBase::Reset(void) {
      currentOvershootDb = DC_OFFSET;
      currentOvershootLin = SR::Utils::DBToAmp(DC_OFFSET);
      mAverageOfSquares = DC_OFFSET;
    }


    //-------------------------------------------------------------
    // SRCompressor methods
    //-------------------------------------------------------------
    SRCompressor::SRCompressor()
      : AttRelEnvelope(10.0, 100.0)
      , SRDynamicsBase(0.0, 1.0)
      , mSidechainFc(0.0)
      , mTopologyFeedback(false)
      , mMaxGr(0.0)
      , sidechainSignal1(0.0)
      , sidechainSignal2(0.0)
    {
    }

    void SRCompressor::InitCompressor(double threshDb, double ratio, double attackMs, double releaseMs, double sidechainFc, double kneeDb, bool isFeedbackCompressor, bool autoMakeup, double samplerate) {
      AttRelEnvelope::SetSampleRate(samplerate);
      SRDynamicsBase::SetThresh(threshDb);
      SRDynamicsBase::SetRatio(ratio);
      SRDynamicsBase::SetIsAutoMakeup(autoMakeup);
      AttRelEnvelope::SetAttack(attackMs);
      AttRelEnvelope::SetRelease(releaseMs);
      InitSidechainFilter(sidechainFc);
      SRDynamicsBase::SetKnee(kneeDb);
      SetTopologyFeedback(isFeedbackCompressor);
    }

    void SRCompressor::SetMaxGrDb(double maxGrDb, bool sigmoid) {
      if (!sigmoid)
        mMaxGr = maxGrDb;
      else {
        const double tempratio = 1. / mRatio;
        mMaxGr = (maxGrDb + (maxGrDb * 9.) / (maxGrDb * tempratio - maxGrDb - 9.)); // Simplified P4 sigmoid fitting with d+\frac{da}{dx-d-a} with f(1) = 0.0
      }
    }

    void SRCompressor::InitSidechainFilter(double sidechainFc) {
      mSidechainFc = sidechainFc;
      fSidechainFilter.SetFilter(SRFilterIIR<double, 2>::EFilterType::BiquadHighpass, sidechainFc, 0.7071, 0., AttRelEnvelope::GetSampleRate());
    }

    void SRCompressor::SetSidechainFilterFreq(double sidechainFc) {
      mSidechainFc = sidechainFc;
      fSidechainFilter.SetFreq(mSidechainFc);
    }

    void SRCompressor::SetTopologyFeedback(bool isFeedbackCompressor) {
      mTopologyFeedback = isFeedbackCompressor;
    }



    //-------------------------------------------------------------
    // SRCompressorRMS methods
    //-------------------------------------------------------------
    SRCompressorRMS::SRCompressorRMS()
      : mEnvelopeDetectorAverager(5.0)
    {
    }

    void SRCompressorRMS::InitCompressor(double dB, double ratio, double attackMs, double releaseMs, double sidechainFc, double kneeDb, double rmsWindowMs, bool feedback, bool autoMakeup, double samplerate) {
      SetSampleRate(samplerate);
      SRDynamicsBase::SetThresh(dB);
      SRDynamicsBase::SetRatio(ratio);
      SRDynamicsBase::SetIsAutoMakeup(autoMakeup);
      AttRelEnvelope::SetAttack(attackMs);
      AttRelEnvelope::SetRelease(releaseMs);
      SRCompressor::InitSidechainFilter(sidechainFc);
      SRDynamicsBase::SetKnee(kneeDb);
      SRCompressor::SetTopologyFeedback(feedback);
      mEnvelopeDetectorAverager.setTc(rmsWindowMs);
    }

    void SRCompressorRMS::SetSampleRate(double sampleRate) {
      SRCompressor::SetSampleRate(sampleRate);
      mEnvelopeDetectorAverager.setSampleRate(sampleRate);
    }

    void SRCompressorRMS::SetWindow(double ms) {
      mEnvelopeDetectorAverager.setTc(ms);
    }

    //-------------------------------------------------------------
    // SRLimiter Methods
    //-------------------------------------------------------------
    SRLimiter::SRLimiter()
      : SRDynamicsBase(0.0, 1.0)
      , mPeakHoldSamples(0)
      , mPeakHoldTimer(0)
      , mMaxPeak(1.0)
      , mEnvelopeDetectorAttack(1.0)
      , mEnvelopeDetectorRelease(10.0)
      , mBufferMask(BUFFER_SIZE - 1)
      , mCursor(0)
    {
      SetAttack(1.0);
      mOutputBuffer[0].resize(BUFFER_SIZE, 0.0);
      mOutputBuffer[1].resize(BUFFER_SIZE, 0.0);
    }

    void SRLimiter::SetAttack(double ms) {
      unsigned int samp = int(0.001 * ms * mEnvelopeDetectorAttack.getSampleRate());

      assert(samp < BUFFER_SIZE);

      mPeakHoldSamples = samp;
      mEnvelopeDetectorAttack.setTc(ms);
    }

    void SRLimiter::SetRelease(double ms) {
      mEnvelopeDetectorRelease.setTc(ms);
    }

    void SRLimiter::SetSampleRate(double sampleRate) {
      mEnvelopeDetectorAttack.setSampleRate(sampleRate);
      mEnvelopeDetectorRelease.setSampleRate(sampleRate);
    }

    void SRLimiter::Reset(void) {
      mPeakHoldTimer = 0;
      mMaxPeak = mThreshLin;
      currentOvershootLin = mThreshLin;
      mCursor = 0;
      mOutputBuffer[0].assign(BUFFER_SIZE, 0.0);
      mOutputBuffer[1].assign(BUFFER_SIZE, 0.0);
    }

    void SRLimiter::FastEnvelope::setCoef(void) {
      mRuntimeCoeff = pow(0.01, (1000.0 / (mTimeConstantMs * mSampleRate))); // rises to 99% of in value over duration of time constant
    }


    //-------------------------------------------------------------
    // SRGate Methods
    //-------------------------------------------------------------
    SRGate::SRGate()
      : AttRelEnvelope(1.0, 100.0)
      , SRDynamicsBase(0.0, 1.0)
    {
    }

    //-------------------------------------------------------------
    // SRGateRMS Methods
    //-------------------------------------------------------------
    SRGateRMS::SRGateRMS()
      : mEnvelopeDetectorAverager(5.0)
    {
    }

    void SRGateRMS::setSampleRate(double sampleRate) {
      SRGate::SetSampleRate(sampleRate);
      mEnvelopeDetectorAverager.setSampleRate(sampleRate);
    }

    void SRGateRMS::setWindow(double ms) {
      mEnvelopeDetectorAverager.setTc(ms);
    }


    //-------------------------------------------------------------
    // SRDeesser methods
    //-------------------------------------------------------------
    SRDeesser::SRDeesser()
      : AttRelEnvelope(10.0, 100.0)
      , SRDynamicsBase(0.0, 1.0)
      , mFilterFreq(0.5)
      , mFilterQ(0.707)
      , mFilterGain(0.0)
      , fSidechainBandpass(SRFilterIIR<double, 2>(SRFilterIIR<double, 2>::EFilterType::BiquadBandpass, 0.5, 0.707, 0.0, 44100.0))
      , fDynamicEqFilter(SRFilterIIR<double, 2>(SRFilterIIR<double, 2>::EFilterType::BiquadPeak, 0.5, 0.707, 0.0, 44100.0))
    {
    }

    void SRDeesser::SetDeesser(double threshDb, double ratio, double attackMs, double releaseMs, double normalizedFreq, double q, double kneeDb, double samplerate) {
      SRDynamicsBase::SetThresh(threshDb);
      SRDynamicsBase::SetRatio(ratio);
      AttRelEnvelope::SetAttack(attackMs);
      AttRelEnvelope::SetRelease(releaseMs);
      AttRelEnvelope::SetSampleRate(samplerate);
      InitFilter(normalizedFreq, q);
      SetKnee(kneeDb);
    }

    void SRDeesser::InitFilter(double freq, double q) {
      mFilterFreq = freq;
      mFilterQ = q;
      fSidechainBandpass.SetFilter(SRFilterIIR<double, 2>::EFilterType::BiquadBandpass, mFilterFreq, mFilterQ, 0.0, GetSampleRate());
      fDynamicEqFilter.SetFilter(SRFilterIIR<double, 2>::EFilterType::BiquadPeak, mFilterFreq, mFilterQ, 0.0, GetSampleRate());
    }

    void SRDeesser::SetFrequency(double freq) {
      mFilterFreq = freq;
      fSidechainBandpass.SetFreq(mFilterFreq);
      fDynamicEqFilter.SetFreq(mFilterFreq);
    }

    void SRDeesser::SetQ(double q) {
      mFilterQ = q;
      fSidechainBandpass.SetQ(q);
      fDynamicEqFilter.SetQ(q);


    }
  }
} // end namespace
