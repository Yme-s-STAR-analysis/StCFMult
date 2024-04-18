#ifndef __STCFMULT_HEADER__
#define __STCFMULT_HEADER__

#include <limits>
#include "TVector3.h"

#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoPhysicalHelix.h"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StThreeVectorF.hh"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StBTofUtil/tofPathLength.hh"
#include "phys_constants.h"

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
