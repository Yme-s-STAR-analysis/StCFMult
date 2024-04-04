#ifndef __STCFMULT_HEADER__
#define __STCFMULT_HEADER__

/*
    Version: 2.1
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

#include "TpcShiftTool/TpcShiftTool.h"

class StCFMult {
    public:

        Int_t mRefMult;
        Int_t mRefMult3;
        Int_t mRefMult3X;
        Int_t mNTofBeta;
        Int_t mNTofMatch;
        // Int_t mNTofMatchZ;
        Int_t mTofMult;

        TpcShiftTool* mtShift;

        StCFMult();
        ~StCFMult(){}

        void clean();
        
        void ImportShiftTool(TpcShiftTool* shift_ptr);
        void IgnoreShift();
        bool make(StPicoDst *picoDst);
};

#endif
