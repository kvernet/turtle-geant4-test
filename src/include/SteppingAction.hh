#include "G4UserSteppingAction.hh"
#include "G4LogicalVolume.hh"
#include "globals.hh"
#include "G4Step.hh"


struct SteppingAction : public G4UserSteppingAction {
    SteppingAction();
    ~SteppingAction() override = default;
    
    static SteppingAction *Singleton();
    
    // method from the base class
    void UserSteppingAction(const G4Step*) override;
    
    G4double totalLength = 0.;
};
