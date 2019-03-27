#include "SRSaturation.h"
//#include this and that
//#include <math.h>


namespace SR {
	namespace DSP {



		SRSaturation::SRSaturation() {
			mType = kMusicDSP;
			mDriveNormalized = 1.;
			mAmountNormalized = 0.;
			mHarmonicsNormalized = .5;
			mPositive = true;
			mSkewNormalized = 0.;
			mWetNormalized = 1.;
      mSamplerate = 44100.;
		}

		// Constructor
		SRSaturation::SRSaturation(SRSaturation::ESaturationType pType, double pDriveDb, double pAmountNormalized, double pHarmonicsNormalized, bool pPositiveSide, double pSkewNormalized, double pWet, double pSamplerate) {
			// internal variables
			setSaturation(pType, pDriveDb, pAmountNormalized, pHarmonicsNormalized, pPositiveSide, pSkewNormalized, pWet, pSamplerate);
			mAmount = 0.;
			prev = 0.;
			dry = 0.;
		}

		// Destructor
		SRSaturation::~SRSaturation() {
		}

		void SRSaturation::setType(SRSaturation::ESaturationType pType) {
			this->mType = pType;
			calcSaturation();
		}

		void SRSaturation::setDrive(double pDriveDb) {
			this->mDriveNormalized = SR::Utils::DBToAmp(pDriveDb);
		}

		void SRSaturation::setAmount(double pAmountNormalized) {
			this->mAmountNormalized = pAmountNormalized;
			calcSaturation();
		}

		void SRSaturation::setHarmonics(double pHarmonicsNormalized) {
			this->mHarmonicsNormalized = pHarmonicsNormalized;
		}

		void SRSaturation::setPositive(bool pPositive) {
			this->mPositive = pPositive;
		}

		void SRSaturation::setSkew(double pSkewNormalized) {
			this->mSkewNormalized = pSkewNormalized;
		}

		void SRSaturation::setWet(double pWetNormalized) {
			this->mWetNormalized = pWetNormalized;
		}

    void SRSaturation::setSamplerate(double mSamplerate) {
      this->mSamplerate = mSamplerate;
    }

		void SRSaturation::setSaturation(SRSaturation::ESaturationType pType, double pDriveDb, double pAmountNormalized, double pHarmonicsNormalized, bool pPositive, double pSkewNormalized, double pWetNormalized, double pSamplerate) {
			this->mType = pType;
			this->mDriveNormalized = SR::Utils::DBToAmp(pDriveDb);
			this->mAmountNormalized = pAmountNormalized;
			this->mHarmonicsNormalized = pHarmonicsNormalized;
			this->mPositive = pPositive;
			this->mSkewNormalized = pSkewNormalized;
			this->mWetNormalized = pWetNormalized;
      this->mSamplerate = pSamplerate;
      //this->mOversampler.SetOverSampling(OverSampler<double>::k16x);
			calcSaturation();
		}


		// Internal calculations. Example on calculation depending on mType:
		void SRSaturation::calcSaturation(void) {
			switch (this->mType) {
			case kPirkle: break;
			case kPirkleMod: break;
			case kZoelzer: break;
			case kMusicDSP: this->mAmount = (1. - mAmountNormalized); break;
			case kRectHalf: break;
			case kRectFull: break;
			default: break;
			}
			return;
		}
	}
}