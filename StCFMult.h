#ifndef __STCFMULT_HEADER__
#define __STCFMULT_HEADER__

/*
    Version: 1.0
    Author: Yige HUANG
    StCFMult module:
        For picoDst only.
        For CF group (especially proton number analysis).

    Demo:

        StCFMult* multCounter = new StCFMult();
        if (multCounter->make(mPicoDst)) {
            refMult3 = multCounter->mRefMult3;
        }
    
    Note: I also put the calculation of beta_eta1 and nbTOFMatch in this class.
*/

#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StThreeVectorF.hh"
#include "StPicoEvent/StPicoBTofPidTraits.h"

#include "ShiftUtil.h"

class StCFMult{
    public:

        Int_t mRefMult3;
        Int_t mBetaEta1;
        Int_t mNTofMatch;
        Int_t mTofMult;

        ShiftUtil* shift;

        StCFMult();
        ~StCFMult(){}

        void clean();
        
        bool make(StPicoDst *picoDst);
};

#endif
