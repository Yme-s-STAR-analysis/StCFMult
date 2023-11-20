# StCFMult

`version: 2.0`

`author: Yige Huang`

## Usage

1. Put your n sigma shift module into ShiftUtil.cxx.

2. Include `StCFMult.h`.

3. `StCFMult* multCounter = new StCFMult()`.

4. `if (multCounter->make(mPicoDst)) { refMult3 = multCounter->mRefMult3; }`

## Change Log

20.11.2023 by yghuang (v2.0):

> De-coupled with TpcShiftUtil, now TpcShiftUtil is a independent module.
>> Will import the shift tool with ImportShiftTool() instead of using constructor.
>> Can ignore the shift by calling IgnoreShift(), or just don't import it.
> New quantities: RefMult, RefMult3X, nTofMatch (from pico), nTofMatchZ (previous nTofMatch).
> New definition of beta_eta1: nTofBeta, and beta range is from 0.1 to 1.1 now.

06.08.2023 by yghuang (v1.1):

> Add an example of TpcShiftUtil. One can still use shift util with his/her-self's code.

05.07.2023 by yghuang (v1.0):

> First version of StCFMult module.
