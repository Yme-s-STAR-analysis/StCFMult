#include "StCFMult.h"
#include "TMath.h"

StCFMult::StCFMult() {
    clean();
    mtShift = 0;
}

void StCFMult::clean() {
    mRefMult = 0;
    mRefMult3 = 0;
    mRefMult3X = 0;
    mNTofBeta = 0;
    mNTofMatch = 0;
    // mNTofMatchZ = 0;
    mTofMult = 0;
}

void StCFMult::ImportShiftTool(TpcShiftTool* shift_ptr) {
    mtShift = shift_ptr;
}

void StCFMult::IgnoreShift() {
    mtShift = 0;
}

bool StCFMult::make(StPicoDst *picoDst) {
    clean();
    Int_t nTracks = picoDst->numberOfTracks();

    StPicoEvent* picoEvent = (StPicoEvent*)picoDst->event();
    if (!picoEvent) { return false; }

    Int_t runId = picoEvent->runId();
    TVector3 vertex = picoEvent->primaryVertex();
    Float_t vx = vertex.X();
    Float_t vy = vertex.Y();
    Float_t vz = vertex.Z();
    const Float_t mField = picoEvent->bField();

    for (Int_t iTrack=0; iTrack<nTracks; iTrack++) {
        StPicoTrack* picoTrack = (StPicoTrack *)picoDst->track(iTrack);
        if (!picoTrack || !picoTrack->isPrimary()) { continue; }

        Int_t nHitsFit = picoTrack->nHitsFit();
        if (nHitsFit <= 10) { continue; }

        Int_t nHitsDedx = picoTrack->nHitsDedx();

        // Double_t dca = fabs(picoTrack->gDCA(vx, vy, vz));
        StPicoPhysicalHelix helix = picoTrack->helix(mField);
        Double_t dca = fabs(helix.geometricSignedDistance(vertex));
        if (dca > 3) { continue; }

        TVector3 pmomentum = picoTrack->pMom();
        Double_t pcm = pmomentum.Mag();
        Double_t pt = pmomentum.Perp();
        if (pcm < 1e-10) { continue; }
        Double_t eta = pmomentum.PseudoRapidity();
        Double_t nsig = picoTrack->nSigmaProton();
        if (mtShift != 0) { // apply n sigma shift
            nsig -= mtShift->GetShift(runId, pt, eta);
        }

        Int_t tofId = picoTrack->bTofPidTraitsIndex();
        Int_t btofMatchFlag = 0;
        Double_t beta = -1.0;
        Double_t btofYLocal = -999.0;
        if (tofId >= 0) {
            StPicoBTofPidTraits* tofPid = picoDst->btofPidTraits(tofId);
            btofMatchFlag = tofPid->btofMatchFlag();
            if (tofPid) {
                beta = tofPid->btofBeta();
                btofYLocal = tofPid->btofYLocal();
                if (beta < 1e-4) { // recalculate time of flight
                    Double_t tof = tofPid->btof();
                    TVector3 btofHitPos = tofPid->btofHitPos();
                    const StThreeVectorF* btofHitsPosSt = new StThreeVectorF(
                        btofHitPos.X(),btofHitPos.Y(),btofHitPos.Z()
                    );
                    const StThreeVectorF* vtxPosSt = new StThreeVectorF(
                        vx, vy, vz
                    );
                    Double_t L = tofPathLength(vtxPosSt, btofHitsPosSt, helix.curvature());
                    beta = tof > 0 ? L / (tof * (C_C_LIGHT/1.e9)) : std::numeric_limits<Float_t>::quiet_NaN(); // note: quiet nan will never pass > N or < N
                }
            }
        }
        Double_t mass2 = -999;
        // if (btofMatchFlag > 0 && beta > 1e-5) {
        if (btofMatchFlag > 0 && beta > 0 && fabs(btofYLocal) < 1.8) {
            mass2 = pcm * pcm * (pow(1.0 / beta, 2) - 1);
        }

        if (
            (nHitsDedx > 5 || mass2 > -990) &&
            nsig < -3 &&
            mass2 < 0.4 && 
            fabs(eta) < 1.0
        ) { 
            /*
                Important note for this cut: - From Fan Si
                    If nHitsDedx is less than 5, dE/dx quantity would fail to be calculated, 
                    and nSigmaProton will be negative infty and set to be -32768,
                    from getter of nSigmaProton, it will be -32.768 and naturally less than -3.
                    If one is not aware of this mechanism, he/she can not understand the logic here.
            */
            mRefMult3 += 1;
        }
        if (
            (nHitsDedx > 5 || mass2 > -990) &&
            nsig < -3 &&
            mass2 < 0.4 &&
            fabs(eta) < 1.6
        ) { 
            mRefMult3X += 1;
        }
        // if (beta > 0.1 && beta < 1.1 && fabs(eta) < 0.5) { // according to Ashish's cut
        if (beta > 0.1 && fabs(eta) < 0.5) { // according to Ashish's cut
            mNTofBeta += 1;
        }
        // if (fabs(eta) < 0.5 && btofMatchFlag > 0){
        //     mNTofMatchZ += 1;
        // }
    }

    mRefMult = picoEvent->refMult();
    mNTofMatch = picoEvent->nBTOFMatch();
    mTofMult = picoEvent->btofTrayMultiplicity();
    return true;
}
