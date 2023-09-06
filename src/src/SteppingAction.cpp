#include "SteppingAction.hh"
#include "G4Turtle.hh"

SteppingAction::SteppingAction() {}

SteppingAction *SteppingAction::Singleton() {
    static SteppingAction *action = nullptr;
    if(action == nullptr) {
        action = new SteppingAction;
    }
    return action;
}

void SteppingAction::UserSteppingAction(const G4Step *step) {
    /* Get volume of the current step */
    G4LogicalVolume * volume = step->GetPreStepPoint()->GetTouchableHandle()
        ->GetVolume()->GetLogicalVolume();

    if(volume == G4Turtle::GetInstance()->GetRockLogicalVolume()) {
        totalLength += step->GetStepLength();
    }
}
