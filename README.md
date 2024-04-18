# StCFMult

`version: 2.3`

`author: Yige Huang`

## Usage

1. Include `StCFMult.h`

2. `StCFMult* multCounter = new StCFMult()`

3. Put the pointer of your `TpcShiftTool` into `StCFMult` by calling `ImportShiftTool()` or ignore it by calling `IgnoreShift()`

4. `if (multCounter->make(mPicoDst)) { refMult3 = multCounter->mRefMult3; }`

## Change Log

18.04.2024 by yguang (v2.3):

> Definition: if bTOF beta is less than 1e-4, we will recalculate it
>
    > For more details in the code
>
> Definition: now we add TOF local Y cut on mass square calculation

04.04.2024 by yghuang (v2.2.1):

> Fix a bug: when load `mtShift`, it was written as `shift`

04.04.2024 by yghuang (v2.2):

> Cancel: nTofMatchZ, we do not need this quantity any more, thanks for your effort!
>
> Definition: nTofBeta now would not ask for beta < 1.1, and eta range is limited to 0.5

01.04.2024 by yghuang (v2.1):

> Since the interfaces of `TpcShiftTool` are changed, multiplicity tool is also changed
>
> And some additional changes are also made, may not change the results since the logic there are not changed

20.11.2023 by yghuang (v2.0):

> De-coupled with TpcShiftUtil, now TpcShiftUtil is a independent module.
>> Will import the shift tool with ImportShiftTool() instead of using constructor.
>>
>> Can ignore the shift by calling IgnoreShift(), or just don't import it.
>
> New quantities: RefMult, RefMult3X, nTofMatch (from pico), nTofMatchZ (previous nTofMatch).
>
> New definition of beta_eta1: nTofBeta, and beta range is from 0.1 to 1.1 now.

06.08.2023 by yghuang (v1.1):

> Add an example of TpcShiftUtil. One can still use shift util with his/her-self's code.

05.07.2023 by yghuang (v1.0):

> First version of StCFMult module.
