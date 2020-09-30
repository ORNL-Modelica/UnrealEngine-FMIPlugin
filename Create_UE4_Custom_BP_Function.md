
- Open UE4

- Games>Blank>C++ and give it a name `PROJECT`

- `Settings>Plugins>NewPlugin>Blank` and give it a `NAME`

- In Content Browser : `View Options>Show Plugin Content` Folder will now show up in sources panel

- On the now visible folder `NAME` click and then select `Add New > New C++ Class > Bluprint Library Function` and give it a name `BP_NewFunctions`

- To test, under `Games>PROJECT>Plugins>NAME>Source>NAME>Private>BP_NewFunctions.cpp` add the following below `#include "BP_SimulateFMU.h"`.

    ```
	float UBP_SimulateFMU::SquareNumber(const float In) {
    return FMath::Pow(In, 2);
	}
    ```
- and under `Games>PROJECT>Plugins>NAME>Source>NAME>Public>BP_NewFunctions.h` add the following below `GENERATED_BODY()` but before `}`.

    ```
    // UFUNCTION(BlueprintCallable, Category = "My Nodes")
    UFUNCTION(BlueprintPure, meta = (DisplayName = "Square Number (Fancy~)", CompactNodeTitle = "Square"), Category = "My Nodes")
    static float SquareNumber(const float In);
    ```
    - Note that the commented UFUNCTION is there for the user to begin to explore different options for blueprints

